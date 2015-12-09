#ifdef USE_VOLTDB

#ifndef USER_RECORD_VOLTDB_H
#define USER_RECORD_VOLTDB_H

#include <string>
#include <functional>

#include "db/voltdb/volt_command_invoker.h"

class UserRecordVoltdb : public VoltCommandInvoker
{
public:
    static UserRecordVoltdb* getInstance();

    void add(const std::string name, 
        std::function<void(VoltdbResult&)> result_callback);

private:
    void setupAddParam(voltdb::Procedure& procedure,
        const std::string& name);

    UserRecordVoltdb();
};

#endif

#endif
