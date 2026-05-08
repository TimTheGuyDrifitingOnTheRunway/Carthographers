#pragma once

#include "Data.h"



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
void copyCarte(FeuilleCarte Ffrom, FeuilleCarte Fto);
void flipShape(Piece shape);
int compareShape(Piece shape1, Piece shape2);
void copyFeuilleCarte(FeuilleCarte Ffrom, FeuilleCarte Fto);

int GroupNextStep(FeuilleCarte temp, int material, int materialToAvoid, int includeBorder);
int isGroupAtPosNeighborWithMaterial(FeuilleCarte f, Position pos, int material, int includeBorder);
int isAllProcessed(FeuilleCarte f);


void exploreGroup(FeuilleCarte f, int x, int y, FeuilleCarte visited, FeuilleCarte voisinsVisites, InfoGroupe* info);
//Trouver tous les groupes de la carte, leur taille, leur material, et le nombre de voisins en fonction de leur material
// Retourne le nombre de groupes trouvé
int RecenseEveryGroups(FeuilleCarte f, InfoGroupe listeGroupes[SIZE * SIZE]);

int NombreVoisionDeMat(FeuilleCarte f, int material, int ignoreRuins, int margin);

//dessins de formes//////////////////////////////

int drawU(FeuilleCarte f, Position pos, int rotation, int material);//DEPRECIé
int drawShape(FeuilleCarte f, Piece piec, Position pos, int rotation, int material);//fonction Universelle




//placement des formes/////////////////////////////////////////////////
int placementU(FeuilleCarte f, int material);//DEPRECIée
int placementDefault(FeuilleCarte f, int material);

int placementShape(FeuilleCarte f, const Piece shape, int material);//SP à utiliser pour placer une forme sur une feuilleCarte


int autoPlacement(FeuilleCarte f, const Piece shape, int material);



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









