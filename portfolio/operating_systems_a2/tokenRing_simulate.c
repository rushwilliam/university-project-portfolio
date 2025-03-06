/*
 * William Rush
 * Dr. A. Hamilton-Wright
 * CIS3110-W25 Assignment 2
 * February 24, 2025
 * 
 * This assignment was made using the starting code provided in the course directory.
 * * author: Hamilton-Wright, Andrew
 * * name: Token Ring Starter Program
 * * year: 2025
 * * source: Operating Systems Course Directory, School of Computer Science,
 * 	University of Guelph
 * 
 * The basic semaphore and fork structure was taken from the examples in the course directory.
 * * author: Hamilton-Wright, Andrew
 * * name: Semaphore examples
 * * year: 2025
 * * source: Operating Systems Course Directory, School of Computer Science,
 * 	University of Guelph
 * 
 * The program simulates a Token Ring LAN by forking off a process
 * for each LAN node, that communicate via shared memory, instead
 * of network cables. To keep the implementation simple, it jiggles
 * out bytes instead of bits.
 *
 * It keeps a count of packets sent and received for each node.
 */
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "tokenRing.h"

#define NODE_DATA control->shared_ptr->node[num]

/*
 * This function is the body of a child process emulating a node.
 */
void
token_node(control, num)
	struct TokenRingData *control;
	int num;
{
	// This implementation does not use the "sending" flag, instead for loops are used within the rcv_state TOKEN_FLAG case. This is to make it
	// easier to control the aggressive send algorithm.
	int rcv_state = TOKEN_FLAG, not_done = 1, /* sending = 0, */ len;
	unsigned char byte;

	/*
	 * If this is node #0, start the ball rolling by creating the
	 * token.
	 */
	if (num == 0) {
		// Tokens are hardcoded as 1 followed by three zeros.
		send_byte(control, num, 1);
		for (int i = 0; i < 3; i++) {
			send_byte(control, num, 0);
		}
	}

	/*
	 * Loop around processing data, until done.
	 */
	while (not_done) {
		byte = rcv_byte(control, num);
		/*
		* Handle the byte, based upon current state.
		*/
		switch (rcv_state) {
		case TOKEN_FLAG:
			if (byte) {
				// Take the rest of the token
				for (int i = 0; i < 3; i++) {
					rcv_byte(control, num);
				}
				// If there is a message to send
				if (NODE_DATA.to_send.length > 0) {
					control->snd_state = TOKEN_FLAG;

					// This is the "aggressive send" algorithm, ie. it sends bytes faster than the "send one, receive one" approach, and it sends the
					// token sooner.
					// Assuming the number of tokens is infinite, immediately after sending a message, the next byte received will always be the token
					// flag of the message we sent. This is because all other messages in between the message and our node must have been sent before
					// our message (because they would have needed the token), and therefore will be discarded by the sending node before our message
					// completes a full loop. This means we can say with certainty if a
					// message is ours and should be discard without reading the 'from' byte. And, if we know the length of this message, then we can
					// simply receive exactly that many bytes to discard the message.
					//
					// However, this approach will create deadlock if the message length is higher than a certain limit. In this case, first that many
					// bytes are sent, then the remainder of the bytes are sent with a rcv_byte call in between, then the remainder of the bytes are
					// received. This means that the token can be sent before all of the bytes have been received.

					// Total number of bytes in the message
					int message_length = NODE_DATA.to_send.length + 4;
					// Number of bytes that must be sent with a receive in between
					int remainder = message_length - (N_NODES * 2 - 3);
					if (remainder < 0) {
						remainder = 0;
					}
					message_length -= remainder;
					// Send initial bytes
					for (int i = 0; i < message_length; i++) {
						send_pkt(control, num);
					}
					// Send-receive
					for (int i = 0; i < remainder; i++) {
						send_pkt(control, num);
						rcv_byte(control, num);
					}
					// Send the token as soon as possible
					send_byte(control, num, byte);
					for (int i = 0; i < 3; i++) {
						send_byte(control, num, 0);
					}
					// Discard the remainder of the message
					for (int i = 0; i < message_length; i++) {
						rcv_byte(control, num);
					}
					// Update shared data
					NODE_DATA.sent += 1;
					NODE_DATA.to_send.length = 0;
					// Signal that this node is available to receive another message
					SIGNAL_SEM(control, TO_SEND(num));
				} else {
					// No message to send, pass on the token
					send_byte(control, num, byte);
					for (int i = 0; i < 3; i++) {
						send_byte(control, num, 0);
					}
				}
				// The terminate flag is checked after the token is received and passed on.
				// One the terminate flags are set, all messages will have been sent and received (since cleanupSystem waits for all TO_SEND
				// semaphores), so the token will be floating around. This means we can use the token to safely terminate each node, one at a time.
				// Each node will receive the token, pass it on, and then terminate, and the next node will be triggered to do the same.
				if (NODE_DATA.terminate) {
					// Create three excess EMPTY signals in case we are the first node to terminate, so that the final node can dump the token before terminating
					for (int i = 0; i < 3; i++) {
						SIGNAL_SEM(control, EMPTY(num));
					}
					not_done = 0;
				}
			} else {
				send_byte(control, num, byte);
				rcv_state = TO;
			}
			break;

		case TO:
			// Check if the message is addressed to this node
			if (byte == num) {
				NODE_DATA.received += 1;
			}
			send_byte(control, num, byte);
			rcv_state = FROM;
			break;

		case FROM:
			send_byte(control, num, byte);
			rcv_state = LEN;
			break;

		case LEN:
			len = (int)byte;
			send_byte(control, num, byte);
			// Skip receiving data if len is 0
			rcv_state = len == 0 ? TOKEN_FLAG : DATA;
			break;

		case DATA:
			send_byte(control, num, byte);
			len--;
			// Check if all data has been sent
			if (len == 0) {
				rcv_state = TOKEN_FLAG;
			}
			break;
		};
	}
}

/*
 * This function sends a data packet followed by the token, one byte each
 * time it is called.
 */
void
send_pkt(control, num)
	struct TokenRingData *control;
	int num;
{
	static int sndpos, sndlen;
	char length;
	switch (control->snd_state) {
	case TOKEN_FLAG:
		send_byte(control, num, NODE_DATA.to_send.token_flag);
		control->snd_state = TO;
		break;

	case TO:
		send_byte(control, num, NODE_DATA.to_send.to);
		control->snd_state = FROM;
		break;

	case FROM:
		send_byte(control, num, NODE_DATA.to_send.from);
		control->snd_state = LEN;
		break;

	case LEN:
		length = NODE_DATA.to_send.length;
		send_byte(control, num, length);
		// Skip sending data is length is 0
		control->snd_state = length == 0 ? DONE : DATA;
		// Initialize send data
		sndpos = 0;
		sndlen = (int)length;
		break;

	case DATA:
		send_byte(control, num, NODE_DATA.to_send.data[sndpos]);
		sndpos++;
		// Check if all data has been sent
		if (sndpos == sndlen) {
			control->snd_state = DONE;
		}
		break;

	case DONE:
		break;
	};
}

/*
 * Send a byte to the next node on the ring.
 */
void
send_byte(control, num, byte)
	struct TokenRingData *control;
	int num;
	unsigned byte;
{
	int nextNode = (num + 1) % N_NODES;
	WAIT_SEM(control, EMPTY(nextNode));
	WAIT_SEM(control, CRIT);
	control->shared_ptr->node[nextNode].data_xfer = (unsigned char)byte;
	SIGNAL_SEM(control, CRIT);
	SIGNAL_SEM(control, FILLED(nextNode));
}

/*
 * Receive a byte for this node.
 */
unsigned char
rcv_byte(control, num)
	struct TokenRingData *control;
	int num;
{
	unsigned char byte;
	WAIT_SEM(control, FILLED(num));
	byte = NODE_DATA.data_xfer;
	SIGNAL_SEM(control, EMPTY(num));
	return byte;
}

