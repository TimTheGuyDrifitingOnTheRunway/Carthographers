#include "Lib3d.h"


/**************************************************Fontions jeux******************************************/

void GUIDrawFeuille(FeuilleCarte f, FeuilleCarte temp, Model mountains[], Position moutainPos[NOMBREMONTAGNE], Seed s, ModelList models) {
	Image treeImage = s.treeImage;
	int nb = 0;
	bool troll = IsModelValid(models.cat);
	static int rot = 0;
	static int direction = 1;
	if (troll) {
		rot += 25 * direction;
		direction = rot > 600 ? -1 : rot < 0 ? 1 : direction;

	}

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

					Color c = (Color){ blue * WATER_RED_FACTOR, blue * WATER_GREEN_FACTOR, blue,  WATER_TRANSPARENCY };
					DrawCube((Vector3) { x - 0.5f + (float)k / TREE_DIVIDER, y + 0.9 + lum / 2 +WATER_CUBE_OFSET, z - 0.5 + (float)l / TREE_DIVIDER }, 0.1f, 0.5 * lum* WATER_CUBE_HEIGHT_MULTIPLYER, 0.1f, c);
				}
			}
#endif



			if (temp[i][j] >= RUINE) {
				float x = i - (SIZE - 1) / 2.0f;
				float z = j - (SIZE - 1) / 2.0f;
				float y = - CASE_HEIGHT / 2.f;   // Half height so cube is under the grid

				float orientation = pow(ColorToHSV(GetImageColor(s.OfsetImagex, j , i )).z+1.0f, 10.0F);
				DrawModelEx(models.ruin, (Vector3) { x, y, z }, (Vector3) { 0, 1, 0 }, 47.0f* orientation, (Vector3) { RUIN_SIZE, RUIN_SIZE*1.1f, RUIN_SIZE }, /*(Color) { 000, 100, 255, 255 }*/ WHITE);
				DrawCube((Vector3) { x, y-0.5, z }, 1.0f, 0.50f, 1.0f, DARKGRAY);
				DrawCubeWires((Vector3) { x, y, z }, 1.0f, 1.0f, 1.0f, BLACK);
			}
			if (getMaterialAtPos(temp, (Position) { i, j }) != 0) {
				float x = i - (SIZE - 1) / 2.0f;
				float z = j - (SIZE - 1) / 2.0f;
				float y = 0.5;   // Half height so cube sits on grid
				if (temp[i][j] != f[i][j]) y = PLACEMENT_HEIGHT; // Raise cube if it's part of the shape being placed


				Color color;
				int drawcube = 1;
				switch (getMaterialAtPos(temp, (Position) { i, j })) {
				case EAU:
					color = BLUE;
					DrawModel(models.water, (Vector3) { x, y + 0.51f, z }, 1, WHITE);//dessin de la tyles avant tout

					break;
				case FORET:
					color = GREEN;
					DrawModel(models.forestTile, (Vector3) { x, y + 0.51f, z }, 1, WHITE);

					for (int k = 0; k < TREE_DIVIDER + 1; k++) for (int l = 0; l < TREE_DIVIDER + 1; l++) {
						Vector3 color;
						color = ColorToHSV(GetImageColor(treeImage, j * 10 + l, i * 10 + k));


						unsigned char green = (pow((color.z), GREEN_POWER) + GREEN_OFSET) * 255 > 254 ? 254 : ((pow((color.z), GREEN_POWER) + GREEN_OFSET) * 255 < 50) ? 50 : (pow((color.z), GREEN_POWER) + GREEN_OFSET) * 255;
						Color c = (Color){ (unsigned char)20, green, (unsigned char)10, 255 };//couleur de base pour les arbres
						Color c2 = (Color){ (unsigned char)150, green, (unsigned char)10, 255 };//couleur de base pour les arbres
						Color c3 = (Color){ green, green * 0.8f ,(unsigned char)50 , 255 };//couleur de base pour les arbres

						//calcul des ofset d'arbres
						float rxOfset = (ColorToHSV(GetImageColor(s.OfsetImagex, j * 10 + l, i * 10 + k)).z-0.5f)*0.5f;
						float rzOfset = (ColorToHSV(GetImageColor(s.OfsetImagey, j * 10 + l, i * 10 + k)).z - 0.5f) * 0.5f;

						clamp(rxOfset, -0.1f, 0.1f);
						clamp(rzOfset, -0.1f, 0.1f);


						//choix du type de model à dessiner
						if ((-0.5f + (float)k / TREE_DIVIDER + rxOfset < FOREST_BORDER) && (-0.5f + (float)k / TREE_DIVIDER + rxOfset) > -FOREST_BORDER && (-0.5 + (float)l / TREE_DIVIDER + rzOfset) < FOREST_BORDER && (-0.5 + (float)l / TREE_DIVIDER + rzOfset) > -FOREST_BORDER) {
							if (color.z < FOREST_TRESHOLD) DrawModelEx(models.tree, (Vector3) { x - 0.5f + (float)k / TREE_DIVIDER + rxOfset, y + TREE_Y_OFSET, z - 0.5 + (float)l / TREE_DIVIDER + rzOfset }, (Vector3) { 1, 0, 0 }, 0, (Vector3) { TREE_SIZE, TREE_SIZE, TREE_SIZE }, c2);

							else if ((color.z > FOREST_TRESHOLD) && color.z < BUSH_TRESHOLD) DrawModelEx(models.buisson, (Vector3) { x - 0.5f + (float)k / TREE_DIVIDER + rxOfset, y + 0.5, z - 0.5 + (float)l / TREE_DIVIDER + rzOfset }, (Vector3) { 1, 0, 0 }, 0, (Vector3) { BUSH_SIZE, BUSH_SIZE, BUSH_SIZE }, c);

							else if (color.z > BUSH_TRESHOLD && color.z < FOREST_END_TRESHOLD)DrawModelEx(models.buisson, (Vector3) { x - 0.5f + (float)k / TREE_DIVIDER + rxOfset, y + 0.5, z - 0.5 + (float)l / TREE_DIVIDER + rzOfset }, (Vector3) { 1, 0, 0 }, 0, (Vector3) { BUSH_SIZE, BUSH_SIZE * 1.2f, BUSH_SIZE }, c3);
						}

					}
					break;
				case VILLAGE:
					color = BROWN;
					DrawModel(models.vilageTile, (Vector3) { x, y + 0.51f, z }, 1, WHITE);

					for (int k = 0; k < HOUSE_DIVIDER + 1; k++) for (int l = 0; l < HOUSE_DIVIDER + 1; l++) {
						float lum = ColorToHSV(GetImageColor(s.villageImage, j * 10 + l, i * 10 + k)).z;
						//calcul des ofset d'arbres
						float rxOfset = (ColorToHSV(GetImageColor(s.OfsetImagex, j * 10 + l, i * 10 + k)).z - 0.5f) * 0.5f;
						float rzOfset = (ColorToHSV(GetImageColor(s.OfsetImagey, j * 10 + l, i * 10 + k)).z - 0.5f) * 0.5f;

						clamp(rxOfset, -0.1f, 0.1f);
						clamp(rzOfset, -0.1f, 0.1f);
						if ((-0.5f + (float)k / HOUSE_DIVIDER + rxOfset < FOREST_BORDER) && (-0.5f + (float)k / HOUSE_DIVIDER + rxOfset) > -FOREST_BORDER && (-0.5 + (float)l / HOUSE_DIVIDER + rzOfset) < FOREST_BORDER && (-0.5 + (float)l / HOUSE_DIVIDER + rzOfset) > -FOREST_BORDER) {


							if (lum > HOUSE_TRESHOLD) DrawModelEx(models.house, (Vector3) { x - 0.5f + (float)k / HOUSE_DIVIDER + rxOfset, y + 0.53f, z - 0.5 + (float)l / HOUSE_DIVIDER + rzOfset }, (Vector3) { 0, 1, 0 }, 0, (Vector3) { HOUSE_SIZE, HOUSE_SIZE, HOUSE_SIZE }, WHITE);


						}
					}

					break;
				case CHAMPS:

					DrawModel(models.champs, (Vector3) { x, y + CASE_HEIGHT / 2 + .01f, z }, 1, WHITE);
					color = YELLOW;
					break;

				case MONTAGNE:
					color = GRAY;
					DrawModel(mountains[nb], (Vector3) { x - 0.5, 0.5f + y, z - 0.5 }, 1, GRAY);
					nb++;
					break;
				case MONSTRE:
					color = PURPLE;
					DrawModel(models.monsterTile, (Vector3) { x, y + 0.51f, z }, 1, WHITE);

					for (int k = 0; k < HOUSE_DIVIDER + 1; k++) for (int l = 0; l < HOUSE_DIVIDER + 1; l++) {//dessine les monstres comme les mainsos
						float lum = ColorToHSV(GetImageColor(s.villageImage, j * 10 + l, i * 10 + k)).z;
						//calcul des ofset d'arbres
						float rxOfset = (ColorToHSV(GetImageColor(s.OfsetImagex, j * 10 + l, i * 10 + k)).z - 0.5f) * 0.5f;
						float rzOfset = (ColorToHSV(GetImageColor(s.OfsetImagey, j * 10 + l, i * 10 + k)).z - 0.5f) * 0.5f;

						clamp(rxOfset, -0.1f, 0.1f);
						clamp(rzOfset, -0.1f, 0.1f);
						if ((-0.5f + (float)k / HOUSE_DIVIDER + rxOfset < FOREST_BORDER) && (-0.5f + (float)k / HOUSE_DIVIDER + rxOfset) > -FOREST_BORDER && (-0.5 + (float)l / HOUSE_DIVIDER + rzOfset) < FOREST_BORDER && (-0.5 + (float)l / HOUSE_DIVIDER + rzOfset) > -FOREST_BORDER) {

							if (lum > MONSTER_TRESHOLD) DrawModelEx(models.monster, (Vector3) { x - 0.5f + (float)k / HOUSE_DIVIDER + rxOfset, y + 0.53f, z - 0.5 + (float)l / HOUSE_DIVIDER + rzOfset }, (Vector3) { 0, 1, 0 }, (k+l)*30, (Vector3) { MONSTER_SIZE, MONSTER_SIZE, MONSTER_SIZE}, DARKPURPLE);

						}
					}
					break;
				case CONFLICTVALUE:
					color = RED;
					if (troll)  DrawModelEx(models.cat, (Vector3) { x, y + 0.73f + (float)rot / 600.0f, z }, (Vector3) { 0, 1, 0 }, rot, (Vector3) { CAT_SIZE * 2, CAT_SIZE * 2, CAT_SIZE * 2 }, WHITE);

					for (int k = 0; k < NOMBREMONTAGNE; k++) {
						if (moutainPos[k].x == i && moutainPos[k].y == j) {
							nb++;//avance le compte montagne si collision avec une montagne pour ne pas faire spawn une montagne sur une autre

						}
					}
					break;
				default:
					color = WHITE;
				}
				if(drawcube)DrawCube((Vector3) { x, y, z }, 1.0f, CASE_HEIGHT, 1.0f, color);
				DrawCubeWires((Vector3) { x, y, z }, 1.0f, CASE_HEIGHT, 1.0f, BLACK);
			}
		}
	}
}



void GUIdisplayFinal(GameState gs, int mountainSeed[2], Seed s, ModelList models, Camera3D camera) {
	int i = 0;


    while (i < gs.playerNumber) {


		gs.playerIndex = i;
		PlayerState* ps = &gs.players[i];

		int pointsParEdit[5] = { 0, 0, 0, 0 , 0};
		for (int i = 0; i < 4; i++) {
			if (gs.edits[i]) pointsParEdit[i] = (gs.edits[i]->fctCaluls)(ps->map);
		}
		pointsParEdit[4] = calcEnenmyPoints(ps->map);
		Model mountains[NOMBREMONTAGNE];

		/* Get mountain positions (malloc'd) and generate models once. */
		Position* mountainPos = getPositionsOfMaterial(ps->map, MONTAGNE);
		generateMountainsModels(mountains, ps->map, mountainSeed);

		/* Prepare a local temp map for rendering so GUIDrawFeuille sees the
		   actual placed tiles (and no empty map). */
		FeuilleCarte tempMap;
		copyFeuilleCarte(ps->map, tempMap);

        /* Render loop for this player's final view. Break on SPACE. */
      /* Debug: print non-empty cell count to verify map content */
		int nonEmpty = SIZE * SIZE - getEmptySpots(ps->map);
		printf("[GUIdisplayFinal] Player %s non-empty cells = %d\n", ps->name, nonEmpty);
		while (!WindowShouldClose()) {
			GUIUpdateCustomCamera(&camera);
			camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

			BeginDrawing(); // Début de l'affichage
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);

			Position emptyMountainPos[NOMBREMONTAGNE];
			for (int mi = 0; mi < NOMBREMONTAGNE; mi++) { emptyMountainPos[mi].x = -100; emptyMountainPos[mi].y = -100; }
			GUIDrawFeuille(ps->map, tempMap, mountains, mountainPos ? mountainPos : emptyMountainPos, s, models);

			GUIdrawGrille();

			EndMode3D();

			drawFinalUi(&gs, pointsParEdit);
			EndDrawing(); // Fin de l'affichage

			if (IsKeyPressed(END_PREV_KEY)) {
				i = i > 0 ? i - 1 : i;
				break;
			}
			if (IsKeyPressed(END_NEXT_KEY)) {
				i = i <gs.playerNumber-1 ? i + 1 : i;
				break;
			}
			if (IsKeyPressed(KEY_SPACE)) {
				i = gs.playerNumber; // Exit outer loop
				break;
			}

		}

		if (mountainPos) free(mountainPos);
		if (WindowShouldClose()) break;


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
			return 0;
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


int GUIPlacementCard(GameState* gs, FeuilleCarte f, const ExploreCard* card, int score, int isRuin, int* coinCount, Camera3D *camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models) {

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
		UpdatePlacement(f, &state, *camera);
		RenderPlacement(gs, f, &state, score, *camera, mountains, mountainPos, s, models);
		GUIUpdateCustomCamera(camera);
		camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
	}

	if (WindowShouldClose()) { EndProgram(gs); exit(1); }

	ApplyPlacement(f, &state, coinCount);
	return 1;
}

void UpdatePlacement(FeuilleCarte f, PlacementState* state, Camera camera) {

	// Recalcul de la preview
	initCarte(state->feuilleVide, FALSE);
	state->drawable = drawShape(state->feuilleVide, state->shapeCopy,
		state->pos, state->rotation, state->material);
	tryDraw(f, state->feuilleVide, state->temp);

	state->drawable = state->drawable && (state->isRuin ? coversRuin(f, state->feuilleVide) :1) && isDrawable(f, state->feuilleVide);

	Vector3 forward = { camera.target.x - camera.position.x, 0, camera.target.z - camera.position.z };
	normalize(&forward);
	Vector3 right = crossProduct(forward, camera.up);
	normalize(&right);


	state->drawable = state->drawable && (state->isRuin ? coversRuin(f, state->feuilleVide) : isDrawable(f, state->feuilleVide));
	if (IsKeyPressed(KEY_SPACE) && state->drawable) {
		state->status = 1;
		return;//retourne immédiatement si la pièce est placée pour éviter de devoir attendre la fin de la boucle et éviter les problèmes de placement + déplacement en même temps (duplicatrion)
	}

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

	if (!drawShape(state->feuilleVide, state->shapeCopy, state->pos, state->rotation, state->material)) {	//out of bounds, pour reset la position si la pièce sort de la carte
		if (state->pos.x < 1)        state->pos.x += 1;
		if (state->pos.x > SIZE - 2) state->pos.x -= 1;
		if (state->pos.y < 1)        state->pos.y += 1;
		if (state->pos.y > SIZE - 2) state->pos.y -= 1;
	}






	//if (OOB) { state->pos.x = 5; state->pos.y = 5; }// reset si hors limite (se produit rarement, mais peut arriver lors de switch de forme si les 2 formes ne peuvent pas être placées au même endroit)
}

void RenderPlacement(GameState* gs, FeuilleCarte f, const PlacementState* state, int score, Camera3D camera, Model mountain[NOMBREMONTAGNE], Position mountainPos[NOMBREMONTAGNE], Seed s, ModelList models) {
	//printf("\nPosition : %d,%d", state->pos.x, state->pos.y);
	int midX = GetScreenWidth() / 2;
	int midY = GetScreenHeight() / 2;
	static bool openPlayerPanel = 0;
	static bool openCard = 0;
	static float historyScrollOffset = 0.0f; // Offset de scroll pour l'historique de cartes
	static bool openSeasonCard = 0;


	//Rectangle playerPanel = { midX * 2 - 400, 100, 400, 250 };

	Rectangle editsRec = (Rectangle){ midX - 40, -60, 80, 120 };

	// Calcul de la largeur de chaque edit séparément
	const char* editLabels[4];
	Rectangle editRects[4];

	{
		int len;		// réduction de la portée car nom commun
		for (int i = 0; i < 4; i++) {
			char lbl[] = "       %s  ";
			len = strlen(gs->edits[i]->name) + strlen(lbl) - 2;
			editLabels[i] = (char*)malloc(len * sizeof(char));
			snprintf(editLabels[i], len, lbl, gs->edits[i]->name);
			//printf("%d\n", (int)strlen(gs->edits[i]->name));
			editRects[i].width = MeasureTextEx(EDITS_FONT, editLabels[i], EDITS_FS, NORMAL_SPACING).x;
			editRects[i].height = editsRec.height / 2;
			editsRec.width += editRects[i].width;
			editsRec.x -= editRects[i].width / 2;
		}
	}

	// Les positionner côte à côte
	int startX = midX - editsRec.width / 2;
	for (int i = 0; i < 4; i++) {
		editRects[i].x = startX + 40;
		editRects[i].y = 0;
		startX += editRects[i].width + 8;
	}

	// Hover timer
	static float hoverTime[5] = { 0 };		// Le dernier sert à éviter de désafficher puis réafficher si on bouge vite entre 2 édits
	Vector2 mouse = GetMousePosition();
	int tooltipTarget = -1;

	for (int i = 0; i < 4; i++) {
		if (CheckCollisionPointRec(mouse, editRects[i])) {
			hoverTime[i] += GetFrameTime();
			hoverTime[4] = hoverTime[4] > 0 ? 4 * .4f : hoverTime[4] + 4 * GetFrameTime();				// délai de 1 seconde avant de désafficher / réafficher (on multiplie par 4, car le temps est modifié 4 fois par frame, lors des vérifications des autres Edits)
			//printf("\nSet Hover Delta Time = %.4f", hoverTime[4] / 4);
		} else {
			hoverTime[i] = 0.f;
			hoverTime[4] -= GetFrameTime();
			hoverTime[4] = max(hoverTime[4], -.4f);
			//printf("\nDec Hover Delta Time = %.4f", hoverTime[4] / 4);
		}

		if (hoverTime[i] >= .4f || (hoverTime[4] > 0 && hoverTime[i] >= .1f)) // .6 secondes ou .1 sec si déjà hover
			tooltipTarget = i;
	}

	const char* seasonLabel = TextFormat("%s  %d/%d", seasons[gs->currentSeason]->name, gs->currentTime, seasons[gs->currentSeason]->maxTime);

	Vector2 seasonLabelSize = MeasureTextEx(SEASON_FONT, seasonLabel, EDITS_FS, NORMAL_SPACING);
	Rectangle seasonRec = (Rectangle){ midX - seasonLabelSize.x / 2 - 40, editsRec.y + editsRec.height - 50, seasonLabelSize.x + 80, 100 };

	Vector2 playerPanelSize = MeasureTextEx(PLAYER_PANEL_FONT, gs->players[gs->playerIndex].name, PLAYER_PANEL_FS + 10, NORMAL_SPACING);
	Rectangle playerPanel = (Rectangle){ GetScreenWidth() - 50 - (openPlayerPanel ? max(PLAYER_REC_WIDTH, playerPanelSize.x - 20) : 0), midY - PLAYER_REC_HEIGHT / 2, playerPanelSize.x + PLAYER_REC_HEIGHT, PLAYER_REC_HEIGHT };
	openPlayerPanel = CheckCollisionPointRec(mouse, playerPanel);

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

	/**************************************************************** Affichage 2D ****************************************************************/

	// UI 2D — lecture seule sur state

	// Infos relatives à tous les joueurs
	//DrawRectangleStroke(infoPanel, 3, WHITE, RED);
	//int y1 = 5;
	//DrawText(TextFormat("Carte : %s", state->card->name), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	//DrawText(TextFormat("Temps de la saison : %d + %d", gs->currentTime, seasons[gs->currentSeason]->maxTime), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	//DrawText(TextFormat("2 formes : %d", state->hasTwoShapes), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	//DrawText(TextFormat("2 matériaux : %d", state->hasTwoMat), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	//DrawText(TextFormat("Saison : %s", seasons[gs->currentSeason]->name), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	//DrawText(TextFormat("Edits en vigueur : %s / %s", gs->edits[seasons[gs->currentSeason]->EditA]->name, gs->edits[seasons[gs->currentSeason]->EditB]->name), 5, infoPanel.y + y1, 25, BLACK); y1 += 30;
	//
	//if (state->isRuin) DrawText("Doit être placé sur une Ruine", 5, infoPanel.y + y1, 30, RED); y1 += 35;
	//if (state->isRiftLands) DrawText("Tous matériaux disponibles !", 5, infoPanel.y + y1, 30, RED); y1 += 35;


	/******** Gestion de la Carte ********/

	// Recherche de la carte par le nom
	char* cardName = state->card->name;
	int cardIndex = 0;
	for (int i = 0; i < NUM_CARDS; i++) if (!strcmp(expCards[i]->name, cardName)) { cardIndex = i; break; }



	// Affichage de la carte
	Texture2D cardTex = gs->assets.cardImages[cardIndex];
	int targetHeight = GetScreenHeight() * (openCard ? .6f : .4f);
	float ratioCards = 1.4f;					// Format des cartes de Cartographers (ou poker)
	int targetWidth = targetHeight / ratioCards;
	float histVSmainCardRatio = 3.0f / 5.0f;

	int cardFS = 100 * (openCard ? .6f : .4f) * ((float)GetScreenHeight() / 1000.0f);
	int typeFS = 40 * (openCard ? .6f : .4f) * ((float)GetScreenHeight() / 1000.0f);


	int ruinOffsetX = 0;
	int ruinOffsetY = 0;
	int mainOffsetX = 0;

	if (openCard) {
		int historyCount = gs->exploreIndex - 1;
		int histCardHeight = targetHeight * histVSmainCardRatio;
		int histSpacing = histCardHeight + 10;

		float maxScroll = (float)(historyCount * histSpacing) + midY - targetHeight/* / 2.0f*/;
		historyScrollOffset += GetMouseWheelMove() * histSpacing * .4f;
		if (historyScrollOffset < 0)			historyScrollOffset = 0;
		if (historyScrollOffset > maxScroll)	historyScrollOffset = maxScroll;

		if (historyCount > 0) {
			int histCardHeight = targetHeight * histVSmainCardRatio;
			int histCardWidth = histCardHeight / ratioCards;
			int histSpacing = histCardHeight + 10;
			int histX = 30 + ((targetWidth - histCardWidth) / 2);

			int mainCardTop = midY - targetHeight / 2 + (int)historyScrollOffset;  // suit le scroll

			// Scissor pour ne pas déborder sous la carte principale
			BeginScissorMode(histX, 0, histCardWidth + 10, mainCardTop);

			for (int i = historyCount - 1; i >= 0; i--) {
				int posInColumn = (historyCount - 1 - i);
				int cardY = mainCardTop - histSpacing - posInColumn * histSpacing;

				if (cardY + histCardHeight < 0 || cardY > mainCardTop) continue;

				const ExploreCard* hCard = gs->exploreDeck[i];
				int hCardIndex = 0;
				for (int j = 0; j < NUM_CARDS; j++) if (!strcmp(expCards[j]->name, hCard->name)) { hCardIndex = j; break; }

				Texture2D hTex = gs->assets.cardImages[hCardIndex];
				Rectangle hDest = (Rectangle){ (float)histX, (float)cardY, (float)histCardWidth, (float)histCardHeight };
				Rectangle hSource = (Rectangle){ 0, 0, (float)hTex.width, (float)hTex.height };

				float alpha = 0.6f + 0.4f * ((float)(i + 1) / historyCount);
				DrawTexturePro(hTex, hSource, hDest, (Vector2) { 0 }, 0, ColorAlpha(WHITE, alpha));

				// Nom
				int hFS = (int)(cardFS * histVSmainCardRatio);
				DrawStrokeTextEx(CARD_FONT, hCard->name,
					hDest.x + histCardWidth / 2 - MeasureTextEx(CARD_FONT, hCard->name, hFS, NORMAL_SPACING).x / 2,
					hDest.y + histCardHeight * 49 / 100,
					hFS, NORMAL_SPACING, WHITE, BLACK, 2);

				// Type
				int hTypeFS = (int)(typeFS * histVSmainCardRatio);
				char* hTypeLbl = hCard->isEnemy ? "AMBUSH" : "EXPLORE";
				DrawStrokeTextEx(CARD_FONT, hTypeLbl,
					hDest.x + histCardWidth / 2 - MeasureTextEx(CARD_FONT, hTypeLbl, hTypeFS, NORMAL_SPACING).x / 2,
					hDest.y + histCardHeight * 95.5f / 100 - hTypeFS,
					hTypeFS, NORMAL_SPACING, WHITE, BLACK, 1);
			}

			EndScissorMode();
		}
	}
	else {
		historyScrollOffset = 0.0f;
	}


	bool showRuin = state->isRuin && !state->card->isEnemy && !openCard;
	if (showRuin) {
		// Chercher la carte ruine dans exploreDeck avant la carte actuelle
		const ExploreCard* ruinCard = NULL;
		for (int i = gs->exploreIndex - 1; i >= 0; i--) {
			if (gs->exploreDeck[i]->isRuin) { // ou toute autre condition identifiant une ruine
				ruinCard = gs->exploreDeck[i];
				break;
			}
		}

		if (ruinCard) {
			int ruinIndex = 0;
			for (int i = 0; i < NUM_CARDS; i++) if (!strcmp(expCards[i]->name, ruinCard->name)) { ruinIndex = i; break; }

			Texture2D ruinTex = gs->assets.cardImages[ruinIndex];

			// Décalage : ruine en haut-gauche, carte principale décalée à droite
			ruinOffsetX = -15;
			ruinOffsetY = -15;
			mainOffsetX = 20;

			Rectangle ruinDest = (Rectangle){
				50 + ruinOffsetX,
				midY - targetHeight / 2 + (int)historyScrollOffset + ruinOffsetY,
				targetWidth,
				targetHeight
			};
			Rectangle ruinSrc = (Rectangle){ 0, 0, ruinTex.width, ruinTex.height };
			DrawTexturePro(ruinTex, ruinSrc, ruinDest, (Vector2) { 0 }, 0, ColorAlpha(WHITE, 0.85f));

		}
	}

	//  Rendu de la carte principale
	Rectangle destRec = (Rectangle){ (openCard ? 30 : 50 + mainOffsetX), midY - targetHeight / 2 + (int)historyScrollOffset, targetWidth, targetHeight };
	Rectangle sourceRect = (Rectangle){ 0, 0, cardTex.width, cardTex.height };
	DrawTexturePro(cardTex, sourceRect, destRec, (Vector2) { 0 }, 0, WHITE);

	DrawStrokeTextEx(CARD_FONT, cardName,
		destRec.x + targetWidth / 2 - MeasureTextEx(CARD_FONT, cardName, cardFS, NORMAL_SPACING).x / 2,
		destRec.y + targetHeight * 49 / 100,
		cardFS, NORMAL_SPACING, WHITE, BLACK, 2);
	char* typeLbl = state->card->isEnemy ? "AMBUSH" : "EXPLORE";
	DrawStrokeTextEx(CARD_FONT, typeLbl,
		destRec.x + targetWidth / 2 - MeasureTextEx(CARD_FONT, typeLbl, typeFS, NORMAL_SPACING).x / 2,
		destRec.y + targetHeight * 95.5f / 100 - typeFS,
		typeFS, NORMAL_SPACING, WHITE, BLACK, 1);

	openCard = CheckCollisionPointRec(mouse, openCard ? (Rectangle) { destRec.x, 0, destRec.width, GetScreenHeight() } : destRec);


	//Rectangle destRec = (Rectangle){ (openCard ? 30 : 50), midY - targetHeight / 2 + (int)historyScrollOffset, targetWidth, targetHeight };
	//Rectangle sourceRect = (Rectangle){ 0, 0, cardTex.width, cardTex.height };
	//DrawTexturePro(cardTex, sourceRect, destRec, (Vector2) { 0 }, 0, WHITE);

	//// Affichage du titre de la carte et de son Type
	//int cardFS = 100 * (openCard ? .7f : .4f) * ((float)GetScreenHeight() / 1000.0f);
	//DrawStrokeTextEx(CARD_FONT, cardName, destRec.x + targetWidth / 2 - MeasureTextEx(CARD_FONT, cardName, cardFS, NORMAL_SPACING).x / 2, destRec.y + targetHeight * 49 / 100, cardFS, NORMAL_SPACING, WHITE, BLACK, 2);
	//int typeFS = 40 * (openCard ? .7f : .4f) * ((float)GetScreenHeight() / 1000.0f);
	//char* typeLbl = state->card->isEnemy ? "AMBUSH" : "EXPLORE";
	//DrawStrokeTextEx(CARD_FONT, typeLbl, destRec.x + targetWidth / 2 - MeasureTextEx(CARD_FONT, typeLbl, typeFS, NORMAL_SPACING).x / 2, destRec.y + targetHeight * 95.5f / 100 - typeFS, typeFS, NORMAL_SPACING, WHITE, BLACK, 1);
	//openCard = CheckCollisionPointRec(mouse, destRec);


	// Infos relatives au joueur actuel
	int y2 = 20;
	//DrawRectangleStroke(playerPanel, 3, SKYBLUE, DARKBLUE);
	//DrawText("Joueur : ", playerPanel.x + 5, y2, 30, BLACK); y2 += 35;
	//DrawText(gs->players[gs->playerIndex].name, playerPanel.x + 5, y2, 30, BLACK); y2 += 35;
	//DrawText(TextFormat("Score : %d", score), playerPanel.x + 5, y2, 30, BLACK); y2 += 35;
	//DrawText(TextFormat("Coins : %d", gs->players[gs->playerIndex].coinCount), playerPanel.x + 5, y2, 30, BLACK); y2 += 35;
	DrawRectangleRoundedStrokeEx(playerPanel, .1f, 10, 3, LIGHTGRAY, DARKBROWN);
	DrawStrokeTextEx(PLAYER_PANEL_FONT, gs->players[gs->playerIndex].name, playerPanel.x + 10, playerPanel.y + 5 , PLAYER_PANEL_FS + 10, NORMAL_SPACING, WHITE, BLACK, 1);
	DrawStrokeTextEx(PLAYER_PANEL_FONT, TextFormat("SCORE  %d", score), playerPanel.x + 10, playerPanel.y + PLAYER_PANEL_FS + y2, PLAYER_PANEL_FS, NORMAL_SPACING, WHITE, BLACK, 1);										y2 += PLAYER_PANEL_FS + 10;
	DrawStrokeTextEx(PLAYER_PANEL_FONT, TextFormat("PIECES  %d", gs->players[gs->playerIndex].coinCount), playerPanel.x + 10, playerPanel.y + PLAYER_PANEL_FS + y2, PLAYER_PANEL_FS, NORMAL_SPACING, WHITE, BLACK, 1);		y2 += PLAYER_PANEL_FS + 10;
	//if (gs->isOnline) { DrawText(TextFormat("COINS  %d", gs->players[gs->playerIndex].coinCount), playerPanel.x + 10, playerPanel.y + PLAYER_PANEL_FS + y2, PLAYER_PANEL_FS, BLACK); }

	// Affichage des saisons
	//DrawRectangleRoundedStrokeEx(seasonRec, 1.f, 10, 2, BGCOLOR, GREEN);

	Texture2D seasonTex = gs->assets.seasonImages[gs->currentSeason];

	float scale = 250.f / (float)seasonTex.width;
	//printf("\nTexure : %d %d, scale : %f",IsTextureValid(seasonTex), seasonTex.width, scale);
	Vector2 seasonCardV = (Vector2){ midX - (seasonTex.width / 2 * scale), editsRec.y + editsRec.height + (openSeasonCard ? -5 : SEASON_FS + 15 - (seasonTex.height * scale)) };
	Rectangle seasonTexPlace = (Rectangle){ seasonCardV.x, seasonCardV.y, seasonTex.width * scale, seasonTex.height * scale };
	DrawTextureEx(seasonTex, seasonCardV, 0, scale, WHITE);

	DrawStrokeTextEx(SEASON_FONT, seasonLabel, seasonRec.x + 40, editsRec.y + editsRec.height + (openSeasonCard ? seasonTexPlace.height * 3 / 4 : 0), EDITS_FS, NORMAL_SPACING, GOLD, BLACK, 1);

	Rectangle openSeasonRec = GetCollisionRec(editsRec, seasonTexPlace);
	bool openSeasonCardA = CheckCollisionPointRec(mouse, openSeasonRec);
	bool openSeasonCardB = CheckCollisionPointRec(mouse, seasonTexPlace);
	openSeasonCard = (!openSeasonCardA && openSeasonCardB);

	// Edits & Saison
	DrawRectangleRoundedStrokeEx(editsRec, 1.f, 10, 2, BGCOLOR, GOLD);
	for (int i = 0; i < 4; i++) {
		DrawStrokeTextEx(gs->assets.fonts[FONT_GRENZE_GOTISCH_L], editLabels[i], editRects[i].x, editRects[i].y + 10 - EDITS_FS / 5, EDITS_FS, NORMAL_SPACING, GOLD, BLACK, 1);
		if (i == gs->currentSeason || i == (gs->currentSeason + 1) % NUM_SEASONS) {
			Texture2D letterScrollTex = gs->assets.letterScrollsImage[i];
			DrawTextureEx(letterScrollTex, (Vector2) { editRects[i].x, editRects[i].y + 10 - EDITS_FS / 5 }, 0, (float)EDITS_FS / (float)letterScrollTex.height * 1.2f, WHITE);
		}
		else {
			Texture2D letterScrollTex = gs->assets.letterScrollsImage[i + NUM_SEASONS];
			DrawTextureEx(letterScrollTex, (Vector2) { editRects[i].x, editRects[i].y + 10 - EDITS_FS / 5 }, 0, (float)EDITS_FS / (float)letterScrollTex.height * 1.2f, WHITE);
		}
		//printf("Impression de l'edit %d (nom %s) a la place (%d,%d), taille %d\n", i, editLabels[i], (int)editRects[i].x + 10, (int)editRects[i].y + 10, EDITS_FS);
	}



#define A_MID(a,b) (midX < mouse.x ? a : b)
	if (tooltipTarget != -1) {	// Affiche les tooltips
		const char* desc = gs->edits[tooltipTarget]->description;
		Rectangle maxBounds = (Rectangle){ A_MID(20, mouse.x), mouse.y + 25, A_MID(mouse.x, GetScreenWidth() - mouse.x) - 20, 0 };
		Vector2 recSize = MeasureTextWrappedEx(TOOLTIP_FONT, desc, maxBounds, 20);
		maxBounds.x = A_MID(mouse.x - recSize.x, maxBounds.x - 20) - 10;
		maxBounds.width = recSize.x + 40;
		maxBounds.height = recSize.y + 20;

		DrawRectangleRoundedStrokeEx(maxBounds, 0.3f, 10, 2, ColorAlpha(BLACK, 0.65f), BLACK);

		DrawTextWrappedEx(TOOLTIP_FONT, desc, (Rectangle) { maxBounds.x + 20, maxBounds.y + 10, maxBounds.width - 40, 0 }, 20, 1, WHITE, BLACK);

	}
#undef A_MID

	EndDrawing();

	for (int i = 0; i < 4; i++) free(editLabels[i]);
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

int GUIplacementDefaultCard(GameState* gs, FeuilleCarte f, const ExploreCard* card, int score, int isRuin, int* coinCount, Camera3D *camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models) {
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

	if (newPos.y > MINCAMERAHEIGHT && newPos.y < (float)MAXCAMERAHEIGHT) {	// évite les postions négatives
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
	vector->x *= (float)a;
	vector->y *= (float)a;
	vector->z *= (float)a;
}


/*****************génération de heighmap et models ***/


Model generateMountain(int x, int y) {
	// pour éviter d'avoir toujours la même montagne au lancement du jeu
	Image perlinNoise = GenImagePerlinNoise(PERLIN_SIZE, PERLIN_SIZE, x * 100, y * 100, PERLIN_SCALE);


	for (int y = 0; y < PERLIN_SIZE; y++) {// fallof pour avoir des bords smooths
		for (int x = 0; x < PERLIN_SIZE; x++) {




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
	int count = getOccurencesOf(f, MONTAGNE);
	if (pos == NULL || count == 0) return;
	for (int i = 0; i < count && i < NOMBREMONTAGNE; i++) {
		mountains[i] = generateMountain(pos[i].x + mountainSeed[1], pos[i].y + mountainSeed[0]);
	}
	free(pos);
}

Image generateForestImage(int x, int y) {
	Image perlinNoise = GenImagePerlinNoise(FORET_SIZE, FORET_SIZE, x * 100, y * 100, FORET_SCALE);

	return perlinNoise;

}



ModelList loadModels(bool troll) {
	ModelList models;
	printf("Loading models \n");
	models.tree = LoadModel(PATH_TO_TREE_MODEL);
	models.buisson = LoadModel(PATH_TO_BUSH_MODEL);
	models.skybox = loadSkybox(false);
	models.house = LoadModel(PATH_TO_HOUSE);
	models.monster = LoadModel(PATH_TO_MONSTER);
	models.ruin = LoadModel(PATH_TO_RUIN);
	// génération du plan pour afficher la texture de champs

	Mesh plane = GenMeshPlane(1.0f, 1.0f, 1, 1);//plan unique pour générer plusieurs modèles différents, pour éviter de générer plusieurs meshes identiques


	Texture2D textureChamps = LoadTexture(PATH_TO_CHAMPS_TEXTURE);
	models.champs = LoadModelFromMesh(plane);
	SetTextureWrap(textureChamps, TEXTURE_WRAP_CLAMP);
	SetTextureFilter(textureChamps, TEXTURE_FILTER_POINT);
	models.champs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureChamps;

	//idem mais pour l'eau :


	Texture2D textureWater = LoadTexture(PATH_TO_WATER_TEXTURE);
	models.water = LoadModelFromMesh(plane);
	SetTextureWrap(textureWater, TEXTURE_WRAP_CLAMP);
	SetTextureFilter(textureWater, TEXTURE_FILTER_POINT);
	models.water.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureWater;

	//idem mais pour le sol des monstres :
	{

		Texture2D texture = LoadTexture(PATH_TO_MONSTER_TEXTURE);
		models.monsterTile = LoadModelFromMesh(plane);
		SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
		SetTextureFilter(texture, TEXTURE_FILTER_POINT);
		models.monsterTile.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	}

	//idem mais pour le sol de la foret
	{

		Texture2D texture = LoadTexture(PATH_TO_FORET_TEXTURE);
		models.forestTile = LoadModelFromMesh(plane);
		SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
		SetTextureFilter(texture, TEXTURE_FILTER_POINT);
		models.forestTile.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	}

	//idem mais pour le sol de la foret
	{

		Texture2D texture = LoadTexture(PATH_TO_VILLAGE_TEXTURE);
		models.vilageTile = LoadModelFromMesh(plane);
		SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
		SetTextureFilter(texture, TEXTURE_FILTER_POINT);
		models.vilageTile.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	}


	if (troll) {
		models.cat = LoadModel(PATH_TO_CAT);
	}


	printf("models succesfully laoded\n");




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




	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]) { MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]) { useHDR ? 1 : 0 }, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]) { useHDR ? 1 : 0 }, SHADER_UNIFORM_INT);

	// Load cubemap shader and setup required shader locations
	Shader shdrCubemap = LoadShader(TextFormat(SKYBOX_CUBEMAP_SHADER_PATH, GLSL_VERSION),
		TextFormat(SKYBOX_CUBEMAP_SHADER_PATH2, GLSL_VERSION));

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
	//printf("\n La skybox a mis %f temps à se charger\n", (float)(GetTime() - start));
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


Image generateOffsetImage(int x, int y) {
	printf("[SEED] Generating perlin noise map \n");
	Image perlinNoise = GenImagePerlinNoise(OFSET_IMAGE_SIZE, OFSET_IMAGE_SIZE, x * 100, y * 100, OFSET_IMAGE_SCALE);

	return perlinNoise;


}

void *generateRandomOfsetImagesThread(void* arg) {
	Image* img;
	srand(time(NULL));//besoin de re-initialiser le génératuer random parce que thread séparé
	do {
		img = malloc(sizeof(Image));
		*img = generateOffsetImage(randInt(0, 100) * 10, randInt(0, 100) * 10);
	} while (img == NULL);
	return img;
}

Seed generateSeed(int mountainSeed[2]) {//génère une seed aléatoire pour les montagnes et les images d'offset, en utilisant un thread pour générer les images d'offset en parallèle
	Seed s;
	pthread_t thread1, thread2, thread3;
	Image* pResult1, *pResult2, *pResult3;
	pthread_create(&thread1, NULL, generateRandomOfsetImagesThread, NULL);
	pthread_create(&thread2, NULL, generateRandomOfsetImagesThread, NULL);
	pthread_create(&thread3, NULL, generateRandomVilageImagesThread, NULL);

	s.isGenerated = 1;
	s.treeImage = generateForestImage(mountainSeed[0] * 10, mountainSeed[1] * 10);

	pthread_join(thread2, (void**)&pResult2);
	pthread_join(thread1, (void**)&pResult1);
	pthread_join(thread3, (void**)&pResult3);



	s.OfsetImagex = *pResult1;
	s.OfsetImagey = *pResult2;
	s.villageImage = *pResult3;
	free(pResult3);
	free(pResult1);
	free(pResult2);

	return s;
}


void* generateSeedThread(void* arg) {
	int* mountainSeed = (int*)arg;
	Seed* s = malloc(sizeof(Seed));
	*s = generateSeed(mountainSeed);
	return s;
}



Image generateVillageImage(int x, int y) {
	printf("[seed] Generating perlin noise Ofset \n");
	Image perlinNoise = GenImagePerlinNoise(VILLAGE_IMAGE_SIZE, VILLAGE_IMAGE_SIZE, x * 100, y * 100, VILLAGE_IMAGE_SCALE);

	return perlinNoise;


}



void* generateRandomVilageImagesThread(void* arg) {
	Image* img;
	srand(time(NULL));//besoin de re-initialiser le génératuer random parce que thread séparé
	do {
		img = malloc(sizeof(Image));
		*img = generateVillageImage(randInt(0, 100) * 10, randInt(0, 100) * 10);
	} while (img == NULL);
	return img;
}

void UnloadSeed(Seed* s) {
	UnloadImage(s->OfsetImagex);
	UnloadImage(s->OfsetImagey);
	UnloadImage(s->villageImage);
	UnloadImage(s->treeImage);
	s->isGenerated = 0;
	printf("[INFO] seed unloaded \n");
}
void UnloadModels(ModelList* models) {//décharge manuellement les models meme si raylib le fait automatiquement
	UnloadModel(models->tree);
	UnloadModel(models->buisson);
	UnloadModel(models->skybox);
	UnloadModel(models->house);
	UnloadModel(models->monster);
	UnloadModel(models->champs);
	UnloadModel(models->water);
	UnloadModel(models->monsterTile);
	UnloadModel(models->forestTile);
	UnloadModel(models->vilageTile);
	printf("[INFO] models unloaded \n");
}






/***************************Sound management****************/

void* SoundThread(void* args) {		//thread de gestion de l'audio séparé afin d'éviter des freeze de la musique
	InitAudioDevice();              // Initialize audio device
	int* keep = args;
	srand(time(NULL));				//besoin de re-initialiser le génératuer random parce que thread séparé
	int musicIndex = randInt(0, MAX_MUSIC_INDEX);

	char musicPath[256];
	sprintf(musicPath, "%s%d.mp3", PATH_TO_AUDIO, musicIndex);
	Music music = LoadMusicStream(musicPath);

	PlayMusicStream(music);

	float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
	float pan = 0.0f;               // Default audio pan center [-1.0f..1.0f]
	SetMusicPan(music, pan);

	float volume = 0.8f;            // Default audio volume [0.0f..1.0f]
	SetMusicVolume(music, volume);


	while (*keep==1) {
		UpdateMusicStream(music);
		timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music);
		//printf("time played : %.3f \n", timePlayed);
		if (timePlayed > 0.999f) {		// 3 chiffres après la virgule parce que le temps entre .99 et .999 est de plus d'1s, ce qui peut couper la musique avant la fin si elle est trop longue
			timePlayed = 0.0f;
			UnloadMusicStream(music);

			int new_index;
				do {
					new_index = randInt(0, MAX_MUSIC_INDEX);
					Sleep(MIN_MUSIC_DELAY);//attends un temps aléatoire et choisit une nouvelle musique différente de l'actuelle
				} while (musicIndex == new_index);


			musicIndex = new_index;
			sprintf(musicPath, "%s%d.mp3", PATH_TO_AUDIO, musicIndex);
			music = LoadMusicStream(musicPath);

			SetMusicPan(music, pan);
			SetMusicVolume(music, volume);
			PlayMusicStream(music);
		}
	}
	UnloadMusicStream(music);   // Unload music stream buffers from RAM

	CloseAudioDevice();
	return NULL;
}



