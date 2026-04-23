#pragma once

//Ce document est là pour réunir toutes les constantes au même endroit

#define _CRT_SECURE_NO_WARNINGS







/********************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <pthread.h>


/*OPTIONS DE DEBOGAGE */
#define cc// DEBUG_FORET : affiche la noisemap source de foret


/*****MACROS****/

#define clamp(x, min, max) ((x < min) ? min : (x > max) ? max : x)

/******************************** LibCartographie.h ********************************/

#define SIZE 11
#define PIECESIZE 3
#define NOMBREMONTAGNE 5
#define MONTAGNEDIST 3.0f
#define NOMBRERUINE 6
#define RUINEDIST 2.7f
#define TRUE 1
#define FALSE 0
#define CONFLICTVALUE 9
#define OUTOFBOUND -1

// Ne rien metre sur 1

#define FORET 2
#define VILLAGE 3
#define CHAMPS 4
#define EAU 5
#define MONSTRE 6
#define RUINE 100 // Have to be big to not interfere with the material of the pieces
#define MONTAGNE 8

#define ACTUALFOREST 10
#define NEXTFOREST 11

#define ACTUAL 12
#define NEXT 13


/******************************** main.c ********************************/

#define MAX_PLAYER 100


/******************************** Lib3d.h ********************************/

#define BACKGROUND_COLOR SKYBLUE
#define BGCOLOR (Color){40, 40, 40, 255}

#define GRIDCOLOR GRAY
#define BORDERCOLOR MAROON
#define PLACEMENT_HEIGHT 1.0f
#define CASE_HEIGHT 1.0f

#define SPEEDY 5 // constantes inverse de vitesse
#define SPEEDX 7

// keybinds des pièces
#define UPP KEY_W
#define DOWNP KEY_S
#define RIGHTP KEY_D
#define LEFTP KEY_A

#define ROTATEP KEY_R
#define FLIPP KEY_F
#define SWITCHP KEY_E
#define SWITCHMP KEY_Q

// keybinds camera

#define UPC KEY_UP
#define DOWNC KEY_DOWN
#define RIGHTC KEY_RIGHT
#define LEFTC KEY_LEFT

#define MAXCAMERAHEIGHT 0.8


// Constantes du Menu
#define SEASON_FS 40
#define SEASON_FONT gs->assets.fonts[FONT_GRENZE_GOTISCH_B]

#define EDITS_FS 50
#define EDITS_FONT gs->assets.fonts[FONT_GRENZE_GOTISCH_B]
#define TOOLTIP_FONT gs->assets.fonts[FONT_FREDOKA_CM]

#define PLAYER_PANEL_FONT gs->assets.fonts[FONT_METAMORPHOUS]

#define PLAYER_REC_WIDTH 250
#define PLAYER_REC_HEIGHT 500
#define PLAYER_PANEL_FS 30



/******************************** GameManager.h ********************************/
// Rien

/******************************** UI.h ********************************/

// Global

#define NORMAL_SPACING 1


// Titre
#define C_TITLE "CARTOGRAPHERS"
#define TITLE_FS 170
#define TITLE_SPACING 3


// Main Menu

#define MAIN_MENU_BTN_WIDTH 800
#define MAIN_MENU_Y 700

#define MAIN_BTN_FONT gs->assets.fonts[FONT_METAMORPHOUS]
#define MAIN_BUTTON_FS 50
#define MAIN_BUTTON_STROKE 2
#define BUTTON_DELTA 30		// Espacement entre les boutons
#define MAIN_BUTTON_CORNER 35


// Add Menu

#define MAX_NAME_LENGTH 20

#define ADD_MENU_WIDTH		MAIN_MENU_BTN_WIDTH
#define ADD_MENU_HEIGHT		300

#define ADD_BUTTON_FS		MAIN_BUTTON_FS
#define ADD_BUTTON_STROKE	MAIN_BUTTON_STROKE


// Rules Menu

#define PAGE_NB 6

#define TEXT_ZONE_WIDTH_MIN 900
#define TEXT_ZONE_PADDING 10


#define OBJECTIF_TEXT "Vous avez été choisi par la Reine pour cartographier les terres sauvages du Nord. Votre mission s'étendra comme la harpe, sur 4 Saisons :-).\nVotre but est simple : accumuler le maximum d'Étoiles de Réputation avant la fin de l'Hiver. Ces étoiles s'obtiennent en répondant aux exigences de la Reine, représentées par 4 Décrets actifs pendant la partie."
#define TURN_TEXT "À chaque tour, une nouvelle carte 'Exploration' est dévoilée.\nElle vous impose deux choix : un polyomino (forme géométrique) précis, et un type de terrain.\nSelon la carte (sauf carte spécifique) vous aurez la possibilité de switcher entre 2 polyominos OU 2 terrains.\nVous devez placer cette forme sur votre parchemin (la grille).\n  - Manipulation : Vous êtes libre de faire pivoter la forme ou de l'inverser (effet miroir).\n  - Contraintes : La forme doit tenir entièrement dans la grille. Elle ne peut jamais chevaucher une case déjà remplie.\n  - Si une carte Ruine a été révélée précédement, il est impératif de placer ce polyomino sur une case Ruine"
#define PLACEMENT_TEXT "Vous dessinerez principalement 4 types de terrains : Forêt (Vert), Champ (Jaune), Eau (Bleu) et Village (Rouge). L'emplacement de ces terrains déterminera votre score selon les Décrets de la partie (les détails de chaque Décret sont consultables en jeu).\n  - Les Montagnes : Elles sont déjà présentes sur la carte. Si vous réussissez à remplir les 4 cases qui entourent une Montagne, vous gagnez immédiatement une Pièce d'Or. Chaque Pièce d'Or acquise vous rapportera 1 point supplémentaire à chaque fin de Saison."
#define RYTHME_TEXT "Chaque carte Exploration qui apparaît possède une 'valeur de temps'.\nUn compteur additionne ce temps au fur et à mesure des tours. Dès que le total atteint la limite de la Saison en cours, la Saison s'arrête immédiatement.\nLe jeu calcule alors automatiquement votre score pour cette Saison, et la Saison suivante commence avec un compteur de temps remis à zéro."
#define ENEMY_TEXT "Prenez garde : cartographier est dangereux !\nLorsqu'une carte Monstre est piochée, les joueurs ne dessinent plus sur leur parchemin, mais sur le parchemin du voisin (gauche ou droite, en fonction de la carte). Vous êtes libre de placer l'ennemi ou vous voulez, pour embêter votre adversaire le plus possible.\n  - Utilité : les monstres font perdre des points en fin de saison\n  - Les cartes monstre ne sont pas impactées par les cartes ruines\n  - Règle spéciale solo : les monstres sont placés automatiquement par le jeu"
#define SCORE_TEXT "Il y a 4 Décrets de la Reine actifs durant toute la partie (nommés A, B, C et D). Cependant, ils ne rapportent pas des points tout le temps. À la fin de chaque Saison, seuls deux Décrets spécifiques sont évalués : \n Printemps : Décrets A et B;  Été : Décrets B et C;  Automne : Décrets C et D;  Hiver : Décrets D et A\nChaque Décret est donc noté exactement deux fois dans la partie.\nUne fois la Saison terminée, le jeu calcule votre score ainsi :\n + Points des deux Décrets de la saison \n + Votre total actuel de Pièces d'Or \n - Les malus des Monstres (1 point perdu par case vide touchant un monstre)\nLa somme de vos 4 scores de Saison donne votre Réputation Finale."

#define PAGE_FONT gs->assets.fonts[FONT_GRENZE_GOTISCH_L]
#define PAGE_FS (int)min(textZone.width / 25, textZone.height / 11)

// KeyBinds Menu

#define KEY_FS 50
#define KEY_PAD 0
#define KEY_LINE_WIDTH 150

#define KEY_FONT gs->assets.fonts[FONT_GRENZE_GOTISCH_L]

#define KEY_TEXT_MOVE "Z / Q / S / D : Permet de bouger la pièce"
#define KEY_TEXT_COLOR_SWITCH "A : Permet de changer la couleur "
#define KEY_TEXT_SHAPE_SWITCH "E : Permet de changer la forme"
#define KEY_TEXT_ROTATE "R : Permet de tourner la pièce à 90° sur la droite"
#define KEY_TEXT_FLIP "F : Permet de faire un symétrie axiale à la pièce, comme un effet miroir"
#define KEY_TEXT_PLACE "Espace : Permet de placer la pièce"
#define KEY_TEXT_CAMERA "Flèches Directionelles : Permet de bouger la caméra"

/********Constantes de montagnes ******/
#define PERLIN_SIZE 100
#define MOUNTAIN_MODEL_SIZE 1
#define MOUNTAIN_MODEL_HEIGHT 1.5
#define PERLIN_SCALE 0.8
#define MOUNTAIN_MODEL_COLOR BLACK
#define PERLIN_MODEL_SMOOTHING 3.0f
/****Constantes des forets ***************/

#define FORET_SIZE 1000
#define FORET_SCALE 60
#define TREE_SIZE 0.025f
#define FOREST_TRESHOLD 0.45f
#define FOREST_END_TRESHOLD 0.85f
#define TREE_DIVIDER 7
#define  TREE_Y_OFSET 0.55f
#define FOREST_BORDER 0.5f

#define BUSH_TRESHOLD 0.66f
#define BUSH_SIZE 0.05f
#define  GREEN_POWER 1 // puissance du vert pour faire plus de variation dans les couleurs des arbres, à ajuster en fonction de la noisemap de foret générée
#define GREEN_OFSET 0.3f // offset du vert pour éviter d'avoir des arbres trop sombre, à ajuster en fonction de la noisemap de foret générée


#define WATER_OFSET 0.2f
#define WATER_POWER 1
#define WATER_GREEN_FACTOR 0.0f
#define WATER_RED_FACTOR 0.5f
#define WATER_CUBE_OFSET -0.5f
#define WATER_TRANSPARENCY 255
#define WATER_CUBE_HEIGHT_MULTIPLYER 2


#define OFSET_IMAGE_SIZE 1000
#define OFSET_IMAGE_SCALE 500


#define VILLAGE_IMAGE_SIZE 1000
#define  VILLAGE_IMAGE_SCALE 100
#define HOUSE_TRESHOLD 0.7f 
#define HOUSE_DIVIDER 5
#define HOUSE_SIZE 0.05f
#define RED_POWER 1.0f
#define RED_OFSET 0.0f

#define MONSTER_SIZE 0.05f
#define MONSTER_TRESHOLD 0.7f

/**********Chemin des models 3d **************/

#define PATH_TO_TREE_MODEL "Assets/tree.obj"
#define PATH_TO_BUSH_MODEL "Assets/bush.obj"
#define PATH_TO_HOUSE "Assets/maison.obj"
#define PATH_TO_MONSTER "Assets/monster.obj"

#define PATH_TO_HDR_SKYBOX "Assets/skybox.hdr"
#define PATH_TO_SKYBOX "Assets/skybox2.png"
#define PATH_TO_CHAMPS_TEXTURE "Assets/champs.png"
#define PATH_TO_WATER_TEXTURE "Assets/water.png"
#define PATH_TO_MONSTER_TEXTURE "Assets/monster.png"
#define PATH_TO_FORET_TEXTURE "Assets/foret.png"
#define PATH_TO_VILLAGE_TEXTURE "Assets/village.png"

#define SKYBOX_SHADER_PATH "Assets/shaders/glsl%i/skybox.vs"
#define SKYBOX_SHADER_PATH2 "Assets/shaders/glsl%i/skybox.fs"
#define SKYBOX_CUBEMAP_SHADER_PATH "Assets/shaders/glsl%i/cubemap.vs"
#define SKYBOX_CUBEMAP_SHADER_PATH "Assets/shaders/glsl%i/cubemap.fs"


#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


#define NUM_CARDS 21
#define NUM_SEASONS 4
#define NUM_EDITS NUM_SEASONS






