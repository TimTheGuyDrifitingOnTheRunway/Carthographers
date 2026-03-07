#define _CRT_SECURE_NO_WARNINGS
#include "LibCarthographie.h"
#include "Lib3d.h"
#include "GameManager.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include <string.h>

#define BGCOLOR RAYWHITE



int main()
{
    srand(time(NULL));


    FeuilleCarte f;
    initCarte2(f, TRUE, TRUE);



    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;
    printf("\n\n\n\n\n\n\n Debug 1 \n\n\n\n\n\n\n");


    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    printf("\n\n\n\n\n\n\n Debug 2 \n\n\n\n\n\n\n");


    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 1.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
    //DisableCursor();
    

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    printf("\n\n\n\n\n\n\n Debug 3 \n\n\n\n\n\n\n");


	SetupGame();


	Piece* pieceTab[] = {&POINT, &NOTHING, &U, &L, &L_L, &B_L, &LINE, &L_LINE, &CUBE, &T, &L_T, &RECT, &RECT_WITH_HOLE, &DIAG, &L_DIAG, &CROSS, &STAIRS, &Z, &B_Z, &STRANGE};
    Piece radompiece;
	copyPiece(pieceTab[3], radompiece);
    //for (int i = 0; i < 2; i++) {GUIplacementShape(f, pieceTab[3], FORET, camera); }
    GUIplacementDefault(f, 5, camera);
	WaitTime(1.0f);
	printf("\n\n\n\n\n\n\n Debug de fin de partie \n\n\n\n\n\n\n");

    printf("\n\n stoneSideQuest points = %d\n", calcStoneSideQuest(f));
    WaitTime(1.0f);


    



    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------






    FeuilleCarte test = {
        {4, 0, 0, 104, 0, 0, 0, 5, 4, 6, 1},
        {0, 3, 0, 2, 0, 0, 0, 100, 3, 5, 2},
        {4, 3, 8, 2, 0, 0, 0, 0, 0, 5, 3},
        {0, 3, 0, 2, 0, 0, 6, 0, 0, 103, 3},
        {0, 3, 3, 2, 2, 8, 0, 0, 0, 5},
        {2 ,2 ,2 ,2 ,6 ,6 ,6 ,3 ,2 ,4},
        {0 ,3 ,3 ,3 ,3 ,3 ,3 ,3 ,3 ,5},
        {4 ,4 ,4 ,4 ,4 ,104 ,4 ,4 ,4 ,5},
        {5 ,5 ,5 ,5 ,5 ,5 ,5 ,2 ,5 ,5},
        {5 ,5 ,5 ,3 ,5 ,5 ,2 ,2 ,5 ,5},
        {2, 4, 5, 3, 3, 3, 4, 4, 2, 2, 9}

	}; //Borderlands devrait faire 24 points, Broken Road 15 points
    displayCarte(test);
    printf("\n\n\n");
    /*initCarte2(test, FALSE, FALSE);
    for (int i = 0; i < SIZE; i++) test[0][i] = FORET;
    for (int i = 0; i < SIZE; i++) test[5][i] = RUINE;
    for (int i = 0; i < SIZE; i++) test[7][i] = CHAMPS;
    for (int i = 0; i < SIZE; i++) test[i][3] = FORET;
    for (int i = 1; i < SIZE; i++) test[i][1] = VILLAGE;
    for (int i = 0; i < SIZE; i++) test[i][9] = EAU;
    test[0][0] = EAU;
    test[6][9] = EAU;*


    printf("\nBorderlands points = %d\n", calcBorderlands(test));

    test[1][10] = FORET;

    displayCarte(test);
    printf("\nBroken Road points = %d", calcBrokenRoad(test));

    */
	InfoGroupe listeGroupes[SIZE * SIZE];
    int taille = RecenseEveryGroups(test, listeGroupes);
	for (int i = 0; i < taille; i++) {
        printf("Groupe %d : material %d, taille %d, voisins : ", i, listeGroupes[i].material, listeGroupes[i].taille);
        for (int j = 0; j < 10; j++) {
            if (listeGroupes[i].materialVoisin[j] != 0) {
                printf("(%d : %d) ", j, listeGroupes[i].materialVoisin[j]);
            }
        }
        printf("\n");
    }

    return 0;

}