#ifndef SETTING_LOADER_H
#define SETTING_LOADER_H

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>

#include "setting.h"

BEGIN_NS

class SettingLoader
{
public:
    static Setting::ptr load(const std::string& filename);

private:
    struct UdpServerSetting
    {
        bool enable;
        bool is_ipv6;
        unsigned short port;
        
        UdpServerSetting()
        {
            enable = false;
            is_ipv6 = false;
            port = 0;
        }
    };
    
    static void createUdpServer(const boost::property_tree::ptree& p,
        UdpServerSetting& setting);
    static void createVariables(const boost::property_tree::ptree& p,
        std::map<std::string, std::string>& variables);
    static void createClusterNodes(const boost::property_tree::ptree& p, 
        std::list<ClusterNodeSetting::ptr>& cluster_nodes);
};

END_NS

#endif
