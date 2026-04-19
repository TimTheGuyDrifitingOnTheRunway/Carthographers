#include "Lib3d.h"


/**************************************************Fontions jeux******************************************/

void GUIDrawFeuille(FeuilleCarte f, FeuilleCarte temp, Model mountains[], Position moutainPos[NOMBREMONTAGNE], Seed s, ModelList models) {
	Image treeImage = s.treeImage;
	int nb = 0;

	//rendu de skybox:

	rlDisableBackfaceCulling();
	rlDisableDepthMask();
	DrawModel(models.skybox, (Vector3) { 0, 0, 0 }, 1.0f, WHITE);
	rlEnableBackfaceCulling();
	rlEnableDepthMask();


	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {

#ifdef DEBUG_FORET
			{
				float x = i - (SIZE - 1) / 2.0f;
				float z = j - (SIZE - 1) / 2.0f;
				float y = -1;   // Half height so cube sits on grid
				for (int k = 0; k < TREE_DIVIDER + 1; k++) for (int l = 0; l < TREE_DIVIDER + 1; l++) {

					DrawCube((Vector3) { x - 0.5f + (float)k / TREE_DIVIDER, y + 0.9, z - 0.5 + (float)l / TREE_DIVIDER }, 0.1f, 0.1, 0.1f, GetImageColor(treeImage, j * 10 + l, i * 10 + k));
				}
			}
#else // DEBUG_FORET

			//couleur du fond de carte
			{
				float x = i - (SIZE - 1) / 2.0f;
				float z = j - (SIZE - 1) / 2.0f;
				float y = -1;
				for (int k = 0; k < TREE_DIVIDER + 1; k++) for (int l = 0; l < TREE_DIVIDER + 1; l++) {
					float lum = ColorToHSV(GetImageColor(treeImage, j * 10 + l, i * 10 + k)).z;
					unsigned char blue = (pow((lum), WATER_POWER) + WATER_OFSET) * 255 > 254 ? 254 : ((pow((lum), WATER_POWER) + WATER_OFSET) * 255 < 50) ? 50 : (pow((lum), WATER_POWER) + WATER_OFSET) * 255;

					Color c = (Color){ blue * WATER_RED_FACTOR, blue * WATER_GREEN_FACTOR, blue, 255 };
					DrawCube((Vector3) { x - 0.5f + (float)k / TREE_DIVIDER, y + 0.9 + lum / 2, z - 0.5 + (float)l / TREE_DIVIDER }, 0.1f, 0.5 * lum, 0.1f, c);
				}
			}
#endif



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
					for (int k = 0; k < TREE_DIVIDER + 1; k++) for (int l = 0; l < TREE_DIVIDER + 1; l++) {
						Vector3 color;
						color = ColorToHSV(GetImageColor(treeImage, j * 10 + l, i * 10 + k));


						unsigned char green = (pow((color.z), GREEN_POWER) + GREEN_OFSET) * 255 > 254 ? 254 : ((pow((color.z), GREEN_POWER) + GREEN_OFSET) * 255 < 50) ? 50 : (pow((color.z), GREEN_POWER) + GREEN_OFSET) * 255;
						Color c = (Color){ (unsigned char)20, green, (unsigned char)10, 255 };//couleur de base pour les arbres
						Color c2 = (Color){ (unsigned char)150, green, (unsigned char)10, 255 };//couleur de base pour les arbres
						Color c3 = (Color){ green, green * 0.8f ,(unsigned char)50 , 255 };//couleur de base pour les arbres



						//choix du type de model à dessiner
						if (color.z < FOREST_TRESHOLD) DrawModelEx(models.tree, (Vector3) { x - 0.5f + (float)k / TREE_DIVIDER, y + TREE_Y_OFSET, z - 0.5 + (float)l / TREE_DIVIDER }, (Vector3) { 1, 0, 0 }, 0, (Vector3) { TREE_SIZE, TREE_SIZE, TREE_SIZE }, c2);

						else if ((color.z > FOREST_TRESHOLD) && color.z < BUSH_TRESHOLD) DrawModelEx(models.buisson, (Vector3) { x - 0.5f + (float)k / TREE_DIVIDER, y + 0.5, z - 0.5 + (float)l / TREE_DIVIDER }, (Vector3) { 1, 0, 0 }, 0, (Vector3) { BUSH_SIZE, BUSH_SIZE, BUSH_SIZE }, c);

						else DrawModelEx(models.buisson, (Vector3) { x - 0.5f + (float)k / TREE_DIVIDER, y + 0.5, z - 0.5 + (float)l / TREE_DIVIDER }, (Vector3) { 1, 0, 0 }, 0, (Vector3) { BUSH_SIZE, BUSH_SIZE * 1.2f, BUSH_SIZE }, c3);

					}
					break;
				case VILLAGE:
					color = BROWN;
					break;
				case CHAMPS:
					color = YELLOW;
					break;
				case MONTAGNE:
					color = GRAY;
					DrawModel(mountains[nb], (Vector3) { x - 0.5, 0.5f + y, z - 0.5 }, 1, GRAY);
					nb++;
					break;
				case MONSTRE:
					color = PURPLE;
					break;
				case CONFLICTVALUE:
					color = RED;
					for (int k = 0; k < NOMBREMONTAGNE; k++) {
						if (moutainPos[k].x == i && moutainPos[k].y == j) {
							nb++;//avance le compte montagne si collision avec une montagne pour ne pas faire spawn une montagne sur une autre

						}
					}
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


int GUIplacementShape(FeuilleCarte f, const Piece* shape, int material, Camera3D camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models) {
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
		Position* mountainPos = getPositionsOfMaterial(f, MONTAGNE);//envoi la positions des montagnes pour avancer si un conflit à lieu entre une montagne et autre
		while (!done && !WindowShouldClose()) {
			initCarte(feuilleVide, FALSE);
			drawable = drawShape(feuilleVide, shapeCopy, pos, rotation, material);
			tryDraw(f, feuilleVide, temp);

			BeginDrawing(); // Début de l'affichage
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
			GUIDrawFeuille(f, temp, mountains, mountainPos, s, models);

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
		return GUIplacementDefault(f, material, camera, mountains, s, models);
	}
}



int GUIPlacementCard(GameState* gs, FeuilleCarte f, const ExploreCard* card,
	int score, int isRuin, int* coinCount, Camera3D camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models) {

	// Vérification placabilité
	int canFitA = isRuin ? checkShapeOnRuin(f, card->pieceA) : checkShape(f, card->pieceA);
	int canFitB = card->pieceB ? (isRuin ? checkShapeOnRuin(f, card->pieceB) : checkShape(f, card->pieceB)) : 0;

	if (!canFitA && !canFitB)
		return GUIplacementDefaultCard(gs, f, card, score, 0, coinCount, camera, mountains, s, models);

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
	Position* mountainPos = getPositionsOfMaterial(f, MONTAGNE);//envoi la positions des montagnes pour avancer si un conflit à lieu entre une montagne et autre
	// Boucle principale — logique et rendu séparés
	while (state.status == 0 && !WindowShouldClose()) {
		UpdatePlacement(f, &state, camera);
		RenderPlacement(gs, f, &state, score, camera, mountains, mountainPos, s, models);
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

void RenderPlacement(GameState* gs, FeuilleCarte f, const PlacementState* state, int score, Camera3D camera, Model mountain[NOMBREMONTAGNE], Position mountainPos[NOMBREMONTAGNE], Seed s, ModelList models) {
	int midX = GetScreenWidth() / 2;
	int midY = GetScreenHeight() / 2;

	Rectangle infoPanel = { 0, midY - 700 / 2, 400, 700 };
	Rectangle playerPanel = { midX * 2 - 400, 0, 400, 250 };

	BeginDrawing();
	ClearBackground(BACKGROUND_COLOR);


	BeginMode3D(camera);


	GUIDrawFeuille(f, state->temp, mountain, mountainPos, s, models);

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (state->temp[i][j] != f[i][j])
				DrawCubeWires((Vector3) { (float)i - SIZE / 2, PLACEMENT_HEIGHT, (float)j - SIZE / 2 }, 1.0f, 1.0f, 1.0f, BORDERCOLOR);




	GUIdrawGrille();
	EndMode3D();

	// UI 2D — lecture seule sur state

	// Infos relatives à tous les joueurs
	DrawRectangleStroke(infoPanel, 3, WHITE, RED);
	int y1 = 5;
	DrawText(TextFormat("Carte : %s", state->card->name), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	DrawText(TextFormat("2 formes : %d", state->hasTwoShapes), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	DrawText(TextFormat("2 matériaux : %d", state->hasTwoMat), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	DrawText(TextFormat("Saison : %s", seasons[gs->currentSeason]->name), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	DrawText(TextFormat("Edits : %s / %s", gs->edits[seasons[gs->currentSeason]->EditA]->name, gs->edits[seasons[gs->currentSeason]->EditB]->name), 5, infoPanel.y + y1, 15, BLACK); y1 += 20;
	for (int i = 0; i < 4; i++) {
		DrawText(TextFormat("Edit %d : %s", i, gs->edits[i]->name), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	}
	if (state->isRuin) DrawText("Doit être placé sur une Ruine", 5, infoPanel.y + y1, 30, RED); y1 += 35;
	if (state->isRiftLands) DrawText("Tous matériaux disponibles !", 5, infoPanel.y + y1, 30, RED); y1 += 35;

	// Infos relatives au joueur actuel
	int y2 = 5;
	DrawRectangleStroke(playerPanel, 3, SKYBLUE, DARKBLUE);
	DrawText("Joueur : ", playerPanel.x + 5, y2, 30, BLACK); y2 += 35;
	DrawText(gs->players[gs->playerIndex].name, playerPanel.x + 5, y2, 30, BLACK); y2 += 35;
	DrawText(TextFormat("Score : %d", score), playerPanel.x + 5, y2, 30, BLACK); y2 += 35;
	DrawText(TextFormat("Coins : %d", gs->players[gs->playerIndex].coinCount), playerPanel.x + 5, y2, 30, BLACK); y2 += 35;




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

int GUIplacementDefault(FeuilleCarte f, int  material, Camera3D camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models) {
	if (getEmptySpots(f) == 0) return 0;
	GUIplacementShape(f, POINT, material, camera, mountains, s, models);
	return 1;
}

int GUIplacementDefaultCard(GameState* gs, FeuilleCarte f, const ExploreCard* card, int score, int isRuin, int* coinCount, Camera3D camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models) {
	if (getEmptySpots(f) == 0) return 0;
	ExploreCard def = *card;
	def.pieceA = &POINT;
	def.iscoinA = 0;

	GUIPlacementCard(gs, f, &def, score, isRuin, coinCount, camera, mountains, s, models);
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


/*****************génération de heighmap et models ***/


Model generateMountain(int x, int y) {
	// pour éviter d'avoir toujours la même montagne au lancement du jeu
	Image perlinNoise = GenImagePerlinNoise(PERLIN_SIZE, PERLIN_SIZE, x * 100, y * 100, PERLIN_SCALE);


	for (int y = 0; y < PERLIN_SIZE; y++) {// fallof pour avoir des bords smooths
		for (int x = 0; x < PERLIN_SIZE; x++) {

			float nx = (float)x / (float)PERLIN_SIZE * 2.0f - 1.0f;
			float ny = (float)y / (float)PERLIN_SIZE * 2.0f - 1.0f;

			float fx = 1.0f - powf(fabsf((float)x / PERLIN_SIZE * 2.0f - 1.0f), PERLIN_MODEL_SMOOTHING);
			float fy = 1.0f - powf(fabsf((float)y / PERLIN_SIZE * 2.0f - 1.0f), PERLIN_MODEL_SMOOTHING);
			float falloff = fx * fy;

			if (falloff < 0) falloff = 0;

			Color c = GetImageColor(perlinNoise, x, y);
			float height = (float)c.r / 255.0f;

			height *= falloff;

			unsigned char h = (unsigned char)(height * 255.0f);
			ImageDrawPixel(&perlinNoise, x, y, (Color) { h, h, h, 255 });
		}
	}


	ImageDrawLineV(&perlinNoise, (Vector2) { 0, 0 }, (Vector2) { 0, PERLIN_SIZE }, MOUNTAIN_MODEL_COLOR);
	ImageDrawLineV(&perlinNoise, (Vector2) { PERLIN_SIZE - 1, 0 }, (Vector2) { PERLIN_SIZE - 1, PERLIN_SIZE }, MOUNTAIN_MODEL_COLOR);// bordure noire pour éviter les artefacts de texture sur les bords du modèle
	ImageDrawLineV(&perlinNoise, (Vector2) { 0, 0 }, (Vector2) { PERLIN_SIZE, 0 }, MOUNTAIN_MODEL_COLOR);
	ImageDrawLineV(&perlinNoise, (Vector2) { 0, PERLIN_SIZE - 1 }, (Vector2) { PERLIN_SIZE, PERLIN_SIZE - 1 }, MOUNTAIN_MODEL_COLOR);

	Mesh mesh = GenMeshHeightmap(perlinNoise, (Vector3) { MOUNTAIN_MODEL_SIZE, MOUNTAIN_MODEL_HEIGHT, MOUNTAIN_MODEL_SIZE }); // Generate heightmap mesh (RAM and VRAM)
	Model model = LoadModelFromMesh(mesh);

	ImageColorBrightness(&perlinNoise, 100);//redresse la couleur des montagnes

	Texture2D texture = LoadTextureFromImage(perlinNoise);




	// Load model from generated mesh

	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set map diffuse texture          // Define model position

	UnloadImage(perlinNoise);             // Unload heightmap image from RAM, already uploaded to VRAM

	return model;


}

void generateMountainsModels(Model mountains[NOMBREMONTAGNE], FeuilleCarte f, int mountainSeed[2]) {
	Position* pos = getPositionsOfMaterial(f, MONTAGNE);
	for (int i = 0; i < NOMBREMONTAGNE; i++) {
		mountains[i] = generateMountain(pos[i].x + mountainSeed[1], pos[i].y + mountainSeed[0]);
	}
}

Image generateForestImage(int x, int y) {
	Image perlinNoise = GenImagePerlinNoise(FORET_SIZE, FORET_SIZE, x * 100, y * 100, FORET_SCALE);

	return perlinNoise;

}



ModelList loadModels() {
	ModelList models;
	printf("Loading models \n");
	models.tree = LoadModel(PATH_TO_TREE_MODEL);
	models.buisson = LoadModel(PATH_TO_BUSH_MODEL);
	models.skybox = loadSkybox(false);
	//Texture2D texture = LoadTexture("resources/models/iqm/guytex.png");         // Load model texture and set material
	//SetMaterialTexture(&(models.tree).materials[0], MATERIAL_MAP_DIFFUSE, texture);
	return models;
}


Model loadSkybox(bool useHDR) {
	Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
	Model skybox = LoadModelFromMesh(cube);

	// Set this to true to use an HDR Texture
	// NOTE: raylib must be built with HDR Support for this to work: SUPPORT_FILEFORMAT_HDR


	// Load skybox shader and set required locations
	// NOTE: Some locations are automatically set at shader loading
	skybox.materials[0].shader = LoadShader(TextFormat(SKYBOX_SHADER_PATH, GLSL_VERSION),
		TextFormat(SKYBOX_SHADER_PATH2, GLSL_VERSION));

#define SKYBOX_SHADER_PATH "Assets/shaders/glsl%i/skybox.vs"
#define SKYBOX_SHADER_PATH2 "Assets/shaders/glsl%i/skybox.fs"
#define SKYBOX_CUBEMAP_SHADER_PATH "Assets/shaders/glsl%i/cubemap.vs"
#define SKYBOX_CUBEMAP_SHADER_PATH "Assets/shaders/glsl%i/cubemap.fs"

	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]) { MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]) { useHDR ? 1 : 0 }, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]) { useHDR ? 1 : 0 }, SHADER_UNIFORM_INT);

	// Load cubemap shader and setup required shader locations
	Shader shdrCubemap = LoadShader(TextFormat(SKYBOX_CUBEMAP_SHADER_PATH, GLSL_VERSION),
		TextFormat(SKYBOX_CUBEMAP_SHADER_PATH, GLSL_VERSION));

	SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]) { 0 }, SHADER_UNIFORM_INT);

	char skyboxFileName[256] = { 0 };

	if (useHDR)
	{
		TextCopy(skyboxFileName, PATH_TO_HDR_SKYBOX);

		// Load HDR panorama (sphere) texture
		Texture2D panorama = LoadTexture(skyboxFileName);

		// Generate cubemap (texture with 6 quads-cube-mapping) from panorama HDR texture
		// NOTE 1: New texture is generated rendering to texture, shader calculates the sphere->cube coordinates mapping
		// NOTE 2: It seems on some Android devices WebGL, fbo does not properly support a FLOAT-based attachment,
		// despite texture can be successfully created.. so using PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 instead of PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
		skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = GenTextureCubemap(shdrCubemap, panorama, 1024, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

		UnloadTexture(panorama);        // Texture not required anymore, cubemap already generated
	}
	else
	{

		Image image = LoadImage(PATH_TO_SKYBOX);
		skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(image, CUBEMAP_LAYOUT_AUTO_DETECT);
		UnloadImage(image);
	}
	return skybox;
}


static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format)//fonctions du tuto raylib
{
	TextureCubemap cubemap = { 0 };

	rlDisableBackfaceCulling();     // Disable backface culling to render inside the cube

	// STEP 1: Setup framebuffer
	//------------------------------------------------------------------------------------------
	unsigned int rbo = rlLoadTextureDepth(size, size, true);
	cubemap.id = rlLoadTextureCubemap(0, size, format, 1);

	unsigned int fbo = rlLoadFramebuffer();
	rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
	rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

	// Check if framebuffer is complete with attachments (valid)
	if (rlFramebufferComplete(fbo)) TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);
	//------------------------------------------------------------------------------------------

	// STEP 2: Draw to framebuffer
	//------------------------------------------------------------------------------------------
	// NOTE: Shader is used to convert HDR equirectangular environment map to cubemap equivalent (6 faces)
	rlEnableShader(shader.id);

	// Define projection matrix and send it to shader
	Matrix matFboProjection = MatrixPerspective(90.0 * DEG2RAD, 1.0, rlGetCullDistanceNear(), rlGetCullDistanceFar());
	rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

	// Define view matrix for every side of the cubemap
	Matrix fboViews[6] = {
		MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 1.0f,  0.0f,  0.0f }, (Vector3) { 0.0f, -1.0f,  0.0f }),
		MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { -1.0f,  0.0f,  0.0f }, (Vector3) { 0.0f, -1.0f,  0.0f }),
		MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 0.0f,  1.0f,  0.0f }, (Vector3) { 0.0f,  0.0f,  1.0f }),
		MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 0.0f, -1.0f,  0.0f }, (Vector3) { 0.0f,  0.0f, -1.0f }),
		MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 0.0f,  0.0f,  1.0f }, (Vector3) { 0.0f, -1.0f,  0.0f }),
		MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 0.0f,  0.0f, -1.0f }, (Vector3) { 0.0f, -1.0f,  0.0f })
	};

	rlViewport(0, 0, size, size);   // Set viewport to current fbo dimensions

	// Activate and enable texture for drawing to cubemap faces
	rlActiveTextureSlot(0);
	rlEnableTexture(panorama.id);

	for (int i = 0; i < 6; i++)
	{
		// Set the view matrix for the current cube face
		rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);

		// Select the current cubemap face attachment for the fbo
		// WARNING: This function by default enables->attach->disables fbo!!!
		rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
		rlEnableFramebuffer(fbo);

		// Load and draw a cube, it uses the current enabled texture
		rlClearScreenBuffers();
		rlLoadDrawCube();

		// ALTERNATIVE: Try to use internal batch system to draw the cube instead of rlLoadDrawCube
		// for some reason this method does not work, maybe due to cube triangles definition? normals pointing out?
		// TODO: Investigate this issue...
		//rlSetTexture(panorama.id); // WARNING: It must be called after enabling current framebuffer if using internal batch system!
		//rlClearScreenBuffers();
		//DrawCubeV(Vector3Zero(), Vector3One(), WHITE);
		//rlDrawRenderBatchActive();
	}
	//------------------------------------------------------------------------------------------

	// STEP 3: Unload framebuffer and reset state
	//------------------------------------------------------------------------------------------
	rlDisableShader();          // Unbind shader
	rlDisableTexture();         // Unbind texture
	rlDisableFramebuffer();     // Unbind framebuffer
	rlUnloadFramebuffer(fbo);   // Unload framebuffer (and automatically attached depth texture/renderbuffer)

	// Reset viewport dimensions to default
	rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
	rlEnableBackfaceCulling();
	//------------------------------------------------------------------------------------------

	cubemap.width = size;
	cubemap.height = size;
	cubemap.mipmaps = 1;
	cubemap.format = format;

	return cubemap;
}
