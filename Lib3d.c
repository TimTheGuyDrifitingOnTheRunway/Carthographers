#include "Lib3d.h"
#include "GameManager.h"


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
            if (getMaterialAtPos(temp, (Position){ i,j } ) != 0) {
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
                    if (temp[i][j] !=f[i][j])
                        DrawCubeWires((Vector3) { (float)i - SIZE/2, PLACEMENT_HEIGHT, (float)j - SIZE/2 }, 1.0f, 1.0f, 1.0f, BORDERCOLOR);
                
            GUIdrawGrille();
            // DrawMapGrid(SIZE, 1.0f);
            EndMode3D();
			EndDrawing(); // Fin de l'affichage
            drawable = drawable && isDrawable(f, feuilleVide);

            UpdateCamera(&camera,CAMERA_THIRD_PERSON);//gestion caméra : toujours au même endraoit
            camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
            
            pos.x += -(int)IsKeyPressed(KEY_J) + (int)IsKeyPressed(KEY_L); // déplacement pièce
            pos.y += -(int)IsKeyPressed(KEY_I) + (int)IsKeyPressed(KEY_K);

            if (IsKeyPressed(KEY_O)) { pos.x = 6; pos.y = 6; }//possibilité de resset

            if (IsKeyPressed(KEY_R)) {
                rotation = (rotation + 1);
            }
			if (IsKeyPressed(KEY_F)) {
                flipShape(shapeCopy);
            }
            //printf("rotation %d(%d, %d) \n", rotation, pos.x, pos.y);
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

int GUIPlacementCard(FeuilleCarte f, const ExploreCard* card, int isRuin, Camera3D camera) {
    int hasTwoShapes = card->pieceB != NULL;
    int hasTwoMat = card->terrainB != 0;

    int isRiftLands = card->isRiftLands;
    int RiftLandsMat = 2;

    int material = card->terrainA;
    int canFitA = isRuin ? checkShapeOnRuin(f, card->pieceA) : checkShape(f, card->pieceA);
    int canFitB = hasTwoShapes ? isRuin ? checkShapeOnRuin(f, card->pieceB) : checkShape(f, card->pieceB) : 0;

    if (!canFitA && !canFitB) {
        printf("IL n'y a pas la place pour rentrer votre piece \n");
        return GUIplacementDefaultCard(f, card, isRuin, camera);
    }

    Piece shapeCopy;

    if (!canFitA && canFitB) copyPiece(card->pieceB, shapeCopy);
    else copyPiece(card->pieceA, shapeCopy);

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
        // 2D
        DrawRectangle(0, 0, 400, 400, BLACK);
        DrawRectangle(2, 2, 396, 396, RED);
        DrawText(TextFormat("Nom de la carte : %s", card->name), 5, 5, 15, BLACK);
        DrawText(TextFormat("Peut changer de couleur : %d", hasTwoMat), 5, 25, 15, BLACK);
        DrawText(TextFormat("Peut changer de forme : %d", hasTwoShapes), 5, 45, 15, BLACK);
        DrawText(TextFormat("Doit être placé sur une Ruine : %d", isRuin), 5, 65, 15, BLACK);
        DrawText(TextFormat("Saison en cours : %s", seasons[currentSeason]->name), 5, 85, 15, BLACK);
        DrawText(TextFormat("Edits en cours : %s et %s", edits[seasons[currentSeason]->EditA]->name, edits[seasons[currentSeason]->EditB]->name), 5, 105, 15, BLACK);
        DrawText("tous les édits : ", 5, 125, 15, BLACK);
        for (int i = 0; i < 4; i++) {
            DrawText(TextFormat("Edits %d : %s", i, edits[i]->name), 5, 145 + 20 * i, 15, BLACK);
        }
        DrawText(TextFormat("Piece en A ? %d", card->iscoinA), 5, 225, 15, BLACK);


        EndDrawing(); // Fin de l'affichage
        drawable = drawable && isDrawable(f, feuilleVide);
        if (isRuin) {
            drawable = drawable && coversRuin(f, feuilleVide);
        }
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);// gestion caméra : toujours au même endraoit
        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

        if (IsKeyPressed(KEY_O) && hasTwoShapes) {
            if (card->pieceA == shapeCopy && canFitB) copyPiece(card->pieceB, shapeCopy);
            else if (canFitA) copyPiece(card->pieceA, shapeCopy);
        }

        if (IsKeyPressed(KEY_U)) {
            if (isRiftLands) {
                RiftLandsMat = ((RiftLandsMat - 1) % 5) + 2;
                material = RiftLandsMat;
            }
            else if (hasTwoMat) {
                if (material == card->terrainA) material = card->terrainB;
                else material = card->terrainA;
            }
            
        }

        pos.x += -(int)IsKeyPressed(KEY_J) + (int)IsKeyPressed(KEY_L); // déplacement pièce
        pos.y += -(int)IsKeyPressed(KEY_I) + (int)IsKeyPressed(KEY_K);



        if (IsKeyPressed(KEY_P)) { pos.x = 6; pos.y = 6; } //possibilité de resset

        if (IsKeyPressed(KEY_R)) {
            rotation++;
        }
        if (IsKeyPressed(KEY_F)) {
            flipShape(shapeCopy);
        }
        //printf("rotation %d(%d, %d) \n", rotation, pos.x, pos.y);
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

int GUIplacementDefaultCard(FeuilleCarte f, const ExploreCard* card, int isRuin, Camera3D camera) {
    if (getEmptySpots(f) == 0) return 0;
    ExploreCard def = *card;
    def.pieceA = &POINT;
    def.iscoinA = 0;

    GUIPlacementCard(f, &def, isRuin, camera);
    return 1;
}


void GUIdrawGrille() {
	for (int i = -SIZE / 2-1; i <= SIZE / 2; i++) {
        DrawLine3D((Vector3) { (float)i + 0.5f, 0.0f, (float)-SIZE / 2 }, (Vector3) { (float)i + 0.5f, 0.0f, (float)SIZE / 2 }, GRIDCOLOR);
        DrawLine3D((Vector3) { (float)-SIZE / 2, 0.0f, (float)i + 0.5f }, (Vector3) { (float)SIZE / 2, 0.0f, (float)i + 0.5f }, GRIDCOLOR);
    }
}