/**
 * gsocket.h
 * 
 * Created by Kainet
 *
 * Non-blocking connection to google speech api
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/**
 * Recieve sockaddr IPv4 or IPv6
 */
void *kaitalk_in_addr(struct sockaddr *sa);

/**
 * Establish non-blocking socket connection to google
 */
int kaitalk_socket_connect();

/**
 * Send buffer to socket
 */
int kaitalk_socket_send(int sockfd, char** buffer, int buffer_len);

/**
 * Read data from socket in non-blocking mode
 */
int kaitalk_socket_read(int sockfd, char** buffer);