#pragma once
#include "../../Common/IDD.h"
#include "../../Common/UDP_Socket.h"
#include <thread>

class MsgGenerator {
	int runningTime;
	int numMsgSent;
	int numMsgReceived;
	UDP_Socket socketToSend;
	UDP_Socket socketToReceive;

	//sending eacm second a Data Massage
	void sendDataMsg() {
		Header h;
		DataMessage dataMsg;
		while (1) {
			h.msgSync = MSG_SYNC;
			h.msgCounter = this->numMsgSent++;
			h.Opcode = DATA_MSG_OPCODE;
			dataMsg.header = h;
			Message msg;
			msg.dataMessage = dataMsg;
			this->socketToSend.send(msg, getPortManager[TRANSMITTER]);
			if (this->numMsgSent == this->runningTime) {
				Sleep(1000);
				printf("%d messages sent, %d messages received. The duration of the test was %d seconds.", this->numMsgSent, this->numMsgReceived, this->runningTime);
				exit(0);
			}
			this_thread::sleep_for(chrono::milliseconds(1000));
		}
	}

	//getting the Data Message
	void receiveDataMsg() {
		while (1) {
			Message* answer = this->socketToReceive.receive();
			printf("[  dataMsgNum:: %d  ]\n", (int)answer->header.msgCounter);
			unsigned short opCode = answer->header.Opcode;
			if (opCode == DATA_MSG_OPCODE) {
				this->numMsgReceived++;
			}
			else {
				printf("Error Message\n");
			}
		}
	}

public:
	MsgGenerator(int runTime) {
		this->runningTime = runTime;
		this->numMsgSent = 0;
		this->numMsgReceived = 0;
		this->socketToReceive.createBind(MESSEGE_GENERATOR);
		thread sendMsg(&MsgGenerator::sendDataMsg, this);
		sendMsg.detach();
		thread receiveMsg(&MsgGenerator::receiveDataMsg, this);
		receiveMsg.detach();
	}
};

