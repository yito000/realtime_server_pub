#ifdef USE_VOLTDB

#include "volt_command_invoker.h"
#include "common_object.h"

#include "log/logger.h"

BEGIN_NS

namespace {
    const int RETRY_DELAY_MILLIS = 1000 * 30;
};

// protected member function
void VoltCommandInvoker::invoke(const std::string command_name, 
    VoltParamCallback param_callback,
    VoltResultCallback result_callback, int retry)
{
    VoltdbIOService::ptr v_ios =
        CommonObject::getInstance()->getVoltdbIOService();
    boost::asio::io_service& ios = v_ios->getIOService();

    ios.post(std::bind(&VoltCommandInvoker::invokeAsync, this,
        std::move(command_name), std::move(param_callback),
        std::move(result_callback), true));
}

// private member function
void VoltCommandInvoker::invokeAsync(const std::string command_name,
    VoltParamCallback param_callback,
    VoltResultCallback result_callback, int retry)
{
    if (command_name == "") {
        return;
    }

    voltdb::Procedure procedure(command_name);

    VoltdbIOService::ptr v_ios =
        CommonObject::getInstance()->getVoltdbIOService();
    VoltdbConnection::ptr conn =
        v_ios->getVoltdbConnection();

    if (param_callback) {
        param_callback(procedure);
    }

    //
    conn->invoke(procedure, [this, command_name, param_callback,
        result_callback, retry](voltdb::InvocationResponse& res) {
            VoltdbResult v_ret;
            v_ret.res = &res;

            if (res.success()) {
                if (result_callback) {
                    result_callback(v_ret);
                }
            } else {
                if (res.statusCode() == -4 && retry > 0) {
                    retryQuery(std::move(command_name),
                        std::move(param_callback), 
                        std::move(result_callback), retry);
                } else {
                    if (result_callback) {
                        result_callback(v_ret);
                    }
                }
            }
        }, [this, command_name, param_callback, 
            result_callback, retry](std::exception& e) {

            if (retry > 0) {
                retryQuery(std::move(command_name),
                    std::move(param_callback), 
                    std::move(result_callback), retry);
            } else {
                // todo error event
                Logger::log("voltdb proc error");
            }
        });
}

void VoltCommandInvoker::retryQuery(const std::string command_name,
    VoltParamCallback param_callback,
    VoltResultCallback result_callback, int retry)
{
    if (retry > 0) {
        auto sche = CommonObject::getInstance()->getDelayScheduler();

        DelayedTask::ptr task = new DelayedTask;
        task->interval = RETRY_DELAY_MILLIS;
        task->max_loop = 1;
        task->func = std::bind(&VoltCommandInvoker::invoke, this,
            std::move(command_name), std::move(param_callback),
            std::move(result_callback), --retry);

        std::string name = "_retry_invoke" +
            std::to_string(reinterpret_cast<int64_t>(task.get()));

        sche->addTask(name, task);
    }
}

END_NS

#endif
