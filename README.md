# uart-communication
# Question:
Imagine that you have a microcontroller that communicates to a generic system that may consist of several other boards via UART. 
How do you ensure that each message is properly sent and that its content is correct?
Now imagine that you receive a message and an interruption is triggered every time a new information is received. 
Inside every message you have a different command, and for every command you have different payloads, receiving positive and negative values.
Implement the interrupt and the parser functions in a generic way using the C language and share this code through a public repository on GitHub. 
Low-level methods can be abstracted. If you prefer, use frameworks like Arduino, ESP32 or even other market platforms in this low-level code abstraction.

# Answer:
To ensure that each message is properly sent and that its content is correct, we can use one error detection method like CRC (Cyclic Redundancy Check).
Since it has several boards, we will use one byte from the data to identify the board who is sending the message. We will consider the length 
of the message too, so the receiver can read the exaclty size of bits that was sent. The whole data sent and received by these boards will follow the pattern below:
![protoviz_2025-03-28_21-33-24](https://github.com/user-attachments/assets/3eed8084-2822-40a5-bec1-803d91abeb5c)

To receive messages using this protocol it was used a abstracted function to receive the bytes from UART. 
Then it was read the bytes according to the structure mentioned above. To store the message, was created a buffer.
This buffer will receive all the bytes until the end of the message or until the limit of the buffer (which will cancel the message).
Check the result on the code question_4.c.
