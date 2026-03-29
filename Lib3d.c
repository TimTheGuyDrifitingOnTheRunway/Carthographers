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
                    if (temp[i][j] !=f[i][j])
                        DrawCubeWires((Vector3) { (float)i - SIZE/2, PLACEMENT_HEIGHT, (float)j - SIZE/2 }, 1.0f, 1.0f, 1.0f, BORDERCOLOR);
                
            GUIdrawGrille();
            // DrawMapGrid(SIZE, 1.0f);
            EndMode3D();
			EndDrawing(); // Fin de l'affichage
            drawable = drawable && isDrawable(f, feuilleVide);

            GUIUpdateCustomCamera(& camera);
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

int GUIPlacementCard(FeuilleCarte f, const ExploreCard* card, int score, int isRuin, Camera3D camera) {
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
        printf("IL n'y a pas la place pour rentrer votre piece \n");
        return GUIplacementDefaultCard(f, card, score, 0, camera);
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
            if (compareShape(card->pieceA, shapeCopy) && canFitB) copyPiece(card->pieceB, shapeCopy);
            else if (canFitA) copyPiece(card->pieceA, shapeCopy);
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
        pos.y += -(int)IsKeyPressed(UPP) + (int)IsKeyPressed(DOWNP);



        if (IsKeyPressed(KEY_P)) { pos.x = 6; pos.y = 6; } //possibilité de resset

        if (IsKeyPressed(ROTATEP)) {
            rotation++;
        }
        if (IsKeyPressed(FLIPP)) {
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

    int mountainBefore = countSurroundedMountains(f);
    draw(f, feuilleVide);
    int mountainAfter = countSurroundedMountains(f);
    coinCount += mountainAfter - mountainBefore;

    if (compareShape(shapeCopy, card->pieceA) && card->iscoinA) coinCount++;

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

int GUIplacementDefaultCard(FeuilleCarte f, const ExploreCard* card, int score, int isRuin, Camera3D camera) {
    if (getEmptySpots(f) == 0) return 0;
    ExploreCard def = *card;
    def.pieceA = &POINT;
    def.iscoinA = 0;

    GUIPlacementCard(f, &def, score, isRuin, camera);
    return 1;
}


void GUIdrawGrille() {
	for (int i = -SIZE / 2-1; i <= SIZE / 2; i++) {
        DrawLine3D((Vector3) { (float)i + 0.5f, 0.0f, (float)-SIZE / 2 }, (Vector3) { (float)i + 0.5f, 0.0f, (float)SIZE / 2 }, GRIDCOLOR);
        DrawLine3D((Vector3) { (float)-SIZE / 2, 0.0f, (float)i + 0.5f }, (Vector3) { (float)SIZE / 2, 0.0f, (float)i + 0.5f }, GRIDCOLOR);
    }
}

void GUIUpdateCustomCamera(Camera3D *camera) {
    // vers le haut/bas

    Vector3 newPos;
    newPos = camera->position;
    double rhoCam = sqrt(pow(camera->position.x, 2) + pow(camera->position.y, 2) + pow(camera->position.z, 2));
    Vector3 mouvement = (Vector3){ camera->up.x * ((int)IsKeyDown(UPC) - (int)IsKeyDown(DOWNC)), camera->up.y * ((int)IsKeyDown(UPC) - (int)IsKeyDown(DOWNC)), camera->up.z * ((int)IsKeyDown(UPC) - (int)IsKeyDown(DOWNC)) };
    multiplyVector(&mouvement, GetFrameTime()*SPEEDY);
    multiplyVector(&mouvement, 1+abs((int)camera->position.y/2));//compensation de vitesse
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
    multiplyVector(&mouvement, GetFrameTime()*SPEEDX);
    
    

    newPos = addVectors(newPos, mouvement);

    normalize(&newPos);//normalise le vecteur de position

    camera->position.x = newPos.x * rhoCam;// replace la caméra à son écart cible
    camera->position.y = newPos.y * rhoCam;
    camera->position.z = newPos.z * rhoCam;
    
    


}


/****************************************Opérations de vecteurs**************************/

void normalize(Vector3 *vector) {//normalise un vecteur
    double rho = sqrt(pow(vector->x, 2) + pow(vector->y, 2) + pow(vector->z, 2));
    vector->x /= rho;
    vector->y /= rho;
    vector->z /= rho;
}

Vector3 addVectors(Vector3 vectora, Vector3 vectorb) {
    return (Vector3) { vectora.x + vectorb.x, vectora.y + vectorb.y, vectora.z + vectorb.z };

}

Vector3 crossProduct(Vector3 vectora, Vector3 vectorb) {
    return (Vector3) { vectora.y * vectorb.z - vectora.z*vectorb.y , vectora.z * vectorb.x - vectora.x*vectorb.z, vectora.x * vectorb.y - vectora.y*vectorb.x };

}
void multiplyVector(Vector3* vector, double a) {
    vector->x *= a;
    vector->y *= a;
    vector->z *= a;
}