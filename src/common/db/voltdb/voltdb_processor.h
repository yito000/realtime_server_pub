#ifdef USE_VOLTDB

#ifndef VOLTDB_PROCESSOR_H
#define VOLTDB_PROCESSOR_H

#include "fw_env.h"
#include <boost/asio.hpp>

#include "voltdb_connection.h"

BEGIN_NS

class VoltdbIOService;

class VoltdbProcessor : public SmartPtr<VoltdbProcessor>
{
    friend class VoltdbIOService;

public:
    typedef boost::intrusive_ptr<VoltdbProcessor> ptr;

    VoltdbProcessor(VoltdbConnection::ptr v_conn);
    void run();

private:
    boost::asio::io_service io_service;
    boost::asio::io_service::work work;

    VoltdbConnection::ptr voltdb_conn;
};

END_NS

#endif

#endif
