/**
 * gsocket.h
 * 
 * Created by Kainet
 *
 * Non-blocking connection to google speech api
 * also controlling HDL udp socket
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

/**
 * Send hdl command via udp socket
 */
int kaitalk_create_hdl_send_cmd(char *addr, int addr_port, unsigned char **data, int data_len);