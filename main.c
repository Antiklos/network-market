#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <glib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>

#include "main.h"
#include "link_test.c"
#include "network_test.c"
#include "payment_test.c"

static T_CONFIG read_config()
{
  GKeyFile* gkf = g_key_file_new();
  GError* error = NULL;

  char* filename = (gchar*)malloc(256 * sizeof(gchar));
  strcpy(filename, "net.conf");
  const gchar* conf_file = filename;

  if (!g_key_file_load_from_file(gkf, conf_file, G_KEY_FILE_NONE, &error)){
    fprintf (stderr, "Could not read config file %s\n", conf_file);
  }

  gchar *link_value = g_key_file_get_value(gkf, "Group", "LINK_INTERFACE", NULL);
  gchar *network_value = g_key_file_get_value(gkf, "Group", "NETWORK_INTERFACE", NULL);
  gchar *payment_value = g_key_file_get_value(gkf, "Group", "PAYMENT_INTERFACE", NULL);
  gchar *user_value = g_key_file_get_value(gkf, "Group", "USER_INTERFACE", NULL);

  g_key_file_free(gkf);
  free(filename);

  T_CONFIG config;
  config.link_interface = (int)strtol(link_value,NULL,10);
  config.network_interface = (int)strtol(network_value,NULL,10);
  config.payment_interface = (int)strtol(payment_value,NULL,10);
  return config;
}

static bool input_loop()
{
  bool loop = true;
  int input;
  scanf("%d", &input);

  switch(input) {
    case INPUT_RECEIVE_REQUEST:
      receive_request();
    break;
    case INPUT_RECEIVE_PROPOSE:
      receive_propose();
    break;
    case INPUT_RECEIVE_ACCEPT:
      receive_accept();
    break;
    case INPUT_RECEIVE_REJECT:
      receive_reject();
    break;
    case INPUT_RECEIVE_BEGIN:
      receive_begin();
    break;
    case INPUT_RECEIVE_STOP:
      receive_stop();
    break;
    case INPUT_COUNT_PACKETS:
      count_packets(5);
    break;
    case INPUT_RECEIVE_PAYMENT:
      receive_payment(10);
    break;
    default:
      loop = false;
      break;
  }

  return loop;
}

main(int args, char *argv[])
{

//Create interface arrays
T_LINK_INTERFACE link_interfaces[LINK_INTERFACES_NUMBER];
T_NETWORK_INTERFACE network_interfaces[NETWORK_INTERFACES_NUMBER];
T_PAYMENT_INTERFACE payment_interfaces[PAYMENT_INTERFACES_NUMBER];

//Populate interface arrays
link_interfaces[LINK_INTERFACE_TEST_IDENTIFIER] = link_test_interface();
network_interfaces[NETWORK_INTERFACE_TEST_IDENTIFIER] = network_test_interface();
payment_interfaces[PAYMENT_INTERFACE_TEST_IDENTIFIER] = payment_test_interface();

//Read config file
T_CONFIG config = read_config();

//Get interfaces from config file
T_LINK_INTERFACE link_interface = link_interfaces[config.link_interface];
T_NETWORK_INTERFACE network_interface = network_interfaces[config.network_interface];
T_PAYMENT_INTERFACE payment_interface = payment_interfaces[config.payment_interface];

/* Our process ID and Session ID */
pid_t pid, sid;

/* Fork off the parent process */
pid = fork();
if (pid < 0) {
        exit(EXIT_FAILURE);
}
/* If we got a good PID, then
   we can exit the parent process. */
if (pid > 0) {
        exit(EXIT_SUCCESS);
}

/* Change the file mode mask */
umask(0);
        
/* Open any logs here */
        
/* Create a new SID for the child process */
sid = setsid();
if (sid < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
}

/* Change the current working directory */
if ((chdir("/")) < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
}

//Set up socket for communication
int sockfd, newsockfd;
socklen_t clilen;
char buffer[256];
struct sockaddr_un serv_addr, cli_addr;
int n;
sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
if (sockfd < 0) { 
  error("ERROR opening socket");
}
serv_addr.sun_family = AF_UNIX;
//TODO: For some reason it's truncating the socket file by two characters
strncpy(serv_addr.sun_path, SOCK_PATH,
            sizeof(serv_addr.sun_path) - 1);
int result = bind(sockfd, (struct sockaddr *) &serv_addr, strlen(SOCK_PATH));
if (result < 0) {
      printf("Binding error%i\n",result);
}
listen(sockfd,2);
clilen = sizeof(cli_addr);
newsockfd = accept(sockfd, 
         (struct sockaddr *) &cli_addr, 
         &clilen);
if (newsockfd < 0) error("ERROR on accept");
bzero(buffer,256);
n = read(newsockfd,buffer,255);
if (n < 0) error("ERROR reading from socket");
n = write(newsockfd,"I got your message",18);
if (n < 0) error("ERROR writing to socket");
close(newsockfd);
close(sockfd);

/* Close out the standard file descriptors */
//close(STDIN_FILENO);
//close(STDOUT_FILENO);
//close(STDERR_FILENO);

/* Daemon-specific initialization goes here */

/* The Big Loop */
while (1) {
  sleep(30);
}

exit(EXIT_SUCCESS);
}

// Link Interface

void receive_request() {
  printf("Executing receive_request\n");
}

void receive_propose() {
  printf("Executing receive_propose\n");
}

void receive_accept() {
  printf("Executing receive_accept\n");
}

void receive_reject() {
  printf("Executing receive_reject\n");
}

void receive_begin() {
  printf("Executing receive_begin\n");
}

void receive_stop() {
  printf("Executing receive_stop\n");
}

// Network Interface

void count_packets(int count) {
  printf("Executing count_packets with %d packets having been counted.\n", count);
}


// Payment Interface

void receive_payment(int amount) {
  printf("Executing receive_payment with %d having been received.\n", amount);
}

// User Interface


