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
                
            if (getMaterialAt(temp, (Position){ i,j } ) != 0) {
                float x = i - (SIZE - 1) / 2.0f;
                float z = j - (SIZE - 1) / 2.0f;
                float y = 0.5;   // Half height so cube sits on grid
				if (temp[i][j] != f[i][j]) y = PLACEMENT_HEIGHT; // Raise cube if it's part of the shape being placed
                Color color;
                switch (getMaterialAt(temp, (Position) { i, j })) {
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
};


int GUIplacementShape(FeuilleCarte f, Piece shape, int material, Camera3D camera) {
    if (checkShape(f, shape)) {
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
            drawable = drawShape(feuilleVide, shape, pos, rotation, material);
            tryDraw(f, feuilleVide, temp);

            BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
            GUIDrawFeuille(f, temp);

            for (int i = 0; i < SIZE; i++) 
                for (int j = 0; j < SIZE; j++) 
                    if (temp[i][j] !=f[i][j])
                        DrawCubeWires((Vector3) { i-SIZE/2, PLACEMENT_HEIGHT, j-SIZE/2 }, 1.0f, 1.0f, 1.0f, BORDERCOLOR);
                
            GUIdrawGrille();
            // DrawCarteGrid(SIZE, 1.0f);
            EndMode3D();
            EndDrawing();
            drawable = drawable && isDrawable(f, feuilleVide);

            UpdateCamera(&camera,CAMERA_THIRD_PERSON);//gestion caméra : toujours au même endraoit
            camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
            
            pos.x += -IsKeyPressed(KEY_J) + IsKeyPressed(KEY_L);// déplacement pièce
            pos.y += -IsKeyPressed(KEY_I) + IsKeyPressed(KEY_K);

            if (IsKeyPressed(KEY_O)) { pos.x = 6; pos.y = 6; }//possibilité de resset

            if (IsKeyPressed(KEY_R)) {
                rotation = (rotation + 1);
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

void DrawCarteGrid(int slices, float spacing) {
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

void GUIdrawGrille() {
	for (int i = -SIZE / 2-1; i <= SIZE / 2; i++) {
        DrawLine3D((Vector3) { (float)i + 0.5f, 0.0f, (float)-SIZE / 2 }, (Vector3) { (float)i + 0.5f, 0.0f, (float)SIZE / 2 }, GRIDCOLOR);
        DrawLine3D((Vector3) { (float)-SIZE / 2, 0.0f, (float)i + 0.5f }, (Vector3) { (float)SIZE / 2, 0.0f, (float)i + 0.5f }, GRIDCOLOR);
    }
}