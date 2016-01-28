#ifndef SSL_MT_H
#define SSL_MT_H

#include "fw_env.h"

BEGIN_NS

int ssl_thread_setup();
int ssl_thread_cleanup();

END_NS

#endif
