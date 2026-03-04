#define _CRT_SECURE_NO_WARNINGS
#include "LibCarthographie.h"
#include "Lib3d.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#define BGCOLOR RAYWHITE





int main()
{
    srand(time(NULL));


    FeuilleCarte f;
    initCarte(f, TRUE);

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

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

    GUIplacementShape(f, U, FORET, camera);
    GUIplacementShape(f, L, FORET, camera);

    while(0) GUIplacementShape(f, U, FORET, camera);
 
    printf("%d", isGroupAtPosNeighborWithMaterial(f, (Position) { 1, 1 }, MONTAGNE, FALSE));

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;



}