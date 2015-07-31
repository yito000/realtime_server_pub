#include "app.h"

#include "global.h"
#include "server.h"
#include "node_server.h"
#include "setting_loader.h"
#include "cluster_builder.h"

#include "common/common.h"
#include "lib/lib.h"

#include "common_object.h"
#include "app_session_delegate.h"
#include "node_session_delegate.h"

#include "db/redis/redis_service.h"

int App::start(int argc, char** argv)
{
    ArgsInfo args_info;
    args_info.config_file = "config.json";

    parseArgs(argc, argv, args_info);
    Setting::ptr setting = initSettings(args_info);

    try {
        AppGlobalSetting g_setting;
        g_setting.user_pb_route_map = user_pb_route_map.get();
        g_setting.system_pb_route_map = system_pb_route_map.get();

        //
        Global global;
        global.onStart(g_setting);

        if (setting->master_node) {
            AppSessionDelegate* inst = new AppSessionDelegate(task_comm);

            Server::AddrType addr_type = Server::ADDR_V4;
            if (setting->addr_v6) {
                addr_type = Server::ADDR_V6;
            }

            Server s(addr_type, setting->port);

            s.setDelegate(inst);
            s.setTimeoutMillis(setting->timeout_millis);
            s.setRetry(setting->retry);

            Logger::log("start server port=%d", setting->port);

            s.accept();
            s.start();

            delete inst;
        } else {
            auto sleep_time = boost::chrono::milliseconds(100);

            while (1) {
                // todo
                boost::this_thread::sleep_for(sleep_time);
            }
        }

        global.onEnd();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

void App::parseArgs(int argc, char** argv, ArgsInfo& args)
{
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::string str = argv[i];

            if (str == "-d") {
                if (i + 1 < argc) {
                    std::string config_file = argv[++i];

                    args.config_file = config_file;
                }
            }
        }
    }
}

Setting::ptr App::initSettings(ArgsInfo& args)
{
    auto file_util = FileUtil::getInstance();
    std::string filepath = file_util->getCurrentPath() + args.config_file;
    Logger::debug("load config file: %s", filepath.c_str());

    auto setting = SettingLoader::load(filepath);

    int additional_thread = 1; // cluster IO
    if (setting->connect_voltdb) {
        additional_thread++;
    }
    if (setting->connect_redis) {
        additional_thread++;
    }

    setting->thread_size += additional_thread;

    //
    user_pb_route_map = new ProtobufRouteMap;
    system_pb_route_map = new ProtobufRouteMap;
    err_handle_route_map = new ErrorHandleRouteMap;

    initRandomGenerator(setting);
    initThreadPool(setting);
    initScheduler(setting);
    initProtobufRouter(setting);
    initErrorHandleRouter(setting);

    initActorManager(setting);
    setupCluster(setting);

    initMainLoop(setting);
    initIOThread(setting);
    initNodeServer(setting);
    initKeyValueCache(setting);

    initVoltdbThread(setting);
    initRedisClient(setting);

    return setting;
}

void App::initRandomGenerator(Setting::const_ptr setting)
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch());

    int seed = ms.count();
    auto rand = new Random(seed);

    CommonObject::getInstance()->setRandomGenerator(rand);
}

void App::initThreadPool(Setting::const_ptr setting)
{
    task_comm = BidirectionalCommunicator::create(setting->thread_size);

    CommonObject::getInstance()->setBidirectionalCommunicator(task_comm);
}

void App::initScheduler(Setting::const_ptr setting)
{
    app_scheduler = new AppScheduler;
    app_scheduler->initialize(setting->scheduler_interval);

    task_comm->postWorker(std::bind(&AppScheduler::run, app_scheduler));

    //
    CommonObject::getInstance()->setDelayScheduler(app_scheduler);
}

void App::initProtobufRouter(Setting::const_ptr setting)
{
    user_pb_router = new ProtobufRouter(*user_pb_route_map);
    CommonObject::getInstance()->setUserProtobufRouter(user_pb_router);

    //
    system_pb_router = new ProtobufRouter(*system_pb_route_map);
    CommonObject::getInstance()->setSystemProtobufRouter(system_pb_router);
}

void App::initErrorHandleRouter(Setting::const_ptr setting)
{
    err_handle_router = new ErrorHandleRouter(*err_handle_route_map);

    CommonObject::getInstance()->setErrorHandleRouter(err_handle_router);
}

void App::initActorManager(Setting::const_ptr setting)
{
    user_actor_manager = new ActorManager(task_comm);
    CommonObject::getInstance()->setUserActorManager(user_actor_manager);

    //
    up_actor_manager = new ActorManager(task_comm);
    CommonObject::getInstance()->setUpActorManager(up_actor_manager);

    //
    down_actor_manager = new ActorManager(task_comm);
    CommonObject::getInstance()->setDownActorManager(down_actor_manager);
}

void App::initMainLoop(Setting::const_ptr setting)
{
    main_loop = new MainLoop;
    main_loop->setBidirectionalCommunicator(task_comm);

    main_loop->addActorManager(user_actor_manager);
    main_loop->addActorManager(up_actor_manager);
    main_loop->addActorManager(down_actor_manager);

    auto task = new DelayedTask;
    task->interval = 5;
    task->func = std::bind(&MainLoop::update, main_loop);

    app_scheduler->addTask("_main_loop", task);
}

void App::initIOThread(Setting::const_ptr setting)
{
    const int SCHEDULER_INTERVAL = 5;
    const int IO_SCHEDULER_INTERVAL = 60;

    for (int i = 0; i < setting->io_thread_size; i++) {
        TaskDispatcher::ptr td = new TaskDispatcher;
        io_threads.push_back(td);

        //
        AppScheduler::ptr sche = new AppScheduler;
        sche->initialize(SCHEDULER_INTERVAL);

        //
        auto task = new DelayedTask;
        task->interval = IO_SCHEDULER_INTERVAL;
        task->func = std::bind(&TaskDispatcher::update, td);

        sche->addTask("_io_event_loop", task);

        //
        task_comm->postWorker(std::bind(&AppScheduler::run, sche));
    }
}

void App::initNodeServer(Setting::const_ptr setting)
{
    using boost::asio::ip::tcp;

    if (!setting->worker_node) {
        return;
    }

    //
    auto addr_type = NodeServer::ADDR_V4;
    if (setting->node_addr_v6) {
        addr_type = NodeServer::ADDR_V6;
    }

    const int SCHEDULER_INTERVAL = 3;
    const int SERVER_SCHEDULER_INTERVAL = 30;

    auto node_server = new NodeServer(
        addr_type, setting->node_port);
    auto session_inst = new NodeSessionDelegate(task_comm);

    node_server->setDelegate(session_inst);
    node_server->accept();

    //
    AppScheduler::ptr sche = new AppScheduler;
    sche->initialize(SCHEDULER_INTERVAL);

    //
    auto task = new DelayedTask;
    task->interval = SERVER_SCHEDULER_INTERVAL;
    task->func = std::bind(&NodeServer::run, node_server);

    sche->addTask("_node_server_task", task);

    //
    task_comm->postWorker(std::bind(&AppScheduler::run, sche));

    Logger::log("start node server port=%d", setting->node_port);
}

void App::initKeyValueCache(Setting::const_ptr setting)
{
    auto kv_cache = new KeyValueCacheStorage;

    auto it = setting->variables.begin();
    for (; it != setting->variables.end(); ++it) {
        auto key = it->first;
        auto value = it->second;

        Logger::debug("set variable: %s = %s", key.c_str(), value.c_str());

        kv_cache->set(key, value);
    }

    CommonObject::getInstance()->setKeyValueCacheStorage(kv_cache);
}

void App::initVoltdbThread(Setting::const_ptr setting)
{
    if (!setting->connect_voltdb) {
        return;
    }

    const int SCHEDULER_INTERVAL = 5;
    const int IO_SCHEDULER_INTERVAL = 20;

    //
    VoltdbConnection::Params v_params;
    VoltdbConnection::ptr v_conn = new VoltdbConnection(v_params);
    v_conn->connect(setting->voltdb_host, setting->voltdb_port);

    v_processor = new VoltdbProcessor(v_conn);

    VoltdbIOService::ptr v_ios = new VoltdbIOService(*v_processor);
    CommonObject::getInstance()->setVoltdbIOService(v_ios);

    //
    AppScheduler::ptr sche = new AppScheduler;
    sche->initialize(SCHEDULER_INTERVAL);

    //
    auto task = new DelayedTask;
    task->interval = IO_SCHEDULER_INTERVAL;
    task->func = std::bind(&VoltdbProcessor::run, v_processor);

    sche->addTask("_voltdb_task", task);

    //
    task_comm->postWorker(std::bind(&AppScheduler::run, sche));
}

void App::initRedisClient(Setting::const_ptr setting)
{
    if (!setting->connect_redis) {
        return;
    }

    const int SCHEDULER_INTERVAL = 5;
    const int IO_SCHEDULER_INTERVAL = 20;

    //
    RedisService::ptr r_service = new RedisService;
    r_service->connect(setting->redis_host, setting->redis_port);

    //
    CommonObject::getInstance()->setRedisService(r_service);

    //
    AppScheduler::ptr sche = new AppScheduler;
    sche->initialize(SCHEDULER_INTERVAL);

    //
    auto task = new DelayedTask;
    task->interval = IO_SCHEDULER_INTERVAL;
    task->func = std::bind(&RedisService::update, r_service);

    sche->addTask("_redis_task", task);

    //
    task_comm->postWorker(std::bind(&AppScheduler::run, sche));
}

void App::setupCluster(Setting::const_ptr setting)
{
    const int SCHEDULER_INTERVAL = 5;
    const int CLUSTER_SCHEDULER_INTERVAL = 5000;

    cluster = ClusterBuilder::build(setting, task_comm);

    CommonObject::getInstance()->setCluster(cluster);

    //
    AppScheduler::ptr sche = new AppScheduler;
    sche->initialize(SCHEDULER_INTERVAL);

    //
    {
        auto task = new DelayedTask;
        task->interval = CLUSTER_SCHEDULER_INTERVAL;
        task->func = std::bind(&Cluster::run, cluster);

        sche->addTask("_cluster_task", task);
    }

    //
    {
        auto task = new DelayedTask;
        task->interval = 100;
        task->max_loop = 1;
        task->func = [this]() {
            cluster->setWatchMode(true);
        };

        sche->addTask("_watch_cluster_node", task);
    }

    //
    task_comm->postWorker(std::bind(&AppScheduler::run, sche));
}
