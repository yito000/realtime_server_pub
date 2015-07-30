#include "user_record_voltdb.h"
#include "common_object.h"

#include "db/voltdb/voltdb_result.h"
#include "db/voltdb/voltdb.h"

UserRecordVoltdb* UserRecordVoltdb::getInstance()
{
    static UserRecordVoltdb inst;

    return &inst;
}

void UserRecordVoltdb::add(const std::string name,
    std::function<void(VoltdbResult&)> result_callback)
{
    std::string command_name = "UserAdd";
    auto param_setter_callback = 
        std::bind(&UserRecordVoltdb::setupAddParam, this, 
        std::placeholders::_1, name);

    invoke(command_name, param_setter_callback, result_callback);
}

// private member function
void UserRecordVoltdb::setupAddParam(voltdb::Procedure& procedure,
    const std::string& name)
{
    voltdb::ParameterSet* params = procedure.params();

    params->addString(name);
}

UserRecordVoltdb::UserRecordVoltdb()
{
    //
}
