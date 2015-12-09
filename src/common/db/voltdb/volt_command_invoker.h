#ifdef USE_VOLTDB

#ifndef VOLT_COMMAND_INVOKER_H
#define VOLT_COMMAND_INVOKER_H

#include <string>
#include <map>

#include "db/voltdb/voltdb_result.h"
#include "db/voltdb/voltdb.h"

typedef std::function<void(voltdb::Procedure&)> VoltParamCallback;
typedef std::function<void(VoltdbResult&)> VoltResultCallback;

#define VOLTDB_DEFAULT_RETRY 3

class VoltCommandInvoker
{
public:
    virtual ~VoltCommandInvoker() {}

protected:
    void invoke(const std::string command_name, 
        VoltParamCallback param_callback, 
        VoltResultCallback result_callback, 
        int retry = VOLTDB_DEFAULT_RETRY);

private:
    void invokeAsync(const std::string command_name,  
        VoltParamCallback param_callback, 
        VoltResultCallback result_callback, int retry);

    void retryQuery(const std::string command_name,
        VoltParamCallback param_callback,
        VoltResultCallback result_callback, int retry);
};

#endif

#endif
