#pragma once

#include "Data.h"



ScreenID RunMenu(GameState* gs);
ScreenID RunAddPlayer(GameState* gs);
ScreenID RunRules(GameState* gs);
ScreenID RunKeybinds(GameState* gs);

void DrawBackgroudMenu(Rectangle r);
//void DrawTitleEx(GameState* gs, Rectangle r);
int DrawTitle(GameState* gs);
void SortRectangles(Rectangle** rlist, int listLen, int pad, float anchorPoint);		// Trie les rectangles de manière à les rendre alignés sur le 1er élément, en fonction du point d'encrage (0 : en colonne à partir de la gauche, .5f : milieu, 1 : droite)

Vector2 MeasureTextWrapped(const char* text, Rectangle r, int fs);
Vector2 MeasureTextWrappedEx(Font font, const char* text, Rectangle r, int fs);
Vector2 DrawTextWrapped(const char* text, Rectangle r, int fs, Color textColor);
Vector2 DrawTextWrappedEx(Font font, const char* text, Rectangle r, int fs, int stroke, Color textColor, Color strokeColor);

void DrawButton(Button* btn);

void ImageRoundCorner(Image* image, float radius);
void ImageRoundedCorner(Image* image, float roundness);


void UpdateInputBox(InputBox* box);
bool IsCharAllowed(char c);
void DrawFullInputBoxEx(InputBox* box, int stroke, Color bkgColor, Color strokeColor);

// Fonctions pour gérer le chargement des ressources
void* LoadAssetsWorker(void* arg);
void LoadAssetToVRAM(GameState* gs);



void DrawStrokeTextEx(Font font, const char* text, int x, int y, int fontSize, int spacing, Color textColor, Color strokeColor, int thickness);
void DrawStrokeText(Font font, const char* text, int x, int y, int fontSize, Color textColor, Color strokeColor);
void DrawRectangleRoundedStrokeEx(Rectangle rec, float roundness, int segments, float lineThick, Color rectColor, Color strokeColor);
void DrawRectangleRoundedStroke(Rectangle rec, float roundness, int segments, Color rectColor, Color strokeColor);
void DrawRectangleStroke(Rectangle rec, float lineThick, Color rectColor, Color strokeColor);

Color multiplyColor(Color color, float factor);

void DebugAssetViewer(GameState* gs);


// Lib3d functions but used in UI.c (to avoid circular inclusion)

Model loadSkybox(bool useHDR, Image skyboxImage);

