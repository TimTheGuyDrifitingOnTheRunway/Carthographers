#pragma once

//Ce document est là pour réunir toutes les constantes au même endroit

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <raylib.h>


/*OPTIONS DE DEBOGAGE */
#define DEBUG_FORET// DEBUG_FORET : affiche la noisemap source de foret

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

#define BACKGROUND_COLOR LIGHTGRAY
#define BGCOLOR RAYWHITE

#define GRIDCOLOR GRAY
#define BORDERCOLOR MAROON
#define PLACEMENT_HEIGHT 1.0f

#define SPEEDY 5// constantes inverse de vitesse
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


#define MAX_NAME_LENGTH 20

/******** Constantes du Menu ********/

#define MENUX 800
#define MENUY 600

/********Constantes de montagnes ******/
#define PERLIN_SIZE 100
#define MOUNTAIN_MODEL_SIZE 1
#define MOUNTAIN_MODEL_HEIGHT 1.5
#define PERLIN_SCALE 0.8
#define MOUNTAIN_MODEL_COLOR BLACK
#define PERLIN_MODEL_SMOOTHING 3.0f
/****Constantes des forets ***************/
#define FORET_SIZE 1000
#define FORET_SCALE 20
#define TREE_SIZE 0.0005f
#define FOREST_TRESHOLD 0.4f
#define TREE_DIVIDER 10

#define BUSH_SIZE 0.05f
#define  GREEN_POWER 1 // puissance du vert pour faire plus de variation dans les couleurs des arbres, à ajuster en fonction de la noisemap de foret générée
#define GREEN_OFSET 0.3f // offset du vert pour éviter d'avoir des arbres trop sombre, à ajuster en fonction de la noisemap de foret générée


/******************************** GameManager.h ********************************/

#define EXP_CARD_NUMBER 34

/**********Chemin des models 3d **************/

#define PATH_TO_TREE_MODEL "assets/tree2.obj"
#define PATH_TO_BUSH_MODEL "assets/bush.obj"