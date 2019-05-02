#pragma once

#include<thread>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

class Bluetooth
{
public:
	Bluetooth() = default;
	~Bluetooth();

	int Configure(bool isServer, char* dest);
	int WriteBuffer(void* buffer, int size);

private:
	typedef struct Connections {
		int socket;
		struct sockaddr_rc rem_addr;
	} Connection;

	const char* m_Address;

	int sId;
	Connection _connection;

	static void  ReadClient(Connection*);
	
	int RunAsClient(char*);
	int RunAsServer();
	
	std::thread processor;
};

