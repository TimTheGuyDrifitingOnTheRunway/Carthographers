#pragma once
//Ce document est là pour réunir toutes les déclarations de Structures au même endroit

#include "Consts.h"
#include "NetworkManager.h"


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

typedef enum {
	FONT_GRENZE_GOTISCH_B,	// Police d'écriture GrenzeGotish Bold
	FONT_GRENZE_GOTISCH_L,	// Police d'écriture GrenzeGotish Light
	FONT_PIRATA_ONE,		// Police d'écriture PirataOne
	FONT_FREDOKA_SB,		// Police d'écriture Fredoka en Semi Bold
	FONT_FREDOKA_CM,		// Police d'écriture Fredoka en Compressed et Medium
	FONT_METAMORPHOUS,		// Police d'écriture Metamorphous
	FONT_COUNT				// Nombre de Polices d'écriture
} FontNames;

typedef enum {
	PHASE_MAIN_MENU,		// Dans le menu de jeu
	PHASE_LOBBY,			// Joueur dans un lobby, en attendant le début de la partie
	PHASE_INIT_GAME,		// Lancement de la partie
	PHASE_INIT_SEASON,		// Tirage des cartes, setup
	PHASE_DRAW_CARD,		// Phase de tirage de la prochaine carte
	PHASE_PLACEMENT,		// Le joueur manipule sa pièce
	PHASE_WAITING_OTHERS,	// Le joueur a validé, on attend les autres joueurs
	PHASE_END_SEASON,		// Calcul des scores de la saison
	PHASE_END_GAME,			// Fin de la partie
} GamePhase;


typedef enum {
	SCREEN_MENU,
	SCREEN_ADD_PLAYER,
	SCREEN_RULES,
	SCREEN_KEYBINDS,
	SCREEN_LANGUAGE,
	SCREEN_GAME,
	SCREEN_EXIT
} ScreenID;


// GameManager.h


typedef struct Saison {
	int maxTime;
	int EditA;
	int EditB;
	char name[10];
	char path[7];
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
	int soloGrid; // pour le placement en solo; 1 : haut gauche, 2 : haut droit, 3 : bas gauche, 4 : Bas droit

	int isRuin;
	int isRiftLands;

	char* imageFileName;
	char name[30];
	int fontSize;
} ExploreCard;

typedef struct ScoringCard {
	int type;
	int (*fctCaluls)(FeuilleCarte f);
	char name[20];
	char* description;
} ScoringCard;

typedef struct {
	Image cardsRAM[NUM_CARDS];
	Image seasonsRAM[NUM_SEASONS];
	Image editsRAM[NUM_EDITS * 2];
	Image skyboxImg;

	int cardsLoadedRAM;
	int seasonsLoadedRAM;
	int editsLoadedRAM;

	int cardsLoadedVRAM;
	int seasonsLoadedVRAM;
	int editsLoadedVRAM;

	int modelsLoaded;

	pthread_mutex_t mutex;
} LoadContext;

typedef struct PlayerState {
	FeuilleCarte map;
	int coinCount;
	int score;
	char name[MAX_NAME_LENGTH];
} PlayerState;



// Lib3D.h


typedef struct PlacementState {
	Piece       shapeCopy;
	Position    pos;
	int         rotation;
	int         material;
	int         drawable;
	int         isRuin;
	int			isDefault;
	int         hasTwoShapes;
	int         hasTwoMat;
	int         isRiftLands;
	int         RiftLandsMat;
	FeuilleCarte feuilleVide;   // grille temporaire pour preview
	FeuilleCarte temp;          // f + feuilleVide fusionnés pour rendu
	const ExploreCard* card;
	int status;                 // 0 = En placement, 1 = placé
	Position mountainPos[NOMBREMONTAGNE];
} PlacementState;


// UI.h

typedef struct Button {
	Rectangle bounds;
	char* label;
	Font labelFont;
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
	char text[64];
	int fontSize;
	Font textFont;
	int maxLength;
	int length;
	bool hovered;
	bool active;
	bool validated;
} InputBox;


typedef struct RulePage {
	char* Text;
	char* Title;
} RulePage;


typedef struct {
    Model tree;
    Model buisson;
    Model skybox;
    Model house;
    Model champs;
    Model water;
    Model monster;
    Model monsterTile;
    Model forestTile;
	Model vilageTile;
}ModelList;		// Liste des modèles à charger au lancement. Si ajout, penser à changer TOTAL_MODELS_TO_LOAD


typedef struct {
	bool isGenerated;
	Image treeImage;
	Image OfsetImagex;
    Image OfsetImagey;
	Image villageImage;
	Image mountainImages[NOMBREMONTAGNE];	// Stockage temporaire des images
	Model mountains[NOMBREMONTAGNE];		// Modèles finaux

}Seed;


typedef struct AssetBank {
	Font fonts[FONT_COUNT];
	Texture2D cardImages[NUM_CARDS];
	Texture2D seasonImages[NUM_SEASONS];
	Texture2D letterScrollsImage[NUM_EDITS * 2];		// 2 fois plus pour stocker les Textures en nuances de Gris
	ModelList models;
} AssetBank;


typedef struct GameState {
	int playerNumber;			// Nombre de joueurs
	PlayerState* players;
	int playerIndex;			// Index du joueur actuel
	int currentTime;
	int currentSeason;
	const ScoringCard* edits[4];
	const ExploreCard* exploreDeck[17];
	int exploreIndex;			// Index de la carte actuelle
	int deckSize;				// Taille actuelle du deck

	int gameType;				// 0 : Solo, 1 : Multijoueur local, 2 : Online

	AssetBank* assets;
	LoadContext* loadCtx;
	PlacementState placementState;
	Seed seed;
	NetworkState net;
} GameState;