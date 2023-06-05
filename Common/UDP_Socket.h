#pragma once
#include <iostream>
#include <winsock2.h>
#include "IDD.h"
#include <stdio.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib") // Winsock Library
#pragma warning(disable:4996) 
#define BUFLEN sizeof(Message)
#define SERVER "127.0.0.1"  

class UDP_Socket {
	WSADATA wsa;
	SOCKET udpSocket;
public:
	//intilize winSocket
	UDP_Socket() {
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("Failed. Error Code: %d", WSAGetLastError());
			exit(0);
			return;
		}
		if ((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		{
			printf("Could not create socket: %d", WSAGetLastError());
		}
	}
	//bind
	void createBind(int port) {
		// prepare the sockaddr_in structure
		sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(port);
		if (bind(udpSocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			printf("Bind failed with error code: %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}
	// print details of the client/peer and the data received
	Message* receive() {
		fflush(stdout);
		sockaddr_in client;
		int clientLen = sizeof(sockaddr_in);
		char message[BUFLEN] = {};
		int message_len;

		if (message_len = recvfrom(udpSocket, message, BUFLEN, 0, (sockaddr*)&client, &clientLen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code: %d", WSAGetLastError());
			exit(0);
		}

		printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		Message* msg = (Message*)&message;
		return msg;
	}

	void send(Message msg, int port) {
		// prepare the sockaddr_in structure
		sockaddr_in client;
		client.sin_family = AF_INET;
		client.sin_addr.s_addr = inet_addr(SERVER);//ip
		client.sin_port = htons(port);
		if (sendto(udpSocket, (char*)&msg, BUFLEN, 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code: %d", WSAGetLastError());
			exit(0);
		}
	}
};