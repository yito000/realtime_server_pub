#ifndef SETTING_H
#define SETTING_H

#include "smart_ptr.hpp"

#include <string>
#include <list>
#include <map>

struct ClusterNodeSetting : public SmartPtr<ClusterNodeSetting>
{
    typedef boost::intrusive_ptr<ClusterNodeSetting> ptr;

    long node_id;
    std::string host;
    unsigned short port;
    std::string protocol;

    ClusterNodeSetting()
    {
        node_id = -1;
        port = -1;
        protocol = "default";
    }
};

struct Setting : public SmartPtr<Setting>
{
    typedef boost::intrusive_ptr<Setting> ptr;
    typedef boost::intrusive_ptr<Setting const> const_ptr;

    bool master_node;
    bool addr_v6;
    unsigned short port;
    std::string protocol;

    bool worker_node;
    bool node_addr_v6;
    unsigned short node_port;
    std::string node_protocol;

    int timeout_millis;
    int retry;

    int thread_size;
    int io_thread_size;
    int scheduler_interval;
    
    std::string cert_path;
    std::string pkey_path;
    
    // udp server
    bool enable_udp_server;
    bool udp_server_is_ipv6;
    unsigned short udp_server_port;

    // voltdb
    bool connect_voltdb;
    std::string voltdb_host;
    unsigned short voltdb_port;

    bool connect_redis;
    std::string redis_host;
    unsigned short redis_port;

    std::map<std::string, std::string> variables;
    std::list<ClusterNodeSetting::ptr> cluster_nodes;

    Setting()
    {
        master_node = true;
        addr_v6 = false;
        port = 9000;
        protocol = "default";

        worker_node = false;
        node_addr_v6 = false;
        node_port = 12000;
        node_protocol = "default";

        timeout_millis = 30 * 1000;
        retry = 3;

        thread_size = 1;
        io_thread_size = 1;
        scheduler_interval = 1;
        
        enable_udp_server = false;
        udp_server_is_ipv6 = false;
        udp_server_port = 0;

        connect_voltdb = false;
        voltdb_host = "0.0.0.0";
        voltdb_port = 21212;

        connect_redis = false;
        redis_host = "0.0.0.0";
        redis_port = 6379;
    }
};

#endif

