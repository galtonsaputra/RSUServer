Regex for 2 decimal points
[\d].*(['.'].{0,2})


//TODO:: TO find out which lever click comes from which one
		if (digitalRead(LeverAStart) == 0 || digitalRead(LeverBStart) == 0)
		{
			startStopwatch = true;
			if (digitalRead(LeverAStart) == 0)
			{
				printf("LEVER A - START stopwatch \n");
				int startStopwatch = waitForInterrupt(LeverAStart, int timeOut);

				//Each thread is a stopwatch, fully contained and waiting for its stop lever click.
				//std::thread awaitForStopLever{ WaitForStopLever,'A', true, 0};
			}

			else if (digitalRead(LeverBStart) == 0)
			{
				startStopwatch = true;
				printf("LEVER B - START stopwatch \n");
				//std::thread awaitForStopLever{ WaitForStopLever,'B', true, 0 };
			}

			else 
			{
				break;
			}
		}


		
int SocketConnection::OpenMultipleConnection()
{
	int client_socket[30];

	int status = 0;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	//Socket creation & IPv4 config
	int socketId = socket(AF_INET, SOCK_STREAM, 0);
	if (socketId == -1) return -1;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(IPPort);
	addrlen = sizeof(address);

	//Bind and Listen
	status = bind(socketId, (struct sockaddr *)&address, sizeof(address));
	if (status == -1) return -2;

	status = listen(socketId, 5);
	if (status == -1) return -3;

	printf("Running\n");
	fd_set listen_set;

	while (true)
	{
		FD_ZERO(&listen_set);
		FD_SET(socketId, &listen_set);

		int currentIncomingSocket;
		
		// pass max descriptor and wait indefinitely until connection arrives
		int numready = select(socketId + 1, &listen_set, NULL, NULL, NULL);
		if (FD_ISSET(socketId, &listen_set)) // new client connection
		{
			// Accept one connection (blocks)
			int incomingSocket = accept(socketId, (struct sockaddr *)&address, (socklen_t*)&addrlen);

			//Adds other connection
			printf("New connection , socket fd is %d , ip is : %s , port : %\n", incomingSocket, inet_ntoa(address.sin_addr), ntohs
			(address.sin_port));

			clients.push_back(incomingSocket);
			sc.ReadClient(incomingSocket, sc.fileDes[1]);

			////add new socket to array of sockets 
			//for (int i = 0; i < 3; i++)
			//{
			//	//if position is empty 
			//	if (client_socket[i] == 0)
			//	{
			//		client_socket[i] = incomingSocket;
			//		printf("Adding to list of sockets as %d\n", i);
			//		//start a thread to read;
			//		sc.ReadClient(incomingSocket, sc.fileDes[1]);
			//		break;
			//	}
			//}


		}

	}

	return 0;
}

