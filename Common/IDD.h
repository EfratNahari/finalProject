#pragma once

#define DATA_MSG_OPCODE 0xD001 

//OpCode For Control Messages
#define MANAGER_TO_AMPLIFIER_CONTROL_MSG_OPCODE  0xA002
#define AMPLIFIER_TO_MANAGER_CONTROL_MSG_OPCODE  0xA003


#define MANAGER_TO_MODEM_CONTROL_MSG_OPCODE 0xA004
#define MODEM_TO_MANAGER_CONTROL_MSG_OPCODE 0xA005

#define MANAGER_TO_ANTENNA_CONTROL_MSG_OPCODE 0xA006
#define ANTENNA_TO_MANAGER_CONTROL_MSG_OPCODE 0xA007

//MSG_SYNC and MSG_ERR to check validation
#define MSG_SYNC 0xAA55
#define MSG_ERR 0xFFFF

//Time to check returning messages
#define ON_TIME 5

//sockets
#define MESSEGE_GENERATOR 8787

typedef struct {
	unsigned short msgSync;
	unsigned short Opcode;
	unsigned int msgCounter;
} Header;

typedef struct {
	Header header;
}DataMessage;

typedef struct {
	Header header;
	char Ack;
}ControlMessage;

typedef union {
	Header header;
	ControlMessage controlMessage;
	DataMessage dataMessage;
}Message;

typedef enum {
	TRANSMITTER,
	RECEIVER
}typeDevice;

typedef enum {
	ANTENA,
	MODEM,
	AMPLIFIER
}controlerType;

#include <map>
#include<iostream>
#include <vector>
using namespace std;

//put opcode in msg by control
static map<controlerType, unsigned short> opcodeByCtrl{ {ANTENA, MANAGER_TO_ANTENNA_CONTROL_MSG_OPCODE},{AMPLIFIER, MANAGER_TO_AMPLIFIER_CONTROL_MSG_OPCODE},{MODEM, MANAGER_TO_MODEM_CONTROL_MSG_OPCODE} };

//port of cntrollers in deveces
static map<controlerType, vector<unsigned short>> getPort{ {ANTENA,{8000,8001}} ,{AMPLIFIER,{8002,8652}},{MODEM,{8003,8004} } ,{MODEM,{8005,8006}} };

//port of manager
static map<typeDevice, unsigned short> getPortManager{ {TRANSMITTER,8080} ,{RECEIVER,8092 } };

//get the control by opcode 
static map<unsigned short, controlerType> getControlByOpcode{ {MANAGER_TO_ANTENNA_CONTROL_MSG_OPCODE, ANTENA },{MANAGER_TO_AMPLIFIER_CONTROL_MSG_OPCODE, AMPLIFIER },{MANAGER_TO_MODEM_CONTROL_MSG_OPCODE, MODEM} };

class IDD
{
};





