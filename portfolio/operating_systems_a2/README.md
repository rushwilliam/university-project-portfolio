## William Rush
## Dr. A. Hamilton-Wright
## CIS3110-W25 Assignment 2
## February 24, 2025

This assignment was made using the starting code provided in the course directory.
* author: Hamilton-Wright, Andrew
* name: Token Ring Starter Program
* year: 2025
* source: Operating Systems Course Directory, School of Computer Science,
	University of Guelph

The basic semaphore and fork structure was taken from the examples in the course directory.
* author: Hamilton-Wright, Andrew
* name: Semaphore examples
* year: 2025
* source: Operating Systems Course Directory, School of Computer Science,
	University of Guelph

**Program Description**

The basic token ring structure is implemented using the code structure from the examples. This includes initializing shared data, semaphores, and creating token nodes. The examples also provided useful information about how to coordinates processes using semaphores, and I was soon able to pass a byte around using:
```send_byte(control, num, rcv_byte(control, num));```

The main logic of this program is contained within the ```TOKEN_FLAG``` case of the ```rcv_state``` switch statement, as this is when the majority of the decision making has to be made. My initial algorithm waited for each byte to travel the entire ring before sending a new one, for simplicity. Once messages were being sent and received successfully, I modified the code to send bytes faster. When a node sends a message, it first sends as many bytes as it can without causing deadlock, then it receives and discards a byte each time to make room for another, then it receives the remainder of the bytes. The token is passed on once all the bytes are sent, which means there may still be data on the ring when another node is sending. This won't cause an issue even if many messages are on the ring at the same time, as all messages will be in the order that they need to be received. That means after a message has been sent, the next byte received will always be the start of the same message, so there is no need to check the 'from' bit.