#define _CRT_SECURE_NO_WARNINGS
#include "LibCarthographie.h"
#include "Lib3d.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>





int main()
{
    srand(time(NULL));


    FeuilleCarte f;
    initCarte(f, TRUE);
	placementShape(f, U, FORET);

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 8.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
		GUIDrawFeuille(f);
		
        
        

        DrawGrid(11, 1.0f);

        EndMode3D();


        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;



}