#include <stdio.h>
#include <iostream>
#include <string>

//RakNet Includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"

#define USERNAME_MAX_LENGTH 16
#define MESSAGE_MAX_LENGTH 512

// memory aligned message packet
#pragma pack(push, 1)
struct MessageRequestPacket
{
	unsigned char typeId; // Your type here
	char receiverUsername[USERNAME_MAX_LENGTH];
	// Your data here
	char message[MESSAGE_MAX_LENGTH];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MessageReceivePacket
{
	unsigned char typeId; // Your type here
	char senderUsername[USERNAME_MAX_LENGTH];
	// broadcast or private
	bool isBroadcast;
	// Your data here
	char message[MESSAGE_MAX_LENGTH];
};
#pragma pack(pop)

enum GameMessages
{
	CUSTOM_GAME_MESSAGE = ID_USER_PACKET_ENUM + 1,
	SEND_PRIVATE_MESSAGE,
	SEND_BROADCAST_MESSAGE,
	RECEIVE_MESSAGE
};

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;
	bool isServer;
	unsigned int maxClients;
	unsigned short serverPort;
	char ipAddress[512];

	// initialize ip address
	printf("Enter server IP or hit enter for 127.0.0.1\n");
	fgets(ipAddress, 512, stdin);
	if (ipAddress[0] == '\n') {
		strcpy(ipAddress, "127.0.0.1");
	}

	// initialize server port
	printf("Enter server port number or hit enter for 60000\n");
	fgets(str, 512, stdin);
	if (str[0] == '\n') // str[0] is '\n' with fgets
	{
		serverPort = 60000;
	}
	else
	{
		serverPort = short(atoi(str));
	}

	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else
	{

		// initialize max clients
		printf("Enter max clients or hit enter for 10\n");
		fgets(str, 512, stdin);
		if (str[0] == '\n')
		{
			maxClients = 10;
		}
		else
		{
			maxClients = unsigned(atoi(str));
		}

		RakNet::SocketDescriptor sd(serverPort, ipAddress); // changed server address to localhost
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}


	// TODO - Add code body here
	if (isServer)
	{
		//Set up server things
		peer->SetMaximumIncomingConnections(maxClients);
		std::cout << "Starting Server\n";
	}
	else
	{
		printf("Starting the client.\n");
		peer->Connect(ipAddress, serverPort, 0, 0);
	}

	while (1)
	{
		// MessagePacket welcomeMessage = MessagePacket
		// {
		// 	SERVER_WELCOME_MESSAGE, "Welcome - Love, Server"
		// };
		// // send message packet struct pointer as char*
		// peer->Send((char*)&welcomeMessage, sizeof(MessagePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);


		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
				// remote nortifications not triggering
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			{
				printf("Another client has connected.\n");
			}
			break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("A connection is incoming.\n");
			}
			break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) {
					printf("A client has disconnected.\n");
				}
				else {
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer) {
					printf("A client lost the connection.\n");
				}
				else {
					printf("Connection lost.\n");
				}
				break;

			case RECEIVE_MESSAGE:
			{
				// cast packet data char* to MessagePacket*
				MessageReceivePacket* messagePacket = (MessageReceivePacket*)packet->data;
				// print message
				if (messagePacket->isBroadcast)
				{
					printf("%s broadcast: %s\n", messagePacket->senderUsername, messagePacket->message);
				}
				else
				{
					printf("%s private: %s\n", messagePacket->senderUsername, messagePacket->message);
				}
			}
			break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
