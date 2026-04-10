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

void UpdatePlacement(FeuilleCarte f, PlacementState* state, Camera camera);
void RenderPlacement(GameState* gs, FeuilleCarte f, const PlacementState* state, int score, Camera3D camera);
void ApplyPlacement(FeuilleCarte f, PlacementState* state, int* coinCount);

bool DisplayMenu(GameState* gs);
bool AddPlayer(GameState* gs);

/** Fonctions utilitaires UI **/
void DrawButton(Button* btn);

void UpdateInputBox(InputBox* box);
bool IsCharAllowed(char c);
void DrawFullInputBoxEx(InputBox* box, int stroke, Color bkgColor, Color strokeColor);


void DrawStrokeTextEx(const char* text, int x, int y, int fontSize, Color textColor, Color strokeColor, int thickness);
void DrawStrokeText(const char* text, int x, int y, int fontSize, Color textColor, Color strokeColor);
void DrawRectangleRoundedStrokeEx(Rectangle rec, float roundness, int segments, float lineThick, Color rectColor, Color strokeColor);
void DrawRectangleRoundedStroke(Rectangle rec, float roundness, int segments, Color rectColor, Color strokeColor);
void DrawRectangleStroke(Rectangle rec, float lineThick, Color rectColor, Color strokeColor);

Color multiplyColor(Color color, float factor);



/*OPERATIONS vecteurs*/
void normalize(Vector3* vector);
Vector3 addVectors(Vector3 vectora, Vector3 vectorb);
Vector3 crossProduct(Vector3 vectora, Vector3 vectorb);
void multiplyVector(Vector3* vector, double a);