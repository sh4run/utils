#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#include "utils.h"

void dump_buffer(uint8_t *data, int len)
{
    int i, j;
    printf("\nlen = %d\n", len);
    for (i = 0, j = 0; i < len; i++, j++) {
        printf("%02x ", data[i]);
        if ((j & 0x1f) == 0x1f) {
            printf("\n");
        }
    }
    printf("\n");
}

void get_readable_timestamp(char *buffer)
{
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm);

    sprintf(buffer + strlen(buffer), ".%06ld", tv.tv_usec);
}

int form_sockaddr(const char *addr, 
                  int port, 
                  struct sockaddr_storage *saddr)
{
    int family;
    char buf[sizeof(struct in6_addr)];
    int rtn = 0;

    memset(saddr, 0, sizeof(struct sockaddr_storage));
    if (inet_pton(AF_INET, addr, buf) == 1) {
        family = AF_INET;
        struct sockaddr_in *in4 = (struct sockaddr_in *)saddr;
        in4->sin_family = (sa_family_t)family;
        in4->sin_port = (in_port_t)htons(port);
        in4->sin_addr.s_addr = *(in_addr_t*)buf;
    } else if (inet_pton(AF_INET6, addr, buf) == 1) {
        family = AF_INET6;
        struct sockaddr_in6 *in6 = (struct sockaddr_in6 *)saddr;
        in6->sin6_family = (sa_family_t)family;
        in6->sin6_port = (in_port_t)htons(port);
        memcpy(in6->sin6_addr.s6_addr, buf, sizeof(struct in6_addr));
    } else {
        rtn = -1;
    }
    return rtn;
}

// Function to convert sockaddr_storage to string
int
sockaddr_to_string(struct sockaddr_storage *addr, 
                   char *buffer, 
                   size_t buffer_size) 
{
    int rtn = -1;

    if (!buffer || !addr) {
        return rtn;
    }

    buffer[0] = '0';

    // Check the address family
    if (addr->ss_family == AF_INET) {
        struct sockaddr_in *ipv4_addr = (struct sockaddr_in *)addr;
        if (inet_ntop(AF_INET, &ipv4_addr->sin_addr, 
                      buffer, buffer_size)) { 
            snprintf(buffer + strlen(buffer), 
                     buffer_size - strlen(buffer), 
                     ":%d", ntohs(ipv4_addr->sin_port));
            rtn = 0;
        }
    } else if (addr->ss_family == AF_INET6) {
        struct sockaddr_in6 *ipv6_addr = (struct sockaddr_in6 *)addr;
        if (inet_ntop(AF_INET6, &ipv6_addr->sin6_addr, 
                      buffer, buffer_size)) {
            snprintf(buffer + strlen(buffer), 
                     buffer_size - strlen(buffer), 
                     ":%d", ntohs(ipv6_addr->sin6_port));
            rtn = 0;
        }
    }
    return rtn;
}

void
get_peer_name(int fd, char *peername, size_t buflen)
{
    struct sockaddr_storage addr;
    socklen_t len = sizeof(struct sockaddr_storage);
    int err = getpeername(fd, (struct sockaddr *)&addr, &len);
    if (err == 0) {
        err = sockaddr_to_string(&addr, peername, buflen);
    }
    if (err) {
        sprintf(peername, "unknown(%d)", errno);
    }
}
