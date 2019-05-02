//SocketConnection HEADER config (WiFi or Bluetooth) 
#pragma once
//Old school
#ifndef	_SOCKETCONNECTION_H_
#define	_SOCKETCONNECTION_H_
#endif

#include <cstdio>
#include <cstdlib>
#include <cerrno>
//#include <iostream>
#include <map>
#include <thread>

//Piping
#include <unistd.h>
#include <stack>
//Sockets + Network
#include <sys/socket.h>
#include <netinet/in.h> 

#include "MessageFrame.h"
#include "BasicSafetyMessage.h"
#include "ConcurrentQueue.h"

extern Queue<MessageFrame_t*> MsgQueue;

class SocketConnection
{
    public:
		int OpenWifiSocket();
        void CloseWifiSocket();

		int ConnectUsing(char *addrFamily, char *address, uint8_t channelOrPort)
		{
			int _connectionStatus;
			//Build lookup 
			std::map<char*, int> m;
			m["IPv4"] = 0;
			m["IPv6"] = 1;
			m["BT_RFComm"] = 2;
			//enum?

			std::map<char*, int>::iterator it;
			it = m.find(addrFamily);

			if (it != m.end())
			{
				switch (it->second)
				{
					case 0:
						_connectionStatus = ConnectWith(0, address, channelOrPort);
						break;
					case 1:
						_connectionStatus = ConnectWith(1, address, channelOrPort);
						break;
					case 2:
						_connectionStatus = ConnectWith(2, address, channelOrPort);
						break;
					default:
						_connectionStatus = 5; //all error
						break;
				}
			}	
			return _connectionStatus;
		}

		typedef struct WiFiConfig {
			int socket;
			int addrlen;
			uint16_t PORT;
			struct sockaddr_in address;
		} WiFiConfig;

		//Whats the use of static?
		int socketId;
		WiFiConfig _wifiConnection;

		static void ReadClient(int currentActiveSocket, int pipe);

		//Piping
		int CreatePipe();
		int fileDes[2];

		void StartThread();

		static void ReadPipeToProcessMessage(int pipe);

		static void DummyMethod(SocketConnection* pThis)
		{
			pThis->_wifiConnection.socket;
		}

	private:
		int ConnectWith(int addrFamily, char *address, uint8_t channelOrPort);
		std::thread processRecvMessage;

};