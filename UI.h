#pragma once

#include "Data.h"



bool RunMenu(GameState* gs);
bool RunAddPlayer(GameState* gs);

typedef enum {
	SCREEN_MENU,
	SCREEN_ADD_PLAYER,
	SCREEN_RULES,
	SCREEN_KEYBINDS,
	SCREEN_EXIT
} ScreenID;

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








