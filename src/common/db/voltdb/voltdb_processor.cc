#include "voltdb_processor.h"

VoltdbProcessor::VoltdbProcessor(VoltdbConnection::ptr v_conn) : 
    work(io_service), voltdb_conn(v_conn)
{
    //
}
 
void VoltdbProcessor::run()
{
    voltdb_conn->watchConnection();
    io_service.poll();

    voltdb_conn->drain();
}
