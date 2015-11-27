#include "global.h"

#include "app_director.h"
#include "common_object.h"
#include "log/logger.h"

#include "router/detail/route_map.h"

#include "app.h"
#include "test_udp.h"

#include "battle/battle_manager.h"

#include <boost/bind.hpp>

//
//#include "protobuf/event/pb_ev_uiid.h"
//#include "protobuf/event/pb_ev_res_uiid.h"
//#include "protobuf/event/pb_master.h"

//
#include "flatbuffers/event/fb_event_uiid.h"
#include "flatbuffers/event/fb_event_res_uiid.h"
#include "flatbuffers/event/fb_master.h"

#include "app/flatbuffers/register_glue_fb.h"
#include "app/flatbuffers/register_demo_battle_cli.h"
#include "app/flatbuffers/register_udp_handshake.h"

void Global::onStart(AppGlobalSetting& g_setting, App* app)
{
    //
    auto kv_cache = AppDirector::getInstance()->getKeyValueCacheStorage();
    auto flag_opt = kv_cache->get("flag");
    
    bool master_node = false;
    if (flag_opt && flag_opt.get() == "1") {
        master_node = true;
    }

    if (master_node) {
        auto server = app->getServer();
        auto udp_server = server->getUdpServer();
        
        TestUdp::ptr test_udp_callback = new TestUdp;
        udp_server->setSendCallback(boost::bind(&TestUdp::sendCallback,
            test_udp_callback, _1, _2, _3));
        udp_server->setReceiveCallback(boost::bind(&TestUdp::receiveCallback,
            test_udp_callback, _1, _2, _3, _4, _5));
    }
    
    //
    if (g_setting.user_route_map) {
        auto route_map = g_setting.user_route_map;

        if (master_node) {
            Logger::debug("set master mode");

            route_map->addRoute(1, FbMaster::sendWorker);
            route_map->addRoute(2, FbEvResponseUiid::response);
        } else {
            Logger::debug("set worker mode");

            route_map->addRoute(1, FbEvUiid::addUser);
        }
        
        // NOTE: glue code test
        register_glue_fb(*route_map);
        register_demo_battle_cli(*route_map);
    }
    
    if (g_setting.user_route_map_udp) {
        auto route_map_udp = g_setting.user_route_map_udp;
        
        register_udp_handshake(*route_map_udp);
        // TODO: register player input command UDP
    }

    //
    if (g_setting.system_route_map) {
        // todo
    }
    
    {
        auto bd = BattleManager::getInstance();
        bd->initialize(4);
        
        auto route_map = g_setting.user_err_route_map;
        route_map->addCallback(-1, boost::bind(&BattleManager::receiveError, bd, _1));
    }
}

void Global::onEnd()
{
    Logger::debug("end application");
    
    auto bd = BattleManager::getInstance();
    bd->endAllBattle();
}
