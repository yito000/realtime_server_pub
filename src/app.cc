#include "app.h"

#include "global.h"

#include "server.h"
#include "server/tcp_server.h"

#include "node_server.h"
#include "setting_loader.h"
#include "cluster_builder.h"

#include "common/common.h"
#include "common/global_rand.h"
#include "common/file/file_util.h"
#include "lib/lib.h"

#include "common_object.h"
#include "app_director.h"
#include "app_session_delegate.h"
#include "node_session_delegate.h"

#include "db/redis/redis_service.h"
#include "network/ssl_mt.h"

#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_MACOSX)
#include <signal.h>

static volatile sig_atomic_t sig_flag = 0;
static Server* g_server_cache = nullptr;

void HandleSigint(int no)
{
    sig_flag = 1;
    if (g_server_cache) {
        g_server_cache->setEndFlag(true);
    }
}

void HandleSighup(int no)
{
    //
}

void HandleSigterm(int no)
{
    sig_flag = 1;
    if (g_server_cache) {
        g_server_cache->setEndFlag(true);
    }
}

void SetupSignalAction()
{
    {
        struct sigaction st;
        memset(&st, 0, sizeof(st));

        st.sa_handler = HandleSigint;
        st.sa_mask |= SA_RESTART;

        sigaction(SIGINT, &st, NULL);
    }

    {
        struct sigaction st;
        memset(&st, 0, sizeof(st));

        st.sa_handler = HandleSighup;
        st.sa_mask |= SA_RESTART;

        sigaction(SIGHUP, &st, NULL);
    }

    {
        struct sigaction st;
        memset(&st, 0, sizeof(st));

        st.sa_handler = HandleSigterm;
        st.sa_mask |= SA_RESTART;

        sigaction(SIGTERM, &st, NULL);
    }
}
#endif

namespace {
    const char* DEFAULT_CONFIG_FILE = "config.json";
    const char* DEFAULT_PROTOCOL = "default";
    const char* DEFAULT_CLUSTER_PROTOCOL = "default";
};

int App::start(int argc, char** argv)
{
    auto file_util = FileUtil::getInstance();
    file_util->addSearchPathPrefix(file_util->getCurrentPath());
    
    //
    ArgsInfo args_info;
    args_info.config_file = DEFAULT_CONFIG_FILE;
    
    parseArgs(argc, argv, args_info);
    Setting::ptr setting = initSettings(args_info);
    
    try {
        AppGlobalSetting g_setting;
        g_setting.user_route_map = user_route_map.get();
        g_setting.system_route_map = system_route_map.get();
        g_setting.user_err_route_map = user_err_handle_route_map.get();
        g_setting.cluster_err_route_map = cluster_err_handle_route_map.get();
        g_setting.server_err_route_map = server_err_handle_route_map.get();
        
        ssl_thread_setup();

#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_MACOSX)
        SetupSignalAction();
#endif

        if (setting->master_node) {
            server = new Server;
            setupTcpServer(setting);
            setupUdpServer(setting);
            
            g_server_cache = server.get();
        }
        
        Global global;
        global.onStart(g_setting, this);
        
        if (setting->master_node) {
            server->start();
        } else {
            auto sleep_time = boost::chrono::milliseconds(100);

            while (sig_flag == 0) {
                boost::this_thread::sleep_for(sleep_time);
            }
        }
        
        global.onEnd();
        
        stopApp();
        ssl_thread_cleanup();
        
        AppDirector::getInstance()->finalize();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}

// private member function
void App::setupTcpServer(Setting::const_ptr setting)
{
    std::string cert = setting->cert_path;
    std::string pkey = setting->pkey_path;
    
    AppSessionDelegate* inst = new AppSessionDelegate(task_comm);
    
    TcpServer::AddrType addr_type = TcpServer::ADDR_V4;
    if (setting->addr_v6) {
        addr_type = TcpServer::ADDR_V6;
    }
    auto tcp_server = new TcpServer(server->getIOService());
    
    std::string protocol = setting->protocol != "" ?
        setting->protocol : DEFAULT_PROTOCOL;
    
    tcp_server->setPassword("test"); // TODO
    
    tcp_server->initialize(addr_type, setting->port,
        cert, pkey);
    
    tcp_server->setDelegate(inst);
    tcp_server->setTimeoutMillis(setting->timeout_millis);
    tcp_server->setValidProtocol(protocol);
    
    server->setTcpServer(tcp_server);
    
    Logger::log("start tcp server port=%d", setting->port);
    
    tcp_server->accept();
}

void App::setupUdpServer(Setting::const_ptr setting)
{
    if (!setting->enable_udp_server) {
        return;
    }
    
    UdpServer::AddrType addr_type = UdpServer::ADDR_V4;
    if (setting->udp_server_is_ipv6) {
        addr_type = UdpServer::ADDR_V6;
    }
    auto udp_server = new UdpServer(server->getIOService(),
        addr_type, setting->udp_server_port);
    server->setUdpServer(udp_server);
    
    Logger::log("start udp server port=%d", setting->udp_server_port);
    
    udp_server->start();
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
    user_route_map = new RouteMap;
    system_route_map = new RouteMap;
    user_err_handle_route_map = new ErrorHandleRouteMap;
    cluster_err_handle_route_map = new ErrorHandleRouteMap;
    server_err_handle_route_map = new ErrorHandleRouteMap;

    initRandomGenerator(setting);
    initThreadPool(setting);
    initScheduler(setting);
    initRouter(setting);
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

void App::stopApp()
{
    for (auto sche: scheduler_list) {
        sche->endScheduler();
    }
    
    auto sleep_time = boost::chrono::milliseconds(100);
    boost::this_thread::sleep_for(sleep_time);
    
    task_comm->stop();
}

void App::initRandomGenerator(Setting::const_ptr setting)
{
    GlobalRand::getInstance();
}

void App::initThreadPool(Setting::const_ptr setting)
{
    task_comm = BidirectionalCommunicator::create(setting->thread_size);
    
    AppDirector::getInstance()->setBidirectionalCommunicator(task_comm);
}

void App::initScheduler(Setting::const_ptr setting)
{
    app_scheduler = new AppScheduler;
    app_scheduler->initialize(setting->scheduler_interval);
    
    scheduler_list.push_back(app_scheduler);
    task_comm->postWorker(std::bind(&AppScheduler::run, app_scheduler));

    //
    CommonObject::getInstance()->setDelayScheduler(app_scheduler);
}

void App::initRouter(Setting::const_ptr setting)
{
    user_router = new Router(*user_route_map);
    CommonObject::getInstance()->setUserRouter(user_router);

    //
    system_router = new Router(*system_route_map);
    CommonObject::getInstance()->setSystemRouter(system_router);
}

void App::initErrorHandleRouter(Setting::const_ptr setting)
{
    user_err_handle_router = new ErrorHandleRouter(*user_err_handle_route_map);
    cluster_err_handle_router = new ErrorHandleRouter(*cluster_err_handle_route_map);
    server_err_handle_router = new ErrorHandleRouter(*server_err_handle_route_map);

    CommonObject::getInstance()->setUserErrorHandleRouter(user_err_handle_router);
    CommonObject::getInstance()->setClusterErrorHandleRouter(cluster_err_handle_router);
    CommonObject::getInstance()->setServerErrorHandleRouter(server_err_handle_router);
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
//    main_loop->addActorManager(down_actor_manager); // TODO: delete

    auto task = new DelayedTask;
    task->interval = 5;
    task->func = std::bind(&MainLoop::update, main_loop);

    app_scheduler->addTask("_main_loop", task);
}

void App::initIOThread(Setting::const_ptr setting)
{
    const int SCHEDULER_INTERVAL = 30;
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
        scheduler_list.push_back(sche);
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

    const int SCHEDULER_INTERVAL = 10;
    const int SERVER_SCHEDULER_INTERVAL = 20;

    auto node_server = new NodeServer(addr_type, setting->node_port);
    
    auto session_inst = new NodeSessionDelegate(task_comm);
    std::string node_protocol = setting->node_protocol != "" ?
        setting->node_protocol : DEFAULT_CLUSTER_PROTOCOL;
    
    node_server->setDelegate(session_inst);
    node_server->setValidProtocol(node_protocol);
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
    scheduler_list.push_back(sche);
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

    AppDirector::getInstance()->setKeyValueCacheStorage(kv_cache);
}

void App::initVoltdbThread(Setting::const_ptr setting)
{
    if (!setting->connect_voltdb) {
        return;
    }

    const int SCHEDULER_INTERVAL = 10;
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
    scheduler_list.push_back(sche);
    task_comm->postWorker(std::bind(&AppScheduler::run, sche));
}

void App::initRedisClient(Setting::const_ptr setting)
{
    if (!setting->connect_redis) {
        return;
    }

    const int SCHEDULER_INTERVAL = 10;
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
    scheduler_list.push_back(sche);
    task_comm->postWorker(std::bind(&AppScheduler::run, sche));
}

void App::setupCluster(Setting::const_ptr setting)
{
    if (setting->worker_node) {
        return;
    }
    
    const int SCHEDULER_INTERVAL = 100;
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
    scheduler_list.push_back(sche);
    task_comm->postWorker(std::bind(&AppScheduler::run, sche));
}
