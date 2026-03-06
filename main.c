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
    DisableCursor();
    

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    printf("\n\n\n\n\n\n\n Debug 3 \n\n\n\n\n\n\n");


	SetupGame();


	Piece* pieceTab[] = {POINT, NOTHING, U, L, L_L, B_L, LINE, L_LINE, CUBE, T, L_T, RECT, RECT_WITH_HOLE, DIAG, L_DIAG, CROSS, STAIRS, Z, B_Z};
    Piece radompiece;
	copyPiece(pieceTab[3], radompiece);
    for (int i = 0; i < 2; i++) {GUIplacementShape(f, pieceTab[3], FORET, camera); }
    GUIplacementDefault(f, 5, camera);
	WaitTime(1.0f);
	printf("\n\n\n\n\n\n\n Debug de fin de partie \n\n\n\n\n\n\n");

    printf("\n\n stoneSideQuest points = %d\n", calcStoneSideQuest(f));
    WaitTime(10.0f);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;



}