#include <stdio.h>
#include <iostream>
#include <string>
#include "KeyboardReader.h"
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
	REQUEST_MESSAGE,
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
	RakNet::RakPeerInterface* peer;
	RakNet::Packet* packet;
	bool isServer;
	unsigned int maxClients;
	unsigned short serverPort;
	char ipAddress[512];
	char username[USERNAME_MAX_LENGTH];
	const char HOST_USERNAME[USERNAME_MAX_LENGTH] = "Host";
	// map from usernames to system addresses, maintained by server
	std::unordered_map<std::string, RakNet::SystemAddress> participantSystemAddresses;
	// store server system address to request messages
	RakNet::SystemAddress serverSystemAddress;
	const char BROADCAST_KEYWORD[USERNAME_MAX_LENGTH] = "BROADCAST";
	std::string message = "";
	bool doNetworkLoop = true;
	bool messageTest = false;
	bool printTest = false;
	bool exitTest = false;

	// "Upon starting up the application or leaving a chat room, choose whether you want to host a new chat room or join a chat room"
	// TODO: reconnection not working in this loop, need to do cleanup
	while (1)
	{
		peer = RakNet::RakPeerInterface::GetInstance();
		doNetworkLoop = true;
		
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
			strcpy(username, HOST_USERNAME);

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


		if (isServer)
		{
			//Set up server things
			peer->SetMaximumIncomingConnections(maxClients);

			// server guid is me
			serverSystemAddress = peer->GetMyBoundAddress();
			participantSystemAddresses[HOST_USERNAME] = serverSystemAddress;
			
			std::cout << "Starting Server\n";
		}
		else
		{
			printf("Starting the client.\n");
			peer->Connect(ipAddress, serverPort, 0, 0);
		}

		// lobby / network loop
		std::string lastKeys = "";
		while (doNetworkLoop)
		{
			// On demand, print user names and IP addresses of all connected users to the host console or to a log file with a time stamp
			if (isServer && !printTest && !!(GetAsyncKeyState('W') & 0x8000))
			{
				printTest = true;

				printf("\nUsername : IP\n");
				for (std::pair<const std::basic_string<char>, RakNet::SystemAddress> participant : participantSystemAddresses)
				{
					printf("%s : %s\n", participant.first.c_str(), participant.second.ToString());
				}
				printf("\n");
			}
			// message request
			if (username[0] == 'A' && !messageTest && !!(GetAsyncKeyState('E') & 0x8000))
			{
				messageTest = true;
				// printf("enter BROADCAST_KEYWORD in receiverUsername to broadcast")
				// from instructions: "if the message is public (broadcast), use some recognizable keyword or symbol"
				char receiverUsername[USERNAME_MAX_LENGTH] = "bob";
				if (!!(GetAsyncKeyState('P') & 0x8000)) strcpy(receiverUsername, "bob");
				else if (!!(GetAsyncKeyState('B') & 0x8000)) strcpy(receiverUsername, BROADCAST_KEYWORD);
				char message[MESSAGE_MAX_LENGTH] = "Sup";

				MessageRequestPacket privateMessageRequestPacket = MessageRequestPacket();
				// private
				privateMessageRequestPacket.typeId = REQUEST_MESSAGE;
				// requester is me
				strcpy(privateMessageRequestPacket.requesterUsername, username);
				// to input receiver
				strcpy(privateMessageRequestPacket.receiverUsername, receiverUsername);
				// input message
				strcpy(privateMessageRequestPacket.message, message);
				// sender to server for routing
				peer->Send((char*)& privateMessageRequestPacket, sizeof(MessageRequestPacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverSystemAddress, false);

				printf("Sending message to %s: %s\n", receiverUsername, message);
			}
			// client leave chat gracefully
			if (username[0] == 'A' && !exitTest && !!(GetAsyncKeyState('Q') & 0x8000))
			{
				exitTest = true;

				printf("\nExiting chat.\n");
				peer->CloseConnection(serverSystemAddress, true, 0, HIGH_PRIORITY);
			}

			// //MESSAGE INPUT
			// {
			// std::string pressedKeys = KeyboardReader::getPressedKeys();
			// if (pressedKeys == "-1" && lastKeys != "-1")
			// {
			// 	if (message != "")
			// 		message = KeyboardReader::trimMessage(message);
			// 	//std::cout << "Character Deleted" << std::endl;
			// 	lastKeys = "-1";
			// }
			// else if (pressedKeys == "-2" && lastKeys != "-2")
			// {
			// 	//MESSAGE READY TO SEND
			// 	MessageReceivePacket packet = MessageReceivePacket();
			// 	packet.isBroadcast = true;
			// 	packet.typeId = RECEIVE_MESSAGE;
			// 	strcpy(packet.message, message.c_str());
			// 	peer->Send((char*)& packet, sizeof(MessageReceivePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverSystemAddress, false);
			// 	//Clear text
			// 	message = "";
			// 	lastKeys = "-2";
			// }
			// else if (pressedKeys == "")
			// {
			// 	lastKeys = "";
			// }
			// else
			// {
			// 	if (lastKeys == "")
			// 	{
			// 		message += pressedKeys;
			// 		lastKeys = pressedKeys;
			// 	}
			// 	else
			// 	{
			// 		std::string nextLastKeys = pressedKeys;
			// 		for (int i = 0; i < lastKeys.length(); i++)
			// 		{
			// 			char key = lastKeys[i];
			// 			int index = pressedKeys.find(key);
			// 			if (index > 0)
			// 			{
			// 				pressedKeys.erase(index, 1);
			// 			}
			// 		}
			// 		pressedKeys.shrink_to_fit();
			// 		message += pressedKeys;
			// 		lastKeys = nextLastKeys;
			// 	}
			// }
			// }

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

					// save server system adress for requesting messages
					serverSystemAddress = packet->systemAddress;

					//serverSystemAddress = packet->systemAddress;
					ClientHelloPacket helloMessage = ClientHelloPacket();
					// copy username value into hellomessage value
					strcpy(helloMessage.username, username);
					// send message packet struct pointer as char*
					peer->Send((char*)& helloMessage, sizeof(ClientHelloPacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverSystemAddress, false);
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
						doNetworkLoop = false;
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
				case REQUEST_MESSAGE:
				{
					// only server can route messages
					if (!isServer) break;

					MessageRequestPacket* requestMessagePacket = (MessageRequestPacket*)packet->data;

					// verify requester is who they say they are
					if (packet->systemAddress.systemIndex == participantSystemAddresses[requestMessagePacket->requesterUsername].systemIndex)
					{
						MessageReceivePacket receiveMessagePacket = MessageReceivePacket();
						// private or broadcast by receiver username / broadcast keyword
						if (strcmp(requestMessagePacket->receiverUsername, BROADCAST_KEYWORD) == 0)
						{
							receiveMessagePacket.isBroadcast = true;
						}
						else
						{
							receiveMessagePacket.isBroadcast = false;
						}
						// sender is requester
						strcpy(receiveMessagePacket.senderUsername, requestMessagePacket->requesterUsername);
						// copy over message
						strcpy(receiveMessagePacket.message, requestMessagePacket->message);

						if (receiveMessagePacket.isBroadcast)
						{
							// broadcast to all but requester
							peer->Send((char*)& receiveMessagePacket, sizeof(MessageReceivePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);

							printf("(broadcast) %s: %s\n", receiveMessagePacket.senderUsername, receiveMessagePacket.message);
						}
						else
						{
							// get receiver system address by requested receiver username
							RakNet::SystemAddress receiverSystemAddress = participantSystemAddresses[requestMessagePacket->receiverUsername];
							// send to receiver
							peer->Send((char*)& receiveMessagePacket, sizeof(MessageReceivePacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, receiverSystemAddress, false);

							printf("(private) %s to %s: %s\n", receiveMessagePacket.senderUsername, requestMessagePacket->receiverUsername, receiveMessagePacket.message);
						}
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
	}
	return 0;
}