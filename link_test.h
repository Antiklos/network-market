#include <stdio.h>

#define LINK_INTERFACE_TEST_IDENTIFIER 0

#ifndef LINK_TEST_H

#define LINK_TEST_H

T_LINK_INTERFACE link_test_interface();

void link_test_init();

void send_request_test(struct interface_id_udp *interface_id, char *address);

void send_propose_test(struct interface_id_udp *interface_id, char *address, int64_t price, long int payment_advance, time_t time_expiration);

void send_accept_test(struct interface_id_udp *interface_id, char *address);

void send_reject_test(struct interface_id_udp *interface_id, char *address, int64_t price, long int payment_advance, time_t time_expiration);

void send_begin_test(struct interface_id_udp *interface_id, char *address);

void link_test_destroy();

#endif
