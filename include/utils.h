/*                                                                                              
 * utils.h 
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <sys/socket.h>

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define htonll(x) ((1==htonl(1)) ? \
                   (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? \
                   (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

void get_readable_timestamp(char *buffer);

#define log_error(format, ...)  \
        {               \
            char __buffer[27]; \
            get_readable_timestamp(__buffer); \
            fprintf(stderr, "(%s)(ERROR) " format "\n", __buffer, ## __VA_ARGS__); \
        }

#define log_info(format, ...)   \
        {               \
            char __buffer[27]; \
            get_readable_timestamp(__buffer); \
            fprintf(stdout, "(%s)(INFO) " format "\n", __buffer, ## __VA_ARGS__); \
        }

void dump_buffer(uint8_t *data, int len);

int form_sockaddr(const char *addr,
                  int port,
                  struct sockaddr_storage *saddr);

int
sockaddr_to_string(struct sockaddr_storage *addr,
                   char *buffer,
                   size_t buffer_size);

void
get_peer_name(int fd, char *peername, size_t buflen);

#endif
