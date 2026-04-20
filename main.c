//#include <windows.h>

#include "LibCarthographie.h"
#include "Lib3d.h"
#include "GameManager.h"
#include "UI.h"

void sort_players_by_score(GameState* gs);

int main()
{
	srand(time(NULL));



	// Initialization
	int screenWidth = 1280;
	int screenHeight = 720;
	int flag = 0;
	printf("\n\n\n\n\n\n Debug 1 \n\n\n\n\n\n");


	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
	InitWindow(screenWidth, screenHeight, "Cartographer");
	SetWindowMinSize(1280, 720);

	SetTargetFPS(60);
	//ToggleBorderlessWindowed();

	//while (!WindowShouldClose() && !flag) {
	//	BeginDrawing();

	//	

	//	EndDrawing();
	//}


	//DisableCursor();


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
	//gs.playerNumber = 98;
	ScreenID current = SCREEN_MENU;

	while (current != SCREEN_GAME && current != SCREEN_EXIT) {
		switch (current) {
		case SCREEN_MENU:       current = RunMenu(&gs);       break;
		case SCREEN_ADD_PLAYER: current = RunAddPlayer(&gs);  break;
		case SCREEN_RULES:      current = RunRules(&gs);      break;
		case SCREEN_KEYBINDS:   current = RunKeybinds(&gs);   break;
		default:                current = SCREEN_EXIT;        break;
		}
	}

	if (current == SCREEN_EXIT) {
		CloseWindow();
		printf("\n\n\n\n Debug Fin de partie !! \n\n\n\n");
		return 1;
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

	StartGame(&gs, camera);

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
	printf("\n\n\n\n\n\n Debug de fin de partie \n\n\n\n\n\n");

	printf("\n\n stoneSideQuest points = %d\n", calcStoneSideQuest(f));
	Sleep(1000);





	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
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

void sort_players_by_score(GameState* gs)
{
	PlayerState* arr = gs->players;
	int n = gs->playerNumber;

	for (int i = 1; i < n; i++)
	{
		PlayerState key = arr[i];
		int j = i;

		while (j > 0 && arr[j - 1].score < key.score)  /* décroissant */
		{
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = key;
	}
}


