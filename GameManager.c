#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "GameManager.h"


/******************Définition du Contenu******************/
// Cartes Saison

const Saison Spring = { 8, 0, 1, "Spring" };
const Saison Summer = { 8, 1, 2, "Summer" };
const Saison Autumn = { 7, 2, 3, "Autumn" };
const Saison Winter = { 6, 3, 0, "Winter" };


// Cartes Scoring

const ScoringCard SentinelWood = { 1, calcSentinelWood };
const ScoringCard TreeTower = { 1, calcTreeTower };
const ScoringCard GreenBough = { 1, calcGreenBough };
const ScoringCard StoneSideQuest = { 1, calcStoneSideQuest };

const ScoringCard CanalLake = { 2, calcCanalLake };
const ScoringCard ShoreSideExpanse = { 2, calcShoreSideExpanse };

ScoringCard* scoringCards[16] = { &SentinelWood, &TreeTower, &GreenBough, &StoneSideQuest, &CanalLake, &ShoreSideExpanse };

// Et plus, quand les fonctions seront définies


// Cartes Exploration
const ExploreCard FarmLands = { .time = 1, .pieceA = L_LINE, .iscoinA = 1, .pieceB = CROSS, .terrainA = CHAMPS };
const ExploreCard ForgottenForest = { .time = 1, .pieceA = L_DIAG, .iscoinA = 1, .pieceB = Z, .terrainA = FORET };
const ExploreCard Hamlet = { .time = 1, .pieceA = L_L, .iscoinA = 1, .pieceB = CUBE_WITH_POINT, .terrainA = VILLAGE };
const ExploreCard GreatRiver = { .time = 1, .pieceA = LINE, .iscoinA = 1, .pieceB = STAIRS, .terrainA = EAU };
const ExploreCard HinterlandStream = { .time = 2, .pieceA = B_L, .terrainA = CHAMPS, .terrainB = EAU };
const ExploreCard Homestead = { .time = 2, .pieceA = L_T, .terrainA = VILLAGE, .terrainB = CHAMPS };
const ExploreCard Orchard = { .time = 2, .pieceA = L, .terrainA = FORET, .terrainB = CHAMPS };
const ExploreCard Marshlands = { .time = 2, .pieceA = T, .terrainA = FORET, .terrainB = EAU };
const ExploreCard TreetopVillage = { .time = 2, .pieceA = STRANGE, .terrainA = FORET, .terrainB = VILLAGE };
const ExploreCard FishingVillage = { .time = 2, .pieceA = B_Z, .terrainA = EAU, .terrainB = VILLAGE };

// Cartes Ennemis
const ExploreCard BugbearAssault = { .isEnemy = 1, .rotation = 1, .pieceA = RECT_WITH_HOLE };
const ExploreCard GoblinAttack = { .isEnemy = 1, .rotation = -1, .pieceA = DIAG };
const ExploreCard FlayerIncursion = { .isEnemy = 1, .rotation = -1, .pieceA = L_L };
const ExploreCard GnollRaid = { .isEnemy = 1, .rotation = -1, .pieceA = U };
const ExploreCard InsectoidInvasion = { .isEnemy = 1, .rotation = 1, .pieceA = Z };
const ExploreCard OgreCharge = { .isEnemy = 1, .rotation = -1, .pieceA = CUBE };
const ExploreCard RatmanStrike = { .isEnemy = 1, .rotation = 1, .pieceA = LINE };
const ExploreCard KoboldOnlaught = { .isEnemy = 1, .rotation = 1, .pieceA = L_T };

// Ruines et RiftLands
const ExploreCard OutpostRuins = { .isRuin = 1 };
const ExploreCard TempleRuins = { .isRuin = 1 };
const ExploreCard RiftLands = { .isRiftLands = 1 };

int scores[4] = { 0, 0, 0, 0 };

ExploreCard* expCards[21] = { &FarmLands, &ForgottenForest, &Hamlet, &GreatRiver, &HinterlandStream, &Homestead, &Orchard, &Marshlands, &TreetopVillage, &FishingVillage, &BugbearAssault, &GoblinAttack, &FlayerIncursion, &GnollRaid, &InsectoidInvasion, &OgreCharge, &RatmanStrike, &KoboldOnlaught, &OutpostRuins, &TempleRuins, &RiftLands };


/*TODO: 
* Mise en place du jeu complet
* Tour de jeu
* Passer d'un tour à l'autre
* 
*/



void SetupGame() {
	// Initialisation du jeu
	// Initialiser la Map


	// Initialiser les cartes Scores

	ScoringCard* edits[4];




	// Mélange des cartes, Définition des packets, Saison, Cartes de Score, etc
	ExploreCard* exploreDeck[40];
	InitDeck(exploreDeck);

	// Initialisation de la 



}
int CalcPointsFromCards(FeuilleCarte f, ScoringCard *cards, int numberOfCards) {
	int somme =0;
	for (int i = 0; i < numberOfCards; i++) {
		somme += cards[i].fctCaluls(f) * cards[i].type;
	}
	return somme;
	
}





void InitDeck(ExploreCard* exploreDeck[40]) {
	// Setup du deck
	for (int i = 0; i < 10; i++) {
		exploreDeck[3 * i] = expCards[i];
		exploreDeck[3 * i + 1] = expCards[i];
		exploreDeck[3 * i + 2] = expCards[i];
	}
	for (int i = 0; i < 3; i++) {
		exploreDeck[30 + 2 * i] = expCards[18 + i];
		exploreDeck[31 + 2 * i] = expCards[18 + i];
	}
	exploreDeck[36] = expCards[randInt(10, 17)];
	exploreDeck[37] = NULL;
	exploreDeck[38] = NULL;
	exploreDeck[39] = NULL;

	//Melanger le deck :
	for (int k = 0; k < 100; k++) {
		int j = randInt(0, 36);
		int i = randInt(0, 36);
		ExploreCard* temp = exploreDeck[i];
		exploreDeck[i] = exploreDeck[j];
		exploreDeck[j] = temp;
	}



}










