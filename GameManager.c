#include "GameManager.h"



/******************Définition du Contenu******************/
// Cartes Saison

const Saison Spring = { .maxTime = 8, .EditA = 0, .EditB = 1, .path = "Spring", .name = "Printemps"};
const Saison Summer = { .maxTime = 8, .EditA = 1, .EditB = 2, .path = "Summer", .name = "Eté" };
const Saison Autumn = { .maxTime = 7, .EditA = 2, .EditB = 3, .path = "Autumn", .name = "Automne" };
const Saison Winter = { .maxTime = 6, .EditA = 3, .EditB = 0, .path = "Winter", .name = "Hiver" };

const Saison* seasons[4] = { &Spring, &Summer, &Autumn, &Winter };

// Cartes Scoring

// 🌳 Cartes liées à la Forêt
const ScoringCard SentinelWood = { 1, calcSentinelWood, "Sentinel Wood", "Gagnez une Étoile de Réputation pour chaque case Forêt adjacente au bord de la Carte." };
const ScoringCard TreeTower = { 1, calcTreeTower, "Tree Tower", "Gagnez une Étoile de Réputation pour chaque case Forêt entourée des quatre côtés par des cases remplies ou le bord de la Carte." };
const ScoringCard GreenBough = { 1, calcGreenBough, "Green Bough", "Gagnez une Étoile de Réputation pour chaque ligne et colonne avec au moins une case Forêt. La même case Forêt peut compter pour une rangée et une colonne." };
const ScoringCard StoneSideQuest = { 1, calcStoneSideQuest, "Stone Side Quest", "Gagnez trois Étoiles de Réputation pour chaque case Montagne connectée à une autre case Montagne par un Groupe de cases Forêt." };

// 🌾 Cartes liées à l'Eau et aux Champs
const ScoringCard CanalLake = { 2, calcCanalLake, "Canal Lake", "Gagnez une Étoile de Réputation pour chaque case Eau adjacente à au moins une case Champs. Gagnez une Étoile de Réputation pour chaque case Champs adjacente à au moins une case Eau." };
const ScoringCard ShoreSideExpanse = { 2, calcShoreSideExpanse, "Shore Side Expanse", "Gagnez trois Étoiles de Réputation pour chaque Groupe de cases Champs non adjacents à une case Eau ou au bord de la Carte. Gagnez trois Étoiles de Réputation pour chaque Groupe de cases Eau non adjacents à une case Champs ou au bord de la Carte." };
const ScoringCard GoldenGranary = { 2, calcGoldenGranary, "The Golden Granary", "Gagnez une Étoile de Réputation pour chaque case Eau adjacente à une case Ruine. Gagnez trois Étoiles de Réputation pour chaque case Champs sur une case Ruines." };
const ScoringCard MagesValley = { 2, calcMagesValley, "Mages Valley", "Gagnez deux Étoiles de Réputation pour chaque case Eau adjacente à une case Montagne. Gagnez une Étoile de Réputation pour chaque case Champs adjacente à une case Montagne." };

// 🏠 Cartes liées aux Villages
const ScoringCard Wildholds = { 3, calcWildholds, "Wildholds", "Gagnez huit Étoiles de Réputation pour chaque Groupe de six Villages ou plus." };
const ScoringCard GreengoldPlains = { 3, calcGreengoldPlains, "Greengold Plains", "Gagnez trois Étoiles de Réputation pour chaque Groupe de Villages adjacents à trois types de terrain différents ou plus." };
const ScoringCard GreatCity = { 3, calcGreatCity, "Great City", "Gagnez une Étoile de Réputation pour chaque case Village dans le plus grand Groupe de Village non adjacents à une case Montagne." };
const ScoringCard Shieldgate = { 3, calcShieldgate, "Shieldgate", "Gagnez deux Étoiles de Réputation pour chaque case Village dans le deuxième plus grand Groupe de Village." };

// 🗺️ Cartes liées à la Structure et au Remplissage Global
const ScoringCard Borderlands = { 4, calcBorderlands, "Borderlands", "Gagnez six Étoiles de Réputation pour chaque ligne ou colonne complète (cases remplies)." };
const ScoringCard BrokenRoad = { 4, calcBrokenRoad, "The Broken Road", "Gagnez trois Étoiles de Réputation pour chaque ligne diagonale complète (cases remplies) qui touche les bords gauche et inférieur de la Carte." };
const ScoringCard LostBarony = { 4, calcLostBarony, "Lost Barony", "Gagnez trois Étoiles de Réputation pour chaque case d'un côté du plus grand carré de cases remplies (ex: un carré de 4x4=12 Étoiles)." };
const ScoringCard TheCauldrons = { 4, calcTheCauldrons, "The Cauldrons", "Gagnez une Étoile de Réputation pour chaque case vide entouré des quatre côtés par des cases remplies ou le bord de la Carte." };

/*// 🌳 Cartes liees a la Foret
const ScoringCard SentinelWood = { 1, calcSentinelWood, "Sentinel Wood", "Gagnez une Etoile de Reputation pour chaque case Foret adjacente au bord de la Carte." };
const ScoringCard TreeTower = { 1, calcTreeTower, "Tree Tower", "Gagnez une Etoile de Reputation pour chaque case Foret entouree des quatre cotes par des cases remplies ou le bord de la Carte." };
const ScoringCard GreenBough = { 1, calcGreenBough, "Green Bough", "Gagnez une Etoile de Reputation pour chaque ligne et colonne avec au moins une case Foret. La meme case Foret peut compter pour une rangee et une colonne." };
const ScoringCard StoneSideQuest = { 1, calcStoneSideQuest, "Stone Side Quest", "Gagnez trois Etoiles de Reputation pour chaque case Montagne connectee a une autre case Montagne par un Groupe de cases Foret." };

// 🌾 Cartes liees a l'Eau et aux Champs
const ScoringCard CanalLake = { 2, calcCanalLake, "Canal Lake", "Gagnez une Etoile de Reputation pour chaque case Eau adjacente a au moins une case Champs. Gagnez une Etoile de Reputation pour chaque case Champs adjacente a au moins une case Eau." };
const ScoringCard ShoreSideExpanse = { 2, calcShoreSideExpanse, "Shore Side Expanse", "Gagnez trois Etoiles de Reputation pour chaque Groupe de cases Champs non adjacents a une case Eau ou au bord de la Carte. Gagnez trois Etoiles de Reputation pour chaque Groupe de cases Eau non adjacents a une case Champs ou au bord de la Carte." };
const ScoringCard GoldenGranary = { 2, calcGoldenGranary, "The Golden Granary", "Gagnez une Etoile de Reputation pour chaque case Eau adjacente a une case Ruine. Gagnez trois Etoiles de Reputation pour chaque case Champs sur une case Ruines." };
const ScoringCard MagesValley = { 2, calcMagesValley, "Mages Valley", "Gagnez deux Etoiles de Reputation pour chaque case Eau adjacente a une case Montagne. Gagnez une Etoile de Reputation pour chaque case Champs adjacente a une case Montagne." };

// 🏠 Cartes liees aux Villages
const ScoringCard Wildholds = { 3, calcWildholds, "Wildholds", "Gagnez huit Etoiles de Reputation pour chaque Groupe de six Villages ou plus." };
const ScoringCard GreengoldPlains = { 3, calcGreengoldPlains, "Greengold Plains", "Gagnez trois Etoiles de Reputation pour chaque Groupe de Villages adjacents a trois types de terrain differents ou plus." };
const ScoringCard GreatCity = { 3, calcGreatCity, "Great City", "Gagnez une Etoile de Reputation pour chaque case Village dans le plus grand Groupe de Village non adjacents a une case Montagne." };
const ScoringCard Shieldgate = { 3, calcShieldgate, "Shieldgate", "Gagnez deux Etoiles de Reputation pour chaque case Village dans le deuxieme plus grand Groupe de Village." };

// 🗺️ Cartes liees a la Structure et au Remplissage Global
const ScoringCard Borderlands = { 4, calcBorderlands, "Borderlands", "Gagnez six Etoiles de Reputation pour chaque ligne ou colonne complete (cases remplies)." };
const ScoringCard BrokenRoad = { 4, calcBrokenRoad, "The Broken Road", "Gagnez trois Etoiles de Reputation pour chaque ligne diagonale complete (cases remplies) qui touche les bords gauche et inferieur de la Carte." };
const ScoringCard LostBarony = { 4, calcLostBarony, "Lost Barony", "Gagnez trois Etoiles de Reputation pour chaque case d'un cote du plus grand carre de cases remplies (ex: un carre de 4x4=12 Etoiles)." };
const ScoringCard TheCauldrons = { 4, calcTheCauldrons, "The Cauldrons", "Gagnez une Etoile de Reputation pour chaque case vide entoure des quatre cotes par des cases remplies ou le bord de la Carte." };*/

// Liste de toutes les cartes de scoring
const ScoringCard* scoringCards[] = { &SentinelWood, &TreeTower, &GreenBough, &StoneSideQuest, &CanalLake, &ShoreSideExpanse, &GoldenGranary, &MagesValley, &Wildholds, &GreengoldPlains, &GreatCity, &Shieldgate, &Borderlands, &BrokenRoad, &LostBarony, &TheCauldrons };

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

// Ruines et RiftLands
const ExploreCard OutpostRuins = { .name = "Outpost Ruins", .isRuin = 1 };
const ExploreCard TempleRuins = { .name = "Temple Ruins", .isRuin = 1 };
const ExploreCard RiftLands = { .pieceA = &POINT, .terrainA = FORET, .name = "Rift Lands", .isRiftLands = 1 };

// Cartes Ennemis
const ExploreCard GoblinAttack = { .name = "Goblin Attack", .isEnemy = 1, .rotation = -1, .pieceA = &DIAG,  };
const ExploreCard BugbearAssault = { .name = "Bugbear Assault", .isEnemy = 1, .rotation = 1, .pieceA = &RECT_WITH_HOLE };
const ExploreCard KoboldOnslaught = { .name = "Kobold Onslaught", .isEnemy = 1, .rotation = 1, .pieceA = &L_T };
const ExploreCard GnollRaid = { .name = "Gnoll Raid", .isEnemy = 1, .rotation = -1, .pieceA = &U };
const ExploreCard OgreCharge = { .name = "Ogre Charge", .isEnemy = 1, .rotation = -1, .pieceA = &CUBE };
const ExploreCard InsectoidInvasion = { .name = "Insectoid Invasion", .isEnemy = 1, .rotation = 1, .pieceA = &Z };
const ExploreCard RatmanStrike = { .name = "Ratman Strike", .isEnemy = 1, .rotation = 1, .pieceA = &LINE };
const ExploreCard FlayerIncursion = { .name = "Flayer Incursion", .isEnemy = 1, .rotation = -1, .pieceA = &L_L };

const ExploreCard* expCards[NUM_CARDS] = { &FarmLands, &ForgottenForest, &Hamlet, &GreatRiver, &HinterlandStream, &Homestead, &Orchard, &Marshlands, &TreetopVillage, &FishingVillage, &OutpostRuins, &TempleRuins, &RiftLands, &GoblinAttack, &BugbearAssault, &KoboldOnslaught, &GnollRaid, &OgreCharge, &InsectoidInvasion, &RatmanStrike, &FlayerIncursion };

/*TODO:
* Mise en place du jeu complet
* Tour de jeu
* Passer d'un tour à l'autre
*
*/



// Initialise l'état du jeu, crée les joueurs, mélange le deck et réinitialise les variables
void SetupGame(GameState* gs) {
	if (gs->playerNumber == 0) {
		gs->players = malloc(sizeof(PlayerState));
		strcpy(gs->players[gs->playerNumber++].name, "Teapot-418");
	}
	gs->playerIndex = 0;
	gs->deckSize = 13;

	FeuilleCarte temp;
	initCarte2(temp, TRUE, TRUE);

	for (int i = 0; i < gs->playerNumber; i++) {
		copyCarte(temp, gs->players[i].map);
		gs->players[i].score = 0;
		gs->players[i].coinCount = 0;
	}

	InitScoringCards(gs);

	for (int i = 0; i < 13; i++) {
		gs->exploreDeck[i] = expCards[i];
	}
	InitDeck(gs);

	gs->currentSeason = 0;
	gs->currentTime = 0;

}

// Affiche les informations de débogage du jeu (édits, deck)
void DebugGameStats(GameState* gs) {
	printf("\n\n\n");
	printf("\n\nEdits :\n");
	for (int i = 0; i < 4; i++) {
		printf("edit %d : %s, type : %d;\n", i, gs->edits[i]->name, gs->edits[i]->type);
	}
	printf("\n");

	printf("\n\nExplore Deck (size = %d) :\n", gs->deckSize);
	for (int i = 0; i < gs->deckSize; i++) {
		printf("card %d : %s, isEnemy = %d, Time = %d;\n", i, gs->exploreDeck[i]->name, gs->exploreDeck[i]->isEnemy, gs->exploreDeck[i]->time);
	}

}

// Affiche le classement final des joueurs et termine le jeu
void EndGame(GameState* gs, int nbPlayers) {
	printf("\n\n\n\nFin de la partie ! \n\n\n\n");
	printf("Joueurs et scores :\n\n");
	for (int i = 0; i < nbPlayers; i++) {
		PlayerState p = gs->players[i];
		printf("%s a obtenu %d points", p.name, p.score);
	}

}





// Calcule le total des points d'une feuille de carte pour un ensemble de cartes de scoring
int CalcPointsFromCards(FeuilleCarte f, ScoringCard *cards, int numberOfCards) {
	int somme =0;
	for (int i = 0; i < numberOfCards; i++) {
		somme += cards[i].fctCaluls(f) * cards[i].type;
	}
	return somme;
}

// Réinitialise le deck d'exploration en supprimant les cartes ennemies jouées et en ajoute une nouvelle
void InitDeck(GameState* gs) {
	printf("\nDeck Actuel :\n");
	for (int i = 0; i < gs->deckSize; i++) {
		printf("\n %d : %s", i + 1, gs->exploreDeck[i]->name);
	}

	for (int i = 0; i < gs->exploreIndex; i++) {
		if (gs->exploreDeck[i]->isEnemy) gs->exploreDeck[i] = NULL;
	}

	int j = 0;
	for (int i = 0; i < gs->deckSize; i++) {
		if (gs->exploreDeck[i] == NULL) {
			gs->exploreDeck[i] = gs->exploreDeck[16 - j];
			gs->exploreDeck[16 - j] = NULL;
			j++;
			i--;
		}
	}

	gs->exploreDeck[gs->deckSize] = expCards[randInt(13, 20)];
	gs->deckSize++;


	ShakeDeck(gs);

	printf("\nDeck after : \n");
	for (int i = 0; i < gs->deckSize; i++) {
		printf("Carte %d = %s\n", i, gs->exploreDeck[i]->name);
	}

}

// Mélange aléatoirement le deck d'exploration
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

// Initialise les 4 cartes de scoring aléatoires de la partie (une par type)
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

// Lance le jeu en démarrant la première saison
void StartGame(GameState* gs, Camera3D camera, ModelImage modelsImage, Seed s, int seed[2]) {
	printf("seed : (%d, %d)\n", seed[0], seed[1]);
	ModelList models = loadModelsFromImage(modelsImage,strcmp(gs->players[0].name, SPECIAL_PLAYER_NAME) == 0);
	printf("\n\n\n\nLancement du jeu !\n\n\n\n");
	Season2(gs, camera, models, s, seed);
}

// Exécute une saison complète avec tous les tours jusqu'à la limite de temps, génère les seeds en parallèle
void Season(GameState* gs, Camera3D camera, ModelList models) {

	int mountainSeed[2] = { randInt(0, 100), randInt(0, 100) };
	int mountainSeed2[2] = { randInt(0, 100), randInt(0, 100) };
	clock_t begin = clock();
	printf("generating seed data\n");
	Seed s = generateSeed(mountainSeed);
	Seed* s2;
	pthread_t thread;
	pthread_create(&thread, NULL, generateSeedThread, mountainSeed2);

	printf("seed data generated en : %.2f secondes \n", (double)(clock() - begin)/1000);

	gs->currentTime = 0;
	int isRuin = 0;
	while (gs->currentTime < seasons[gs->currentSeason]->maxTime) {
		Turn(gs, &isRuin, &camera, mountainSeed, models, s);
	}
	pthread_join(thread, (void**)&s2);
	Seed S2 = *s2;
	free(s2);
	NextSeason(gs, camera, models, S2, mountainSeed2);
}

// Exécute une saison avec une seed déjà générée
void Season2(GameState* gs, Camera3D camera, ModelList models, Seed s, int mountainSeed[2]) {
;
	int mountainSeed2[2] = { randInt(0, 100), randInt(0, 100) };

	Seed* s2;
	pthread_t thread;
	pthread_create(&thread, NULL, generateSeedThread, mountainSeed2);
	printf("seed in Season : %d %d\n", mountainSeed[0], mountainSeed[1]);
	gs->currentTime = 0;
	int isRuin = 0;
	while (gs->currentTime < seasons[gs->currentSeason]->maxTime) {
		Turn(gs, &isRuin, &camera, mountainSeed, models, s);
	}
	pthread_join(thread, (void**)&s2);
	Seed S2 = *s2;
	free(s2);
	NextSeason(gs, camera, models, S2, mountainSeed2);
}



// Termine la saison en calculant les points et passe à la saison suivante ou à l'écran final
void NextSeason(GameState *gs, Camera3D camera, ModelList models, Seed s2, int mountainSeed[2]) {
	InitDeck(gs);
	gs->exploreIndex = 0;


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
	GUIDisplayNewSeason(&camera, gs);

#ifdef DEBUG_UI_FIN
	if (++gs->currentSeason < 1) Season2(gs, camera, models, s2, mountainSeed);
#else
	if (++gs->currentSeason < 4) Season2(gs, camera, models, s2, mountainSeed);
#endif
	else {
		sort_players_by_score(gs);
		GUIdisplayFinal(*gs, mountainSeed, s2, models, camera);
	}


}

// Exécute un tour de jeu : révèle une carte et chaque joueur la place sur sa carte
const ExploreCard* Turn(GameState* gs, int* isRuin, Camera3D *camera, int mountainSeed[2], ModelList models, Seed s) {
	const ExploreCard* card = NextExploreCard(gs, isRuin);
	gs->currentTime += card->time;
	
	for (int p = 0; p < gs->playerNumber; p++) {
		gs->playerIndex = p;
		PlayerState* ps = &gs->players[p];
		Model mountains[NOMBREMONTAGNE];


		generateMountainsModels(mountains, gs->players[(p + card->rotation + gs->playerNumber) % gs->playerNumber].map, mountainSeed);
		printf("--- Tour de %s %d/%d---\n", ps->name, p + 1, gs->playerNumber);
		if (gs->playerNumber == 1 && card->isEnemy) {
			autoPlacement(gs->players[0].map, card->pieceA, MONSTRE);
			gs->currentTime += card->time;
		}
		else {
			GUIPlacementCard(gs, gs->players[(p + card->rotation + gs->playerNumber) % gs->playerNumber].map, card, ps->score, (*isRuin && !card->isEnemy), &ps->coinCount, camera, mountains, s, models);
		}
	}

	if (card->isEnemy) gs->deckSize--;
	else *isRuin = 0;
	return card;
}
// Récupère la prochaine carte à explorer en ignorant les ruines
const ExploreCard* NextExploreCard(GameState* gs, int *isRuin) {
	const ExploreCard* card;
	do {
		card = gs->exploreDeck[gs->exploreIndex];
		if (card && card->isRuin) *isRuin = 1;
		gs->exploreIndex++;

		printf("Carte choisie %d, %d\n\n", gs->exploreIndex, *isRuin);

	} while (card && card->isRuin);
	return card;
}



// Trie les joueurs par score décroissant
void sort_players_by_score(GameState* gs) {
	PlayerState* arr = gs->players;
	int n = gs->playerNumber;

	for (int i = 1; i < n; i++) {
		PlayerState key = arr[i];
		int j = i;

		while (j > 0 && arr[j - 1].score < key.score) {
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = key;
	}
}
