#pragma once

#include "Data.h"
#include "LibCarthographie.h"


void DrawMapGrid(int slices, float spacing);
void GUIDrawFeuille(FeuilleCarte f, FeuilleCarte temp);
int GUIplacementShape(FeuilleCarte f, const Piece* shape, int material, Camera3D camera);
int GUIPlacementCard(GameState* gs, FeuilleCarte f, const ExploreCard* card, int score, int isRuin, int* coinCount, Camera3D camera);
int GUIplacementDefault(FeuilleCarte f, int  material, Camera3D camera);
int GUIplacementDefaultCard(GameState* gs, FeuilleCarte f, const ExploreCard* card, int score, int isRuin, int* coinCount, Camera3D camera);
void GUIdrawGrille();
void GUIUpdateCustomCamera(Camera3D *camera);


void UpdatePlacement(FeuilleCarte f, PlacementState* state);
void RenderPlacement(GameState* gs, FeuilleCarte f, const PlacementState* state, int score, Camera3D camera);
void ApplyPlacement(FeuilleCarte f, PlacementState* state, int* coinCount);



/*OP2RATIONS vecteurs*/
void normalize(Vector3* vector);
Vector3 addVectors(Vector3 vectora, Vector3 vectorb);
Vector3 crossProduct(Vector3 vectora, Vector3 vectorb);
void multiplyVector(Vector3* vector, double a);