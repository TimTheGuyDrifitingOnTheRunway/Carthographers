#define _CRT_SECURE_NO_WARNINGS

#include "NetworkManager.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>



int InitNetwork(void) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("[NETWORK] : Erreur d'initialisation de Winsock.\n");
		return 0;
	}
	printf("[NETWORK] : Winsock initialise avec succes.\n");
	return 1;
}

void CloseNetwork(NetworkState* netState) {
	if (netState->hostSocket != 0) closesocket((SOCKET)netState->hostSocket);		// SOCKET == unsigned long long
	if (netState->clientSocket != 0) closesocket((SOCKET)netState->clientSocket);
	WSACleanup();
	printf("[NETWORK] : Winsock ferme proprement.\n");
}

int GetLocalIP(char* buffer, int bufferSize) {									
	char hostname[256];

	// 1. Récupérer le nom de l'ordinateur
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		return 0;
	}

	// 2. Demander à Windows les infos sur ce nom
	struct addrinfo hints = { 0 };
	struct addrinfo* result = NULL;

	hints.ai_family = AF_INET;          // On veut seulement une IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;    // On cible le TCP

	if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
		return 0;
	}

	// 3. Extraire l'adresse IP en texte lisible
	struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in*)result->ai_addr;	// ai_addr est de type générique en binaire. on sait que c'est de l'IPv4 car c'est ce qu'on a demandé juste avant, alors on le cast dans le type (qui n'est pas un type mais un struct) de l'IPv4 : sockaddr_in
	inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, buffer, bufferSize);			// sin_addr est binaire, on le transforme en IPv4 compréhensible et lisible par l'homme

	freeaddrinfo(result);
	return 1;
}


int StartHosting(NetworkState* netState, int port) {
	// 1. Création (On cast le retour de socket() dans notre type opaque)
	netState->hostSocket = (NetSocket)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	// Demande un canal de com avec les paramètres demandés
	if ((SOCKET)netState->hostSocket == INVALID_SOCKET) return 0;

	struct sockaddr_in serverAddr;				// Rappel : si un struct n'est pas défini comme type avec typedef, on doit tjrs l'appeler avec struct devant. c'est donc une simple déclaration
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);			// htons = Host TO Network Short : invers le sens des octets pr la norme réseau

	// 2. Bind
	if (bind((SOCKET)netState->hostSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {	// Prends le canal créé et le branche à un port du PC
		return 0;
	}

	// 3. Listen
	if (listen((SOCKET)netState->hostSocket, SOMAXCONN) == SOCKET_ERROR) return 0;		// Active le mode écoute sur le canal. SOMAXCONN est une constante de Windows pr taille de liste d'attente

	// 4. Non-bloquant
	u_long mode = 1;	// Pour le passer explicitement à non-bloquant
	ioctlsocket((SOCKET)netState->hostSocket, FIONBIO, &mode);							// Enlève le caractère bloquant du réseau. Renvoie 0 si rien a écouter

	netState->isHost = 1;
	return 1;
}

int JoinGame(NetworkState* netState, const char* ipAddress, int port) {
	netState->clientSocket = (NetSocket)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ((SOCKET)netState->clientSocket == INVALID_SOCKET) return 0;

	struct sockaddr_in serverAddr;						// Ne décrit plus la machine actuelle, mais le serveur à atteindre
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress, &serverAddr.sin_addr);	// inet_pton : opération inverse de inet_ntop, IPv4 -> binaire 32bits

	connect((SOCKET)netState->clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));		// Envoie une requête de connexion réseau

	u_long mode = 1;
	ioctlsocket((SOCKET)netState->clientSocket, FIONBIO, &mode);		// pour ne pas attendre de manière bloquante la réponse du serveur

	return 1;
}

int ReceivePacket(NetSocket myListeningSocket, GamePacket* outPacket) {
	int bytesReceived = recv((SOCKET)myListeningSocket, (char*)outPacket, sizeof(GamePacket), 0);

	// Gestion d'erreur
	if (bytesReceived > 0) {
		// Un colis est arrivé et a rempli 'outPacket'
		return 1;
	}
	else if (bytesReceived == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK) {
			// Boîte aux lettres juste vide.
			return 0;
		}
		else {
			// Problème (le câble est débranché, le joueur a alt-f4...)
			printf("[RESEAU] : Connexion perdue (Erreur: %d).\n", error);
			return -1;
		}
	}
	else if (bytesReceived == 0) {
		// La norme TCP dit que si recv renvoie exactement 0, l'autre a fermé la connexion proprement.
		printf("[RESEAU] : L'autre joueur a quitte la partie.\n");
		return -1;
	}
	return 0;
}













