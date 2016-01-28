#ifdef USE_VOLTDB

#include "voltdb_io_service.h"

#include "voltdb_processor.h"

BEGIN_NS

VoltdbIOService::VoltdbIOService(VoltdbProcessor& v_proc) : 
    io_service(v_proc.io_service), voltdb_conn(v_proc.voltdb_conn)
{
    //
}

boost::asio::io_service& VoltdbIOService::getIOService()
{
    return io_service;
}

VoltdbConnection::ptr VoltdbIOService::getVoltdbConnection()
{
    return voltdb_conn;
}

END_NS

#endif
