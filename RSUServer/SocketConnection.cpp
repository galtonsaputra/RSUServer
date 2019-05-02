#include "SocketConnection.h"

#define IPPort 8888
#define SERVER_CHANNEL 3

extern stack<MessageFrame_t*> mystack;

int SocketConnection::OpenWifiSocket ()
{
    int status = 0;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	_wifiConnection.addrlen = addrlen;

    //Socket creation & IPv4 config
    socketId = socket(AF_INET, SOCK_STREAM, 0);
	if (socketId == -1) return -1;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_wifiConnection.PORT);

	//Bind and Listen
    status = bind(socketId, (struct sockaddr *)&address, sizeof(address));
	if (status == -1) return -2;

    status = listen(socketId, 5);
	if (status == -1) return -3;

	_wifiConnection.address = address;
    _wifiConnection.socket = accept(socketId, (struct sockaddr *)&_wifiConnection.address, (socklen_t*)&_wifiConnection.addrlen);

    return 0;
}

void SocketConnection::CloseWifiSocket()
{
	if (_wifiConnection.socket != 0) close(_wifiConnection.socket);
}

void SocketConnection::ReadClient(int currentActiveSocket, int pipe)
{
	int status = 0;
	uint8_t* rcvPipeBuffer = (uint8_t*)calloc(1, 100);
	uint8_t* rcvBuffer = (uint8_t*)calloc(1, 1024);	
	SocketConnection sc;
	if (currentActiveSocket != -1) {
		while (true)
		{
			//bits read. packet length size.
  			int bitsRead = recv(currentActiveSocket, (void*)rcvBuffer, 100, 0);	
			if (bitsRead != -1 && bitsRead != 0) {
				write(pipe, rcvBuffer, bitsRead);

			}
			else { break; }
		}
	}

	return;
}

void SocketConnection::StartThread() {
	processRecvMessage = std::thread { SocketConnection::ReadClient, _wifiConnection.socket, fileDes[1] };
}

/*
	Read the pipe so we can process in-coming messages from the server
	the data is a collection of encoded MessageFrames as bytes.
*/
stack<MessageFrame_t*> mystack;
void SocketConnection::ReadPipeToProcessMessage(int pipe)
{
	//TODO: FIX buffer and garbage collection 
	uint8_t* rcvPipeBuffer = (uint8_t*)calloc(1, 1024);
	//type length prefixing
	// SocketConnection sc; //I'm initialising the class again. Do i have to?
	//close(sc.fileDes[1]);
	// int bitPipeRead = read(sc.fileDes[0], rcvPipeBuffer, 1024);

	//put this in a while loop.
	int bytesRead = read(pipe, rcvPipeBuffer, 100);
	MessageFrame_t* msgFrame = 0;
	asn_dec_rval_t rslt = asn_decode(0, ATS_DER, &asn_DEF_MessageFrame, (void**)&msgFrame, rcvPipeBuffer, bytesRead);

	if (rslt.code == RC_OK) {
		MsgQueue.push(msgFrame);
		mystack.push(msgFrame);
	}

	//else see how many times you get a bad read.
	memset(rcvPipeBuffer, 0, 100);
}

//Create a pipe and used for half-duplex
int SocketConnection::CreatePipe()
{
	int pipeStatus = pipe(fileDes);
	if (pipeStatus == -1) return -1;
	return 0;
}

//Opens socket of AF_TYPE
int SocketConnection::ConnectWith(int af_Type, char *address, uint8_t channelOrPort)
{
	int status = 0;
    switch(af_Type){
        case 0:
            //IPv4
				_wifiConnection.PORT = IPPort;     
				status = OpenWifiSocket();

				if (status != 0)
				{
					status = -1;
					CloseWifiSocket();
				}

				break;
        case 1:
            //IPv6
			break;
		case 2:
			//Bluetooth RFComm
			break;
        default:
            //defaults back to Wifi
            break;
    }
	return status;
}
