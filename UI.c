#include "UI.h"






bool RunMenu(GameState* gs) {
	int i = 0;
	char title[16] = "Cartographers !";		int titleFontSize = 60;
	Rectangle menuBounds = (Rectangle){ 0 };

	Button startBtn = { .color1 = LIME, .color2 = GRAY, .label = "Start !", .fontSize = MAIN_BUTTON_FS, .corner = 20, .stroke = MAIN_BUTTON_STROKE };
	bool addPlayer = 0;
	char tmppl[33] = "Limite de 100 personnes atteinte";	int tmpplSize = 70;
	Button addBtn = { .color1 = SKYBLUE, .color2 = DARKBLUE, .label = "Add 1 Player", .fontSize = MAIN_BUTTON_FS, .corner = 20, .stroke = MAIN_BUTTON_STROKE };
	//char nbPlayers[]
	Button stopBtn = { .color1 = RED, .color2 = DARKGRAY, .label = "Exit The Game", .fontSize = MAIN_BUTTON_FS - 5, .corner = 20, .stroke = MAIN_BUTTON_STROKE };

	while (!WindowShouldClose() && !startBtn.validated && !addPlayer && !stopBtn.validated) {
		menuBounds = (Rectangle){ (GetScreenWidth() - MAIN_MENU_X) / 2 , (GetScreenHeight() - MAIN_MENU_Y) / 2 ,  MAIN_MENU_X, MAIN_MENU_Y };

		addBtn.bounds = (Rectangle){ menuBounds.x + 25, menuBounds.y + titleFontSize + 50, MAIN_MENU_X - 50, addBtn.fontSize + 40 };
		addBtn.hovered = CheckCollisionPointRec(GetMousePosition(), addBtn.bounds);

		startBtn.bounds = (Rectangle){ addBtn.bounds.x, addBtn.bounds.y + addBtn.bounds.height + BUTTON_DELTA, MAIN_MENU_X - 50, startBtn.fontSize + 40 };
		startBtn.hovered = CheckCollisionPointRec(GetMousePosition(), startBtn.bounds);

		stopBtn.bounds = (Rectangle){ addBtn.bounds.x, startBtn.bounds.y + startBtn.bounds.height + BUTTON_DELTA, MAIN_MENU_X - 50, stopBtn.fontSize + 40 };
		stopBtn.hovered = CheckCollisionPointRec(GetMousePosition(), stopBtn.bounds);

		if (startBtn.hovered || addBtn.hovered || stopBtn.hovered) {
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
		}
		else {
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}

		BeginDrawing();
		ClearBackground(BGCOLOR);

		// Dessine un grand rectangle en tant que menu
		DrawRectangleRounded(menuBounds, .2f, 10, BLACK);
		DrawRectangleRoundedLinesEx(menuBounds, .2f, 10, 3, RED);
		DrawText(title, menuBounds.x + MAIN_MENU_X / 2 - MeasureText(title, titleFontSize) / 2, menuBounds.y + 3, titleFontSize, WHITE);

		// Ajoute un bouton pour lancer la partie
		DrawButton(&startBtn);
		if (startBtn.validated) printf("Lancement de la partie");

		// Ajoute un bouton pour ajouter un joueur
		DrawButton(&addBtn);
		if (gs->playerNumber >= MAX_PLAYER) DrawStrokeText(tmppl, (GetScreenWidth() - MeasureText(tmppl, tmpplSize)) / 2, menuBounds.y - tmpplSize - 20, tmpplSize, RED, multiplyColor(DARKGRAY, 0.4f));
		else if (addBtn.validated) addPlayer = 1;

		// afficher le nombre de joueurs
		DrawText(TextFormat("There is %s%d/%d players", gs->playerNumber > 10 ? "already " : "", gs->playerNumber, MAX_PLAYER), addBtn.bounds.x, addBtn.bounds.y + addBtn.bounds.height + 5, 20, WHITE);

		// Ajoute un bouton pour quitter le Jeu
		DrawButton(&stopBtn);

		// Online : Ajoute un bouton pour rejoindre et quitter le lobby


		EndDrawing();
	}
	if (startBtn.validated) {
		printf("\n\nBouton Start Validé");
		return true;
	}
	if (addPlayer) {
		printf("\n\nBouton Add Validé");
		return RunAddPlayer(gs);
	}
	if (stopBtn.validated || WindowShouldClose()) {
		printf("\n\nBouton Stop Validé");
		return false;
	}

	return false;
}

bool RunAddPlayer(GameState* gs) {
	int middleX = 0;
	int middleY = 0;
	char title[16] = "Cartographers !"; 		int titleFontSize = 60;
	Rectangle menuBounds = (Rectangle){ 0 };
	char addPplLabel[19] = "Add a new player !"; 	int addPplFontSize = 50;
	Rectangle addMBounds = (Rectangle){ 0 };
	Rectangle iptNameBox = (Rectangle){ 0 };
	int d = 40; // Espacement entre les boutons

	Button cclBtn = { .color1 = RED, .color2 = GRAY, .label = "Cancel", .fontSize = 40, .corner = 30, .stroke = 3 };
	Button addBtn = { .color1 = LIME, .color2 = GRAY, .label = "Add", .fontSize = 40, .corner = 30, .stroke = 3 };

	InputBox nameIptBox = { .maxLength = MAX_NAME_LENGTH,.dx = 5, .text = "", .length = 0, .fontSize = 20, .active = 1, .validated = 0 };
	char addLabel[19] = "New player name : ";		int addLabelFontSize = 32;

	bool add = false;

	while (!WindowShouldClose() && !add && !cclBtn.validated) {
		middleX = GetScreenWidth() / 2;
		middleY = GetScreenHeight() / 2;

		menuBounds = (Rectangle){ middleX - MAIN_MENU_X / 2 , middleY - MAIN_MENU_Y / 2 ,  MAIN_MENU_X, MAIN_MENU_Y };
		addMBounds = (Rectangle){ menuBounds.x + 10 ,  menuBounds.y + titleFontSize + 50,  MAIN_MENU_X - 20, 300 };

		cclBtn.bounds = (Rectangle){ addMBounds.x + 5, addMBounds.y + addMBounds.height - (cclBtn.fontSize + 30) - 5, addMBounds.width / 2 - 10, cclBtn.fontSize + 30 };
		cclBtn.hovered = CheckCollisionPointRec(GetMousePosition(), cclBtn.bounds);

		addBtn.bounds = (Rectangle){ middleX + 5, addMBounds.y + addMBounds.height - (addBtn.fontSize + 30) - 5, addMBounds.width / 2 - 10, addBtn.fontSize + 30 };
		addBtn.hovered = CheckCollisionPointRec(GetMousePosition(), addBtn.bounds);

		nameIptBox.bounds = (Rectangle){ addMBounds.x + 5 + MeasureText(addLabel, addLabelFontSize) + 5 + 2, addMBounds.y + addPplFontSize + 30 + 5, addMBounds.width - (5 + MeasureText(addLabel, addLabelFontSize) + 5) - 15, nameIptBox.fontSize + 2 };
		nameIptBox.hovered = CheckCollisionPointRec(GetMousePosition(), nameIptBox.bounds);


		// Mise à jour du curseur
		if (nameIptBox.hovered) {
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
		}
		else if (cclBtn.hovered || addBtn.hovered) {
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
		}
		else {
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}

		BeginDrawing();
		ClearBackground(BGCOLOR);
		DrawRectangleRoundedStrokeEx(menuBounds, .2f, 10, 3, BLACK, RED);
		DrawText(title, middleX - MeasureText(title, titleFontSize) / 2, menuBounds.y, titleFontSize, WHITE);

		// Afficher le menu d'ajout
		DrawRectangleRoundedStrokeEx(addMBounds, .1f, 10, 3, BROWN, DARKGRAY);
		DrawText(addPplLabel, middleX - MeasureText(addPplLabel, addPplFontSize) / 2, addMBounds.y + 3, addPplFontSize, BLACK);

		DrawText(addLabel, addMBounds.x + 5, addMBounds.y + addPplFontSize + 30, addLabelFontSize, BLACK);
		DrawFullInputBoxEx(&nameIptBox, 2, GRAY, LIGHTGRAY);

		DrawButton(&cclBtn);
		DrawButton(&addBtn);


		EndDrawing();

		add = (addBtn.validated || nameIptBox.validated);
	}
	if (add) {
		strcpy(gs->players[gs->playerNumber++].name, nameIptBox.text);
	}
	return RunMenu(gs);
}



/**************** Fonctions Utilitaires ****************/

void DrawButton(Button* btn) {
	Vector2 mouse = GetMousePosition();
	bool pressed = btn->hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	bool clicked = btn->hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

	/*if (btn->hovered) printf("Button hovered\n");
	if (pressed) printf("Button Pressed\n");
	if (clicked) printf("Button Clicked\n");*/

	Rectangle r = btn->bounds;
	if (pressed) {
		r.y += 5;
		r.x += 5;
		r.width -= 10;
		r.height -= 10;
	}

	// Ombre portée
	int dShadow = 7;
	DrawRectangleRounded((Rectangle) { r.x + dShadow - btn->stroke, r.y + dShadow - btn->stroke, r.width + 2 * btn->stroke, r.height + 2 * btn->stroke }, btn->corner * 0.01f, 8, (Color) { 0, 0, 0, 100 });

	// assombrir color1 au clic, éclaircir au hover
	Color bg = pressed ? multiplyColor(btn->color1, 0.7f) : btn->hovered ? multiplyColor(btn->color1, 1.2f) : btn->color1;
	if (btn->stroke > 0) {
		Color strokeC = pressed ? multiplyColor(btn->color2, 0.7f) : btn->hovered ? multiplyColor(btn->color2, 1.2f) : btn->color2;
		DrawRectangleRoundedStrokeEx(r, btn->corner * 0.01f, 8, btn->stroke, bg, strokeC);
	}
	else DrawRectangleRounded(r, btn->corner * 0.01f, 8, bg);


	// Label centré
	int fontSize = pressed ? btn->fontSize - 4 : btn->fontSize;
	DrawText(btn->label, r.x + (r.width - MeasureText(btn->label, fontSize)) / 2, r.y + (r.height - fontSize) / 2, fontSize, WHITE);

	btn->validated = clicked;
}

void UpdateInputBox(InputBox* box) {

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))	box->active = box->hovered;

	if (!box->active) return;

	if (IsKeyPressed(KEY_ENTER) && box->length > 0) {
		printf("entré appuyé 1\n");
		box->validated = true;
	}
	else	box->validated = false;


	int key = GetCharPressed();
	while (key > 0) {
		printf("key reçue : %d\n", key);
		if (IsCharAllowed((char)key) && box->length < box->maxLength - 1) {
			box->text[box->length++] = (char)key;
			box->text[box->length] = '\0';
		}
		key = GetCharPressed();
	}

	if (IsKeyPressed(KEY_BACKSPACE) && box->length > 0)
		box->text[--box->length] = '\0';

}

bool IsCharAllowed(char c) {
	return (c >= 'a' && c <= 'z') || // minuscules
		(c >= 'A' && c <= 'Z') ||	 // majuscules
		(c >= '0' && c <= '9') ||	 // chiffres
		(c == '-') ||				 // tiret
		(c == '\'') ||				 // apostrophe
		(c == ' ');					 // espace
}

void DrawFullInputBoxEx(InputBox* box, int stroke, Color bkgColor, Color strokeColor) {

	UpdateInputBox(box);

	// Fond + contour
	DrawRectangleRec(box->bounds, box->active ? multiplyColor(bkgColor, 0.8f) : bkgColor);
	if (stroke > 0)
		DrawRectangleLinesEx(box->bounds, stroke, strokeColor);

	// Texte avec décalage dx et centralisation du texte en y
	DrawText(box->text, box->bounds.x + box->dx, box->bounds.y + (box->bounds.height - box->fontSize) / 2, box->fontSize, BLACK);

	// Curseur clignotant centré en y
	if (box->active && ((int)(GetTime() * 2) % 2 == 0)) {
		int tw = MeasureText(box->text, box->fontSize);
		DrawText("|", box->bounds.x + box->dx + tw + 2, box->bounds.y + (box->bounds.height - box->fontSize) / 2, box->fontSize, DARKGRAY);
	}

	if (box->validated)	printf("entré appuyé 2\n");

}

void DrawStrokeTextEx(const char* text, int x, int y, int fontSize, Color textColor, Color strokeColor, int thickness) {
	for (int dx = -thickness; dx <= thickness; dx++)	for (int dy = -thickness; dy <= thickness; dy++)
		if (dx != 0 || dy != 0)		DrawText(text, x + dx, y + dy, fontSize, strokeColor);

	DrawText(text, x, y, fontSize, textColor);
}

void DrawStrokeText(const char* text, int x, int y, int fontSize, Color textColor, Color strokeColor) {
	DrawStrokeTextEx(text, x, y, fontSize, textColor, strokeColor, 1);
}

void DrawRectangleRoundedStroke(Rectangle rec, float roundness, int segments, Color rectColor, Color strokeColor) {
	DrawRectangleRoundedStrokeEx(rec, roundness, segments, 2, rectColor, strokeColor);
}

void DrawRectangleRoundedStrokeEx(Rectangle rec, float roundness, int segments, float lineThick, Color rectColor, Color strokeColor) {
	DrawRectangleRounded(rec, roundness, segments, rectColor);
	DrawRectangleRoundedLinesEx(rec, roundness, segments, lineThick, strokeColor);
}

void DrawRectangleStroke(Rectangle rec, float lineThick, Color rectColor, Color strokeColor) {
	DrawRectangleRec(rec, rectColor);
	DrawRectangleLinesEx(rec, lineThick, strokeColor);
}

Color multiplyColor(Color color, float factor) {
	return (Color) { fminf(color.r * factor, 255), fminf(color.g * factor, 255), fminf(color.b * factor, 255), color.a };
}


