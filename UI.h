#pragma once

#include "Data.h"



ScreenID RunMenu(GameState* gs);
ScreenID RunAddPlayer(GameState* gs);
ScreenID RunRules(GameState* gs);
ScreenID RunKeybinds(GameState* gs);

void DrawBackgroudMenu(Rectangle r);
void DrawTitleEx(Rectangle r);
int DrawTitle();
void SortRectangles(Rectangle** rlist, int listLen, int pad, float anchorPoint);		// Trie les rectangles de manière à les rendre alignés sur le 1er élément, en fonction du point d'encrage (0 : en colonne à partir de la gauche, .5f : milieu, 1 : droite)

Vector2 MeasureTextWrapped(const char* text, Rectangle r, int fs, Color textColor);
Vector2 DrawTextWrapped(const char* text, Rectangle r, int fs, Color textColor);

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








