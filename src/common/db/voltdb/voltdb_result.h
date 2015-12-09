#ifdef USE_VOLTDB

#ifndef VOLTDB_RESULT_H
#define VOLTDB_RESULT_H

#include "voltdb.h"

struct VoltdbResult
{
    voltdb::InvocationResponse* res;

    VoltdbResult()
    {
        res = NULL;
    }
};

#endif

#endif
