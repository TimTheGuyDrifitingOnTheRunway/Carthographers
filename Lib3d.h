#pragma once

#include "Data.h"
#include "LibCarthographie.h"
#include "UI.h"




void DrawMapGrid(int slices, float spacing);
void GUIDrawFeuille(FeuilleCarte f, FeuilleCarte temp, Model mountains[], Position mountainPos[NOMBREMONTAGNE], Seed s, ModelList models);
int GUIplacementShape(FeuilleCarte f, const Piece* shape, int material, Camera3D camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models);
int GUIPlacementCard(GameState* gs, FeuilleCarte f, const ExploreCard* card, int score, int isRuin, int* coinCount, Camera3D camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models);
int GUIplacementDefault(FeuilleCarte f, int  material, Camera3D camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models);
int GUIplacementDefaultCard(GameState* gs, FeuilleCarte f, const ExploreCard* card, int score, int isRuin, int* coinCount, Camera3D camera, Model mountains[NOMBREMONTAGNE], Seed s, ModelList models);
void GUIdrawGrille();
void GUIUpdateCustomCamera(Camera3D *camera);

void UpdatePlacement(FeuilleCarte f, PlacementState* state, Camera camera);
void RenderPlacement(GameState* gs, FeuilleCarte f, const PlacementState* state, int score, Camera3D camera, Model mountain[NOMBREMONTAGNE], Position mountainPos[NOMBREMONTAGNE], Seed s, ModelList models);
void ApplyPlacement(FeuilleCarte f, PlacementState* state, int* coinCount);


/** Fonctions utilitaires UI **/


/*OPERATIONS vecteurs*/
void normalize(Vector3* vector);
Vector3 addVectors(Vector3 vectora, Vector3 vectorb);
Vector3 crossProduct(Vector3 vectora, Vector3 vectorb);
void multiplyVector(Vector3* vector, double a);


/*G2N2RATIONS procédurale de montagnes*/
Model generateMountain(int x, int y);
void generateMountainsModels(Model mountains[NOMBREMONTAGNE], FeuilleCarte f, int mountainSeed[2]);


/*generation des forets*/


Image generateForestImage(int x, int y);

ModelList loadModels();