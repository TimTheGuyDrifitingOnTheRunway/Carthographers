#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>



/*****************************************CONSTANTES**************************************************/
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

// ne rien metre sur 1
#define FORET 2
#define VILLAGE 3
#define CHAMPS 4
#define EAU 5
#define MONSTRE 6
#define RUINE 100 //Have to be big to not interfere with the material of the pieces
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
}Position;

typedef struct {
    int taille;
	int material;
    int materialVoisin[10];
}InfoGroupe;

/*****************************************PROTOTYPES**************************************************/
void initCarte(FeuilleCarte f, int montagne);
void initCarte2(FeuilleCarte f, int montagneActive, int ruinsActive);
void displayCarte(FeuilleCarte f);
int randInt(int min, int max);
void setupMontagnePosition(int posMontage[NOMBREMONTAGNE][2]);
void setupRuinsPosition(FeuilleCarte f, int posRuins[NOMBRERUINE][2]);
int isPositionInCarte(Position pos);
int isInCarte(int x, int y);
void draw(FeuilleCarte f, FeuilleCarte feuilleVide);
int isDrawable(FeuilleCarte f, FeuilleCarte feuilleVide);
void tryDraw(FeuilleCarte f, FeuilleCarte feuilleVide, FeuilleCarte sortie);
void rotateShape(Piece piece, int rotation);
int getEmptySpots(FeuilleCarte f);
int getMaterialAtPos(FeuilleCarte f, Position pos);
int getMaterialAt(FeuilleCarte f, int x, int y);
int isPosMaterial(FeuilleCarte f, Position pos, int material);
void getVoisinMaterialPos(FeuilleCarte f, Position pos, int material, Position listeVoisins[4]);
int getOccurencesOf(FeuilleCarte f, int material);
Position* getPositionsOfMaterial(FeuilleCarte f, int material);
float distPos(Position a, Position b);
Position* emptyPositionList(int size);
void copyPiece(const Piece pieceFrom, Piece pieceTo);
void flipShape(Piece shape);
int compareShape(Piece shape1, Piece shape2);


int GroupNextStep(FeuilleCarte temp, int material, int materialToAvoid, int includeBorder);
int isGroupAtPosNeighborWithMaterial(FeuilleCarte f, Position pos, int material, int includeBorder);
int isAllProcessed(FeuilleCarte f);


void exploreGroup(FeuilleCarte f, int x, int y, FeuilleCarte visited, FeuilleCarte voisinsVisites, InfoGroupe* info);
//Trouver tous les groupes de la carte, leur taille, leur material, et le nombre de voisins en fonction de leur material
// Retourne le nombre de groupes trouvé
int RecenseEveryGroups(FeuilleCarte f, InfoGroupe listeGroupes[SIZE * SIZE]);



//dessins de formes//////////////////////////////

int drawU(FeuilleCarte f, Position pos, int rotation, int material);//DEPRECIé
int drawShape(FeuilleCarte f, Piece piec, Position pos, int rotation, int material);//fonction Universelle




//placement des formes/////////////////////////////////////////////////
int placementU(FeuilleCarte f, int material);//DEPRECIée
int placementDefault(FeuilleCarte f, int material);

int placementShape(FeuilleCarte f, Piece shape, int material);//SP à utiliser pour placer une forme sur une feuilleCarte




// vérification de la placabilité
int checkU(FeuilleCarte f);//DEPRECIé
int checkShape(FeuilleCarte f, const Piece shape);
int coversRuin(FeuilleCarte f, FeuilleCarte feuilleVide);
int checkShapeOnRuin(FeuilleCarte f, const Piece shape);



//Calcul des points :


//Wood
int calcSentinelWood(FeuilleCarte f);
int calcTreeTower(FeuilleCarte f);
int calcGreenBough(FeuilleCarte f);
int calcStoneSideQuest(FeuilleCarte f);

int nextForestStep(FeuilleCarte f);
int isForestAllProcessed(FeuilleCarte f);

//Water & Farm
int calcCanalLake(FeuilleCarte f);
int calcGoldenGranary(FeuilleCarte f);
int calcMagesValley(FeuilleCarte f);
int calcShoreSideExpanse(FeuilleCarte f);

int pointAdjacensce(FeuilleCarte f, int materia1, int material2);

//int CalcShoreSidePart1(FeuilleCarte f, int material);
//void nextStepShoreside(FeuilleCarte temp);

//Village
int calcWildholds(FeuilleCarte f);
int calcGreengoldPlains(FeuilleCarte f);
int calcGreatCity(FeuilleCarte f);
int calcShieldgate(FeuilleCarte f);


//Map
int calcBorderlands(FeuilleCarte f);
int calcBrokenRoad(FeuilleCarte f);
int calcLostBarony(FeuilleCarte f);
int calcTheCauldrons(FeuilleCarte f);

int IsASquare(FeuilleCarte f, int x, int y, int length);

int calcEnenmyPoints(FeuilleCarte f);

int countSurroundedMountains(FeuilleCarte f);




/*****************************************CONSTANTES PIECES**************************************************/
// Notaion : L_XXX = petit XXX ; B_XXX = GRAND XXX

extern const Piece POINT;
extern const Piece NOTHING;

extern const Piece U;

extern const Piece L;
extern const Piece L_L;
extern const Piece B_L;

extern const Piece LINE;
extern const Piece L_LINE;

extern const Piece T;
extern const Piece L_T;

extern const Piece CUBE;
extern const Piece CUBE_WITH_POINT;
extern const Piece RECT;
extern const Piece RECT_WITH_HOLE;

extern const Piece DIAG;
extern const Piece L_DIAG;

extern const Piece CROSS;
extern const Piece STAIRS;
extern const Piece STRANGE;

extern const Piece Z;
extern const Piece B_Z;







