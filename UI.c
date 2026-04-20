#include "UI.h"






ScreenID RunMenu(GameState* gs) {
	int midX = 0;
	int midY = 0;
	int posY = GetScreenHeight() / 20 + 30 + TITLE_FS + 20;

	Button startBtn = { .color1 = LIME, .color2 = BLACK, .label = "Start !", .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = {0, 0, MAIN_MENU_BTN_WIDTH - 50, MAIN_BUTTON_FS + 30} };
	bool addPlayer = 0;
	char tmppl[33] = "Limite de 100 personnes atteinte";	int tmpplSize = 70;		// tmppl = too many people
	Button addBtn = { .color1 = SKYBLUE, .color2 = BLACK, .label = "+ Add 1 Player", .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE };
	Button ruleBtn = { .color1 = GOLD, .color2 = BLACK, .label = "Game Rules", .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = {0, 0, MAIN_MENU_BTN_WIDTH - 50, MAIN_BUTTON_FS + 30} };
	Button keyBtn = { .color1 = BROWN, .color2 = BLACK, .label = "Key Binds in the Game", .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = {0,0, MAIN_MENU_BTN_WIDTH - 50, MAIN_BUTTON_FS + 30} };
	//char nbPlayers[]
	Button stopBtn = { .color1 = RED, .color2 = BLACK, .label = "Exit The Game", .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS - 5, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = {0, 0, MAIN_MENU_BTN_WIDTH - 250, MAIN_BUTTON_FS + 30} };

	Rectangle* btns[] = { &addBtn.bounds, &ruleBtn.bounds, &keyBtn.bounds, &startBtn.bounds, &stopBtn.bounds };

	while (!WindowShouldClose() && !startBtn.validated && !addPlayer && !stopBtn.validated && !keyBtn.validated && !ruleBtn.validated) {
		midX = GetScreenWidth() / 2;
		midY = GetScreenHeight() / 2;

		addBtn.bounds = (Rectangle){ midX - MAIN_MENU_BTN_WIDTH / 2, posY + 30, MAIN_MENU_BTN_WIDTH - 50, addBtn.fontSize + 40 };

		SortRectangles(btns, 5, BUTTON_DELTA, 0.5f);
		
		addBtn.hovered = CheckCollisionPointRec(GetMousePosition(), addBtn.bounds);
		ruleBtn.hovered = CheckCollisionPointRec(GetMousePosition(), ruleBtn.bounds);
		keyBtn.hovered = CheckCollisionPointRec(GetMousePosition(), keyBtn.bounds);
		startBtn.hovered = CheckCollisionPointRec(GetMousePosition(), startBtn.bounds);
		stopBtn.hovered = CheckCollisionPointRec(GetMousePosition(), stopBtn.bounds);

		if (startBtn.hovered || addBtn.hovered || stopBtn.hovered || ruleBtn.hovered || keyBtn.hovered) {
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
		}
		else {
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}

		BeginDrawing();
		ClearBackground(BGCOLOR);

		// DrawBackgroudMenu(menuBounds);
		posY = DrawTitle();

		// Ajoute un bouton pour lancer la partie
		DrawButton(&startBtn);
		if (startBtn.validated) printf("Lancement de la partie");

		// Ajoute un bouton pour ajouter un joueur
		DrawButton(&addBtn);
		if (gs->playerNumber >= MAX_PLAYER) DrawStrokeText(tmppl, (GetScreenWidth() - MeasureText(tmppl, tmpplSize)) / 2, posY - 15, tmpplSize, RED, multiplyColor(DARKGRAY, 0.4f));
		else if (addBtn.validated) addPlayer = 1;

		// afficher le nombre de joueurs
		DrawText(TextFormat("There is %s%d/%d players", gs->playerNumber > 10 ? "already " : "", gs->playerNumber, MAX_PLAYER), addBtn.bounds.x, addBtn.bounds.y + addBtn.bounds.height + 5, 20, WHITE);

		// Ajoute un bouton pour quitter le Jeu
		DrawButton(&stopBtn);

		DrawButton(&ruleBtn);
		DrawButton(&keyBtn);

		// Online : Ajoute un bouton pour rejoindre et quitter le lobby


		EndDrawing();
	}
	if (startBtn.validated) {
		printf("\n\nBouton Start Validé");
		return SCREEN_GAME;
	}
	if (addPlayer) {
		printf("\n\nBouton Add Validé");
		return SCREEN_ADD_PLAYER;
	}
	if (ruleBtn.validated) {
		printf("\n\nBouton Rules Validé");
		return SCREEN_RULES;
	}
	if (keyBtn.validated) {
		printf("\n\nBouton KeyBinds Validé");
		return SCREEN_KEYBINDS;
	}
	if (stopBtn.validated || WindowShouldClose()) {
		printf("\n\nBouton Stop Validé");
		return SCREEN_EXIT;
	}

	return SCREEN_EXIT;
}

ScreenID RunAddPlayer(GameState* gs) {
	int midX = 0;
	int midY = 0;
	int posY = GetScreenHeight() / 20 + 30 + TITLE_FS + 20;

	Rectangle menuBounds = (Rectangle){ 0 };
	char addPplLabel[19] = "Add a new player !"; 	int addPplFontSize = 50;
	Rectangle iptNameBox = (Rectangle){ 0 };
	int d = 40; // Espacement entre les boutons

	Button cclBtn = { .color1 = RED, .color2 = GRAY, .label = "Cancel", .labelColor = WHITE, .fontSize = 40, .corner = 90, .stroke = 3 };
	Button addBtn = { .color1 = LIME, .color2 = GRAY, .label = "Add", .labelColor = WHITE, .fontSize = 40, .corner = 90, .stroke = 3 };

	InputBox nameIptBox = { .maxLength = MAX_NAME_LENGTH,.dx = 5, .text = "", .length = 0, .fontSize = 20, .active = 1, .validated = 0 };
	char addLabel[19] = "New player name : ";
	int addLabelFontSize = 32;

	bool add = false;

	while (!WindowShouldClose() && !add && !cclBtn.validated) {
		midX = GetScreenWidth() / 2;
		midY = GetScreenHeight() / 2;

		menuBounds = (Rectangle){ midX - ADD_MENU_WIDTH / 2, posY,  ADD_MENU_WIDTH, ADD_MENU_HEIGHT };

		cclBtn.bounds = (Rectangle){ menuBounds.x + 5, menuBounds.y + menuBounds.height - (cclBtn.fontSize + 30) - 10, menuBounds.width / 2 - 10, cclBtn.fontSize + 30 };
		cclBtn.hovered = CheckCollisionPointRec(GetMousePosition(), cclBtn.bounds);

		addBtn.bounds = (Rectangle){ midX + 5, menuBounds.y + menuBounds.height - (addBtn.fontSize + 30) - 10, menuBounds.width / 2 - 10, addBtn.fontSize + 30 };
		addBtn.hovered = CheckCollisionPointRec(GetMousePosition(), addBtn.bounds);

		nameIptBox.bounds = (Rectangle){ menuBounds.x + 5 + MeasureText(addLabel, addLabelFontSize) + 5 + 2, menuBounds.y + addPplFontSize + 30 + 5, menuBounds.width - (5 + MeasureText(addLabel, addLabelFontSize) + 5) - 15, nameIptBox.fontSize + 2 };
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
		//DrawBackgroudMenu(menuBounds);
		posY = DrawTitle();

		// Afficher le menu d'ajout
		//DrawRectangleRoundedStrokeEx(menuBounds, .1f, 10, 3, BROWN, DARKGRAY);
		DrawText(addPplLabel, midX - MeasureText(addPplLabel, addPplFontSize) / 2, menuBounds.y + 3, addPplFontSize, WHITE);

		DrawText(addLabel, menuBounds.x + 5, menuBounds.y + addPplFontSize + 30, addLabelFontSize, WHITE);
		DrawFullInputBoxEx(&nameIptBox, 2, GRAY, LIGHTGRAY);

		DrawButton(&cclBtn);
		DrawButton(&addBtn);


		EndDrawing();

		add = ((addBtn.validated || nameIptBox.validated) && strcmp(nameIptBox.text, ""));
	}
	if (add) {
		strcpy(gs->players[gs->playerNumber++].name, nameIptBox.text);
	}

	return SCREEN_MENU;
}

ScreenID RunRules(GameState* gs) {
	int midX = 0;
	int midY = 0;
	int posY = GetScreenHeight() / 20 + 30 + TITLE_FS + 20;

	Page pages[PAGE_NB] = {
		(Page){.Text = OBJECTIF_TEXT, .Title = "Histoire et But du jeu" },
		(Page){.Text = TURN_TEXT, .Title = "Déroulement d'un tour" },
		(Page){.Text = PLACEMENT_TEXT, .Title = "Explication des terrains" },
		(Page){.Text = RYTHME_TEXT, .Title = "Mecanique des Saisons" },
		(Page){.Text = ENEMY_TEXT, .Title = "Apparitions des Monstres" },
		(Page){.Text = SCORE_TEXT, .Title = "Comment marquer des points" }
	};

	int currentPage = 0;

	Button Next = (Button){ .label = ">", .fontSize = 30, .labelColor = BLACK, .corner = 90, .stroke = 3, .color1 = DARKGRAY, .color2 = LIME };
	Button Previous = (Button){ .label = "<", .fontSize = 30, .labelColor = BLACK, .corner = 90, .stroke = 3, .color1 = DARKGRAY, .color2 = RED };
	Button Exit = (Button){ .label = "OK", .fontSize = 30, .labelColor = BLACK, .corner = 80, .stroke = 3, .color1 = DARKGRAY, .color2 = BLUE };

	Rectangle textZone;


	bool done = 0;
	while (!WindowShouldClose() && !Exit.validated) {
		midX = GetScreenWidth() / 2;
		midY = GetScreenHeight() / 2;

		textZone = (Rectangle){ midX - max(GetScreenWidth() * 3 / 4, TEXT_ZONE_WIDTH_MIN) / 2, posY + 50, max(GetScreenWidth() * 3 / 4, TEXT_ZONE_WIDTH_MIN) , GetScreenHeight() * 18 / 20 - 70 - (posY + 50) };

		Previous.bounds = (Rectangle){ textZone.x, textZone.y + textZone.height + 20, textZone.width / 3 - 10, Previous.fontSize + 20 };
		Previous.hovered = CheckCollisionPointRec(GetMousePosition(), Previous.bounds);

		Exit.bounds = (Rectangle){ midX - textZone.width / (4 * 2), Previous.bounds.y + Previous.bounds.height, textZone.width / 4, Exit.fontSize + 20 };
		Exit.hovered = CheckCollisionPointRec(GetMousePosition(), Exit.bounds);

		Next.bounds = (Rectangle){ textZone.x + textZone.width * 2 / 3 + 10, Previous.bounds.y, textZone.width / 3 - 10, Next.fontSize + 20 };
		Next.hovered = CheckCollisionPointRec(GetMousePosition(), Next.bounds);


		if (Next.validated) currentPage = (currentPage + 1) % PAGE_NB;
		if (Previous.validated) currentPage = (currentPage + PAGE_NB - 1) % PAGE_NB;

		if (Previous.hovered || Exit.hovered || Next.hovered) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
		else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


		BeginDrawing();
		ClearBackground(BGCOLOR);
		posY = DrawTitle();

		DrawTextWrapped(pages[currentPage].Text, textZone, (int)min(textZone.width / 35, textZone.height / 15), WHITE);

		DrawStrokeTextEx(TextFormat("Page : %s", pages[currentPage].Title), textZone.x + 10, posY, 35, BLACK, GOLD, 1);

		DrawRectangleRoundedLinesEx((Rectangle) { textZone.x - TEXT_ZONE_PADDING, textZone.y - TEXT_ZONE_PADDING, textZone.width + 2 * TEXT_ZONE_PADDING, textZone.height + 2 * TEXT_ZONE_PADDING, }, .05, 10, 2, multiplyColor(BROWN, 1.2f));

		DrawStrokeText(TextFormat("%d / %d", currentPage + 1, PAGE_NB), midX - MeasureText(TextFormat("%d / %d", currentPage + 1, PAGE_NB), 40) / 2, Previous.bounds.y, 40, GOLD, BLACK);

		DrawButton(&Previous);
		DrawButton(&Next);
		DrawButton(&Exit);


		EndDrawing();


	}

	return SCREEN_MENU;
}



ScreenID RunKeybinds(GameState* gs) {
	int midX = 0;
	int midY = 0;
	int posY = GetScreenHeight() / 20 + 30 + TITLE_FS + 20;

	Rectangle rMove = (Rectangle){ midX - MeasureText(KEY_TEXT_MOVE, KEY_FS) / 2, posY + 20, MeasureText(KEY_TEXT_MOVE, KEY_FS), KEY_FS };
	Rectangle rColorSwitch = (Rectangle){ 0, 0, MeasureText(KEY_TEXT_COLOR_SWITCH, KEY_FS), KEY_FS };
	Rectangle rShapeSwitch = (Rectangle){ 0, 0, MeasureText(KEY_TEXT_SHAPE_SWITCH, KEY_FS), KEY_FS };
	Rectangle rRotate = (Rectangle){ 0, 0, MeasureText(KEY_TEXT_ROTATE, KEY_FS), KEY_FS };
	Rectangle rFlip= (Rectangle){ 0, 0, MeasureText(KEY_TEXT_FLIP, KEY_FS), KEY_FS };
	Rectangle rCamera = (Rectangle){ 0, 0, MeasureText(KEY_TEXT_CAMERA, KEY_FS), KEY_FS };
	Rectangle rPlace = (Rectangle){ 0, 0, MeasureText(KEY_TEXT_PLACE, KEY_FS), KEY_FS };

	Button Exit = (Button){ .label = "OK", .fontSize = 30, .labelColor = BLACK, .corner = 80, .stroke = 3, .color1 = DARKGRAY, .color2 = BLUE, .bounds = (Rectangle){0, 0, 300, 0} };

	Rectangle* recTextList[] = { &rMove, &rColorSwitch, &rShapeSwitch, &rRotate, &rFlip, &rCamera, &rPlace, &Exit.bounds };

	while (!WindowShouldClose() && !Exit.validated) {
		midX = GetScreenWidth() / 2;
		midY = GetScreenHeight() / 2;

		rMove = (Rectangle){ midX - MeasureText(KEY_TEXT_MOVE, KEY_FS) / 2, posY + 20, MeasureText(KEY_TEXT_MOVE, KEY_FS), KEY_FS };
		SortRectangles(recTextList, 8, KEY_PAD, .5f);
		Exit.bounds = (Rectangle){ Exit.bounds.x, Exit.bounds.y + KEY_PAD, 300, Exit.fontSize + 20 };
		Exit.hovered = CheckCollisionPointRec(GetMousePosition(), Exit.bounds);


		if (Exit.hovered) SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		else SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

		BeginDrawing();
		ClearBackground(BGCOLOR);
		posY = DrawTitle();

		// Affichage des Touches par lignes :
		DrawStrokeTextEx(KEY_TEXT_MOVE, rMove.x, rMove.y, KEY_FS, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rMove.y + KEY_FS + KEY_PAD / 2, midX + KEY_LINE_WIDTH / 2, rMove.y + KEY_FS + KEY_PAD / 2, GOLD);

		DrawStrokeTextEx(KEY_TEXT_COLOR_SWITCH, rColorSwitch.x, rColorSwitch.y, KEY_FS, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rColorSwitch.y + KEY_FS + KEY_PAD / 2, midX + KEY_LINE_WIDTH / 2, rColorSwitch.y + KEY_FS + KEY_PAD / 2, GOLD);

		DrawStrokeTextEx(KEY_TEXT_SHAPE_SWITCH, rShapeSwitch.x, rShapeSwitch.y, KEY_FS, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rShapeSwitch.y + KEY_FS + KEY_PAD / 2, midX + KEY_LINE_WIDTH / 2, rShapeSwitch.y + KEY_FS + KEY_PAD / 2, GOLD);

		DrawStrokeTextEx(KEY_TEXT_ROTATE, rRotate.x, rRotate.y, KEY_FS, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rRotate.y + KEY_FS + KEY_PAD / 2, midX + KEY_LINE_WIDTH / 2, rRotate.y + KEY_FS + KEY_PAD / 2, GOLD);

		DrawStrokeTextEx(KEY_TEXT_FLIP, rFlip.x, rFlip.y, KEY_FS, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rFlip.y + KEY_FS + KEY_PAD / 2, midX + KEY_LINE_WIDTH / 2, rFlip.y + KEY_FS + KEY_PAD / 2, GOLD);

		DrawStrokeTextEx(KEY_TEXT_CAMERA, rCamera.x, rCamera.y, KEY_FS, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rCamera.y + KEY_FS + KEY_PAD / 2, midX + KEY_LINE_WIDTH / 2, rCamera.y + KEY_FS + KEY_PAD / 2, GOLD);

		DrawStrokeTextEx(KEY_TEXT_PLACE, rPlace.x, rPlace.y, KEY_FS, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rPlace.y + KEY_FS + KEY_PAD, midX + KEY_LINE_WIDTH / 2, rPlace.y + KEY_FS + KEY_PAD, GOLD);

		DrawButton(&Exit);


		EndDrawing();


	}
	printf("\n\n%d x %d\n\n", GetScreenHeight(), GetScreenWidth());


	return SCREEN_MENU;
}

int DrawTitle() {
	int posY = GetScreenHeight() / 20 + 20;
	DrawStrokeTextEx(C_TITLE, (GetScreenWidth() - MeasureText(C_TITLE, TITLE_FS)) / 2, posY, TITLE_FS, GOLD, BLACK, 2);
	return posY + TITLE_FS + 20;
}

void DrawTitleEx(Rectangle r) {
	DrawStrokeTextEx(C_TITLE, r.x + (r.width - MeasureText(C_TITLE, TITLE_FS)) / 2, r.y, TITLE_FS, GOLD, BLACK, 2);
}

void DrawBackgroudMenu(Rectangle r) {
	//DrawRectangleRoundedStrokeEx(r, .2f, 10, 3, BLACK, RED);
	//DrawRectangleRoundedLinesEx(r, .2f, 10, 3, YELLOW);
	Rectangle tr = r;
	tr.y += 10;
	DrawTitleEx(tr);
}

void DrawTextWrapped(const char* text, Rectangle r, int fs, Color textColor) {
	float curX = r.x;									// Curseur en X
	float curY = r.y;									// Curseur en Y
	float maxX = r.x + r.width;							// Taille max à ne pas dépasser
	float lineHeight = fs * 1.1f;						// Hauteur de ligne
	int spaceW = (int)(MeasureText(" ", fs) * 1.5f);	// Taille d'espace
	char word[30] = "";
	int wordSize = 0;
	char c = 'a';										// Char actuel
	int drawSize = 0;									// Taille dessinée
	int len = strlen(text);

	for (int i = 0; i < len; i++) {
		strcpy(word, "");
		c = text[i];					
		wordSize = 0;
		while (i < len && c != ' ' && c != '\n') {
			word[wordSize++] = c;
			word[wordSize] = '\0';
			c = text[++i];
		}
		drawSize = MeasureText(word, fs);
		if (curX + drawSize > maxX) {
			curY += lineHeight;
			curX = r.x;
		}
		DrawText(word, curX, curY, fs, textColor);
		curX += drawSize + spaceW;
		if (c == '\n') {
			curY += lineHeight;
			curX = r.x;
		}
	}
}

void SortRectangles(Rectangle** rlist, int listLen, int pad, float anchorPoint) {
	int delta = 0;
	for (int i = 0; i < listLen; i++) {
		rlist[i]->x = rlist[0]->x + (rlist[0]->width - rlist[i]->width) * anchorPoint;
		rlist[i]->y = rlist[0]->y + delta;
		delta += rlist[i]->height + pad;
		//printf(";%d, %d\n", i, delta);
	}
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
	float scale = 1.0f;
	int depthFactor = 0;	// profondeur en %

	if (pressed) {
		scale = 0.93f;
		depthFactor = -10;    // Glisse vers le bas
	}
	else if (btn->hovered) {
		scale = 1.03f;
		depthFactor = 5;
	}

	if (scale != 1.0f) {
		float newWidth = r.width * scale;
		float newHeight = r.height * scale;

		// Centrage
		r.x += (r.width - newWidth) / 2.0f;
		r.y += (r.height - newHeight) / 2.0f;

		// Calcul du décalage proportionnel pour un effet de profondeur
		float offset = r.height * depthFactor / 100;

		// Application
		r.width = newWidth;
		r.height = newHeight;
		//r.x -= offset;
		r.y -= offset;
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
	DrawText(btn->label, r.x + (r.width - MeasureText(btn->label, fontSize)) / 2, r.y + (r.height - fontSize) / 2, fontSize, btn->labelColor);

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
	DrawStrokeTextEx(text, x, y, fontSize, textColor, strokeColor, fontSize / 30);
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


