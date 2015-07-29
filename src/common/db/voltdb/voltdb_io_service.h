#ifndef VOLTDB_IO_SERVICE_H
#define VOLTDB_IO_SERVICE_H

#include "smart_ptr.hpp"
#include <boost/asio.hpp>

#include "voltdb_connection.h"

class VoltdbProcessor;

class VoltdbIOService : public SmartPtr<VoltdbIOService>
{
public:
    typedef boost::intrusive_ptr<VoltdbIOService> ptr;

    VoltdbIOService(VoltdbProcessor& v_proc);
    boost::asio::io_service& getIOService();

    VoltdbConnection::ptr getVoltdbConnection();

private:
    boost::asio::io_service& io_service;
    VoltdbConnection::ptr voltdb_conn;
};

#endif

