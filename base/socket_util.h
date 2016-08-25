/*************************************************************************
> File Name: socket_util.h
> Author:CaiYaodeng 
> Mail: 512324291@qq.com 
> Created Time: Fri 26 Jun 2015 09:25:52 PM
************************************************************************/
#ifndef _SCK_UTIL_H_
#define _SCK_UTIL_H_

#include <sys/types.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "errx.h"

#ifdef __cplusplus
extern "C"{
#endif

int set_nonblocking(int fd);
int set_blocking(int fd);

int read_timeout(int fd, unsigned int wait_seconds);
int write_timeout(int fd, unsigned int wait_seconds);
int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds);
int connect_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds);


ssize_t readn(int fd, void *buf, size_t count);
ssize_t writen(int fd, const void *buf, size_t count);
ssize_t recv_peek(int sockfd, void *buf, size_t len);
ssize_t readline(int sockfd, void *buf, size_t maxline);

#ifdef __cplusplus
}
#endif 

#endif /* _SYS_UTIL_H_ */
