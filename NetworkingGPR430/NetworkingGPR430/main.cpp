#include <stdio.h>
#include <iostream>
#include <string>

//RakNet Includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"

// memory aligned message packet
#pragma pack(push, 1)
struct MessagePacket
{
	unsigned char typeId; // Your type here
	// Your data here
	char message[511];
	RakNet::NetworkID networkId; // NetworkID of the mine, used as a common method to refer to the mine on different computers
	RakNet::SystemAddress systemAddress; // The SystenAddress of the player that owns the mine
};
#pragma pack(pop)

enum GameMessages
{
	CLIENT_HELLO_MESSAGE = ID_USER_PACKET_ENUM + 1,
	SERVER_WELCOME_MESSAGE,
	CLIENT_WELCOME_MESSAGE,
	COUNT_MESSAGE
};

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;
	bool isServer;
	unsigned int maxClients;
	unsigned short serverPort;

	// initialize server port
	printf("Enter server port number or hit enter for 60000\n");
	fgets(str, 512, stdin);
	if (str[1] == 0) // str[0] is '\n' with fgets
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
	else {

		// initialize max clients
		printf("Enter max clients or hit enter for 10\n");
		fgets(str, 512, stdin);
		if (str[1] == 0)
		{
			maxClients = 10;
		}
		else
		{
			maxClients = unsigned(atoi(str));
		}

		printf("Enter server IP or hit enter for 127.0.0.1\n");
		fgets(str, 512, stdin);
		if (str[1] == 0) {
			strcpy(str, "127.0.0.1");
		}
		RakNet::SocketDescriptor sd(serverPort, str); // changed server address to localhost
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
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		fgets(str, 512, stdin);
		if (str[1] == 0) {
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		peer->Connect(str, serverPort, 0, 0);
	}

	while (1)
	{
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
				/*RakNet::BitStream bsOut;
				bsOut.Write(RakNet::MessageID(CLIENT_WELCOME_MESSAGE));
				bsOut.Write("Welcome - Sincerely, Client");
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);*/
				MessagePacket welcomeMessage = MessagePacket
				{
					CLIENT_WELCOME_MESSAGE, "Welcome - Sincerely, Client", 0, packet->systemAddress
				};
				peer->Send((char*)& welcomeMessage, sizeof(MessagePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				printf("Another client has connected.\n");
			}
			break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");

				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				MessagePacket welcomeMessage = MessagePacket
				{
					CLIENT_HELLO_MESSAGE, "Ben and Adam's Client Hello Message", 0, packet->systemAddress
				};
				peer->Send((char*)& welcomeMessage, sizeof(MessagePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
			{
				MessagePacket welcomeMessage = MessagePacket
				{
					SERVER_WELCOME_MESSAGE, "Welcome - Love, Server", 0, packet->systemAddress
				};
					// send message packet struct pointer as char*
				peer->Send((char*)& welcomeMessage, sizeof(MessagePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
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

			case CLIENT_HELLO_MESSAGE:
			case SERVER_WELCOME_MESSAGE:
			case CLIENT_WELCOME_MESSAGE:
			{
					// cast packet data char* to MessagePacket*
				MessagePacket* messagePacket = (MessagePacket*)packet->data;
					// print message
				printf("%s\n", messagePacket->message);
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
