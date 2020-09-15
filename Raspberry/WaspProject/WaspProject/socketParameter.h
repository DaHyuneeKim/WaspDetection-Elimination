#ifndef _CONTROLSERVO_H_
#define _CONTROLSERVO_H_
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "detectState.h"
#include "Threadfunction.h"
#include "socketParameter.h"
#include <queue>
#define PORT 55555
#define PORT_VIDEO 56666
#define DESTINATION_IP  "192.168.0.17"
extern int sock;
extern struct sockaddr_in serv_addr;
extern bool isSocketConnect;
extern bool isconnectedtoserver;
#endif
