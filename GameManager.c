#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "GameManager.h"


int coinCount = 0;

/******************Définition du Contenu******************/
// Cartes Saison

const Saison Spring = { 8, 0, 1, "Spring" };
const Saison Summer = { 8, 1, 2, "Summer" };
const Saison Autumn = { 7, 2, 3, "Autumn" };
const Saison Winter = { 6, 3, 0, "Winter" };

const Saison* seasons[4] = { &Spring, &Summer, &Autumn, &Winter };

// Cartes Scoring

const ScoringCard SentinelWood = { 1, calcSentinelWood, "SentinelWood"};
const ScoringCard TreeTower = { 1, calcTreeTower, "TreeTower"};
const ScoringCard GreenBough = { 1, calcGreenBough, "GreenBough"};
const ScoringCard StoneSideQuest = { 1, calcStoneSideQuest, "StoneSideQuest"};

const ScoringCard CanalLake = { 2, calcCanalLake, "CanalLake"};
const ScoringCard ShoreSideExpanse = { 2, calcShoreSideExpanse, "ShoreSideExpanse"};
const ScoringCard GoldenGranary = { 2, calcGoldenGranary, "TheGoldenGranary"};
const ScoringCard MagesValley = { 2, calcMagesValley, "MagesValley"};

const ScoringCard Wildholds = { 3, calcWildholds, "Wildholds"};
const ScoringCard GreengoldPlains = { 3, calcGreengoldPlains, "GreengoldPlains"};
const ScoringCard GreatCity = { 3, calcGreatCity, "GreatCity"};
const ScoringCard Shieldgate = { 3, calcShieldgate, "Shieldgate"};

const ScoringCard Borderlands = { 4, calcBorderlands, "Borderlands"};
const ScoringCard BrokenRoad = { 4, calcBrokenRoad, "TheBrokenRoad"};
const ScoringCard LostBarony = { 4, calcLostBarony, "LostBarony"};
const ScoringCard TheCauldrons = { 4, calcTheCauldrons, "TheCauldrons"};

const ScoringCard* scoringCards[16] = { &SentinelWood, &TreeTower, &GreenBough, &StoneSideQuest, &CanalLake, &ShoreSideExpanse, &GoldenGranary, &MagesValley, &Wildholds, &GreengoldPlains, &GreatCity, &Shieldgate, &Borderlands, &BrokenRoad, &LostBarony, &TheCauldrons };
// Et plus, quand les fonctions seront définies

// Cartes Exploration
const ExploreCard FarmLands = { .name = "Farm Lands", .time = 1, .pieceA = &L_LINE, .iscoinA = 1, .pieceB = &CROSS, .terrainA = CHAMPS };
const ExploreCard ForgottenForest = { .name = "Forgotten Forest", .time = 1, .pieceA = &L_DIAG, .iscoinA = 1, .pieceB = &Z, .terrainA = FORET };
const ExploreCard Hamlet = { .name = "Hamlet", .time = 1, .pieceA = &L_L, .iscoinA = 1, .pieceB = &CUBE_WITH_POINT, .terrainA = VILLAGE };
const ExploreCard GreatRiver = { .name = "Great River", .time = 1, .pieceA = &LINE, .iscoinA = 1, .pieceB = &STAIRS, .terrainA = EAU };
const ExploreCard HinterlandStream = { .name = "Hinterland Stream", .time = 2, .pieceA = &B_L, .terrainA = CHAMPS, .terrainB = EAU };
const ExploreCard Homestead = { .name = "Homestead", .time = 2, .pieceA = &L_T, .terrainA = VILLAGE, .terrainB = CHAMPS };
const ExploreCard Orchard = { .name = "Orchard", .time = 2, .pieceA = &L, .terrainA = FORET, .terrainB = CHAMPS };
const ExploreCard Marshlands = { .name = "Marshlands", .time = 2, .pieceA = &T, .terrainA = FORET, .terrainB = EAU };
const ExploreCard TreetopVillage = { .name = "Treetop Village", .time = 2, .pieceA = &STRANGE, .terrainA = FORET, .terrainB = VILLAGE };
const ExploreCard FishingVillage = { .name = "Fishing Village", .time = 2, .pieceA = &B_Z, .terrainA = EAU, .terrainB = VILLAGE };

// Cartes Ennemis
const ExploreCard BugbearAssault = { .name = "Bugbear Assault", .isEnemy = 1, .rotation = 1, .pieceA = &RECT_WITH_HOLE };
const ExploreCard GoblinAttack = { .name = "Goblin Attack", .isEnemy = 1, .rotation = -1, .pieceA = &DIAG };
const ExploreCard FlayerIncursion = { .name = "Flayer Incursion", .isEnemy = 1, .rotation = -1, .pieceA = &L_L };
const ExploreCard GnollRaid = { .name = "Gnoll Raid", .isEnemy = 1, .rotation = -1, .pieceA = &U };
const ExploreCard InsectoidInvasion = { .name = "Insectoid Invasion", .isEnemy = 1, .rotation = 1, .pieceA = &Z };
const ExploreCard OgreCharge = { .name = "Ogre Charge", .isEnemy = 1, .rotation = -1, .pieceA = &CUBE };
const ExploreCard RatmanStrike = { .name = "Ratman Strike", .isEnemy = 1, .rotation = 1, .pieceA = &LINE };
const ExploreCard KoboldOnlaught = { .name = "Kobold Onslaught", .isEnemy = 1, .rotation = 1, .pieceA = &L_T };

// Ruines et RiftLands
const ExploreCard OutpostRuins = { .name = "Outpost Ruins", .isRuin = 1 };
const ExploreCard TempleRuins = { .name = "Temple Ruins", .isRuin = 1 };
const ExploreCard RiftLands = { .name = "Rift Lands", .isRiftLands = 1 };
int scores[4] = { 0, 0, 0, 0 };

const ExploreCard* expCards[21] = { &FarmLands, &ForgottenForest, &Hamlet, &GreatRiver, &HinterlandStream, &Homestead, &Orchard, &Marshlands, &TreetopVillage, &FishingVillage, &OutpostRuins, &TempleRuins, &RiftLands, &BugbearAssault, &GoblinAttack, &FlayerIncursion, &GnollRaid, &InsectoidInvasion, &OgreCharge, &RatmanStrike, &KoboldOnlaught };


/*TODO: 
* Mise en place du jeu complet
* Tour de jeu
* Passer d'un tour à l'autre
* 
*/



void SetupGame(FeuilleCarte f, const ScoringCard* edits[4],  const ExploreCard* exploreDeck[17]) {
	// Initialisation du jeu
	// Initialiser la Map, actuellement dans le main
	initCarte2(f, TRUE, TRUE);

	// Initialiser les cartes Scores
	InitScoringCards(edits);

	// Mélange des cartes, Définition des packets, Saison, Cartes de Score, etc
	int deckSize = 14;
	InitDeck(exploreDeck, deckSize);

	// Initialisation du 1er tour



}

int CalcPointsFromCards(FeuilleCarte f, ScoringCard *cards, int numberOfCards) {
	int somme =0;
	for (int i = 0; i < numberOfCards; i++) {
		somme += cards[i].fctCaluls(f) * cards[i].type;
	}
	return somme;
}



void InitDeck(const ExploreCard* exploreDeck[17], int size) {
	// Setup du deck
	size <= 17 ? size : 17;
	for (int i = 0; i < 13; i++) exploreDeck[i] = expCards[i];
	for (int i = 13; i < size; i++) exploreDeck[i] = expCards[randInt(13, 20)];
	for (int i = size; i < 13; i++) exploreDeck[i] = NULL;
	
	//Melanger le deck :
	ShakeDeck(exploreDeck, size);
}

void ShakeDeck(const ExploreCard* exploreDeck[17], int size) {
	const ExploreCard* temp;
	for (int k = 0; k < 100; k++) {
		int j = randInt(0, size - 1);
		int i = randInt(0, size - 1);
		temp = exploreDeck[i];
		exploreDeck[i] = exploreDeck[j];
		exploreDeck[j] = temp;
	}
}

void InitScoringCards(const ScoringCard* edits[4]) {
	const ScoringCard* temp[4];
	int usedEdit[4] = { 0, 0, 0, 0 };
	int b = 0;
	for (int i = 0; i < 4; i++) {
		int a = 0;
		temp[0] = NULL; temp[1] = NULL; temp[2] = NULL; temp[3] = NULL;
		for (int j = 0; j < 16; j++) {
			if (scoringCards[j]->type == i + 1) { temp[a] = scoringCards[j]; a++; }
			if (a == 4) break;
		}
		do b = randInt(0, 3); while (usedEdit[b]);
		edits[b] = temp[randInt(0, 3)];
		usedEdit[b] = 1;
	}
}






