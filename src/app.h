#ifndef APP_H
#define APP_H

#include "smart_ptr.hpp"

#include <string>

#include "setting.h"
#include "server.h"
#include "thread/bidirectional_communicator.h"
#include "scheduler/app_scheduler.h"
#include "protobuf/protobuf_router.h"
#include "error/error_handle_router.h"
#include "actor/actor_manager.h"
#include "cluster/cluster.h"
#include "main_loop.h"
#include "db/voltdb/voltdb_processor.h"

#include "error/internal/error_handle_route_map.h"
#include "protobuf/internal/protobuf_route_map.h"

struct ArgsInfo
{
    std::string config_file;

    ArgsInfo()
    {
        //
    }
};

class Server;

class App
{
public:
    int start(int argc, char** argv);

private:
    void parseArgs(int argc, char** argv, ArgsInfo& args);
    Setting::ptr initSettings(ArgsInfo& args);
    
    void setupTcpServer(Setting::const_ptr setting);

    void initRandomGenerator(Setting::const_ptr setting);
    void initThreadPool(Setting::const_ptr setting);
    void initScheduler(Setting::const_ptr setting);
    void initProtobufRouter(Setting::const_ptr setting);
    void initErrorHandleRouter(Setting::const_ptr setting);
    void initActorManager(Setting::const_ptr setting);
    void initMainLoop(Setting::const_ptr setting);
    void initIOThread(Setting::const_ptr setting);
    void initNodeServer(Setting::const_ptr setting);
    void initKeyValueCache(Setting::const_ptr setting);

    void initVoltdbThread(Setting::const_ptr setting);
    void initRedisClient(Setting::const_ptr setting);

    void setupCluster(Setting::const_ptr setting);
    
    Server::ptr server;
    BidirectionalCommunicator::ptr task_comm;

    AppScheduler::ptr app_scheduler;
    ActorManager::ptr user_actor_manager;
    ActorManager::ptr up_actor_manager;
    ActorManager::ptr down_actor_manager;
    Cluster::ptr cluster;

    MainLoop::ptr main_loop;
    VoltdbProcessor::ptr v_processor;
    ProtobufRouter::ptr user_pb_router;
    ProtobufRouter::ptr system_pb_router;
    ErrorHandleRouter::ptr err_handle_router;

    ProtobufRouteMap::ptr user_pb_route_map;
    ProtobufRouteMap::ptr system_pb_route_map;

    ErrorHandleRouteMap::ptr err_handle_route_map;
    std::vector<TaskDispatcher::ptr> io_threads;
};

#endif
