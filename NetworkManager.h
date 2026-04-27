#pragma once


typedef unsigned long long NetSocket;

// On regroupe toutes les variables réseau ici pour ne pas polluer GameState
typedef struct NetworkState {
    NetSocket hostSocket;     // Le socket d'écoute (si on est serveur)
    NetSocket clientSocket;   // Le socket de connexion (pour parler à l'autre)
    int isHost;               // 1 = Serveur, 0 = Client
    int isConnected;          // 1 = Connexion établie, 0 = Hors ligne
} NetworkState;

typedef enum {
    PACKET_CONNECT_ME,      // Un client dit "Bonjour"
    PACKET_GAME_START,      // Le serveur dit "La partie commence, voici la map"
    PACKET_PLACEMENT,       // Un joueur a posé une pièce, avec les spécifications
    PACKET_MONSTER_ATTACK   // On s'envoie les grilles pour les monstres
} PacketType;


typedef struct {
    PacketType type;
    int senderID;           // 0 = Serveur, 1 = Client 1, etc.

    // Le contenu brut (Payload). On réserve de la place (ex: 256 octets) qui contiendra soit une Seed, soit des coordonnées, selon le type.
    char payload[256];
} GamePacket;




int InitNetwork(void);
void CloseNetwork(NetworkState* netState);

int StartHosting(NetworkState* netState, int port);
int JoinGame(NetworkState* netState, const char* ipAddress, int port);

int ReceivePacket(NetSocket myListeningSocket, GamePacket* outPacket);  // Renvoie 1 si un paquet a été lu, 0 s'il n'y a rien, -1 en cas de déconexion

// Tools
int GetLocalIP(char* buffer, int bufferSize);
























