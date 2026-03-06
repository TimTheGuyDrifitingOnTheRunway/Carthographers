#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "LibCarthographie.h"


#define EXP_CARD_NUMBER 34



int coinCount; //pr suivre le nombre de coin que le joueur a

typedef struct {
	int maxTime;
	int EditA;
	int EditB;
	char name[7];
}Saison;

typedef struct {
	int time;
	Piece* pieceA;
	int iscoinA;
	Piece* pieceB; // NULL si pas de piece
	int iscoinB;

	int terrainA;
	int terrainB;  // 0 si pas de terrain
	int TerrainC;  // 0 si pas de terrain

	int isEnemy;
	int rotation; //1 = Right, -1 = Left, 0 = None

	int isRuin;
	int isRiftLands;
}ExploreCard;

typedef struct {
	int type;
	int (*fctCaluls)(FeuilleCarte f);
}ScoringCard;


/******************Définition du Contenu******************/
// Cartes Saison

extern const Saison Spring;
extern const Saison Summer;
extern const Saison Autumn;
extern const Saison Winter;

// Cartes Scoring
extern const ScoringCard SentinelWood;
extern const ScoringCard TreeTower;
extern const ScoringCard GreenBough;
extern const ScoringCard StoneSideQuest;

extern const ScoringCard CanalLake;
extern const ScoringCard MagesValley;
extern const ScoringCard TheGoldenGranary;
extern const ScoringCard ShoreSideExpanse;

extern const ScoringCard Wildholds;
extern const ScoringCard GreengoldPlains;
extern const ScoringCard GreatCity;
extern const ScoringCard Shieldgate;

extern const ScoringCard Borderlands;
extern const ScoringCard BrokenRoad;
extern const ScoringCard LostBarony;
extern const ScoringCard TheCauldrons;

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



void SetupGame();
void InitDeck(ExploreCard* exploreDeck[40]);









