#pragma once
#include "IDD.h"
#include <thread>
#include "UDP_Socket.h"
class Component
{
protected:
	UDP_Socket socket;
	controlerType ctype;
	typeDevice device;
	int lastOne;//the last number of message

	//getting message and passing it on 
	void getMessage() {
		while (1) {
			Message* msg = socket.receive();
			if (msg->header.msgSync != MSG_SYNC)
				continue;
			//Control Message
			if (msg->header.Opcode != DATA_MSG_OPCODE) {
				ControlMessage ctrlMsg = msg->controlMessage;
				if (ctrlMsg.header.msgCounter == lastOne++) {//The messages came in sequence
					unsigned short newOpCode = opcodeByCtrl[getControlByOpcode[ctrlMsg.header.Opcode]];
					ctrlMsg.header.Opcode = newOpCode;
					ctrlMsg.Ack = '1';
				}
				else
					ctrlMsg.Ack = '0';
				socket.send(*msg, getPortManager[device]);
			}
			//Data Message
			else {
				if (device == TRANSMITTER) {
					socket.send(*msg, getPort[MODEM][RECEIVER]);
				}
				else {
					socket.send(*msg, getPortManager[RECEIVER]);
				}
			}
		}
	};

public:
	Component(typeDevice device, controlerType type) {
		this->ctype = type;
		this->device = device;
		this->socket.createBind(getPort[type][device]);
		this->lastOne = 0;
		thread t1(&Component::getMessage, this);
		t1.detach();
	}

};