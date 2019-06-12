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
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#include "MessageFrame.h"
#include "BasicSafetyMessage.h"
#include "ConcurrentQueue.h"

extern Queue<MessageFrame_t*> MsgQueue;

class SocketConnection
{
    public:
		//Piping
		int CreatePipe();
		int fileDes[2];

		void ReadPipeToProcessMessage(int pipe);

		static void StartServer();
		static void CloseServer();
};