#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "LibCarthographie.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>


// Eviter les inclusions circulaiers : GameManager
typedef struct ExploreCard ExploreCard;

/*****************************************CONSTANTES d'affichage*************************************************/

#define GRIDCOLOR GRAY
#define BORDERCOLOR MAROON
#define PLACEMENT_HEIGHT 1.0f

#define SPEEDY 5// constantes inverse de vitesse
#define SPEEDX 5


void DrawMapGrid(int slices, float spacing);
void GUIDrawFeuille(FeuilleCarte f, FeuilleCarte temp);
int GUIplacementShape(FeuilleCarte f, const Piece* shape, int material, Camera3D camera);
int GUIPlacementCard(FeuilleCarte f, const ExploreCard* card, int isRuin, int score, Camera3D camera);
int GUIplacementDefault(FeuilleCarte f, int  material, Camera3D camera);
void GUIdrawGrille();
void GUIUpdateCustomCamera(Camera3D *camera);


/*OP2RATIONS vecteurs*/
void normalize(Vector3* vector);
Vector3 addVectors(Vector3 vectora, Vector3 vectorb);
Vector3 crossProduct(Vector3 vectora, Vector3 vectorb);
void multiplyVector(Vector3* vector, double a);