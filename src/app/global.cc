#include "global.h"

#include "app_director.h"
#include "common_object.h"
#include "log/logger.h"

//
//#include "protobuf/event/pb_ev_uiid.h"
//#include "protobuf/event/pb_ev_res_uiid.h"
//#include "protobuf/event/pb_master.h"

//
#include "flatbuffers/event/fb_event_uiid.h"
#include "flatbuffers/event/fb_event_res_uiid.h"
#include "flatbuffers/event/fb_master.h"

void Global::onStart(AppGlobalSetting& g_setting)
{
    auto kv_cache = AppDirector::getInstance()->getKeyValueCacheStorage();

    //
    if (g_setting.user_pb_route_map) {
        auto flag_opt = kv_cache->get("flag");
        auto route_map = g_setting.user_pb_route_map;

        if (flag_opt && flag_opt.get() == "1") {
            Logger::debug("set master mode");

            route_map->addRoute(1, FbMaster::sendWorker);
            route_map->addRoute(2, FbEvResponseUiid::response);
        } else {
            Logger::debug("set worker mode");

            route_map->addRoute(1, FbEvUiid::addUser);
        }
    }

    //
    if (g_setting.system_pb_route_map) {
        // todo
    }
}

void Global::onEnd()
{
    //
}
