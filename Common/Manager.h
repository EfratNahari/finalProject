#pragma once
#include<iostream>
#include<thread>
#include "IDD.h"
#include <vector>
#include<list>
#include <mutex>
#include "UDP_Socket.h"
using namespace std;

class Manager
{
	typeDevice device;
	vector<controlerType> controllers;
	UDP_Socket socketToSend;
	UDP_Socket socketToReceive;
	unsigned int counter;
	int numOfControllers;
	Header** checkTimeMsg;
	mutex lock1;

	//create control message every second and send to controllers
	void createMsgAndSend() {
		Header h;
		ControlMessage ctrlMsg;
		while (1) {
			h.msgSync = MSG_SYNC;
			h.msgCounter = this->counter;
			ctrlMsg.header = h;
			ctrlMsg.Ack = '1';
			for (int i = 0; i < numOfControllers; i++)
			{
				controlerType ctype = controllers[i];
				ctrlMsg.header.Opcode = opcodeByCtrl[ctype];
				Message m;
				m.controlMessage = ctrlMsg;
				this->socketToSend.send(m, getPort[ctype][device]);
				lock1.lock();
				this->checkTimeMsg[counter % ON_TIME][i] = h;
				lock1.unlock();

				//5 seconds after sending check the message and delete her if it didn't come back
				if (this->counter > ON_TIME) {
					lock1.lock();
					if (checkTimeMsg[(this->counter - ON_TIME + 1) % ON_TIME][i].msgSync == MSG_SYNC) {
						printf("the %d control doesn't work well from %d minute.\n", getControlByOpcode[checkTimeMsg[(this->counter - 5) % 5][i].Opcode], checkTimeMsg[(this->counter - 5) % 5][i].msgCounter);
						checkTimeMsg[(this->counter - ON_TIME + 1) % ON_TIME][i].msgSync = MSG_ERR;
					}
					lock1.unlock();
				}
			}
			this->counter++;
			this_thread::sleep_for(chrono::milliseconds(1000));
		}
	}
	//get messages and check type(Message/Control) and validation
	void receiveMsg() {
		while (1) {
			Message* answer = this->socketToReceive.receive();
			if (answer->header.msgSync != MSG_SYNC)
				continue;
			//Control Message
			if (answer->header.Opcode != DATA_MSG_OPCODE) {
				//came on time (in 5 seconds) 
				if (this->checkTimeMsg[(answer->header.msgCounter) % ON_TIME][getControlByOpcode[answer->header.Opcode]].msgSync != MSG_ERR) {
					ControlMessage* ctrlMsg = &answer->controlMessage;
					printf("[  counter: %d,  opcode: %x,  Ack: %c  ]\n", ctrlMsg->header.msgCounter, ctrlMsg->header.Opcode, ctrlMsg->Ack);
					if (ctrlMsg->Ack != '1') {
						cout << "wrong message\n";
					}
					//lock checkTimeMsg and delete returned messages who came on time
					lock1.lock();
					this->checkTimeMsg[(answer->header.msgCounter) % ON_TIME][getControlByOpcode[answer->header.Opcode]].msgSync = MSG_ERR;
					lock1.unlock();
				}
			}
			//Data Message
			else
				if (this->device == TRANSMITTER)
					this->socketToSend.send(*answer, getPort[MODEM][device]);
				else
					this->socketToSend.send(*answer, MESSEGE_GENERATOR);
		}
	}

public:
	Manager(vector<controlerType> controllers, typeDevice device) {
		this->counter = 0;
		this->device = device;
		this->controllers = controllers;
		this->socketToReceive.createBind(getPortManager[device]);//open bind listen to messages
		this->numOfControllers = controllers.size();
		//A matrix that stores sent messages and deletes them upon receipt,
		//in addition checks every second with direct access (by counter)
		//and empties the message that has passed 5 seconds
		this->checkTimeMsg = new Header * [ON_TIME];
		for (int i = 0; i < ON_TIME; i++) {
			checkTimeMsg[i] = new Header[numOfControllers];
		}
		//One thread to send control message, another one to get messages
		thread sendCtrlMsg(&Manager::createMsgAndSend, this);
		sendCtrlMsg.detach();
		thread receiveMsg(&Manager::receiveMsg, this);
		receiveMsg.detach();
	}

	~Manager() {
		// free dinamic allocation
		for (int i = 0; i < ON_TIME; i++)
		{
			delete[]checkTimeMsg[i];
		}
		delete[]checkTimeMsg;
	}

};

