#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <raylib.h>
#include "LibCarthographie.h"
#include "Lib3d.h"


#define EXP_CARD_NUMBER 34

typedef struct Saison{
	int maxTime;
	int EditA;
	int EditB;
	char name[7];
}Saison;

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
}ExploreCard;

typedef struct ScoringCard {
	int type;
	int (*fctCaluls)(FeuilleCarte f);
	char name[20];
}ScoringCard;

typedef struct PlayerState{
	FeuilleCarte map;
	int coinCount;
	int score;
	char name[30];
} PlayerState;


typedef struct GameState {
	int playerNumber;		// Nombre de joueurs
	PlayerState* players;	// Tableau alloué dynamiquement
	int playerIndex;		// Index du joueur actuel
	ExploreCard* card;
	int currentTime;
	int currentSeason;
	const ScoringCard* edits[4];
	const ExploreCard* exploreDeck[17];
	int deckSize;
} GameState;


/******************Définition du Contenu******************/
// Cartes Saison

extern const Saison Spring;
extern const Saison Summer;
extern const Saison Autumn;
extern const Saison Winter;

extern const Saison* seasons[4];

// Cartes Scoring
extern const ScoringCard SentinelWood;
extern const ScoringCard TreeTower;
extern const ScoringCard GreenBough;
extern const ScoringCard StoneSideQuest;

extern const ScoringCard CanalLake;
extern const ScoringCard MagesValley;
extern const ScoringCard GoldenGranary;
extern const ScoringCard ShoreSideExpanse;

extern const ScoringCard Wildholds;
extern const ScoringCard GreengoldPlains;
extern const ScoringCard GreatCity;
extern const ScoringCard Shieldgate;

extern const ScoringCard Borderlands;
extern const ScoringCard BrokenRoad;
extern const ScoringCard LostBarony;
extern const ScoringCard TheCauldrons;

extern const ScoringCard* scoringCards[16];


// Et plus, quand les fonctions seront définies


// Cartes Exploration
extern const ExploreCard Farmland;
extern const ExploreCard ForgottenForest;
extern const ExploreCard Hamlet;
extern const ExploreCard GreatRiver;
extern const ExploreCard HinterlandStream;
extern const ExploreCard Homestead;
extern const ExploreCard Orchard;
extern const ExploreCard Marshlands;
extern const ExploreCard TreetopVillage;
extern const ExploreCard FishingVillage;

// Cartes Ennemis
extern const ExploreCard BugbearAssault;
extern const ExploreCard GoblinAttack;
extern const ExploreCard FlayerIncursion;
extern const ExploreCard GnollRaid;
extern const ExploreCard InsectoidInvasion;
extern const ExploreCard OgreCharge;
extern const ExploreCard RatmanStrike;
extern const ExploreCard KoboldOnlaught;

// Ruines et RiftLands
extern const ExploreCard OutpostRuins;
extern const ExploreCard TempleRuins;
extern const ExploreCard RiftLands;

extern const ExploreCard* expCards[21];


// Setup
void SetupGame(GameState* gs, int nbPlayers);
void InitDeck(GameState* gs);
void ShakeDeck(GameState* gs);
void InitScoringCards(GameState* gs);


void StartGame(GameState* gs, Camera3D camera);
// Saison
void NextSeason(GameState* gs, Camera3D camera);
void Season(GameState* gs, Camera3D camera);

// Tour de jeu

const ExploreCard* Turn(GameState* gs, int* index, int* isRuin, Camera3D camera);
const ExploreCard* NextExploreCard(GameState* gs, int* index, int* isRuin);

void DebugGameStats(GameState* gs);

/******************Prototypes******************/
int CalcPointsFromCards(FeuilleCarte f, ScoringCard* cards, int numberOfCards);




