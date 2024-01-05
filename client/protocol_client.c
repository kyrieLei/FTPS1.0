#include "protocol_client.h"
#include <sys/wait.h>



#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAXLINE 1024
const char *USER_PASS[2][2]={{"l","l"}, {"z","z"}};
uint16_t CMD_PORT = 9876;
const int QueueMax = 1024;
































