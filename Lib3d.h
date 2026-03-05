#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "LibCarthographie.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>


/*****************************************CONSTANTES d'affichage*************************************************/

#define GRIDCOLOR GRAY
#define BORDERCOLOR MAROON


/*****************************************PROTOTYPES*************************************************/

void GUIDrawFeuille(FeuilleCarte f);
int GUIplacementShape(FeuilleCarte f, Piece shape, int material, Camera3D camera);
int GUIplacementDefault(FeuilleCarte f, int  material, Camera3D camera);
void GUIdrawGrille();
