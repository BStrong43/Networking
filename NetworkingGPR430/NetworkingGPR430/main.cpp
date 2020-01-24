#include <stdio.h>
#include <iostream>
#include <string>

//RakNet Includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"
#include <unordered_map>

#define USERNAME_MAX_LENGTH 15
#define MESSAGE_MAX_LENGTH 400

enum GameMessageId
{
	CUSTOM_GAME_MESSAGE = ID_USER_PACKET_ENUM + 1,
	CLIENT_HELLO_MESSAGE,
	SEND_PRIVATE_MESSAGE,
	SEND_BROADCAST_MESSAGE,
	RECEIVE_MESSAGE
};

// memory aligned message packet
#pragma pack(push, 1)
struct ClientHelloPacket
{
	unsigned char typeId = CLIENT_HELLO_MESSAGE;
	char username[USERNAME_MAX_LENGTH];
};
#pragma pack(pop)

// memory aligned message packet
#pragma pack(push, 1)
struct MessageRequestPacket
{
	unsigned char typeId;
	char requesterUsername[USERNAME_MAX_LENGTH];
	char receiverUsername[USERNAME_MAX_LENGTH];
	// Your data here
	char message[MESSAGE_MAX_LENGTH];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MessageReceivePacket
{
	unsigned char typeId = RECEIVE_MESSAGE;
	// broadcast or private
	char senderUsername[USERNAME_MAX_LENGTH];
	// Your data here
	char message[MESSAGE_MAX_LENGTH];
	bool isBroadcast;
};
#pragma pack(pop)

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;
	bool isServer;
	unsigned int maxClients;
	unsigned short serverPort;
	char ipAddress[512];
	char username[USERNAME_MAX_LENGTH];
	std::unordered_map<char[USERNAME_MAX_LENGTH], RakNet::SystemAddress> participantSystemAddresses;

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

	printf("(P)articipent or (H)ost?\n");
	fgets(str, 512, stdin);
	if (str[0] == 'p' || str[0] == 'P')
	{
		// input username
		printf("Enter user name (max length %i)\n", USERNAME_MAX_LENGTH);
		fgets(username, USERNAME_MAX_LENGTH, stdin);
		if (username[0] == '\n') {
			strcpy(username, "NoName");
		}
		else
		{
			// remove \n from end of fgets input //https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
			strtok(username, "\n");
		}

		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else
	{
		// set username to "Host"
		strcpy(username, "Host");

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
				ClientHelloPacket helloMessage = ClientHelloPacket();
				// copy username value into hellomessage value
				strcpy(helloMessage.username, username);
				// send message packet struct pointer as char*
				peer->Send((char*)& helloMessage, sizeof(ClientHelloPacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
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
			case CLIENT_HELLO_MESSAGE:
			{
				// cast packet data char* to MessagePacket*
				ClientHelloPacket* clientHelloPacket = (ClientHelloPacket*)packet->data;
				// TODO: check for duplicate username??
				// map participant username to their system address
				participantSystemAddresses[clientHelloPacket->username] = packet->systemAddress;
				printf("%s has joined the chat!\n", clientHelloPacket->username);

				// welcome client privately
				MessageReceivePacket welcomeMessagePacket = MessageReceivePacket();
				// sender is host
				strcpy(welcomeMessagePacket.senderUsername, username);
				// private message
				welcomeMessagePacket.isBroadcast = false;
				// "Welcome to the chat, username!"
				char helloMessage[MESSAGE_MAX_LENGTH] = "Welcome to the chat, ";
				strcat(helloMessage, clientHelloPacket->username);
				strcat(helloMessage, "!");
				strcpy(welcomeMessagePacket.message, helloMessage);
				// send message packet struct pointer as char*
				peer->Send((char*)& welcomeMessagePacket, sizeof(MessageReceivePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

				// broadcast client join to all but joined client
				MessageReceivePacket participantJoinedMessagePacket = MessageReceivePacket();
				// sender is host
				strcpy(participantJoinedMessagePacket.senderUsername, username);
				// broadcast
				participantJoinedMessagePacket.isBroadcast = true;
				// "username has joined the chat!"
				char participantJoinedBroadcastMessage[MESSAGE_MAX_LENGTH];
				strcpy(participantJoinedBroadcastMessage, clientHelloPacket->username);
				strcat(participantJoinedBroadcastMessage, " has joined the chat!");
				strcpy(participantJoinedMessagePacket.message, participantJoinedBroadcastMessage);
				// broadcast to all but new participant
				peer->Send((char*)& participantJoinedMessagePacket, sizeof(MessageReceivePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
			}
			break;
			case SEND_PRIVATE_MESSAGE:
			{
				MessageRequestPacket* requestPrivateMessagePacket = (MessageRequestPacket*)packet->data;

				// verify requester is who they say they are
				if (packet->systemAddress == participantSystemAddresses[requestPrivateMessagePacket->requesterUsername])
				{
					MessageReceivePacket receivePrivateMessagePacket = MessageReceivePacket();
					// private
					receivePrivateMessagePacket.isBroadcast = false;
					// sender is requester
					strcpy(receivePrivateMessagePacket.senderUsername, requestPrivateMessagePacket->requesterUsername);
					// copy over message
					strcpy(receivePrivateMessagePacket.message, requestPrivateMessagePacket->message);
					// get receiver system address by requested receiver username
					RakNet::SystemAddress receiverSystemAddress = participantSystemAddresses[requestPrivateMessagePacket->receiverUsername];
					// send to receiver
					peer->Send((char*)& receivePrivateMessagePacket, sizeof(MessageReceivePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, receiverSystemAddress, false);

					printf("(private) %s to %s: %s\n", receivePrivateMessagePacket.senderUsername, requestPrivateMessagePacket->receiverUsername, receivePrivateMessagePacket.message);
				}
			}
			break;
			case SEND_BROADCAST_MESSAGE:
			{
				MessageRequestPacket* requestBroadcastMessagePacket = (MessageRequestPacket*)packet->data;

				// verify requester is who they say they are
				if (packet->systemAddress == participantSystemAddresses[requestBroadcastMessagePacket->requesterUsername])
				{
					MessageReceivePacket receiveBroadcastMessagePacket = MessageReceivePacket();
					// broadcast
					receiveBroadcastMessagePacket.isBroadcast = true;
					// sender is requester
					strcpy(receiveBroadcastMessagePacket.senderUsername, requestBroadcastMessagePacket->requesterUsername);
					// copy over message
					strcpy(receiveBroadcastMessagePacket.message, requestBroadcastMessagePacket->message);
					// broadcast to all but sender
					peer->Send((char*)& receiveBroadcastMessagePacket, sizeof(MessageReceivePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

					printf("(broadcast) %s: %s\n", receiveBroadcastMessagePacket.senderUsername, receiveBroadcastMessagePacket.message);
				}
			}
			break;
			case RECEIVE_MESSAGE:
			{
				// cast packet data char* to MessagePacket*
				MessageReceivePacket* messagePacket = (MessageReceivePacket*)packet->data;
				// print message
				if (messagePacket->isBroadcast)
				{
					printf("(broadcast) %s: %s\n", messagePacket->senderUsername, messagePacket->message);
				}
				else
				{
					printf("(private) %s: %s\n", messagePacket->senderUsername, messagePacket->message);
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