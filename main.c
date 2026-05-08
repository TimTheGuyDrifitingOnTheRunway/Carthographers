//#include <windows.h>

#include "LibCarthographie.h"
#include "Lib3d.h"
#include "GameManager.h"
#include "UI.h"


int main()
{
	srand(time(NULL));

	// Création d'un thread pour charger les images (et les modèles 3D après)


	// Initialization
	int screenWidth = 1280;
	int screenHeight = 720;
	int flag = 0;
	printf("\n\n\n\n\n\n Debug 1 \n\n\n\n\n\n");


	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
	InitWindow(screenWidth, screenHeight, "Cartographer");
	SetWindowMinSize(1280, 720);

	SetTargetFPS(60);

	pthread_t soundThread, modelThread, SeedThread;
	int keep = 1;
	pthread_create(&soundThread, NULL, SoundThread, &keep);
	int mountainSeed[2] = { randInt(0, 100), randInt(0, 100) };
	pthread_create(&SeedThread, NULL, generateSeedThread, mountainSeed);

	//ToggleBorderlessWindowed();
	


	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	printf("\n\n\n\n\n\n Debug 3 \n\n\n\n\n\n");
	// WaitTime(1.0f);

	


	// Main game loop

	FeuilleCarte f;
	/*const ScoringCard* edits[4];
	const ExploreCard* exploreDeck[17];
	int deckSize = 14;*/
	printf("\n\n\n\n\n\n Debug 4 \n\n\n\n\n\n");
	GameState gs = { 0 };

	{
		const char* charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
			".,;:!?'°^&()[]{}<>_\"|+-/*= \\\n"
			"éàèêëîïôûùçÉÀÈÊËÎÏÔÛÙÇ";

		int codepointCount = 0;  
		int* codepoints = LoadCodepoints(charset, &codepointCount);

		gs.assets.fonts[FONT_GRENZE_GOTISCH_B] = LoadFontEx("Assets/Fonts/GrenzeGotisch-Bold.ttf", 200, codepoints, codepointCount);
		gs.assets.fonts[FONT_GRENZE_GOTISCH_L] = LoadFontEx("Assets/Fonts/GrenzeGotisch-Light.ttf", 200, codepoints, codepointCount);
		gs.assets.fonts[FONT_PIRATA_ONE] = LoadFontEx("Assets/Fonts/PirataOne.ttf", 200, codepoints, codepointCount);
		gs.assets.fonts[FONT_FREDOKA_CM] = LoadFontEx("Assets/Fonts/Fredoka_Condensed-Medium.ttf", 200, codepoints, codepointCount);
		gs.assets.fonts[FONT_FREDOKA_SB] = LoadFontEx("Assets/Fonts/Fredoka-SemiBold.ttf", 200, codepoints, codepointCount);
		gs.assets.fonts[FONT_METAMORPHOUS] = LoadFontEx("Assets/Fonts/Metamorphous.ttf", 200, codepoints, codepointCount);

		UnloadCodepoints(codepoints);
	}
	printf("%f", MeasureTextEx(gs.assets.fonts[FONT_GRENZE_GOTISCH_B], "Printemps  0/8", EDITS_FS, NORMAL_SPACING).x);

	//for (int i = 0; i < FONT_COUNT; i++) {
	//	printf("(%.5f,%.5f) ; ", MeasureTextEx(gs.assets.fonts[i], " ", 30, 2).x, MeasureTextEx(gs.assets.fonts[i], " ", 30, 2).y);
	//	printf("(%.5f,%.5f)\n", MeasureTextEx(gs.assets.fonts[i], "    \n   \n     ", 30, 2).x, MeasureTextEx(gs.assets.fonts[i], "    \n   \n     ", 30, 2).y);
	//}
	//printf("\n");
	//BeginDrawing();
	//ClearBackground(WHITE);
	//DrawTextWrappedEx(gs.assets.fonts[FONT_PIRATA_ONE], "Juste pour tester", (Rectangle) { 200, 100, 600, 800 }, 40, 1, GRAY, BLACK);
	//EndDrawing();
	//system("pause");
	//BeginDrawing();
	//ClearBackground(WHITE);
	//DrawTextWrappedEx(gs.assets.fonts[FONT_PIRATA_ONE], "Juste pour tester", (Rectangle) { 200, 100, 600, 800 }, 40, 1, GRAY, BLACK);
	//EndDrawing();
	//system("pause");

	LoadContext imgCtx = { 0 };
	pthread_mutex_init(&imgCtx.mutex, NULL);

	gs.loadCtx = &imgCtx;

	pthread_t assetWorkerThread;
	pthread_create(&assetWorkerThread, NULL, LoadAssetsWorker, &imgCtx);
	pthread_create(&modelThread, NULL, ModelLoaderThread, &imgCtx);

	//gs.playerNumber = 98;
	ScreenID current = SCREEN_MENU;

	while (current != SCREEN_GAME && current != SCREEN_EXIT) {
		switch (current) {
		case SCREEN_MENU:			current = RunMenu(&gs);			break;
		case SCREEN_ADD_PLAYER:		current = RunAddPlayer(&gs);	break;
		case SCREEN_RULES:			current = RunRules(&gs);		break;
		case SCREEN_KEYBINDS:		current = RunKeybinds(&gs);		break;
		default:					current = SCREEN_EXIT;			break;
		}
	}

	if (current == SCREEN_EXIT) {

		EndProgram(&gs);
		printf("\n\n\n\n Debug Fin de partie !! \n\n\n\n");
		return 1;
	}

	if (current == SCREEN_GAME) {
		pthread_join(assetWorkerThread, NULL);
		int total = gs.loadCtx->cardsLoadedVRAM + gs.loadCtx->seasonsLoadedVRAM + gs.loadCtx->editsLoadedVRAM;
		while (total < NUM_CARDS + NUM_EDITS + NUM_SEASONS) {
			LoadAssetToVRAM(&gs);
			total = gs.loadCtx->cardsLoadedVRAM + gs.loadCtx->seasonsLoadedVRAM + gs.loadCtx->editsLoadedVRAM;
			printf("En attente de tout charger, %d = %d + %d + %d\n", total, gs.loadCtx->cardsLoadedVRAM, gs.loadCtx->seasonsLoadedVRAM, gs.loadCtx->editsLoadedVRAM);
			//Sleep(100);

			// AT THE END : ajouter un ecran de chargement
		}
		
	}

	SetMouseCursor(MOUSE_CURSOR_DEFAULT);

	//int PNbre = 2;
	//printf("Nombre de joueurs : ");     scanf("%d", &PNbre);    PNbre = (PNbre > 0) ? PNbre > MAX_PLAYER ? MAX_PLAYER : PNbre : 1;
	//printf("\nNombre de Joueurs : %d\n", PNbre);
	//gs.playerNumber = PNbre;

	SetupGame(&gs);

	/*gs.edits[0] = &StoneSideQuest;
	gs.edits[1] = &ShoreSideExpanse;
	gs.edits[2] = &GreengoldPlains;
	gs.edits[3] = &BrokenRoad;*/

	DebugGameStats(&gs);
	//DebugAssetViewer(&gs);


	// Ouverture de la fenêtre
	// InitWindow(screenWidth, screenHeight, "Cartographer");

	screenHeight = GetScreenHeight();
	screenWidth = GetScreenWidth();
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second



	printf("\n\n\n\n Debug Window : %dx%d \n\n\n\n", screenWidth, screenHeight);


	//Define the camera to look into our 3d world 
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
	camera.target = (Vector3){ 0.0f, 0.0f, 1.0f };      // Camera looking at point
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

	//printf("\n\n\n\n Debug BSG \n\n\n\n");
	ModelImage *models_ptr;
	pthread_join(modelThread, (void**)&models_ptr);//attente de la fin du thread de chargement des textures de 3d
	ModelImage models = *models_ptr;
	free(models_ptr);

	Seed* s_ptr;
	pthread_join(SeedThread, (void**)&s_ptr);
	Seed s = *s_ptr;
	free(s_ptr);// récupération de la seed et libération de la mémoire

	StartGame(&gs, camera, models, s, mountainSeed);

	//printf("\n\n\n\n Debug ASG \n\n\n\n");


	sort_players_by_score(&gs);
	printf("LeaderBoard Final : \n\n");
	for (int i = 0; i < gs.playerNumber; i++) {
		if (gs.players != NULL) printf("%d : %s, score : %d\n", i + 1, gs.players[i].name, gs.players[i].score);
	}

	// afficher par ordre alphabétique




	/* Multi tests de fonctionnement
	for (int i = 0; i < 5; i++) {
		SetupGame(f, edits, exploreDeck);
		DebugGameStats(f, edits, exploreDeck, deckSize);
		printf("\n\n\n\n\n");
	}*/














	// Tests

	const Piece* pieceTab[] = { &POINT, &NOTHING, &U, &L, &L_L, &B_L, &LINE, &L_LINE, &CUBE, &T, &L_T, &RECT, &RECT_WITH_HOLE, &DIAG, &L_DIAG, &CROSS, &STAIRS, &Z, &B_Z, &STRANGE };
	Piece radompiece;
	copyPiece(pieceTab[3], radompiece);
	//for (int i = 0; i < 2; i++) {GUIplacementShape(f, pieceTab[3], FORET, camera); }
	//GUIplacementDefault(f, 5, camera);
	Sleep(1000);

	keep = 0;

	pthread_join(soundThread, NULL);//arret du thread son

	printf("\n\n\n\n\n\n Debug de fin de partie \n\n\n\n\n\n");

	printf("\n\n stoneSideQuest points = %d\n", calcStoneSideQuest(f));
	Sleep(1000);





	// De-Initialization
	//--------------------------------------------------------------------------------------
	EndProgram(&gs);       // Close window and OpenGL context
	//--------------------------------------------------------------------------------------







	FeuilleCarte test = {
		{4, 0, 0, 104, 0, 0, 0, 5, 4, 6, 1},
		{0, 3, 0, 2, 0, 0, 0, 100, 3, 5, 2},
		{4, 3, 8, 2, 0, 0, 0, 0, 0, 5, 3},
		{0, 3, 0, 2, 0, 0, 6, 0, 0, 103, 3},
		{0, 3, 3, 2, 2, 8, 0, 0, 0, 5},
		{2 ,2 ,2 ,2 ,6 ,6 ,6 ,3 ,2 ,4},
		{0 ,3 ,3 ,3 ,3 ,3 ,3 ,3 ,3 ,5},
		{4 ,4 ,4 ,4 ,4 ,104 ,4 ,4 ,4 ,5},
		{5 ,5 ,5 ,5 ,5 ,5 ,5 ,2 ,5 ,5},
		{5 ,5 ,5 ,3 ,5 ,5 ,2 ,2 ,5 ,5},
		{2, 4, 5, 3, 3, 3, 4, 4, 2, 2, 9}

	}; //Borderlands devrait faire 24 points, Broken Road 15 points
	displayCarte(test);
	printf("\n\n\n");
	/*initCarte2(test, FALSE, FALSE);
	for (int i = 0; i < SIZE; i++) test[0][i] = FORET;
	for (int i = 0; i < SIZE; i++) test[5][i] = RUINE;
	for (int i = 0; i < SIZE; i++) test[7][i] = CHAMPS;
	for (int i = 0; i < SIZE; i++) test[i][3] = FORET;
	for (int i = 1; i < SIZE; i++) test[i][1] = VILLAGE;
	for (int i = 0; i < SIZE; i++) test[i][9] = EAU;
	test[0][0] = EAU;
	test[6][9] = EAU;*


	printf("\nBorderlands points = %d\n", calcBorderlands(test));

	test[1][10] = FORET;

	displayCarte(test);
	printf("\nBroken Road points = %d", calcBrokenRoad(test));

	*/
	InfoGroupe listeGroupes[SIZE * SIZE];
	int taille = RecenseEveryGroups(test, listeGroupes);
	for (int i = 0; i < taille; i++) {
		printf("Groupe %d : material %d, taille %d, voisins : ", i, listeGroupes[i].material, listeGroupes[i].taille);
		for (int j = 0; j < 10; j++) {
			if (listeGroupes[i].materialVoisin[j] != 0) {
				printf("(%d : %d) ", j, listeGroupes[i].materialVoisin[j]);
			}
		}
		printf("\n");
	}

	calcEnenmyPoints(test);



	return 0;

}



void EndProgram(GameState* gs) {
	for (int i = 0; i < FONT_COUNT; i++) {
		UnloadFont(gs->assets.fonts[i]);
	}
	CloseWindow();
}

