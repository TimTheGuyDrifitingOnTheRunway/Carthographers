#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


/*****************************************CONSTANTES**************************************************/
#define SIZE 11
#define PIECESIZE 3
#define PIECESIZE 3
#define NOMBREMONTAGNE 5
#define TRUE 1
#define FALSE 0
#define CONFLICTVALUE 9
#define OUTOFBOUND -1


#define FORET 2
#define VILLAGE 3
#define CHAMPS 4
#define EAU 5
#define MONSTRE 6
#define HEROS 7
#define MONTAGNE 8

#define ACTUALFOREST 10
#define NEXTFOREST 11

#define ACTUAL 12
#define NEXT 13

/*****************************************TYPEDEFS**************************************************/
typedef int FeuilleCarte[SIZE][SIZE];
typedef int Piece[PIECESIZE][PIECESIZE];



typedef struct {
    int x;
    int y;
} Position;

/*****************************************PROTOTYPES**************************************************/
void initCarte(FeuilleCarte f, int montagne);
void displayCarte(FeuilleCarte f);
int randInt(int min, int max);
void setupMontagnePosition(int posMontage[NOMBREMONTAGNE][2]);
int isPositionInCarte(Position pos);
int isInCarte(int x, int y);
void draw(FeuilleCarte f, FeuilleCarte feuilleVide);
int isDrawable(FeuilleCarte f, FeuilleCarte feuilleVide);
void tryDraw(FeuilleCarte f, FeuilleCarte feuilleVide, FeuilleCarte sortie);
void rotateShape(Piece piece, int rotation);
int getEmptySpots(FeuilleCarte f);
int isPosmaterial(FeuilleCarte f, Position pos, int material);
void getVoisinMaterialPos(FeuilleCarte f, Position pos, int material, Position listeVoisins[8]);
int getOccurencesOf(FeuilleCarte f, int material);
Position* getPositionsOfMaterial(FeuilleCarte f, int material);
float distPos(Position a, Position b);
Position* emptyPositionList(int size);

//dessins de formes//////////////////////////////

int drawU(FeuilleCarte f, Position pos, int rotation, int material);//DEPRECIé
int drawShape(FeuilleCarte f, Piece piec, Position pos, int rotation, int material);//fonction Universelle




//placement des formes/////////////////////////////////////////////////
int placementU(FeuilleCarte f, int material);//DEPRECIée
int placementDefault(FeuilleCarte f, int material);

int placementShape(FeuilleCarte f, Piece shape, int material);//SP à utiliser pour placer une forme sur une feuilleCarte




// vérification de la placabilité
int checkU(FeuilleCarte f);//DEPRECIé
int checkShape(FeuilleCarte f, Piece shape);

//Calcul des points :
int calcSentinelWood(FeuilleCarte f);
int calcTreeTower(FeuilleCarte f);
int calcGreenBough(FeuilleCarte f);

int calcStoneSideQuest(FeuilleCarte f);
int nextForestStep(FeuilleCarte f);
int isForestAllProcessed(FeuilleCarte f);


int calcCanalLake(FeuilleCarte f);
int pointAdjacensce(FeuilleCarte f, int materia1, int material2);

int calcShoreSideExpanse(FeuilleCarte f);
int CalcShoreSidePart1(FeuilleCarte f, int material);
void nextStepShoreside(FeuilleCarte temp);

/*****************************************CONSTANTES PIECES**************************************************/
// Notaion : L_XXX = petit XXX ; B_XXX = GRAND XXX

extern const Piece POINT;

extern const Piece U;

extern const Piece L;
extern const Piece L_L;
extern const Piece B_L;

extern const Piece LINE;
extern const Piece L_LINE;

extern const Piece T;
extern const Piece L_T;

extern const Piece CUBE;
extern const Piece RECT;
extern const Piece RECT_WITH_HOLE;

extern const Piece DIAG;
extern const Piece L_DIAG;

extern const Piece CROSS;
extern const Piece STAIRS;

extern const Piece Z;
extern const Piece B_Z;







