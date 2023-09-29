#!/usr/bin/python3

import string    
import random # define the random module 
import argparse
 
# Initialize parser
parser = argparse.ArgumentParser()
 
# Adding arguments
parser.add_argument("-Pnr", "--PacketNumber", help = "Total Number of Network packets")
parser.add_argument("-PMaxSize", "--PacketMaximumSize", help = "Maximum size of Network packets")
parser.add_argument("-Bo", "--BufferOverflow", help = "Number of packets with buffer overflow")
parser.add_argument("-BoMaxSize", "--BoMaximumSize", help = "Maximum size of Network packet with buffer overflow")
# Read arguments from command line
args = parser.parse_args()
 
if args.PacketNumber:
    print("Number of Network Packets: % s" % args.PacketNumber + " maximum size %s" % args.PacketMaximumSize)
if args.BufferOverflow:
    print("Number of Buffer overflow Packets: % s" % args.BufferOverflow + " maximum size %s" % args.BoMaximumSize)

legPacket=int(args.PacketNumber)
MaxlegPacket = int(args.PacketMaximumSize)
BoPacket=int(args.BufferOverflow)
MaxBoPacket = int(args.BoMaximumSize)

data=[]
header="#ifndef __PACKET_GEN\n#define __PACKET_GEN\n"
buffer=" \n static const char Packet[]="
header+=buffer
data.append(header)
s=""
# generate legitemate packets
for i in range(0,legPacket):
    size=random.randint(5,MaxlegPacket)
    s=str(hex(i+1)).replace('0','\\',1)
    if(i>159):
        b = b"\u{:04x}"
        s = b.decode()
        s = s.format(i+1)
    payload="".join(random.choices(string.ascii_uppercase, k = size))
    payload="\"!"+ "\"\"" + str(hex(size)).replace('0','\\',1) + "\"\"" + payload + "\"\"" + s +  "\"\"#\"" + "\n"
    #payload="\"\\x02"+ "\"\"" + str(hex(size)).replace('0','\\',1) + "\"\"" +payload+"\\x04\"" + "\"" + s + "\"\n"
    data.append(payload)
    #(str(hex(i+1)).replace('0x','\\u',2))

# packet huges packets == buffer overflow
for i in range(0,BoPacket):
    size=random.randint(MaxlegPacket+3,MaxBoPacket)
    payload = "".join(random.choices(string.ascii_uppercase, k = size))
    payload="\"\\x02"+ "\"\"" + str(hex(size)).replace('0','\\',1) + "\"\"" + "\"\"" + str(hex(i)).replace('x','u',1) + "\"\""+ payload+"\\x04\"\n" 
    data.append(payload)	

bot=";\n#endif"
data.append(bot)
with open("input_packet.h", "w") as fp:
	fp.writelines(data)


