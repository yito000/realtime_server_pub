#include "global.h"

#include "common_object.h"
#include "log/logger.h"

//
#include "protobuf/event/pb_ev_uiid.h"

#include "protobuf/event/pb_ev_res_uiid.h"
#include "protobuf/event/pb_master.h"

void Global::onStart(AppGlobalSetting& g_setting)
{
    auto kv_cache = CommonObject::getInstance()->getKeyValueCacheStorage();

    //
    if (g_setting.user_pb_route_map) {
        auto flag_opt = kv_cache->get("flag");
        auto route_map = g_setting.user_pb_route_map;

        if (flag_opt && flag_opt.get() == "1") {
            Logger::debug("set master mode");

            route_map->addRoute(1, PbMaster::sendWorker);
            route_map->addRoute(2, PbEvResponseUiid::response);
        } else {
            Logger::debug("set worker mode");

            route_map->addRoute(1, PbEvUiid::addUser);
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
