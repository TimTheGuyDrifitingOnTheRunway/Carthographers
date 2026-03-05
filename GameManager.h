#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "LibCarthographie.h"

int coinCount; //pr suivre le nombre de coin que le joueur a

typedef struct {
	int maxTime;
	int EditA;
	int EditB;
}Saison;

typedef struct {
	int time;
	Piece* pieceA;
	int iscoinA;
	Piece* pieceB; // NOTHING si pas de piece
	int iscoinB;

	int terrainA;
	int terrainB;  // 0 si pas de terrain
	int TerrainC;  // 0 si pas de terrain

	int isEnemy;
	int rotation; //1 = Right, -1 = Left, 0 = None

	int isRuin;
	int isRiftLands;
}ExploreCard;

typedef struct {
	int type;
	int (*fctCaluls)(FeuilleCarte f);
}ScoringCard;




















