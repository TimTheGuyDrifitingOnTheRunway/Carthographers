#include "GameManager.h"



/******************Définition du Contenu******************/
// Cartes Saison

const Saison Spring = { .maxTime = 8, .EditA = 0, .EditB = 1, .name = "Spring" };
const Saison Summer = { .maxTime = 8, .EditA = 1, .EditB = 2, .name = "Summer" };
const Saison Autumn = { .maxTime = 7, .EditA = 2, .EditB = 3, .name = "Autumn" };
const Saison Winter = { .maxTime = 6, .EditA = 3, .EditB = 0, .name = "Winter" };

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

/*TODO: 
* Mise en place du jeu complet
* Tour de jeu
* Passer d'un tour à l'autre
* 
*/



void SetupGame(GameState* gs) { 	// Initialisation du jeu
	if (gs->playerNumber == 0) {
		strcpy(gs->players[gs->playerNumber++].name, "Teapot-418");
	}
	gs->playerIndex = 0;
	gs->deckSize = 13;

	FeuilleCarte temp;
	initCarte2(temp, TRUE, TRUE);

	// Initialiser la Map et les stats pour chaque joueur
	for (int i = 0; i < gs->playerNumber; i++) {
		copyCarte(temp, gs->players[i].map);
		gs->players[i].score = 0;
		gs->players[i].coinCount = 0;
	}

	// Initialiser les cartes Scores
	InitScoringCards(gs);

	// Mélange des cartes, Définition des packets, Saison, etc
	for (int i = 0; i < 13; i++) {
		gs->exploreDeck[i] = expCards[i];
	}
	InitDeck(gs);

	// Initialisation du 1er tour
	gs->currentSeason = 0;
	gs->currentTime = 0;

}

void DebugGameStats(GameState* gs) {
	printf("\n\n\n");
	printf("\n\nEdits :\n");
	for (int i = 0; i < 4; i++) {
		printf("edit %d : %s, type : %d;\n", i, gs->edits[i]->name, gs->edits[i]->type);
	}
	printf("\n");

	printf("\n\nExplore Deck (size = %d) :\n", gs->deckSize);
	for (int i = 0; i < gs->deckSize; i++) {
		printf("card %d : %s, isEnemy = %d;\n", i, gs->exploreDeck[i]->name, gs->exploreDeck[i]->isEnemy);
	}

}

void EndGame(GameState* gs, int nbPlayers) {
	printf("\n\n\n\nFin de la partie ! \n\n\n\n");
	printf("Joueurs et scores :\n\n");
	for (int i = 0; i < nbPlayers; i++) {
		PlayerState p = gs->players[i];
		printf("%s a obtenu %d points", p.name, p.score);
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

void InitDeck(GameState* gs) {
	// Setup du deck
	int j = 0;
	for (int i = 0; i < gs->deckSize; i++) { //Remettre les NULL au fond du Deck
		if (gs->exploreDeck[i] == NULL) {
			gs->exploreDeck[i] = gs->exploreDeck[16 - j];
			gs->exploreDeck[16 - j] = NULL;
			j++;
			i--;
		}
	}

	gs->exploreDeck[gs->deckSize] = expCards[randInt(13, 20)];
	gs->deckSize++;

	
	//Melanger le deck :
	ShakeDeck(gs);

	// display Deck for debug
	for (int i = 0; i < gs->deckSize; i++) {
		printf("Carte %d = %s\n", i, gs->exploreDeck[i]->name);
	}

}

void ShakeDeck(GameState* gs) {
	const ExploreCard* temp;
	for (int k = 0; k < 100; k++) {
		int j = randInt(0, gs->deckSize - 1);
		int i = randInt(0, gs->deckSize - 1);
		temp = gs->exploreDeck[i];
		gs->exploreDeck[i] = gs->exploreDeck[j];
		gs->exploreDeck[j] = temp;
	}
}

void InitScoringCards(GameState* gs) {
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
		gs->edits[b] = temp[randInt(0, 3)];
		usedEdit[b] = 1;
	}
}

void StartGame(GameState* gs, Camera3D camera) {
	printf("\n\n\n\nLancement du jeu !\n\n\n\n");
	ModelList models = loadModels();
	Season(gs, camera, models);
}

// Saisons
void Season(GameState* gs, Camera3D camera, ModelList models) {
	Model mountains[NOMBREMONTAGNE];
	int mountainSeed[2] = { randInt(0, 100), randInt(0, 100) };
	int mountainSeed2[2] = { randInt(0, 100), randInt(0, 100) };
	clock_t begin = clock();
	printf("generating seed data\n");
	Seed s = generateSeed(mountainSeed);
	//prégen de la seed de la saison suivante en parallèle pour gagner du temps
	Seed* s2;
	pthread_t thread;
	pthread_create(&thread, NULL, generateSeedThread, mountainSeed2);

	printf("seed data generated en : %.2f secondes \n", (double)(clock() - begin)/1000);

	gs->currentTime = 0;
	int index = 0;
	int isRuin = 0;
	while (gs->currentTime < seasons[gs->currentSeason]->maxTime) {
		const ExploreCard* card = Turn(gs, &index, &isRuin, camera, mountainSeed, models, s);
		gs->currentTime += card->time;
	}
	pthread_join(thread, (void**)&s2);
	NextSeason(gs, camera, models, *s2, mountainSeed2);
}

void Season2(GameState* gs, Camera3D camera, ModelList models, Seed s, int mountainSeed[2]) {
	Model mountains[NOMBREMONTAGNE];
	int mountainSeed2[2] = { randInt(0, 100), randInt(0, 100) };
	
	Seed* s2;
	pthread_t thread;
	pthread_create(&thread, NULL, generateSeedThread, mountainSeed2);

	gs->currentTime = 0;
	int index = 0;
	int isRuin = 0;
	while (gs->currentTime < seasons[gs->currentSeason]->maxTime) {
		const ExploreCard* card = Turn(gs, &index, &isRuin, camera, mountainSeed, models, s);
		gs->currentTime += card->time;
	}
	pthread_join(thread, (void**)&s2);
	NextSeason(gs, camera, models, *s2, mountainSeed2);
}



void NextSeason(GameState *gs, Camera3D camera, ModelList models, Seed s2, int mountainSeed[2]) {
	InitDeck(gs);
	

	// calculs des points
	for (int p = 0; p < gs->playerNumber; p++) {
		PlayerState* ps = &gs->players[p];
		printf("\n\nCalcul des points pour %s :\n", ps->name);
		int p1 = gs->edits[seasons[gs->currentSeason]->EditA]->fctCaluls(ps->map);
		printf("\n%s a donne %d points", gs->edits[seasons[gs->currentSeason]->EditA]->name, p1);
		int p2 = gs->edits[seasons[gs->currentSeason]->EditB]->fctCaluls(ps->map);
		printf("\n%s a donne %d points", gs->edits[seasons[gs->currentSeason]->EditB]->name, p2);
		printf("\nLes coins ont donne %d points", ps->coinCount);
		int Epts = calcEnenmyPoints(ps->map);
		printf("\nLes ennemis ont enleve %d points", Epts);
		int pts = p1 + p2 + ps->coinCount - Epts;
		ps->score += pts;
		printf("\nPoints cette saison : %d\nPoints totaux : %d\n\n", pts, ps->score);

	}

	if (++gs->currentSeason < 4) Season2(gs, camera, models, s2, mountainSeed);
}

// Tour de jeu
const ExploreCard* Turn(GameState* gs, int* index, int* isRuin, Camera3D camera, int mountainSeed[2], ModelList models, Seed s) {
	const ExploreCard* card = NextExploreCard(gs, index, isRuin);

	for (int p = 0; p < gs->playerNumber; p++) {
		gs->playerIndex = p;
		PlayerState* ps = &gs->players[p];
		Model mountains[NOMBREMONTAGNE];
		
		
		generateMountainsModels(mountains, gs->players[(p + card->rotation + gs->playerNumber) % gs->playerNumber].map, mountainSeed);
		printf("--- Tour de %s %d/%d---\n", ps->name, p + 1, gs->playerNumber);
		GUIPlacementCard(gs, gs->players[(p + card->rotation + gs->playerNumber) % gs->playerNumber].map, card, ps->score, (*isRuin && !card->isEnemy), &ps->coinCount, camera, mountains, s, models);	// p + card->rotation + gs->playerNumber car -1 % playerNumber renvoie -1
	}

	if (card->isEnemy) gs->deckSize--;
	else *isRuin = 0;
	return card;
}
const ExploreCard* NextExploreCard(GameState* gs, int* index, int *isRuin) {
	const ExploreCard* card;
	do {
		card = gs->exploreDeck[*index];
		if (card && card->isRuin) *isRuin = 1;
		if (card && card->isEnemy) gs->exploreDeck[*index] = NULL;
		(*index)++;
		printf("Carte choisie %d, %d\n\n", *index, *isRuin);
	} while (card && card->isRuin);
	return card;
}



