#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Lib3d.h"
#include "GameManager.h"


int coinCount = 0;
int currentTime = 0;

/******************Définition du Contenu******************/
// Cartes Saison

const Saison Spring = { .maxTime = 8, .EditA = 0, .EditB = 1, .name = "Spring" };
const Saison Summer = { .maxTime = 8, .EditA = 1, .EditB = 2, .name = "Summer" };
const Saison Autumn = { .maxTime = 7, .EditA = 2, .EditB = 3, .name = "Autumn" };
const Saison Winter = { .maxTime = 6, .EditA = 3, .EditB = 0, .name = "Winter" };

const Saison* seasons[4] = { &Spring, &Summer, &Autumn, &Winter };
int currentSeason = 0; //[extern] Track the actual season

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
const ScoringCard* edits[4];


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
const ExploreCard RiftLands = { .pieceA = &POINT, .terrainA = FORET, .name = "Rift Lands", .isRiftLands = 1 };

const ExploreCard* expCards[21] = { &FarmLands, &ForgottenForest, &Hamlet, &GreatRiver, &HinterlandStream, &Homestead, &Orchard, &Marshlands, &TreetopVillage, &FishingVillage, &OutpostRuins, &TempleRuins, &RiftLands, &BugbearAssault, &GoblinAttack, &FlayerIncursion, &GnollRaid, &InsectoidInvasion, &OgreCharge, &RatmanStrike, &KoboldOnlaught };
const ExploreCard* exploreDeck[17] = { &FarmLands, &ForgottenForest, &Hamlet, &GreatRiver, &HinterlandStream, &Homestead, &Orchard, &Marshlands, &TreetopVillage, &FishingVillage, &OutpostRuins, &TempleRuins, &RiftLands, NULL, NULL, NULL, NULL };
int deckSize = 13;

/*TODO: 
* Mise en place du jeu complet
* Tour de jeu
* Passer d'un tour à l'autre
* 
*/



void SetupGame(FeuilleCarte f) {
	// Initialisation du jeu
	// Initialiser la Map, actuellement dans le main
	initCarte2(f, TRUE, TRUE);

	// Initialiser les cartes Scores
	InitScoringCards();

	// Mélange des cartes, Définition des packets, Saison, Cartes de Score, etc
	InitDeck();

	// Initialisation du 1er tour
	currentSeason = 0;
	coinCount = 0;
	currentTime = 0;


}

void DebugGameStats(FeuilleCarte f) {
	printf("\n\n\n");
	displayCarte(f);
	printf("\n\nEdits :\n");
	for (int i = 0; i < 4; i++) {
		printf("edit %d : %s, type : %d;\n", i, edits[i]->name, edits[i]->type);
	}
	printf("\n");

	printf("\n\nExplore Deck (size = %d) :\n", deckSize);
	for (int i = 0; i < deckSize; i++) {
		printf("card %d : %s, isEnemy = %d;\n", i, exploreDeck[i]->name, exploreDeck[i]->isEnemy);
	}

}

// ???
int CalcPointsFromCards(FeuilleCarte f, ScoringCard *cards, int numberOfCards) {
	int somme =0;
	for (int i = 0; i < numberOfCards; i++) {
		somme += cards[i].fctCaluls(f) * cards[i].type;
	}
	return somme;
}

void InitDeck() {
	// Setup du deck
	int j = 0;
	for (int i = 0; i < deckSize; i++) { //Remettre les NULL au fond du Deck
		if (exploreDeck[i] == NULL) {
			exploreDeck[i] = exploreDeck[16 - j];
			exploreDeck[16 - j] = NULL;
			j++;
			i--;
		}
	}

	exploreDeck[deckSize] = expCards[randInt(13, 20)];
	deckSize++;

	
	//Melanger le deck :
	ShakeDeck();

	// display Deck for debug
	for (int i = 0; i < deckSize; i++) {
		printf("Carte %d = %s\n", i, exploreDeck[i]->name);
	}

}

void ShakeDeck() {
	const ExploreCard* temp;
	for (int k = 0; k < 100; k++) {
		int j = randInt(0, deckSize - 1);
		int i = randInt(0, deckSize - 1);
		temp = exploreDeck[i];
		exploreDeck[i] = exploreDeck[j];
		exploreDeck[j] = temp;
	}
}

void InitScoringCards() {
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

void StartGame(FeuilleCarte f, int* score, Camera3D camera) {
	printf("\n\n\n\nLancement du jeu !\n\n\n\n");
	Season(f, score, camera);
}

// Saisons
void Season(FeuilleCarte f, int* score, Camera3D camera) {
	int isRuin = 0;
	if (currentSeason >= 4) { printf("Jeu Termine"); return; }
	int actTime = 0;
	int index = 0;
	while (actTime < seasons[currentSeason]->maxTime) {
		const ExploreCard* card = Turn(f, &index, *score, &isRuin, camera);
		actTime += card->time;

	}

	NextSeason(f, score, camera);

}

void NextSeason(FeuilleCarte f, int* score, Camera3D camera) {
	InitDeck();
	// calculs des points
	
	int points1 = edits[seasons[currentSeason]->EditA]->fctCaluls(f);
	printf("%s a donne %d points", edits[seasons[currentSeason]->EditA]->name, points1);
	int points2 = edits[seasons[currentSeason]->EditB]->fctCaluls(f);
	printf("%s a donne %d points", edits[seasons[currentSeason]->EditB]->name, points2);
	printf("Les coins ont donne %d points", coinCount);
	
	int Epoints = calcEnenmyPoints(f);
	int points = points1 + points2 + coinCount - Epoints;
	*score += points;
	printf("Points cette saison : %d\nPoints totaux : %d\n\n", points, *score);

	currentSeason++;
	Season(f, score, camera);
}

// Tour de jeu
const ExploreCard* Turn(FeuilleCarte f, int *index, int score, int* isRuin, Camera3D camera) {
	const ExploreCard* card = NextExploreCard(index, &isRuin);
	printf("Carte Recue\n\n");
	if (card->isEnemy) {
		// Later : Give the map to the other player

		printf("Carte Ennemie\n\n");
		GUIPlacementCard(f, card, score, 0, camera);
		deckSize--;
	}
	else {
		printf("Carte Normale, placement en cours\n\n");
		GUIPlacementCard(f, card, score, *isRuin, camera);
		printf("placement effectué\n\n");
		*isRuin = 0;
	}

	return card;
}

const ExploreCard* NextExploreCard(int* index, int *isRuin) {
	const ExploreCard* card;
	do {
		card = exploreDeck[*index];
		if (card && card->isRuin) *isRuin = 1;
		if (card && card->isEnemy) exploreDeck[*index] = NULL;
		(*index)++;
		printf("Carte choisie %d, %d\n\n", *index, *isRuin);
	} while (card && card->isRuin);
	return card;
}



