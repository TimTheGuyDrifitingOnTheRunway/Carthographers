#include "UI.h"



ScreenID RunMenu(GameState* gs) {
	int midX = 0;
	int midY = 0;
	int posY = GetScreenHeight() / 20 + 30 + TITLE_FS + 20;

	char play[12] = "chargement";
	
	
	bool addPlayer = 0;
	//char tmppl[33] = "Limite de 100 personnes atteinte";	int tmpplSize = 70;		// tmppl = too many people
	Button addBtn = { .color1 = SKYBLUE, .color2 = BLACK, .label = "+ Ajouter un joueur", .labelFont = MAIN_BTN_FONT, .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = (Rectangle){ midX - MAIN_MENU_BTN_WIDTH / 2, posY + 30, MAIN_MENU_BTN_WIDTH - 50, MAIN_BUTTON_FS + 10} };
	Button ruleBtn = { .color1 = GOLD, .color2 = BLACK, .label = "Règles du Jeu", .labelFont = MAIN_BTN_FONT, .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = (Rectangle){0, 0, MAIN_MENU_BTN_WIDTH - 50, MAIN_BUTTON_FS + 10} };
	Button keyBtn = { .color1 = BROWN, .color2 = BLACK, .label = "Commandes du jeu", .labelFont = MAIN_BTN_FONT, .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = (Rectangle){0,0, MAIN_MENU_BTN_WIDTH - 50, MAIN_BUTTON_FS + 10} };

	Button startBtn = { .color1 = LIME, .color2 = BLACK, .label = play, .labelFont = MAIN_BTN_FONT, .labelColor = BLACK, .fontSize = MAIN_BUTTON_FS + 10, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = (Rectangle){0, 0, MAIN_MENU_BTN_WIDTH - 50, MAIN_BUTTON_FS + 25} };
	Button stopBtn = { .color1 = RED, .color2 = BLACK, .label = "Quitter le jeu", .labelFont = MAIN_BTN_FONT, .labelColor = BLACK, .fontSize = (int)MAIN_BUTTON_FS * 0.8f, .corner = MAIN_BUTTON_CORNER, .stroke = MAIN_BUTTON_STROKE, .bounds = (Rectangle){0, 0, MAIN_MENU_BTN_WIDTH * 0.6f, MAIN_BUTTON_FS } };

	

	Rectangle* btns[] = { &addBtn.bounds, &ruleBtn.bounds, &keyBtn.bounds, &startBtn.bounds, &stopBtn.bounds };

	while (!WindowShouldClose() && !(startBtn.validated && (gs->loadCtx->avancement >= NOMBRE_TOTAL_ASSETS)) && !addPlayer && !stopBtn.validated && !keyBtn.validated && !ruleBtn.validated) {

		startBtn.label = gs->loadCtx->avancement >= NOMBRE_TOTAL_ASSETS  ? "Jouer !" : "Chargement en cours";
		startBtn.color1 = gs->loadCtx->avancement >= NOMBRE_TOTAL_ASSETS ? LIME : DARKGRAY;

		midX = GetScreenWidth() / 2;
		midY = GetScreenHeight() / 2;

		addBtn.bounds.x = midX - MAIN_MENU_BTN_WIDTH / 2;
		addBtn.bounds.y = posY + 30, MAIN_MENU_BTN_WIDTH - 50;

		SortRectangles(btns, 5, BUTTON_DELTA, 0.5f);

		addBtn.hovered = CheckCollisionPointRec(GetMousePosition(), addBtn.bounds);
		ruleBtn.hovered = CheckCollisionPointRec(GetMousePosition(), ruleBtn.bounds);
		keyBtn.hovered = CheckCollisionPointRec(GetMousePosition(), keyBtn.bounds);
		startBtn.hovered = CheckCollisionPointRec(GetMousePosition(), startBtn.bounds)&&(gs->loadCtx->avancement >= NOMBRE_TOTAL_ASSETS);
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
		posY = DrawTitle(gs);

		// Ajoute un bouton pour lancer la partie
		DrawButton(&startBtn);
		if (startBtn.validated) printf("Lancement de la partie");

		// Ajoute un bouton pour ajouter un joueur
		DrawButton(&addBtn);
		//if (gs->playerNumber >= MAX_PLAYER) DrawStrokeText(gs->assets.fonts[FONT_FREDOKA_CM], tmppl, (GetScreenWidth() - MeasureTextEx(gs->assets.fonts[FONT_FREDOKA_CM], tmppl, tmpplSize, NORMAL_SPACING).x) / 2, posY - 15, tmpplSize, RED, multiplyColor(DARKGRAY, 0.4f));
		if (addBtn.validated) addPlayer = 1;

		// afficher le nombre de joueurs
		DrawStrokeTextEx(gs->assets.fonts[FONT_FREDOKA_CM], TextFormat("Il y a %s%d joueur%c", gs->playerNumber > 10 ? "déjà " : "", gs->playerNumber, gs->playerNumber > 1 ? 's' : ' '), addBtn.bounds.x, addBtn.bounds.y + addBtn.bounds.height + 5, 20,NORMAL_SPACING, WHITE, BLACK, 1);

		// Ajoute un bouton pour quitter le Jeu
		DrawButton(&stopBtn);

		DrawButton(&ruleBtn);
		DrawButton(&keyBtn);

		// Online : Ajoute un bouton pour rejoindre et quitter le lobby

		EndDrawing();

		LoadAssetToVRAM(gs);
	}
	if (startBtn.validated && (gs->loadCtx->avancement >= NOMBRE_TOTAL_ASSETS)) {
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
	char addPplLabel[] = "Ajouter un nouveau joueur !"; 	int addPplFontSize = 50;
	Rectangle iptNameBox = (Rectangle){ 0 };
	int d = 40; // Espacement entre les boutons

	Button cclBtn = { .color1 = RED, .color2 = BLACK, .label = "Annuler", .labelFont = gs->assets.fonts[FONT_METAMORPHOUS], .labelColor = BLACK, .fontSize = 40, .corner = 90, .stroke = 3 };
	Button addBtn = { .color1 = LIME, .color2 = BLACK, .label = "Ajouter", .labelFont = gs->assets.fonts[FONT_METAMORPHOUS], .labelColor = BLACK, .fontSize = 40, .corner = 90, .stroke = 3 };

	InputBox nameIptBox = { .maxLength = MAX_NAME_LENGTH,.dx = 5, .text = "", .length = 0, .fontSize = 30, .textFont = gs->assets.fonts[FONT_FREDOKA_CM], .active = 1, .validated = 0 };
	char addLabel[] = "Nom du nouveau joueur ";
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

		int addPplw = MeasureTextEx(nameIptBox.textFont, addLabel, addLabelFontSize, NORMAL_SPACING).x;
		nameIptBox.bounds = (Rectangle){ menuBounds.x + 5 + addPplw + 5 + 2, menuBounds.y + addPplFontSize + 30 + 10, menuBounds.width - (5 + addPplw + 5) - 15, nameIptBox.fontSize + 8 };
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
		posY = DrawTitle(gs);

		// Afficher le menu d'ajout
		DrawTextEx(gs->assets.fonts[FONT_PIRATA_ONE], addPplLabel, (Vector2) { midX - MeasureTextEx(gs->assets.fonts[FONT_PIRATA_ONE], addPplLabel, addPplFontSize, NORMAL_SPACING).x / 2, menuBounds.y + 3 }, addPplFontSize, NORMAL_SPACING, WHITE);	// Affiche le titre du menu

		DrawTextEx(nameIptBox.textFont, addLabel, (Vector2) { menuBounds.x + 5, nameIptBox.bounds.y + (nameIptBox.bounds.height - addLabelFontSize) / 2 }, addLabelFontSize, NORMAL_SPACING, WHITE);
		DrawFullInputBoxEx(&nameIptBox, 2, GRAY, LIGHTGRAY);

		DrawButton(&cclBtn);
		DrawButton(&addBtn);


		EndDrawing();

		add = ((addBtn.validated || nameIptBox.validated) && strcmp(nameIptBox.text, ""));
		LoadAssetToVRAM(gs);

	}
	if (add) {

		PlayerState* tmp = (PlayerState*)realloc(gs->players, (gs->playerNumber + 1) * sizeof(PlayerState));
		if (tmp) {
			gs->players = tmp;
			strcpy(gs->players[gs->playerNumber++].name, nameIptBox.text);
		}
		
	}

	return SCREEN_MENU;
}

ScreenID RunRules(GameState* gs) {
	int midX = 0;
	int midY = 0;
	int posY = GetScreenHeight() / 20 + 30 + TITLE_FS + 20;

	RulePage pages[PAGE_NB] = {
		(RulePage) {.Text = OBJECTIF_TEXT, .Title = "Histoire et But du jeu"},
		(RulePage) {.Text = TURN_TEXT, .Title = "Déroulement d'un tour"},
		(RulePage) {.Text = PLACEMENT_TEXT, .Title = "Explication des terrains"},
		(RulePage) {.Text = RYTHME_TEXT, .Title = "Mecanique des Saisons"},
		(RulePage) {.Text = ENEMY_TEXT, .Title = "Apparitions des Monstres"},
		(RulePage) {.Text = SCORE_TEXT, .Title = "Comment marquer des points"}
	};

	int currentPage = 0;

	Button Next = (Button){ .label = ">", .fontSize = 30, .labelColor = BLACK, .labelFont = GetFontDefault(), .corner = 90, .stroke = 3, .color1 = DARKGRAY, .color2 = LIME };
	Button Previous = (Button){ .label = "<", .fontSize = 30, .labelColor = BLACK, .labelFont = GetFontDefault(), .corner = 90, .stroke = 3, .color1 = DARKGRAY, .color2 = RED };
	Button Exit = (Button){ .label = "OK", .fontSize = 30, .labelColor = BLACK, .labelFont = gs->assets.fonts[FONT_FREDOKA_SB], .corner = 80, .stroke = 3, .color1 = DARKGRAY, .color2 = BLUE };

	Rectangle textZone;


	bool done = 0;
	while (!WindowShouldClose() && !Exit.validated) {
		midX = GetScreenWidth() / 2;
		midY = GetScreenHeight() / 2;

		textZone = (Rectangle){ midX - max(GetScreenWidth() * 3 / 4, TEXT_ZONE_WIDTH_MIN) / 2, posY + 50, max(GetScreenWidth() * 3 / 4, TEXT_ZONE_WIDTH_MIN) , GetScreenHeight() * 18 / 20 - 80 - (posY + 50) };

		Previous.bounds = (Rectangle){ textZone.x, textZone.y + textZone.height + 25, textZone.width / 3 - 10, Previous.fontSize + 20 };
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
		posY = DrawTitle(gs);
		DrawTextWrappedEx(PAGE_FONT, pages[currentPage].Text, textZone, PAGE_FS, 1, WHITE, BLACK);

		DrawStrokeTextEx(gs->assets.fonts[FONT_METAMORPHOUS], TextFormat("  %s", pages[currentPage].Title), textZone.x + 10, posY, 35, NORMAL_SPACING, BLACK, GOLD, 1);

		DrawRectangleRoundedLinesEx((Rectangle) { textZone.x - TEXT_ZONE_PADDING, textZone.y - TEXT_ZONE_PADDING, textZone.width + 2 * TEXT_ZONE_PADDING, textZone.height + 2 * TEXT_ZONE_PADDING, }, .05, 10, 2, multiplyColor(BROWN, 1.2f));

		DrawStrokeText(gs->assets.fonts[FONT_METAMORPHOUS], TextFormat("%d / %d", currentPage + 1, PAGE_NB), midX - MeasureTextEx(gs->assets.fonts[FONT_METAMORPHOUS], TextFormat("%d / %d", currentPage + 1, PAGE_NB), 40, NORMAL_SPACING).x / 2, Previous.bounds.y, 40, GOLD, BLACK);

		DrawButton(&Previous);
		DrawButton(&Next);
		DrawButton(&Exit);


		EndDrawing();

		LoadAssetToVRAM(gs);

	}

	return SCREEN_MENU;
}



ScreenID RunKeybinds(GameState* gs) {
	int midX = 0;
	int midY = 0;
	int posY = GetScreenHeight() / 20 + 30 + TITLE_FS + 20;

	Rectangle rMove = (Rectangle){ midX - MeasureTextEx(KEY_FONT, KEY_TEXT_MOVE, KEY_FS, NORMAL_SPACING).x / 2, posY + 20, MeasureTextEx(KEY_FONT, KEY_TEXT_MOVE, KEY_FS, NORMAL_SPACING).x, KEY_FS };
	Rectangle rColorSwitch = (Rectangle){ 0, 0, MeasureTextEx(KEY_FONT, KEY_TEXT_COLOR_SWITCH, KEY_FS, NORMAL_SPACING).x, KEY_FS };
	Rectangle rShapeSwitch = (Rectangle){ 0, 0, MeasureTextEx(KEY_FONT, KEY_TEXT_SHAPE_SWITCH, KEY_FS, NORMAL_SPACING).x, KEY_FS };
	Rectangle rRotate = (Rectangle){ 0, 0, MeasureTextEx(KEY_FONT, KEY_TEXT_ROTATE, KEY_FS, NORMAL_SPACING).x, KEY_FS };
	Rectangle rFlip = (Rectangle){ 0, 0, MeasureTextEx(KEY_FONT, KEY_TEXT_FLIP, KEY_FS, NORMAL_SPACING).x, KEY_FS };
	Rectangle rCamera = (Rectangle){ 0, 0, MeasureTextEx(KEY_FONT, KEY_TEXT_CAMERA, KEY_FS, NORMAL_SPACING).x, KEY_FS };
	Rectangle rPlace = (Rectangle){ 0, 0, MeasureTextEx(KEY_FONT, KEY_TEXT_PLACE, KEY_FS, NORMAL_SPACING).x, KEY_FS };

	Button Exit = (Button){ .label = "OK", .fontSize = 30, .labelColor = BLACK, .labelFont = gs->assets.fonts[FONT_METAMORPHOUS], .corner = 80, .stroke = 3, .color1 = DARKGRAY, .color2 = BLUE, .bounds = (Rectangle){0, 0, 300, 0} };

	Rectangle* recTextList[] = { &rMove, &rColorSwitch, &rShapeSwitch, &rRotate, &rFlip, &rCamera, &rPlace, &Exit.bounds };

	while (!WindowShouldClose() && !Exit.validated) {
		midX = GetScreenWidth() / 2;
		midY = GetScreenHeight() / 2;

		rMove = (Rectangle){ midX - MeasureTextEx(KEY_FONT, KEY_TEXT_MOVE, KEY_FS, NORMAL_SPACING).x / 2, posY + 20, MeasureTextEx(KEY_FONT, KEY_TEXT_MOVE, KEY_FS, NORMAL_SPACING).x, KEY_FS };
		SortRectangles(recTextList, 8, KEY_PAD, .5f);
		Exit.bounds = (Rectangle){ Exit.bounds.x, Exit.bounds.y + KEY_PAD + 30, 300, Exit.fontSize + 20 };
		Exit.hovered = CheckCollisionPointRec(GetMousePosition(), Exit.bounds);


		if (Exit.hovered) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
		else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

		BeginDrawing();
		ClearBackground(BGCOLOR);
		posY = DrawTitle(gs);

		// Affichage des Touches par lignes :
		DrawStrokeTextEx(KEY_FONT, KEY_TEXT_MOVE, rMove.x, rMove.y, KEY_FS, NORMAL_SPACING, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rMove.y + KEY_FS + KEY_PAD / 2 + 5, midX + KEY_LINE_WIDTH / 2, rMove.y + KEY_FS + KEY_PAD / 2 + 5, GOLD);

		DrawStrokeTextEx(KEY_FONT, KEY_TEXT_COLOR_SWITCH, rColorSwitch.x, rColorSwitch.y, KEY_FS, NORMAL_SPACING, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rColorSwitch.y + KEY_FS + KEY_PAD / 2 + 5, midX + KEY_LINE_WIDTH / 2, rColorSwitch.y + KEY_FS + KEY_PAD / 2 + 5, GOLD);

		DrawStrokeTextEx(KEY_FONT, KEY_TEXT_SHAPE_SWITCH, rShapeSwitch.x, rShapeSwitch.y, KEY_FS, NORMAL_SPACING, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rShapeSwitch.y + KEY_FS + KEY_PAD / 2 + 5, midX + KEY_LINE_WIDTH / 2, rShapeSwitch.y + KEY_FS + KEY_PAD / 2 + 5, GOLD);

		DrawStrokeTextEx(KEY_FONT, KEY_TEXT_ROTATE, rRotate.x, rRotate.y, KEY_FS, NORMAL_SPACING, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rRotate.y + KEY_FS + KEY_PAD / 2 + 5, midX + KEY_LINE_WIDTH / 2, rRotate.y + KEY_FS + KEY_PAD / 2 + 5, GOLD);

		DrawStrokeTextEx(KEY_FONT, KEY_TEXT_FLIP, rFlip.x, rFlip.y, KEY_FS, NORMAL_SPACING, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rFlip.y + KEY_FS + KEY_PAD / 2 + 5, midX + KEY_LINE_WIDTH / 2, rFlip.y + KEY_FS + KEY_PAD / 2 + 5, GOLD);

		DrawStrokeTextEx(KEY_FONT, KEY_TEXT_CAMERA, rCamera.x, rCamera.y, KEY_FS, NORMAL_SPACING, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rCamera.y + KEY_FS + KEY_PAD / 2 + 5, midX + KEY_LINE_WIDTH / 2, rCamera.y + KEY_FS + KEY_PAD / 2 + 5, GOLD);

		DrawStrokeTextEx(KEY_FONT, KEY_TEXT_PLACE, rPlace.x, rPlace.y, KEY_FS, NORMAL_SPACING, BLACK, GOLD, 1);
		DrawLine(midX - KEY_LINE_WIDTH / 2, rPlace.y + KEY_FS + KEY_PAD / 2 + 5, midX + KEY_LINE_WIDTH / 2, rPlace.y + KEY_FS + KEY_PAD / 2 + 5, GOLD);

		DrawButton(&Exit);


		EndDrawing();
		LoadAssetToVRAM(gs);

	}
	//printf("\n\n%d x %d\n\n", GetScreenHeight(), GetScreenWidth());


	return SCREEN_MENU;
}

int DrawTitle(GameState* gs) {
	int posY = 0;
	DrawStrokeTextEx(gs->assets.fonts[FONT_GRENZE_GOTISCH_B], C_TITLE, (GetScreenWidth() - MeasureTextEx(gs->assets.fonts[FONT_GRENZE_GOTISCH_B], C_TITLE, TITLE_FS, NORMAL_SPACING).x) / 2, posY, TITLE_FS, TITLE_SPACING, GOLD, BLACK, 2);
	return posY + TITLE_FS - 10;
}

//void DrawTitleEx(GameState* gs, Rectangle r) {
//	DrawStrokeTextEx(gs->assets.fonts[FONT_GRENZE_GOTISCH_B], C_TITLE, r.x + (r.width - MeasureText(C_TITLE, TITLE_FS)) / 2, r.y, TITLE_FS, GOLD, BLACK, 2);
//}

void DrawBackgroudMenu(Rectangle r) {
	//DrawRectangleRoundedStrokeEx(r, .2f, 10, 3, BLACK, RED);
	//DrawRectangleRoundedLinesEx(r, .2f, 10, 3, YELLOW);
	Rectangle tr = r;
	tr.y += 10;
	//DrawTitleEx(tr);
}

Vector2 MeasureTextWrapped(const char* text, Rectangle r, int fs) {
	return MeasureTextWrappedEx(GetFontDefault(), text, r, fs);
}

Vector2 MeasureTextWrappedEx(Font font, const char* text, Rectangle r, int fs) {	// Permet de mesurer la place que va prendre le texte avant de le dessiner. Si modif, penser à mofifier le Draw
	float curX = r.x;									// Curseur en X
	float curY = r.y;									// Curseur en Y
	float maxX = r.x + r.width;							// Taille max à ne pas dépasser
	float lineHeight = fs * 1.1f;						// Hauteur de ligne
	int spaceW = (int)(MeasureTextEx(font, " ", fs, NORMAL_SPACING).x * 1.5f);	// Taille d'espace
	char word[64] = "";
	int wordSize = 0;
	char c = 'a';										// Char actuel
	int drawSize = 0;									// Taille dessinée
	int len = strlen(text);

	float maxReachedX = r.x;  // ← suivi du X max atteint

	for (int i = 0; i < len; i++) {
		word[0] = '\0';
		c = text[i];
		wordSize = 0;
		while (i < len - 1 && c != ' ' && c != '\n') {
			word[wordSize] = c;
			word[++wordSize] = '\0';
			c = text[++i];
		}
		drawSize = MeasureTextEx(font, word, fs, NORMAL_SPACING).x;
		if (curX + drawSize > maxX) {
			curY += lineHeight;
			curX = r.x;
		}
		//if (stroke > 0) DrawStrokeTextEx(font, word, curX, curY, fs, textColor, strokeColor, stroke);
		//else DrawTextEx(font, word, (Vector2) { curX, curY }, fs, NORMAL_SPACING, textColor);
		curX += drawSize + spaceW;
		if (curX > maxReachedX) maxReachedX = curX;
		if (c == '\n') {
			curY += lineHeight;
			curX = r.x;
		}
	}

	return (Vector2) { maxReachedX - r.x, (curY - r.y) + lineHeight };

}


Vector2 DrawTextWrapped(const char* text, Rectangle r, int fs, Color textColor) {			// Permet d'écrire du texte dans un espace contraint (Il ne s'occupe que de la limite de droite, pas de celle du bas. Si modif, penser à mofifier le Measure
	return DrawTextWrappedEx(GetFontDefault(), text, r, fs, 0, textColor, BLANK);
}

Vector2 DrawTextWrappedEx(Font font, const char* text, Rectangle r, int fs, int stroke, Color textColor, Color strokeColor) {
	float curX = r.x;									// Curseur en X
	float curY = r.y;									// Curseur en Y
	float maxX = r.x + r.width;							// Taille max à ne pas dépasser
	float lineHeight = fs * 1.1f;						// Hauteur de ligne
	int spaceW = (int)(MeasureTextEx(font, " ", fs, NORMAL_SPACING).x * 1.5f);	// Taille d'espace
	char word[64] = "";
	int wordSize = 0;
	char c = 'a';										// Char actuel
	int drawSize = 0;									// Taille dessinée
	int len = strlen(text);

	float maxReachedX = r.x;  // ← suivi du X max atteint

	for (int i = 0; i < len; i++) {
		word[0] = '\0';
		c = text[i];
		wordSize = 0;
		while (i < len - 1 && c != ' ' && c != '\n') {
			word[wordSize] = c;
			word[++wordSize] = '\0';
			c = text[++i];
		}
		drawSize = MeasureTextEx(font, word, fs, NORMAL_SPACING).x;
		if (curX + drawSize > maxX) {
			curY += lineHeight;
			curX = r.x;
		}
		if (stroke > 0) DrawStrokeTextEx(font, word, curX, curY, fs, NORMAL_SPACING, textColor, strokeColor, stroke);
		else DrawTextEx(font, word, (Vector2) { curX, curY }, fs, NORMAL_SPACING, textColor);
		curX += drawSize + spaceW;
		if (curX > maxReachedX) maxReachedX = curX;
		if (c == '\n') {
			curY += lineHeight;
			curX = r.x;
		}
	}

	return (Vector2) { maxReachedX - r.x, (curY - r.y) + lineHeight };

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

//void DrawButton(Button* btn) {
//	DrawButtonEx(btn, GetFontDefault());
//}

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
		float offset = r.height * depthFactor / 100.f;

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
	DrawTextEx(btn->labelFont, btn->label, (Vector2) { r.x + (r.width - MeasureTextEx(btn->labelFont, btn->label, fontSize, NORMAL_SPACING).x) / 2, r.y + (r.height - fontSize) / 2 }, fontSize, NORMAL_SPACING, btn->labelColor);

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


	DrawTextEx(box->textFont, box->text, (Vector2) { box->bounds.x + box->dx, box->bounds.y + (box->bounds.height - box->fontSize) / 2 }, box->fontSize, NORMAL_SPACING, BLACK);

	// Curseur clignotant centré en y
	if (box->active && ((int)(GetTime() * 2) % 2 == 0)) {
		int tw = (int)MeasureTextEx(box->textFont, box->text, box->fontSize, NORMAL_SPACING).x;
		DrawText("|", box->bounds.x + box->dx + tw + 2, box->bounds.y + (box->bounds.height - box->fontSize) / 2, box->fontSize, DARKGRAY);
	}

	if (box->validated)	printf("entré appuyé 2\n");

}

void DrawStrokeTextEx(Font font, const char* text, int x, int y, int fontSize, int spacing, Color textColor, Color strokeColor, int thickness) {
	for (int dx = -thickness; dx <= thickness; dx++)	for (int dy = -thickness; dy <= thickness; dy++)
		if (dx != 0 || dy != 0)		DrawTextEx(font, text, (Vector2) { x + dx, y + dy }, fontSize, spacing, strokeColor);

	DrawTextEx(font, text, (Vector2) { x, y }, fontSize, spacing, textColor);
}

void DrawStrokeText(Font font, const char* text, int x, int y, int fontSize, Color textColor, Color strokeColor) {
	DrawStrokeTextEx(font, text, x, y, fontSize, NORMAL_SPACING, textColor, strokeColor, fontSize / 30);
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


void ImageRoundedCorner(Image* image, float roundness) { ImageRoundCorner(image, (float)(roundness * min(image->width, image->height))); }

void ImageRoundCorner(Image* image, float radius) {
	ImageFormat(image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	int width = image->width;
	int height = image->height;
	int rad2 = radius * radius;

	float cx, cy;

	for (int i = 0; i < width; i++)		for (int j = 0; j < height; j++) {
		cx = cy = -1.f;

		if (i < radius && j < radius) {
			cx = cy = radius;
		}
		else if (i < radius && j > height - radius) {
			cx = radius;
			cy = height - radius;
		}
		else if (i > width - radius && j < radius) {
			cx = width - radius;
			cy = radius;
		}
		else if (i > width - radius && j > height - radius) {
			cx = width - radius;
			cy = height - radius;
		}

		if (cx != -1.f && cy != -1.f) {
			float dx = i - cx;
			float dy = j - cy;

			if (dx * dx + dy * dy > rad2) {
				Color pixelColor = GetImageColor(*image, i, j);
				pixelColor.a = 0; // On rend le pixel totalement transparent
				ImageDrawPixel(image, i, j, pixelColor);
			}
		}
	}
}


void* LoadAssetsWorker(void* arg) {
	LoadContext* ctx = (LoadContext*)arg;
	double start = GetTime();
	for (int i = 0; i < NUM_CARDS; i++) {
		const char* path = TextFormat("Assets/Images/Game Card/%s.png", expCards[i]->name);
		Image img = LoadImage(path);
		ImageRoundedCorner(&img, 0.11f);		// 0.11 : ratio entre la largeur et l'arrondi pour les cartes de poker, format des cartes de cartographers

		pthread_mutex_lock(&ctx->mutex);
		ctx->cardsRAM[i] = img;
		ctx->cardsLoadedRAM++;
		pthread_mutex_unlock(&ctx->mutex);
	}

	for (int i = 0; i < NUM_SEASONS; i++) {
		const char* path = TextFormat("Assets/Images/Season/%s.png", seasons[i]->path);
		Image img = LoadImage(path);
		ImageRoundedCorner(&img, 0.11f);

		pthread_mutex_lock(&ctx->mutex);
		ctx->seasonsRAM[i] = img;
		ctx->seasonsLoadedRAM++;
		pthread_mutex_unlock(&ctx->mutex);
	}

	for (int i = 0; i < NUM_EDITS; i++) {
		const char* path = TextFormat("Assets/Images/Letter Scroll/%c.png", 'A' + i);
		Image img = LoadImage(path);
		Image img2 = ImageCopy(img);
		ImageFormat(&img2, PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA);

		pthread_mutex_lock(&ctx->mutex);
		ctx->editsRAM[i] = img;
		ctx->editsRAM[i + NUM_EDITS] = img2;
		ctx->editsLoadedRAM++;
		pthread_mutex_unlock(&ctx->mutex);
	}
	printf("\n\n Temps pris au total : %fs", (float)(GetTime() - start));
	return NULL;
}

void LoadAssetToVRAM(GameState* gs) {
	//printf("\nLoadAssetToVRAM appelé, %d %d cards", gs->loadCtx->cardsLoadedRAM, gs->loadCtx->cardsLoadedVRAM);
	pthread_mutex_lock(&gs->loadCtx->mutex);
	if (gs->loadCtx->cardsLoadedRAM > gs->loadCtx->cardsLoadedVRAM) {
		int i = gs->loadCtx->cardsLoadedVRAM;
		gs->assets.cardImages[i] = LoadTextureFromImage(gs->loadCtx->cardsRAM[i]);
		gs->loadCtx->cardsLoadedVRAM++;
		//printf("\nChargement de la carte %d en VRAM, %d, %d\n", i);
		gs->loadCtx->avancement++;

		UnloadImage(gs->loadCtx->cardsRAM[i]);		// Libération de l'espace mémoire
	}
	else if(gs->loadCtx->seasonsLoadedRAM > gs->loadCtx->seasonsLoadedVRAM) {
		int i = gs->loadCtx->seasonsLoadedVRAM;
		gs->assets.seasonImages[i] = LoadTextureFromImage(gs->loadCtx->seasonsRAM[i]);
		gs->loadCtx->seasonsLoadedVRAM++;
		//printf("\nChargement de la saison %d en VRAM\n", i);
		UnloadImage(gs->loadCtx->seasonsRAM[i]);
		gs->loadCtx->avancement++;
	}
	else if(gs->loadCtx->editsLoadedRAM > gs->loadCtx->editsLoadedVRAM) {
		int i = gs->loadCtx->editsLoadedVRAM;
		gs->assets.letterScrollsImage[i] = LoadTextureFromImage(gs->loadCtx->editsRAM[i]);
		gs->assets.letterScrollsImage[i + NUM_EDITS] = LoadTextureFromImage(gs->loadCtx->editsRAM[i + NUM_EDITS]);
		gs->loadCtx->editsLoadedVRAM++;
		//printf("\nChargement de l'edit %d en VRAM\n", i);
		UnloadImage(gs->loadCtx->editsRAM[i]);
		UnloadImage(gs->loadCtx->editsRAM[i + NUM_EDITS]);
		gs->loadCtx->avancement++;
	}

	pthread_mutex_unlock(&gs->loadCtx->mutex);

}

void DebugAssetViewer(GameState* gs) {
	int currentTab = 0; // 0 = Cartes, 1 = Saisons, 2 = Edits
	int scrollY = 0;

	// Pour ne pas que les images s'affichent en taille réelle et sortent de l'écran, on va les redimensionner visuellement à l'affichage (scale).
	float scale = 0.2f;

	while (!WindowShouldClose()) {
		// --- CONTRÔLES ---
		if (IsKeyPressed(KEY_RIGHT)) currentTab = (currentTab + 1) % 3;
		if (IsKeyPressed(KEY_LEFT)) currentTab = (currentTab + 2) % 3;

		// Molette de la souris pour scroller si tu as beaucoup de cartes
		scrollY += GetMouseWheelMove() * 40;
		if (scrollY > 0) scrollY = 0; // Bloque le scroll vers le haut

		// Quitter le test pour lancer le vrai jeu
		if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) break;

		// --- DESSIN ---
		BeginDrawing();
		ClearBackground(GRAY);

		// Header d'instructions
		DrawRectangle(0, 0, GetScreenWidth(), 60, LIGHTGRAY);
		DrawText("TEST DES IMAGES (Flèches G/D pour changer d'onglet, Molette pour scroller)", 10, 10, 20, DARKGRAY);
		DrawText("Appuyez sur ENTREE pour lancer le jeu normal", 10, 35, 20, MAROON);

		int startX = 20;
		int startY = 80 + scrollY;

		// Affichage dynamique selon l'onglet
		if (currentTab == 0) {
			DrawText("ONGLET 1 : Cartes d'Exploration (Arrondies)", startX, startY, 20, BLACK);
			startY += 40;

			// Affichage en grille (ex: 5 cartes par ligne)
			for (int i = 0; i < 21; i++) { // NUM_CARDS
				Texture2D tex = gs->assets.cardImages[i];
				if (tex.id != 0) {							// Si la texture est bien chargée
					int col = i % 5;
					int row = i / 5;

					int drawX = startX + col * (tex.width * scale + 20);
					int drawY = startY + row * (tex.height * scale + 40);

					DrawTextureEx(tex, (Vector2) { drawX, drawY }, 0.0f, scale, WHITE);
					DrawText(TextFormat("ID: %d", i), drawX, drawY - 20, 15, GRAY);
				}
			}
		}
		else if (currentTab == 1) {
			DrawText("ONGLET 2 : Saisons (Arrondies)", startX, startY, 20, BLACK);
			startY += 40;
			for (int i = 0; i < 4; i++) {					// NUM_SEASONS
				Texture2D tex = gs->assets.seasonImages[i];
				if (tex.id != 0) {
					DrawTextureEx(tex, (Vector2) { startX + i * (tex.width * scale + 20), startY }, 0.0f, scale, WHITE);
				}
			}
		}
		else if (currentTab == 2) {
			DrawText("ONGLET 3 : Edits (Bruts, pas d'arrondi ici)", startX, startY, 20, BLACK);
			startY += 40;
			for (int i = 0; i < NUM_EDITS * 2; i++) {		// NUM_EDITS
				Texture2D tex = gs->assets.letterScrollsImage[i];
				if (tex.id != 0) {
					DrawTextureEx(tex, (Vector2) { startX + i * (tex.width + 20), startY }, 0.0f, 1, WHITE);
					//DrawTexture(tex, startX + i * (tex.width + 20), startY, WHITE);
				}
				else {
					printf("Edit %d non chargé, ", i);
				}
			}
			printf("\n");
		}

		EndDrawing();
	}
}

void drawFinalUi(GameState *gs) {
	int y2 = 20;
	//printf("\nPosition : %d,%d", state->pos.x, state->pos.y);
	int midX = GetScreenWidth() / 2;
	int midY = GetScreenHeight() / 2;
	Vector2 playerPanelSize = MeasureTextEx(PLAYER_PANEL_FONT, gs->players[gs->playerIndex].name, PLAYER_PANEL_FS + 10, NORMAL_SPACING);
	Rectangle playerPanel = (Rectangle){ GetScreenWidth() - 50 - ( max(PLAYER_REC_WIDTH, playerPanelSize.x - 20)), midY - PLAYER_REC_HEIGHT / 2, playerPanelSize.x + PLAYER_REC_HEIGHT, PLAYER_REC_HEIGHT };
	Rectangle editsRec = (Rectangle){ midX - 40, -60, 80, 180 };
	
	Rectangle editRects;
	
	editRects.width = MeasureTextEx(EDITS_FONT, "FIN DU JEUX", EDITS_FS*2, NORMAL_SPACING).x;
	editRects.height = editsRec.height / 2;
	editsRec.width += editRects.width;
	editsRec.x -= editRects.width / 2;
	editRects.x = editsRec.x + (editsRec.width - editRects.width) / 2;
	editRects.y = 0;

	DrawRectangleRoundedStrokeEx(playerPanel, .1f, 10, 3, LIGHTGRAY, DARKBROWN);
	DrawStrokeTextEx(PLAYER_PANEL_FONT, gs->players[gs->playerIndex].name, playerPanel.x + 8, playerPanel.y + 5, PLAYER_PANEL_FS + 10, NORMAL_SPACING, WHITE, BLACK, 1);
	DrawStrokeTextEx(PLAYER_PANEL_FONT, TextFormat("SCORE : %d", gs->players[gs->playerIndex].score), playerPanel.x + 8, playerPanel.y + PLAYER_PANEL_FS + y2, PLAYER_PANEL_FS, NORMAL_SPACING, WHITE, BLACK, 1);
	DrawStrokeTextEx(PLAYER_PANEL_FONT, TextFormat(gs->playerIndex+1 ==1 ? "CLASSEMENT :  %d er" : "CLASSEMENT :  %d eme", gs->playerIndex+1), playerPanel.x + 8, playerPanel.y + PLAYER_PANEL_FS*2 + y2, PLAYER_PANEL_FS, NORMAL_SPACING, WHITE, BLACK, 1);
	DrawRectangleRoundedStrokeEx(editsRec, 1.f, 10, 2, BGCOLOR, GOLD);
	DrawStrokeTextEx(gs->assets.fonts[FONT_GRENZE_GOTISCH_L], "FIN DU JEUX", editRects.x, editRects.y , EDITS_FS*2, NORMAL_SPACING, GOLD, BLACK, 1);
}