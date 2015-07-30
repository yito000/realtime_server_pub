#include "setting_loader.h"
#include "log/logger.h"

#include "common_object.h"

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace {
    const char* ATTR_IS_MASTER_NODE = "is_master_node";
    const char* ATTR_IS_IPV6 = "is_ipv6";
    const char* ATTR_PORT = "port";

    const char* ATTR_IS_WORKER_NODE = "is_worker_node";
    const char* ATTR_IS_IPV6_NODE = "is_ipv6_node";
    const char* ATTR_NODE_PORT = "node_port";

    const char* ATTR_TIMEOUT = "timeout";
    const char* ATTR_RETRY = "retry";

    const char* ATTR_WORKER_THREAD_SIZE = "worker_thread_size";
    const char* ATTR_IO_THREAD_SIZE = "io_thread_size";
    const char* ATTR_SCHEDULER_INTERVAL = "scheduler_interval";

    const char* ATTR_CONNECT_VOLTDB = "connect_voltdb";
    const char* ATTR_VOLTDB_HOST = "voltdb_host";
    const char* ATTR_VOLTDB_PORT = "voltdb_port";

    const char* ATTR_CONNECT_REDIS = "connect_redis";
    const char* ATTR_REDIS_HOST = "redis_host";
    const char* ATTR_REDIS_PORT = "redis_port";

    // variables
    const char* ATTR_VARIABLES = "variables";

    // cluster node
    const char* ATTR_CLUSTER_NODES = "cluster_nodes";
    const char* ATTR_CLUSTER_NODE_ID = "id";
    const char* ATTR_CLUSTER_NODE_HOST = "host";
    const char* ATTR_CLUSTER_NODE_PORT = "port";
};

Setting::ptr SettingLoader::load(const std::string& filename)
{
    const int WORKER_THREAD_SIZE = 4;
    const int SCHEDULER_THREAD_SIZE = 1;
    const int IO_THREAD_SIZE = 2;

    //
    Setting::ptr setting = new Setting;

    bool is_master_node = true;
    bool is_ipv6 = true;
    int port = 9000;

    bool is_worker_node = false;
    bool is_ipv6_node = false;
    int node_port = 12000;

    int timeout_millis = 30 * 1000;
    int retry = 3;

    int worker_thread_size = WORKER_THREAD_SIZE;
    int io_thread_size = IO_THREAD_SIZE;
    int scheduler_thread_size = SCHEDULER_THREAD_SIZE;
    int scheduler_interval = 20;

    bool connect_voltdb = false;
    std::string voltdb_host = "0.0.0.0";
    int voltdb_port = 21212;

    bool connect_redis = false;
    std::string redis_host = "0.0.0.0";
    int redis_port = 6379;

    std::map<std::string, std::string>& variables = setting->variables;
    std::list<ClusterNodeSetting::ptr>& cluster_nodes = setting->cluster_nodes;

    //
    using namespace boost::property_tree;

    std::ifstream fin(filename);
    if (fin.is_open()) {
        ptree pt;
        read_json(fin, pt);

        BOOST_FOREACH (const auto& v, pt) {
            std::string node_name = v.first;

            try {
                if (node_name == ATTR_IS_MASTER_NODE) {
                    int n = std::stoi(v.second.data());

                    is_master_node = n == 1;
                } else if (node_name == ATTR_IS_IPV6) {
                    int n = std::stoi(v.second.data());

                    is_ipv6 = n == 1;
                } else if (node_name == ATTR_PORT) {
                    port = std::stoi(v.second.data());
                } else if (node_name == ATTR_IS_WORKER_NODE) {
                    int n = std::stoi(v.second.data());

                    is_worker_node = n == 1;
                } else if (node_name == ATTR_IS_IPV6_NODE) {
                    int n = std::stoi(v.second.data());

                    is_ipv6_node = n == 1;
                } else if (node_name == ATTR_NODE_PORT) {
                    node_port = std::stoi(v.second.data());
                } else if (node_name == ATTR_TIMEOUT) {
                    timeout_millis = std::stoi(v.second.data());
                } else if (node_name == ATTR_RETRY) {
                    retry = std::stoi(v.second.data());
                } else if (node_name == ATTR_WORKER_THREAD_SIZE) {
                    worker_thread_size = std::stoi(v.second.data());
                } else if (node_name == ATTR_IO_THREAD_SIZE) {
                    io_thread_size = std::stoi(v.second.data());
                } else if (node_name == ATTR_SCHEDULER_INTERVAL) {
                    scheduler_interval = std::stoi(v.second.data());
                } else if (node_name == ATTR_CONNECT_VOLTDB) {
                    int n = std::stoi(v.second.data());

                    connect_voltdb = n == 1;
                } else if (node_name == ATTR_VOLTDB_HOST) {
                    voltdb_host = v.second.data();
                } else if (node_name == ATTR_VOLTDB_PORT) {
                    voltdb_port = std::stoi(v.second.data());
                } else if (node_name == ATTR_CONNECT_REDIS) {
                    int n = std::stoi(v.second.data());

                    connect_redis = n == 1;
                } else if (node_name == ATTR_REDIS_HOST) {
                    redis_host = v.second.data();
                } else if (node_name == ATTR_REDIS_PORT) {
                    redis_port = std::stoi(v.second.data());
                } else if (node_name == ATTR_VARIABLES) {
                    createVariables(v.second, variables);
                } else if (node_name == ATTR_CLUSTER_NODES) {
                    createClusterNodes(v.second, cluster_nodes);
                }
            } catch (std::exception& e) {
                //
            }
        }
    } else {
        Logger::log("warning cannot open file");
    }

    fin.close();

    //
    setting->master_node = is_master_node;
    setting->addr_v6 = is_ipv6;
    setting->port = port;

    setting->worker_node = is_worker_node;
    setting->node_addr_v6 = is_ipv6_node;
    setting->node_port = node_port;

    setting->timeout_millis = timeout_millis;
    setting->retry = retry;

    setting->thread_size = worker_thread_size + 
        io_thread_size + scheduler_thread_size;
    setting->io_thread_size = io_thread_size;
    setting->scheduler_interval = scheduler_interval;

    setting->connect_voltdb = connect_voltdb;
    setting->voltdb_host = voltdb_host;
    setting->voltdb_port = voltdb_port;

    setting->connect_redis = connect_redis;
    setting->redis_host = redis_host;
    setting->redis_port = redis_port;

    return setting;
}

// private member function
void SettingLoader::createVariables(const boost::property_tree::ptree& p,
    std::map<std::string, std::string>& variables)
{
    BOOST_FOREACH (const auto& v, p) {
        std::string key = v.first;
        std::string value = v.second.data();

        if (key != "" && value != "") {
            variables[key] = value;
        }
    }
}

void SettingLoader::createClusterNodes(
    const boost::property_tree::ptree& p,
    std::list<ClusterNodeSetting::ptr>& cluster_nodes)
{
    BOOST_FOREACH (const auto& v, p) {
        auto cp = v.second;
        ClusterNodeSetting::ptr node_info = new ClusterNodeSetting;

        BOOST_FOREACH (const auto& vv, cp) {
            auto node_name = vv.first;

            try {
                if (node_name == ATTR_CLUSTER_NODE_ID) {
                    node_info->node_id = std::stol(vv.second.data());
                } else if (node_name == ATTR_CLUSTER_NODE_HOST) {
                    node_info->host = vv.second.data();
                } else if (node_name == ATTR_CLUSTER_NODE_PORT) {
                    node_info->port = std::stoi(vv.second.data());
                }
            } catch (std::exception& e) {
                // ignore
            }
        }

        if (node_info->node_id != -1 && node_info->host != "" &&
            node_info->port != 0) {

            cluster_nodes.push_back(node_info);
        }
    }
}
