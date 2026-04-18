#pragma once
//Ce document est là pour réunir toutes les déclarations de Structures au même endroit

#include "Consts.h"




//

typedef int FeuilleCarte[SIZE][SIZE];
typedef int Piece[PIECESIZE][PIECESIZE];

typedef struct Position {
	int x;
	int y;
}Position;

typedef struct InfoGroupe {
	int taille;
	int material;
	int materialVoisin[10];
}InfoGroupe;





// GameManager.h


typedef struct Saison {
    int maxTime;
    int EditA;
    int EditB;
    char name[7];
} Saison;

typedef struct ExploreCard {
    int time;
    const Piece* pieceA;
    int iscoinA;
    const Piece* pieceB; // NULL si pas de piece
    int iscoinB;

    int terrainA;
    int terrainB;  // 0 si pas de terrain

    int isEnemy;
    int rotation; // 1 = Right, -1 = Left, 0 = None

    int isRuin;
    int isRiftLands;

    char name[30];
} ExploreCard;

typedef struct ScoringCard {
    int type;
    int (*fctCaluls)(FeuilleCarte f);
    char name[20];
} ScoringCard;

typedef struct PlayerState {
    FeuilleCarte map;
    int coinCount;
    int score;
    char name[20];
} PlayerState;

typedef struct GameState {
    int playerNumber;        // Nombre de joueurs
    PlayerState players[MAX_PLAYER];
    int playerIndex;        // Index du joueur actuel
    ExploreCard* card;
    int currentTime;
    int currentSeason;
    const ScoringCard* edits[4];
    const ExploreCard* exploreDeck[17];
    int deckSize;
} GameState;


// Lib3D.h


typedef struct PlacementState {
    Piece       shapeCopy;
    Position    pos;
    int         rotation;
    int         material;
    int         drawable;
    int         isRuin;
    int         hasTwoShapes;
    int         hasTwoMat;
    int         isRiftLands;
    int         RiftLandsMat;
    FeuilleCarte feuilleVide;   // grille temporaire pour preview
    FeuilleCarte temp;          // f + feuilleVide fusionnés pour rendu
    const ExploreCard* card;
    int status;                 // 0 = En placement, 1 = placé
} PlacementState;


// UI.h

typedef struct Button {
    Rectangle bounds;
    char* label;
    int fontSize;
    Color labelColor;
    int corner;     // Arrondi du Boutton en % (0 si pas d'arrondi)
    int stroke;
    Color color1;   // Couleur du bouton
    Color color2;   // Couleur de la bordure
    bool hovered;
    bool validated;
} Button;

typedef struct {
    Rectangle bounds;
    int dx;
    char text[128];
    int fontSize;
    int maxLength;
    int length;
    bool hovered;
    bool active;
    bool validated;
} InputBox;

typedef enum {
    SCREEN_MENU,
    SCREEN_ADD_PLAYER,
    SCREEN_RULES,
    SCREEN_KEYBINDS,
    SCREEN_LANGUAGE,
    SCREEN_GAME,
    SCREEN_EXIT
} ScreenID;

typedef struct Page {
    char* Text;
    char* Title;
} Page;


typedef struct {
    Model tree;
    Model buisson;
    Model skybox;
}ModelList;


typedef struct {
    Image treeImage;

}Seed;
