#include "Lib3d.h"


/**************************************************Fontions jeux******************************************/

void GUIDrawFeuille(FeuilleCarte f, FeuilleCarte temp) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (temp[i][j] >= RUINE) {
				float x = i - (SIZE - 1) / 2.0f;
				float z = j - (SIZE - 1) / 2.0f;
				float y = -0.5f;   // Half height so cube is under the grid
				DrawCube((Vector3) { x, y, z }, 1.0f, 1.0f, 1.0f, DARKGRAY);
				DrawCubeWires((Vector3) { x, y, z }, 1.0f, 1.0f, 1.0f, BLACK);
			}
			if (getMaterialAtPos(temp, (Position) { i, j }) != 0) {
				float x = i - (SIZE - 1) / 2.0f;
				float z = j - (SIZE - 1) / 2.0f;
				float y = 0.5;   // Half height so cube sits on grid
				if (temp[i][j] != f[i][j]) y = PLACEMENT_HEIGHT; // Raise cube if it's part of the shape being placed
				Color color;
				switch (getMaterialAtPos(temp, (Position) { i, j })) {
				case EAU:
					color = BLUE;
					break;
				case FORET:
					color = GREEN;
					break;
				case VILLAGE:
					color = BROWN;
					break;
				case CHAMPS:
					color = YELLOW;
					break;
				case MONTAGNE:
					color = GRAY;
					break;
				case MONSTRE:
					color = PURPLE;
					break;
				case CONFLICTVALUE:
					color = RED;
					break;
				default:
					color = WHITE;
				}
				DrawCube((Vector3) { x, y, z }, 1.0f, 1.0f, 1.0f, color);
				DrawCubeWires((Vector3) { x, y, z }, 1.0f, 1.0f, 1.0f, BLACK);
			}
		}
	}
}


int GUIplacementShape(FeuilleCarte f, const Piece* shape, int material, Camera3D camera) {
	if (checkShape(f, shape)) {
		Piece shapeCopy;
		copyPiece(*shape, shapeCopy);
		Position pos;
		int rotation = 0;
		pos.x = 6;
		pos.y = 6;
		int drawable = 0;
		FeuilleCarte feuilleVide, temp;
		float accX = (float)pos.x;
		float accY = (float)pos.y;

		int done = 0;
		while (!done && !WindowShouldClose()) {
			initCarte(feuilleVide, FALSE);
			drawable = drawShape(feuilleVide, shapeCopy, pos, rotation, material);
			tryDraw(f, feuilleVide, temp);

			BeginDrawing(); // Début de l'affichage
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
			GUIDrawFeuille(f, temp);

			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++)
					if (temp[i][j] != f[i][j])
						DrawCubeWires((Vector3) { (float)i - SIZE / 2, PLACEMENT_HEIGHT, (float)j - SIZE / 2 }, 1.0f, 1.0f, 1.0f, BORDERCOLOR);

			GUIdrawGrille();
			// DrawMapGrid(SIZE, 1.0f);
			EndMode3D();
			EndDrawing(); // Fin de l'affichage
			drawable = drawable && isDrawable(f, feuilleVide);

			GUIUpdateCustomCamera(&camera);
			camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

			pos.x += -(int)IsKeyPressed(LEFTP) + (int)IsKeyPressed(RIGHTP); // déplacement pièce
			pos.y += -(int)IsKeyPressed(UPP) + (int)IsKeyPressed(DOWNP);

			if (IsKeyPressed(KEY_O)) { pos.x = 6; pos.y = 6; }//possibilité de resset

			if (IsKeyPressed(ROTATEP)) {
				rotation = (rotation + 1);
			}
			if (IsKeyPressed(FLIPP)) {
				flipShape(shapeCopy);
			}
			//printf("rotation %d(%d, %d) \n", rotation, pos.x, pos.y1);
			if (IsKeyPressed(KEY_SPACE) && drawable) {
				done = 1;
			}

		}
		if (WindowShouldClose()) {
			CloseWindow();
			exit(1);
		}

		draw(f, feuilleVide);
		return 1;
	}
	else {
		printf("IL n'y a pas la place pour rentrer votre piece \n");
		return GUIplacementDefault(f, material, camera);
	}
}

/*int GUIPlacementCard(FeuilleCarte f, const ExploreCard* card, int score, int isRuin, Camera3D camera) {

	PlacementState state = { 0 };
	int hasTwoShapes = card->pieceB != NULL;
	int hasTwoMat = card->terrainB != 0;

	int isEnemy = card->isEnemy;
	int isRiftLands = card->isRiftLands;
	int RiftLandsMat = 2;

	int material = card->terrainA;
	if (isEnemy) material = MONSTRE;
	int canFitA = isRuin ? checkShapeOnRuin(f, card->pieceA) : checkShape(f, card->pieceA);
	int canFitB = hasTwoShapes ? isRuin ? checkShapeOnRuin(f, card->pieceB) : checkShape(f, card->pieceB) : 0;

	if (!canFitA && !canFitB) {
		printf("IL n'y1 a pas la place pour rentrer votre piece \n");
		return GUIplacementDefaultCard(f, card, score, 0, camera);
	}

	// Initialisation du placement
	state.card = card;
	state.isRuin = isRuin;
	state.hasTwoShapes = card->pieceB != NULL;
	state.hasTwoMat = card->terrainB != 0;
	state.isRiftLands = card->isRiftLands;
	state.RiftLandsMat = 2;
	state.material = card->isEnemy ? MONSTRE : card->terrainA;
	state.pos = (Position){ 6, 6 };
	state.status = 0;


	Piece shapeCopy;

	if (!canFitA && canFitB) copyPiece(card->pieceB, shapeCopy);
	else copyPiece(card->pieceA, shapeCopy);

	Position pos;
	int rotation = 0;
	pos.x = 6;
	pos.y1 = 6;
	int drawable = 0;
	FeuilleCarte feuilleVide, temp;
	float accX = (float)pos.x;
	float accY = (float)pos.y1;

	int done = 0;
	while (!done && !WindowShouldClose()) {

		canFitB = hasTwoShapes ? isRuin ? checkShapeOnRuin(f, card->pieceB) : checkShape(f, card->pieceB) : 0;


		initCarte(feuilleVide, FALSE);
		drawable = drawShape(feuilleVide, shapeCopy, pos, rotation, material);
		tryDraw(f, feuilleVide, temp);



		BeginDrawing(); // Début de l'affichage
		ClearBackground(RAYWHITE);
		BeginMode3D(camera);
		GUIDrawFeuille(f, temp);

		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				if (temp[i][j] != f[i][j])
					DrawCubeWires((Vector3) { (float)i - SIZE / 2, PLACEMENT_HEIGHT, (float)j - SIZE / 2 }, 1.0f, 1.0f, 1.0f, BORDERCOLOR);

		GUIdrawGrille();
		// DrawMapGrid(SIZE, 1.0f);
		EndMode3D();
		// 2D
		DrawRectangle(0, 0, 400, 400, BLACK);
		DrawRectangle(2, 2, 396, 396, RED);
		DrawText(TextFormat("Nom de la carte : %s", card->name), 5, 5, 15, BLACK);
		DrawText(TextFormat("Peut changer de couleur : %s", hasTwoMat ? "oui" : "non"), 5, 25, 15, BLACK);
		DrawText(TextFormat("Peut changer de forme : %d", hasTwoShapes), 5, 45, 15, BLACK);
		DrawText(TextFormat("Doit être placé sur une Ruine : %d", isRuin), 5, 65, 15, BLACK);
		DrawText(TextFormat("Saison en cours : %s", seasons[currentSeason]->name), 5, 85, 15, BLACK);
		DrawText(TextFormat("Edits en cours : %s et %s", edits[seasons[currentSeason]->EditA]->name, edits[seasons[currentSeason]->EditB]->name), 5, 105, 15, BLACK);
		DrawText("tous les édits : ", 5, 125, 15, BLACK);
		for (int i = 0; i < 4; i++) {
			DrawText(TextFormat("Edits %d : %s", i, edits[i]->name), 5, 145 + 20 * i, 15, BLACK);
		}
		DrawText(TextFormat("Piece en A ? %d", card->iscoinA), 5, 225, 15, BLACK);


		DrawRectangle(1400, 0, 200, 200, PURPLE);
		DrawText(TextFormat("Score : %d", score), 1405, 5, 30, BLACK);
		DrawText(TextFormat("Coins : %d", coinCount), 1405, 35, 30, BLACK);




		EndDrawing(); // Fin de l'affichage


		drawable = drawable && isDrawable(f, feuilleVide);
		if (isRuin) {
			drawable = drawable && coversRuin(f, feuilleVide);
		}
		// gestion caméra : toujours au même endraoit
		//UpdateCameraPro(&camera, (Vector3) { 0, 0, 1 }, (Vector3) { 0, 0, 0 }, 1);
		GUIUpdateCustomCamera(&camera);
		camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

		if (IsKeyPressed(SWITCHP) && hasTwoShapes) {
			printf("switch shape : %d \n", canFitB);
			if (compareShape(card->pieceA, shapeCopy) && canFitB) copyPiece(*card->pieceB, shapeCopy);
			else if (canFitA) copyPiece(*card->pieceA, shapeCopy);
		}

		if (IsKeyPressed(SWITCHMP)) {
			if (isRiftLands) {
				RiftLandsMat = ((RiftLandsMat - 1) % 5) + 2;
				material = RiftLandsMat;
			}
			else if (hasTwoMat) {
				if (material == card->terrainA) material = card->terrainB;
				else material = card->terrainA;
			}


		}

		pos.x += -(int)IsKeyPressed(LEFTP) + (int)IsKeyPressed(RIGHTP); // déplacement pièce
		pos.y1 += -(int)IsKeyPressed(UPP) + (int)IsKeyPressed(DOWNP);



		if (IsKeyPressed(KEY_P)) { pos.x = 6; pos.y1 = 6; } //possibilité de resset

		if (IsKeyPressed(ROTATEP)) {
			rotation++;
		}
		if (IsKeyPressed(FLIPP)) {
			flipShape(shapeCopy);
		}
		//printf("rotation %d(%d, %d) \n", rotation, pos.x, pos.y1);
		if (IsKeyPressed(KEY_SPACE) && drawable) {
			done = 1;
		}

	}
	if (WindowShouldClose()) {
		CloseWindow();
		exit(1);
	}

	int mountainBefore = countSurroundedMountains(f);
	draw(f, feuilleVide);
	int mountainAfter = countSurroundedMountains(f);
	coinCount += mountainAfter - mountainBefore;

	if (compareShape(shapeCopy, card->pieceA) && card->iscoinA) coinCount++;

	return 1;

}*/

int GUIPlacementCard(GameState* gs, FeuilleCarte f, const ExploreCard* card,
	int score, int isRuin, int* coinCount, Camera3D camera) {

	// Vérification placabilité
	int canFitA = isRuin ? checkShapeOnRuin(f, card->pieceA) : checkShape(f, card->pieceA);
	int canFitB = card->pieceB
		? (isRuin ? checkShapeOnRuin(f, card->pieceB) : checkShape(f, card->pieceB))
		: 0;

	if (!canFitA && !canFitB)
		return GUIplacementDefaultCard(gs, f, card, score, 0, coinCount, camera);

	// Init état
	PlacementState state = { 0 };
	state.card = card;
	state.isRuin = isRuin;
	state.hasTwoShapes = card->pieceB != NULL;
	state.hasTwoMat = card->terrainB != 0;
	state.isRiftLands = card->isRiftLands;
	state.RiftLandsMat = 2;
	state.material = card->isEnemy ? MONSTRE : card->terrainA;
	state.pos = (Position){ 6, 6 };
	state.status = 0;

	copyPiece(canFitA ? *card->pieceA : *card->pieceB, state.shapeCopy);

	// Boucle principale — logique et rendu séparés
	while (state.status == 0 && !WindowShouldClose()) {
		UpdatePlacement(f, &state, camera);
		RenderPlacement(gs, f, &state, score, camera);
		GUIUpdateCustomCamera(&camera);
		camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	}

	if (WindowShouldClose()) { CloseWindow(); exit(1); }

	ApplyPlacement(f, &state, coinCount);
	return 1;
}

void UpdatePlacement(FeuilleCarte f, PlacementState* state, Camera camera) {

	// Recalcul de la preview
	initCarte(state->feuilleVide, FALSE);
	state->drawable = drawShape(state->feuilleVide, state->shapeCopy,
		state->pos, state->rotation, state->material);
	tryDraw(f, state->feuilleVide, state->temp);
	state->drawable = state->drawable && isDrawable(f, state->feuilleVide);
	if (state->isRuin)
		state->drawable = state->drawable && coversRuin(f, state->feuilleVide);

	Vector3 forward = { camera.target.x - camera.position.x, 0, camera.target.z - camera.position.z };
	normalize(&forward);
	Vector3 right = crossProduct(forward, camera.up);
	normalize(&right);
	
	if (IsKeyPressed(UPP))		fabs(forward.x) > fabs(forward.z) ? (state->pos.x += forward.x > 0 ? 1 : -1) : (state->pos.y += forward.z > 0 ? 1 : -1);
	if (IsKeyPressed(DOWNP))	fabs(forward.x) > fabs(forward.z) ? (state->pos.x += forward.x > 0 ? -1 : 1) : (state->pos.y += forward.z > 0 ? -1 : 1);
	if (IsKeyPressed(LEFTP))	fabs(right.x) > fabs(right.z) ? (state->pos.x += right.x > 0 ? -1 : 1) : (state->pos.y += right.z > 0 ? -1 : 1);
	if (IsKeyPressed(RIGHTP))	fabs(right.x) > fabs(right.z) ? (state->pos.x += right.x > 0 ? 1 : -1) : (state->pos.y += right.z > 0 ? 1 : -1);

	// Inputs — déplacement
	//state->pos.x += -(int)IsKeyPressed(LEFTP) + (int)IsKeyPressed(RIGHTP);
	//state->pos.y += -(int)IsKeyPressed(UPP) + (int)IsKeyPressed(DOWNP);

	if (IsKeyPressed(KEY_P)) { state->pos.x = 6; state->pos.y = 6; }

	// Rotation / flip
	if (IsKeyPressed(ROTATEP)) state->rotation++;
	if (IsKeyPressed(FLIPP))   flipShape(state->shapeCopy);

	// Switch de forme
	if (IsKeyPressed(SWITCHP) && state->hasTwoShapes) {
		int canFitA = state->isRuin
			? checkShapeOnRuin(f, state->card->pieceA)
			: checkShape(f, state->card->pieceA);
		int canFitB = state->isRuin
			? checkShapeOnRuin(f, state->card->pieceB)
			: checkShape(f, state->card->pieceB);
		if (compareShape(state->card->pieceA, state->shapeCopy) && canFitB)
			copyPiece(*state->card->pieceB, state->shapeCopy);
		else if (canFitA)
			copyPiece(*state->card->pieceA, state->shapeCopy);
	}

	// Switch de matériau
	if (IsKeyPressed(SWITCHMP)) {
		if (state->isRiftLands) {
			state->RiftLandsMat = ((state->RiftLandsMat - 1) % 5) + 2;
			state->material = state->RiftLandsMat;
		}
		else if (state->hasTwoMat) {
			state->material = (state->material == state->card->terrainA)
				? state->card->terrainB : state->card->terrainA;
		}
	}

	// Confirmation
	if (IsKeyPressed(KEY_SPACE) && state->drawable)
		state->status = 1;
}

void RenderPlacement(GameState* gs, FeuilleCarte f, const PlacementState* state, int score, Camera3D camera) {
	BeginDrawing();
	ClearBackground(BACKGROUND_COLOR);
	BeginMode3D(camera);

	GUIDrawFeuille(f, state->temp);

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (state->temp[i][j] != f[i][j])
				DrawCubeWires((Vector3) { (float)i - SIZE / 2, PLACEMENT_HEIGHT, (float)j - SIZE / 2 }, 1.0f, 1.0f, 1.0f, BORDERCOLOR);

	GUIdrawGrille();
	EndMode3D();

	// UI 2D — lecture seule sur state

	// Infos relatives à tous les joueurs
	DrawRectangle(0, 0, 400, 320, RED);
	DrawRectangle(2, 2, 396, 316, RAYWHITE);
	int y1 = 5;
	DrawText(TextFormat("Carte : %s", state->card->name), 5, y1, 15, BLACK); y1 += 20;
	DrawText(TextFormat("2 formes : %d", state->hasTwoShapes), 5, y1, 15, BLACK); y1 += 20;
	DrawText(TextFormat("2 matériaux : %d", state->hasTwoMat), 5, y1, 15, BLACK); y1 += 20;
	DrawText(TextFormat("Saison : %s", seasons[gs->currentSeason]->name), 5, y1, 15, BLACK); y1 += 20;
	DrawText(TextFormat("Edits : %s / %s", gs->edits[seasons[gs->currentSeason]->EditA]->name, gs->edits[seasons[gs->currentSeason]->EditB]->name), 5, y1, 15, BLACK); y1 += 20;
	for (int i = 0; i < 4; i++) {
		DrawText(TextFormat("Edit %d : %s", i, gs->edits[i]->name), 5, y1, 15, BLACK); y1 += 20;
	}
	if (state->isRuin) DrawText("Doit être placé sur une Ruine", 5, y1, 20, RED); y1 += 25;
	if (state->isRiftLands) DrawText("Tous matériaux disponibles !", 5, y1, 20, RED); y1 += 25;

	// Infos relatives au joueur actuel
	int y2 = 5;
	DrawRectangle(1400, 0, 200, 140, PURPLE);
	DrawText("Joueur : ", 1405, y2, 30, BLACK); y2 += 35;
	DrawText(gs->players[gs->playerIndex].name, 1405, y2, 30, BLACK); y2 += 35;
	DrawText(TextFormat("Score : %d", score), 1405, y2, 30, BLACK); y2 += 35;
	DrawText(TextFormat("Coins : %d", gs->players[gs->playerIndex].coinCount), 1405, y2, 30, BLACK); y2 += 35;




	EndDrawing();
}

void ApplyPlacement(FeuilleCarte f, PlacementState* state, int* coinCount) {
	int mountainBefore = countSurroundedMountains(f);
	draw(f, state->feuilleVide);
	int mountainAfter = countSurroundedMountains(f);
	*coinCount += mountainAfter - mountainBefore;
	if (compareShape(state->shapeCopy, *state->card->pieceA) && state->card->iscoinA)
		(*coinCount)++;
}



void DrawMapGrid(int slices, float spacing) {
	float halfSize = (slices * spacing) / 2.0f;
	Color gridColor = LIGHTGRAY; // Couleur par défaut pour rester cohérent

	for (int i = 0; i <= slices; i++) {
		float pos = -halfSize + i * spacing;
		// Lignes parallèles à l'axe X puis Z
		DrawLine3D((Vector3) { -halfSize, 0.0f, pos }, (Vector3) { halfSize, 0.0f, pos }, gridColor);
		DrawLine3D((Vector3) { pos, 0.0f, -halfSize }, (Vector3) { pos, 0.0f, halfSize }, gridColor);
	}
}

int GUIplacementDefault(FeuilleCarte f, int  material, Camera3D camera) {
	if (getEmptySpots(f) == 0) return 0;
	GUIplacementShape(f, POINT, material, camera);
	return 1;
}

int GUIplacementDefaultCard(GameState* gs, FeuilleCarte f, const ExploreCard* card, int score, int isRuin, int* coinCount, Camera3D camera) {
	if (getEmptySpots(f) == 0) return 0;
	ExploreCard def = *card;
	def.pieceA = &POINT;
	def.iscoinA = 0;

	GUIPlacementCard(gs, f, &def, score, isRuin, coinCount, camera);
	return 1;
}


void GUIdrawGrille() {
	for (int i = -SIZE / 2 - 1; i <= SIZE / 2; i++) {
		DrawLine3D((Vector3) { (float)i + 0.5f, 0.0f, (float)-SIZE / 2 }, (Vector3) { (float)i + 0.5f, 0.0f, (float)SIZE / 2 }, GRIDCOLOR);
		DrawLine3D((Vector3) { (float)-SIZE / 2, 0.0f, (float)i + 0.5f }, (Vector3) { (float)SIZE / 2, 0.0f, (float)i + 0.5f }, GRIDCOLOR);
	}
}

void GUIUpdateCustomCamera(Camera3D* camera) {
	// vers le haut/bas

	Vector3 newPos;
	newPos = camera->position;
	double rhoCam = sqrt(pow(camera->position.x, 2) + pow(camera->position.y, 2) + pow(camera->position.z, 2));
	Vector3 mouvement = (Vector3){ camera->up.x * ((int)IsKeyDown(UPC) - (int)IsKeyDown(DOWNC)), camera->up.y * ((int)IsKeyDown(UPC) - (int)IsKeyDown(DOWNC)), camera->up.z * ((int)IsKeyDown(UPC) - (int)IsKeyDown(DOWNC)) };
	multiplyVector(&mouvement, GetFrameTime() * SPEEDY);
	multiplyVector(&mouvement, 1 + abs((int)camera->position.y / 2));//compensation de vitesse
	newPos = addVectors(newPos, mouvement);//calcule la nouvelle position en ajoutant le vecteur déplacement

	normalize(&newPos);//normalise le vecteur de position

	if (newPos.y > 0.0f && newPos.y < (float)MAXCAMERAHEIGHT) {// évite les postions négatives
		camera->position.x = newPos.x * rhoCam;// replace la caméra à son écart cible
		camera->position.y = newPos.y * rhoCam;
		camera->position.z = newPos.z * rhoCam;
	}



	//gauche droite :
	Vector3 sideVect = crossProduct(camera->position, camera->up);

	normalize(&sideVect);

	newPos = camera->position;
	mouvement = (Vector3){ sideVect.x * ((int)IsKeyDown(LEFTC) - (int)IsKeyDown(RIGHTC)), sideVect.y * ((int)IsKeyDown(LEFTC) - (int)IsKeyDown(RIGHTC)), sideVect.z * ((int)IsKeyDown(LEFTC) - (int)IsKeyDown(RIGHTC)) };
	multiplyVector(&mouvement, GetFrameTime() * SPEEDX);



	newPos = addVectors(newPos, mouvement);

	normalize(&newPos);//normalise le vecteur de position

	camera->position.x = newPos.x * rhoCam;// replace la caméra à son écart cible
	camera->position.y = newPos.y * rhoCam;
	camera->position.z = newPos.z * rhoCam;




}


bool DisplayMenu(GameState* gs) {
	int i = 0;
	char title[16] = "Cartographers !";		int titleFontSize = 60;
	Rectangle menuBounds = (Rectangle){ 0 };

	int d = 40; // Espacement entre les boutons
	Button startBtn = { .color1 = LIME, .color2 = GRAY, .label = "Start !", .fontSize = 40, .corner = 20, .stroke = 4 };
	bool addPlayer = 0;
	char tmppl[33] = "Limite de 100 personnes atteinte";	int tmpplSize = 70;
	Button addBtn = { .color1 = SKYBLUE, .color2 = DARKBLUE, .label = "Add 1 Player", .fontSize = 40, .corner = 20, .stroke = 4 };
	//char nbPlayers[]
	Button stopBtn = { .color1 = RED, .color2 = DARKGRAY, .label = "Exit The Game", .fontSize = 35, .corner = 20, .stroke = 4 };

	while (!WindowShouldClose() && !startBtn.validated && !addPlayer && !stopBtn.validated) {
		menuBounds = (Rectangle){ (GetScreenWidth() - MENUX) / 2 , (GetScreenHeight() - MENUY) / 2 ,  MENUX, MENUY };

		startBtn.bounds = (Rectangle){ menuBounds.x + 25, menuBounds.y + titleFontSize + 80, MENUX - 50, startBtn.fontSize + 40 };
		startBtn.hovered = CheckCollisionPointRec(GetMousePosition(), startBtn.bounds);

		addBtn.bounds = (Rectangle){ startBtn.bounds.x, startBtn.bounds.y + startBtn.bounds.height + d, MENUX - 50, startBtn.fontSize + 40 };
		addBtn.hovered = CheckCollisionPointRec(GetMousePosition(), addBtn.bounds);

		stopBtn.bounds = (Rectangle){ startBtn.bounds.x, addBtn.bounds.y + addBtn.bounds.height + d, MENUX - 50, startBtn.fontSize + 40 };
		stopBtn.hovered = CheckCollisionPointRec(GetMousePosition(), stopBtn.bounds);

		BeginDrawing();
		ClearBackground(BGCOLOR);

		// Dessine un grand rectangle en tant que menu
		DrawRectangleRounded(menuBounds, .2f, 10, BLACK);
		DrawRectangleRoundedLinesEx(menuBounds, .2f, 10, 3, RED);
		DrawText(title, menuBounds.x + MENUX / 2 - MeasureText(title, titleFontSize) / 2, menuBounds.y + 3, titleFontSize, WHITE);

		// Ajoute un bouton pour lancer la partie
		DrawButton(&startBtn);
		if (startBtn.validated) printf("Lancement de la partie");

		// Ajoute un bouton pour ajouter un joueur
		DrawButton(&addBtn);
		if (gs->playerNumber >= MAX_PLAYER) DrawStrokeText(tmppl, (GetScreenWidth() - MeasureText(tmppl, tmpplSize)) / 2, menuBounds.y - tmpplSize - 20, tmpplSize, RED, multiplyColor(DARKGRAY, 0.4f));
		else if (addBtn.validated) addPlayer = 1;

		// afficher le nombre de joueurs
		DrawText(TextFormat("Il y a %s%d joueurs sur %d", gs->playerNumber > 10 ? "déjà " : "", gs->playerNumber, MAX_PLAYER), addBtn.bounds.x, addBtn.bounds.y + addBtn.bounds.height + 5, 20, WHITE);

		// Ajoute un bouton pour quitter le Jeu
		DrawButton(&stopBtn);

		// Online : Ajoute un bouton pour rejoindre et quitter le lobby


		EndDrawing();
	}
	if (startBtn.validated) {
		printf("\n\nBouton Start Validé");
		return true;
	}
	if (addPlayer) {
		printf("\n\nBouton Add Validé");
		return AddPlayer(gs);
	}
	if (stopBtn.validated || WindowShouldClose()) {
		printf("\n\nBouton Stop Validé");
		return false;
	}
}

bool AddPlayer(GameState* gs) {
	int middleX = 0;
	int middleY = 0;
	char title[16] = "Cartographers !"; 		int titleFontSize = 60;
	Rectangle menuBounds = (Rectangle){ 0 };
	char addPplLabel[19] = "Add a new player !"; 	int addPplFontSize = 50;
	Rectangle addMBounds = (Rectangle){ 0 };
	Rectangle iptNameBox = (Rectangle){ 0 };
	int d = 40; // Espacement entre les boutons

	Button cclBtn = { .color1 = RED, .color2 = GRAY, .label = "Cancel", .fontSize = 40, .corner = 30, .stroke = 3 };
	Button addBtn = { .color1 = LIME, .color2 = GRAY, .label = "Add", .fontSize = 40, .corner = 30, .stroke = 3 };

	InputBox nameIptBox = { .maxLength = MAX_NAME_LENGTH,.dx = 5, .text = "", .length = 0, .fontSize = 20, .active = 1, .validated = 0 };
	char addLabel[19] = "New player name : ";		int addLabelFontSize = 32;

	bool add = false;

	while (!WindowShouldClose() && !add && !cclBtn.validated) {
		middleX = GetScreenWidth() / 2;
		middleY = GetScreenHeight() / 2;

		menuBounds = (Rectangle){ middleX - MENUX / 2 , middleY - MENUY / 2 ,  MENUX, MENUY };
		addMBounds = (Rectangle){ menuBounds.x + 10 ,  menuBounds.y + titleFontSize + 50,  MENUX - 20, 300 };

		cclBtn.bounds = (Rectangle){ addMBounds.x + 5, addMBounds.y + addMBounds.height - (cclBtn.fontSize + 30) - 5, addMBounds.width / 2 - 10, cclBtn.fontSize + 30 };
		cclBtn.hovered = CheckCollisionPointRec(GetMousePosition(), cclBtn.bounds);

		addBtn.bounds = (Rectangle){ middleX + 5, addMBounds.y + addMBounds.height - (addBtn.fontSize + 30) - 5, addMBounds.width / 2 - 10, addBtn.fontSize + 30 };
		addBtn.hovered = CheckCollisionPointRec(GetMousePosition(), addBtn.bounds);

		nameIptBox.bounds = (Rectangle){ addMBounds.x + 5 + MeasureText(addLabel, addLabelFontSize) + 5 + 2, addMBounds.y + addPplFontSize + 30 + 5, addMBounds.width - (5 + MeasureText(addLabel, addLabelFontSize) + 5) - 15, nameIptBox.fontSize + 2 };
		nameIptBox.hovered = CheckCollisionPointRec(GetMousePosition(), nameIptBox.bounds);


		// Mise à jour du curseur
		if (nameIptBox.hovered) {
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
		}
		else if (cclBtn.hovered || addBtn.hovered) {
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
		}
		else {
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}

		BeginDrawing();
		ClearBackground(BGCOLOR);
		DrawRectangleRoundedStrokeEx(menuBounds, .2f, 10, 3, BLACK, RED);
		DrawText(title, middleX - MeasureText(title, titleFontSize) / 2, menuBounds.y, titleFontSize, WHITE);

		// Afficher le menu d'ajout
		DrawRectangleRoundedStrokeEx(addMBounds, .1f, 10, 3, BROWN, DARKGRAY);
		DrawText(addPplLabel, middleX - MeasureText(addPplLabel, addPplFontSize) / 2, addMBounds.y + 3, addPplFontSize, BLACK);

		DrawText(addLabel, addMBounds.x + 5, addMBounds.y + addPplFontSize + 30, addLabelFontSize, BLACK);
		DrawFullInputBoxEx(&nameIptBox, 2, GRAY, LIGHTGRAY);

		DrawButton(&cclBtn);
		DrawButton(&addBtn);


		EndDrawing();

		add = (addBtn.validated || nameIptBox.validated);
	}
	if (add) {
		strcpy(gs->players[gs->playerNumber++].name, nameIptBox.text);
	}
	return DisplayMenu(gs);
}

void DrawButton(Button* btn) {
	Vector2 mouse = GetMousePosition();
	bool pressed = btn->hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	bool clicked = btn->hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

	/*if (btn->hovered) printf("Button hovered\n");
	if (pressed) printf("Button Pressed\n");
	if (clicked) printf("Button Clicked\n");*/

	Rectangle r = btn->bounds;
	if (pressed) {
		r.y += 5;
		r.x += 5;
		r.width -= 10;
		r.height -= 10;
	}

	// Ombre portée
	int dShadow = 7;
	DrawRectangleRounded((Rectangle) { r.x + dShadow - btn->stroke, r.y + dShadow - btn->stroke, r.width + 2 * btn->stroke, r.height + 2 * btn->stroke }, btn->corner * 0.01f, 8, (Color) { 0, 0, 0, 100 });

	// assombrir color1 au clic, éclaircir au hover
	Color bg = pressed ? multiplyColor(btn->color1, 0.7f) : btn->hovered ? multiplyColor(btn->color1, 1.2f) : btn->color1;
	if (btn->stroke > 0) {
		Color strokeC = pressed ? multiplyColor(btn->color2, 0.7f) : btn->hovered ? multiplyColor(btn->color2, 1.2f) : btn->color2;
		DrawRectangleRoundedStrokeEx(r, btn->corner * 0.01f, 8, btn->stroke, bg, strokeC);
	}
	else DrawRectangleRounded(r, btn->corner * 0.01f, 8, bg);


	// Label centré
	int fontSize = pressed ? btn->fontSize - 4 : btn->fontSize;
	DrawText(btn->label, r.x + (r.width - MeasureText(btn->label, fontSize)) / 2, r.y + (r.height - fontSize) / 2, fontSize, WHITE);

	btn->validated = clicked;
}

void UpdateInputBox(InputBox* box) {

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))	box->active = box->hovered;

	if (!box->active) return;

	if (IsKeyPressed(KEY_ENTER) && box->length > 0) {
		printf("entré appuyé 1\n");
		box->validated = true;
	}
	else	box->validated = false;


	int key = GetCharPressed();
	while (key > 0) {
		printf("key reçue : %d\n", key);
		if (IsCharAllowed((char)key) && box->length < box->maxLength - 1) {
			box->text[box->length++] = (char)key;
			box->text[box->length] = '\0';
		}
		key = GetCharPressed();
	}

	if (IsKeyPressed(KEY_BACKSPACE) && box->length > 0)
		box->text[--box->length] = '\0';

}

bool IsCharAllowed(char c) {
	return (c >= 'a' && c <= 'z') || // minuscules
		(c >= 'A' && c <= 'Z') ||	 // majuscules
		(c >= '0' && c <= '9') ||	 // chiffres
		(c == '-') ||				 // tiret
		(c == '\'') ||				 // apostrophe
		(c == ' ');					 // espace
}

void DrawFullInputBoxEx(InputBox* box, int stroke, Color bkgColor, Color strokeColor) {

	UpdateInputBox(box);

	// Fond + contour
	DrawRectangleRec(box->bounds, box->active ? multiplyColor(bkgColor, 0.8f) : bkgColor);
	if (stroke > 0)
		DrawRectangleLinesEx(box->bounds, stroke, strokeColor);

	// Texte avec décalage dx et centralisation du texte en y
	DrawText(box->text, box->bounds.x + box->dx, box->bounds.y + (box->bounds.height - box->fontSize) / 2, box->fontSize, BLACK);

	// Curseur clignotant centré en y
	if (box->active && ((int)(GetTime() * 2) % 2 == 0)) {
		int tw = MeasureText(box->text, box->fontSize);
		DrawText("|", box->bounds.x + box->dx + tw + 2, box->bounds.y + (box->bounds.height - box->fontSize) / 2, box->fontSize, DARKGRAY);
	}

	if (box->validated)	printf("entré appuyé 2\n");

}

void DrawStrokeTextEx(const char* text, int x, int y, int fontSize, Color textColor, Color strokeColor, int thickness) {
	for (int dx = -thickness; dx <= thickness; dx++)	for (int dy = -thickness; dy <= thickness; dy++)
		if (dx != 0 || dy != 0)		DrawText(text, x + dx, y + dy, fontSize, strokeColor);

	DrawText(text, x, y, fontSize, textColor);
}

void DrawStrokeText(const char* text, int x, int y, int fontSize, Color textColor, Color strokeColor) {
	DrawStrokeTextEx(text, x, y, fontSize, textColor, strokeColor, 1);
}

void DrawRectangleRoundedStroke(Rectangle rec, float roundness, int segments, Color rectColor, Color strokeColor) {
	DrawRectangleRoundedStrokeEx(rec, roundness, segments, 2, rectColor, strokeColor);
}

void DrawRectangleRoundedStrokeEx(Rectangle rec, float roundness, int segments, float lineThick, Color rectColor, Color strokeColor) {
	DrawRectangleRounded(rec, roundness, segments, rectColor);
	DrawRectangleRoundedLinesEx(rec, roundness, segments, lineThick, strokeColor);
}

Color multiplyColor(Color color, float factor) {
	return (Color) { fminf(color.r * factor, 255), fminf(color.g * factor, 255), fminf(color.b * factor, 255), color.a };
}



/****************************************Opérations de vecteurs**************************/

void normalize(Vector3* vector) {//normalise un vecteur
	double rho = sqrt(pow(vector->x, 2) + pow(vector->y, 2) + pow(vector->z, 2));
	vector->x /= rho;
	vector->y /= rho;
	vector->z /= rho;
}

Vector3 addVectors(Vector3 vectora, Vector3 vectorb) {
	return (Vector3) { vectora.x + vectorb.x, vectora.y + vectorb.y, vectora.z + vectorb.z };

}

Vector3 crossProduct(Vector3 vectora, Vector3 vectorb) {
	return (Vector3) { vectora.y* vectorb.z - vectora.z * vectorb.y, vectora.z* vectorb.x - vectora.x * vectorb.z, vectora.x* vectorb.y - vectora.y * vectorb.x };

}
void multiplyVector(Vector3* vector, double a) {
	vector->x *= a;
	vector->y *= a;
	vector->z *= a;
}