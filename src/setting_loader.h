#ifndef SETTING_LOADER_H
#define SETTING_LOADER_H

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>

#include "setting.h"

class SettingLoader
{
public:
    static Setting::ptr load(const std::string& filename);

private:
    static void createVariables(const boost::property_tree::ptree& p,
        std::map<std::string, std::string>& variables);
    static void createClusterNodes(const boost::property_tree::ptree& p, 
        std::list<ClusterNodeSetting::ptr>& cluster_nodes);
};

#endif
