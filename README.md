![protoviz_2025-03-28_21-31-51](https://github.com/user-attachments/assets/92ba2035-1561-45a4-9d21-1c54aca19792)# uart-communication
# Question:
Imagine that you have a microcontroller that communicates to a generic system that may consist of several other boards via UART. 
How do you ensure that each message is properly sent and that its content is correct?
Now imagine that you receive a message and an interruption is triggered every time a new information is received. 
Inside every message you have a different command, and for every command you have different payloads, receiving positive and negative values.
Implement the interrupt and the parser functions in a generic way using the C language and share this code through a public repository on GitHub. 
Low-level methods can be abstracted. If you prefer, use frameworks like Arduino, ESP32 or even other market platforms in this low-level code abstraction.

# Answer:
To ensure that each message is properly sent and that its content is correct, we can use one error detection method like CRC (Cyclic Redundancy Check).
Since it has several boards, we will use two bytes from the data to identify the board who is sending the message. We will consider the length 
of the message too, so the receiver can read the exaclty size of bits that was sent. The whole data sent and received by these boards will follow the pattern below:
![Uploadi
<svg viewBox="0 0 450 180" xmlns="http://www.w3.org/2000/svg" style="background-color:#FFFFFF">
  <polygon points="50,50 50,130 100,130 100,50" fill="#FFFFFF" style="stroke:#000000;stroke-width:2" />
  <polygon points="100,50 100,130 200,130 200,50" fill="#78B4F0" style="stroke:#000000;stroke-width:2" />
  <polygon points="200,50 200,130 250,130 250,50" fill="#1AA866" style="stroke:#000000;stroke-width:2" />
  <polygon points="250,50 250,130 320,130 370,50" fill="#D07C43" style="stroke:#000000;stroke-width:2" />
  <polygon points="380,50 330,130 400,130 400,50" fill="#D07C43" style="stroke:#000000;stroke-width:2" />
  <line x1="150" y1="50" x2="150" y2="70" style="stroke:#000000;stroke-width:2" />
  <line x1="150" y1="110" x2="150" y2="130" style="stroke:#000000;stroke-width:2" />
  <text x="75" y="90" fill="#000000" dominant-baseline="middle" text-anchor="middle" font-size="16">ID</text>
  <text x="150" y="90" fill="#000000" dominant-baseline="middle" text-anchor="middle" font-size="16">CRC</text>
  <text x="225" y="90" fill="#000000" dominant-baseline="middle" text-anchor="middle" font-size="16">Length</text>
  <text x="297.5" y="90" fill="#000000" dominant-baseline="middle" text-anchor="middle" font-size="16">Message</text>
  <text x="75" y="40" fill="#000000" dominant-baseline="auto" text-anchor="middle" font-size="16">0</text>
  <text x="125" y="40" fill="#000000" dominant-baseline="auto" text-anchor="middle" font-size="16">1</text>
  <text x="225" y="40" fill="#000000" dominant-baseline="auto" text-anchor="middle" font-size="16">3</text>
  <text x="275" y="40" fill="#000000" dominant-baseline="auto" text-anchor="middle" font-size="16">4</text>
  <text x="325" y="150" fill="#000000" dominant-baseline="hanging" text-anchor="middle" font-size="16">Length</text>
  <text x="225" y="150" fill="#000000" dominant-baseline="hanging" text-anchor="middle" font-size="16">1</text>
  <text x="150" y="150" fill="#000000" dominant-baseline="hanging" text-anchor="middle" font-size="16">2</text>
  <text x="75" y="150" fill="#000000" dominant-baseline="hanging" text-anchor="middle" font-size="16">1</text>
  <line x1="250" y1="145" x2="400" y2="145" style="stroke:#000000;stroke-width:2" />
  <line x1="250" y1="140" x2="250" y2="150" style="stroke:#000000;stroke-width:2" />
  <line x1="400" y1="140" x2="400" y2="150" style="stroke:#000000;stroke-width:2" />
  <line x1="200" y1="145" x2="250" y2="145" style="stroke:#000000;stroke-width:2" />
  <line x1="200" y1="140" x2="200" y2="150" style="stroke:#000000;stroke-width:2" />
  <line x1="250" y1="140" x2="250" y2="150" style="stroke:#000000;stroke-width:2" />
  <line x1="100" y1="145" x2="200" y2="145" style="stroke:#000000;stroke-width:2" />
  <line x1="100" y1="140" x2="100" y2="150" style="stroke:#000000;stroke-width:2" />
  <line x1="200" y1="140" x2="200" y2="150" style="stroke:#000000;stroke-width:2" />
  <line x1="50" y1="145" x2="100" y2="145" style="stroke:#000000;stroke-width:2" />
  <line x1="50" y1="140" x2="50" y2="150" style="stroke:#000000;stroke-width:2" />
  <line x1="100" y1="140" x2="100" y2="150" style="stroke:#000000;stroke-width:2" />
  <polygon points="40,90 30,100 30,80" fill="#000000" />
</svg>ng protoviz_2025-03-28_21-31-51.svg…]()


To receive messages using this protocol it was used a abstracted function to receive the bytes from UART. 
Then it was read the bytes according to the structure mentioned above. To store the message, was created a buffer.
This buffer will receive all the bytes until the end of the message or until the limit of the buffer (which will cancel the message).
Check the result on the code question_4.c.
