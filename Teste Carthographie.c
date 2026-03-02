// Teste Carthographie.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

// what if i wanted to have fun
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

/*****************************************CONSTANTES PIECES**************************************************/


const Piece POINT = { {0,0,0},{0,1,0}, {0,0,0} };
const Piece U = { 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {0, 0 ,0} };

const Piece L = {
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 1}
};


/*****************************************CODE MAIN**************************************************/




int main()
{
    srand(time(NULL));


    FeuilleCarte f;
    initCarte(f, TRUE);

    /*for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (f[i][j]==0)f[i][j] = FORET;
        }
    }*/
    
    placementShape(f, L, CHAMPS);
    placementShape(f, L, EAU);

    printf("\n nombre of montagnes :%d\n", getOccurencesOf(f, MONTAGNE));

    displayCarte(f);

   
    
  
    printf("\n\n\n\n canal Lake value : %d\n", calcCanalLake(f));
    

    
}
/******************************FONCTIONS SOUS PROGRAMMES************************/
void initCarte(FeuilleCarte f, int montagneActive) {
   
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            f[i][j] = 0;
        }

    }

    if (montagneActive){
        int posMontagne[NOMBREMONTAGNE][2];
        setupMontagnePosition(posMontagne);
        for (int i = 0; i < NOMBREMONTAGNE; i++) {
            f[posMontagne[i][0]][posMontagne[i][1]] = MONTAGNE;
        }
    }
    
}

void setupMontagnePosition(int posMontage[NOMBREMONTAGNE][2]) {
    Position *op = emptyPositionList(NOMBREMONTAGNE);
    
    

    for (int i = 0; i < NOMBREMONTAGNE; i++) {
        Position np;
        np.x = randInt(0, SIZE-1);
        np.y = randInt(0, SIZE-1);
        
        

        int validPos = 1;
        for (int j = 0; j < NOMBREMONTAGNE; j++) {
            
            if (distPos(np, op[j]) < 2.0 && isInCarte(np.x, np.y)) validPos = 0;
        }
        if (validPos) {
            posMontage[i][0] = np.x;
            posMontage[i][1] = np.y;
            op[i] = np;
        }
        else i--;

    }
}

void displayCarte(FeuilleCarte f) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", f[j][i]);

        }
        printf("\n");
    }
}

int randInt(int min, int max) {
    return (rand() % (max+1 - min)) + min;
}

float distPos(Position a, Position b) {
    
    float val = (float)((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
    return sqrtf(val);
}


int isPositionInCarte(Position pos) {
    if ((pos.x < SIZE) && (pos.x >= 0) && (pos.y < SIZE) && (pos.y >= 0)) return 1;
    return 0;
}

int isInCarte(int x, int y) {
    if ((x < SIZE) && (x >= 0) && (y < SIZE) && (y >= 0)) return 1;
    return 0;
}

void copyPiece(Piece pieceFrom, Piece pieceTo) {

    for (int i = 0; i < PIECESIZE; i++) {
        for (int j = 0; j < PIECESIZE; j++) {
            pieceTo[i][j] = pieceFrom[i][j];

        }

    }


}

int isPosmaterial(FeuilleCarte f, Position pos, int material) {
    if(isInCarte(pos.x, pos.y))return f[pos.x][pos.y] == material;
    return 0;
}

void getVoisinMaterialPos(FeuilleCarte f, Position pos, int material, Position listeVoisins[8]) {
    Position posCible;
    posCible.x = -1;
    posCible.y = -1;
    for (int a = 0; a < 8; a++) { listeVoisins[a] = posCible; }

    int k = 0;
    for (int i = -1; i < 2; i++) {
        for(int j=-1; j<2; j++){
            if ((i != 0) && (j != 0)) {
                posCible.x = pos.x + i;
                posCible.y = pos.y + j;
                if (isPosmaterial(f, posCible, material)) {
                    listeVoisins[k] = posCible;
                    k++;
                }
            }
        }
    }

}

int getEmptySpots(FeuilleCarte f) {
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == 0)count++;
        }
    }
    return count;
}

int getOccurencesOf(FeuilleCarte f, int material) {
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == material)count++;
        }
    }
    return count;
}

Position *getPositionsOfMaterial(FeuilleCarte f, int material){
    Position* positions = malloc(sizeof(Position) * getOccurencesOf(f, material));
    int k = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == material) {
                positions[k].x = i;
                positions[k].y = j;
                k++;
            }
        }
    }
    return positions;
}

void copyFeuilleCarte(FeuilleCarte Ffrom, FeuilleCarte Fto) {

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Fto[i][j] = Ffrom[i][j];
        }
    }
}

Position* emptyPositionList(int size) {
    Position *pos = malloc(sizeof(Position)*size);
    Position p;
    p.x = -10;
    p.y = -10;

    for (int i = 0; i < size; i++) {
        pos[i] = p;
    }
    return pos;
}

/*********************************FONCTIONS DE DESSIN DE FEUILLE********************************/

int isDrawable(FeuilleCarte f, FeuilleCarte feuilleVide) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((feuilleVide[i][j] != 0) && (f[i][j] != 0)) return 0;

        }
        
        
    }
    return 1;
}


void draw(FeuilleCarte f, FeuilleCarte feuilleVide) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (feuilleVide[i][j] != 0) {
                f[i][j] = feuilleVide[i][j];
            }

        }
        

    }


}


void tryDraw(FeuilleCarte f, FeuilleCarte feuilleVide, FeuilleCarte sortie) {
    initCarte(sortie, FALSE);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((feuilleVide[i][j] != 0) && (f[i][j] != 0)) sortie[i][j] = CONFLICTVALUE;
            else sortie[i][j] = max(feuilleVide[i][j], f[i][j]);

        }
        

    }
}






/*********************************FONCTIONS DE DESSIN DE FORMES*********************************/

int drawU(FeuilleCarte f, Position pos, int rotation, int material) {
    int rot = rotation % 4;
    if(!(isInCarte(pos.x , pos.y))) return 0;
    f[pos.x][pos.y] = material;

    switch (rot){
    case(0):
        

        if (!(isInCarte(pos.x - 1, pos.y)&isInCarte(pos.x + 1, pos.y)&isInCarte(pos.x + 1, pos.y + 1)&isInCarte(pos.x - 1, pos.y +1))) return 0;
        f[pos.x-1][pos.y] = material;
        f[pos.x + 1][pos.y] = material;
        f[pos.x + 1][pos.y+1] = material;
        f[pos.x - 1][pos.y + 1] = material;
        return 1;

    case(2):
        if (!(isInCarte(pos.x - 1, pos.y) & isInCarte(pos.x + 1, pos.y) & isInCarte(pos.x + 1, pos.y - 1) & isInCarte(pos.x - 1, pos.y - 1))) return 0;
        f[pos.x - 1][pos.y] = material;
        f[pos.x + 1][pos.y] = material; 
        f[pos.x + 1][pos.y - 1] = material;
        f[pos.x - 1][pos.y - 1] = material;
        return 1;

    case(1):
        if (!(isInCarte(pos.x , pos.y+1) & isInCarte(pos.x  , pos.y-1) & isInCarte(pos.x + 1, pos.y + 1) & isInCarte(pos.x + 1, pos.y - 1))) return 0;
        f[pos.x][pos.y + 1] = material;
        f[pos.x][pos.y - 1] = material;
        f[pos.x+1][pos.y + 1] = material;
        f[pos.x + 1][pos.y - 1] = material;
        return 1;

    case(3):
        if (!(isInCarte(pos.x, pos.y + 1) & isInCarte(pos.x, pos.y - 1) & isInCarte(pos.x - 1, pos.y + 1) & isInCarte(pos.x - 1, pos.y - 1))) return 0;
        f[pos.x][pos.y + 1] = material;
        f[pos.x][pos.y - 1] = material;
        f[pos.x - 1][pos.y + 1] = material;
        f[pos.x - 1][pos.y - 1] = material;
        return 1;



    }
    
    return 0;
}

int drawShape(FeuilleCarte f, Piece piece, Position pos, int rotation, int material) {
    FeuilleCarte buff;
    Piece pieceOrientee;
    copyPiece(piece, pieceOrientee);
    rotateShape(pieceOrientee, rotation);
    
    initCarte(buff, FALSE);
    for (int i = -1; i < PIECESIZE-1; i++) {
        for (int j = -1; j < PIECESIZE-1; j++) {
            if (pieceOrientee[i+1][j+1]==1) {
                if (!isInCarte(pos.x + i, pos.y + j)) return 0;
                buff[pos.x + i][pos.y + j] = material;
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (buff[i][j]!=0) f[i][j] = buff[i][j];
        }
    }
    return 1;

}

void rotateShape(Piece piece, int rotation) {
    for (int k = 0; k < rotation%4; k++) {
    int buff[SIZE][PIECESIZE] = { {0,0} };

    for (int i = 0; i < PIECESIZE; i++) {
        for (int j = 0; j < PIECESIZE; j++) {
            buff[j][i] = piece[i][PIECESIZE - j-1];

        }

    }

    
      
    
    for (int i = 0; i < PIECESIZE; i++) {
        for (int j = 0; j < PIECESIZE; j++) {
            piece[j][i] = buff[j][i];
            
        }    
    }
    }
}



/*********************************FONCTIONS DE PLACEMENT DE FORMES*********************************/

int placementU(FeuilleCarte f, int material) {
    if (checkU(f)) {
        Position pos;
        int rotation = 0;
        pos.x = 6;
        pos.y = 6;
        int drawable = 0;
        FeuilleCarte feuilleVide, temp;


        do {
            initCarte(feuilleVide, FALSE);
            displayCarte(f);
            printf("\n");
            drawable = drawU(feuilleVide, pos, rotation, material);
            tryDraw(f, feuilleVide, temp);
            displayCarte(temp);
            drawable = drawable && isDrawable(f, feuilleVide);
            printf(" position actuelle : (%d , %d , %d ) valide : %d \n  entrez la nouvelle position (X Y ROTATION) : ", pos.x, pos.y, rotation, drawable);
            scanf("%d %d %d", &pos.x, &pos.y, &rotation);
            printf("\n");


        } while ((pos.x > -1) || (pos.y > -1) || !(drawable));
        draw(f, feuilleVide);
        return 1;
    }
    else {
        return placementDefault(f, material);
    }


}


int placementDefault(FeuilleCarte f,int  material) {
    if (getEmptySpots(f) == 0) return 0;
    placementShape(f, POINT, material);
    return 1;

}

int placementShape(FeuilleCarte f, Piece shape, int material) {
    if (checkShape(f, shape)) {
        Position pos;
        int rotation = 0;
        pos.x = 6;
        pos.y = 6;
        int drawable = 0;
        FeuilleCarte feuilleVide, temp;


        do {
            initCarte(feuilleVide, FALSE);
            displayCarte(f);
            printf("\n");
            drawable = drawShape(feuilleVide, shape, pos, rotation, material);
            tryDraw(f, feuilleVide, temp);
            displayCarte(temp);
            drawable = drawable && isDrawable(f, feuilleVide);
            printf(" position actuelle : (%d , %d , %d ) valide : %d \n  entrez la nouvelle position (X Y ROTATION) : ", pos.x, pos.y, rotation, drawable);
            scanf("%d %d %d", &pos.x, &pos.y, &rotation);
            printf("\n");


        } while ((pos.x > -1) || (pos.y > -1) || !(drawable));
        draw(f, feuilleVide);
        return 1;
    }
    else {
        printf("IL n'y a pas la place pour rentrer votre piece \n");
        return placementDefault(f, material);
    }


}


/**************************FONCTIONS DE VERIFICATION DE SOLUTION***************************/

int checkU(FeuilleCarte f) {
    FeuilleCarte feuilleVide, temp;
    int drawable = 0;
    Position pos;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int r = 0; r < 4; r++) {
                initCarte(feuilleVide, FALSE);
                pos.x = i;
                pos.y = j;
                

                drawable = drawU(feuilleVide, pos, r, 4);
                tryDraw(f, feuilleVide, temp);
                displayCarte(temp);
                drawable = min(drawable, isDrawable(f, feuilleVide));
                printf("\n %d (%d %d)\n", isDrawable(f, feuilleVide), i, j);
                if (drawable == 1) return 1;



            }

        }


    }
    return 0;
}

int checkShape(FeuilleCarte f, Piece shape) {
    FeuilleCarte feuilleVide, temp;
    int drawable = 0;
    Position pos;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int r = 0; r < 4; r++) {
                initCarte(feuilleVide, FALSE);
                pos.x = i;
                pos.y = j;


                drawable = drawShape(feuilleVide,shape, pos, r, 4);
                tryDraw(f, feuilleVide, temp);
                
                drawable = min(drawable, isDrawable(f, feuilleVide));
                
                if (drawable == 1) return 1;



            }

        }


    }
    return 0;



}



/************************FONCTIONS de points*******************/


/*************************FORESTIERE********/
int calcSentinelWood(FeuilleCarte f) {
    int somme = 0;
    for (int i = 0; i < SIZE; i++) if (f[0][i] == FORET) somme++;
    for (int i = 0; i < SIZE; i++) if (f[SIZE-1][i] == FORET) somme++;
    for (int i = 0; i < SIZE; i++) if (f[i][0] == FORET) somme++;
    for (int i = 0; i < SIZE; i++) if (f[i][SIZE-1] == FORET) somme++;
    if (f[0][0] == FORET) somme--;
    if (f[0][SIZE - 1] == FORET) somme--;
    if (f[SIZE - 1][0] == FORET) somme--;
    if (f[SIZE - 1][SIZE - 1] == FORET) somme--;
    return somme;

}

int calcTreeTower(FeuilleCarte f) {
    int somme = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == FORET) {
                int count = 0;
                for (int k = -1; k < 2; k++) {
                    for (int l = -1; l < 2; l++) {
                        if (isInCarte(i + k, j + l)) {
                            if (f[i + k][j + l] == FORET)count++;
                        }
                        else count++;

                    }
                }
                if (count == 9)somme++;
            }
        }
    }
    return somme;

}

int calcGreenBough(FeuilleCarte f) {
    int somme = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == FORET) {
                somme++;
                j = SIZE;
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[j][i] == FORET) {
                somme++;
                j = SIZE;
            }
        }
    }

    return somme;
}

///Calcul des chemins etc...
int calcStoneSideQuest(FeuilleCarte f) {
    Position* mountainList = getPositionsOfMaterial(f, MONTAGNE);
    FeuilleCarte temp;
    int somme = 0;


    for (int i = 0; i < NOMBREMONTAGNE; i++) {
        copyFeuilleCarte(f, temp);
        temp[mountainList[i].x][mountainList[i].y] = ACTUALFOREST;
        int buff = 0;
        int step = 0;
        do {
            displayCarte(temp);
            printf("\n\n\n");
            buff = nextForestStep(temp);
            displayCarte(temp);
            printf("\n\n%d\n", buff);
            if (buff > -1)somme += buff;
            step++;
        } while ((buff > -1) && (step < 200) &&!(isForestAllProcessed(temp)));


    }

    return somme/2;
}


int nextForestStep(FeuilleCarte f) {
    int total = 0;
    int nbO = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == ACTUALFOREST) {
                f[i][j] = 0;
                for (int k = -1; k < 2; k++) {
                    for (int l = -1; l < 2; l++) {
						if (isInCarte(i + k, j + l)) {
                       
                        if (f[i + k][j + l] == FORET) {
                            f[i + k][j + l] = NEXTFOREST;
                        }
                        else if (f[i + k][j + l] == MONTAGNE) {
                            f[i + k][j + l] = 0;
                            total++;
                            }
                        }
                    }
                }
            }
            else if (f[i][j] == 0) nbO++;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == NEXTFOREST)f[i][j] = ACTUALFOREST;
        }
    }

    if (nbO == 11 * 11) return -1;
    return total;
    

}

int isForestAllProcessed(FeuilleCarte f) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == ACTUALFOREST) return 0;
        }
    }
    return 1;
}

/************aquatiques**********/

int calcCanalLake(FeuilleCarte f) {
    int somme = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == EAU) {

                for (int k = -1; k < 2; k ++) {
                    if (isInCarte(i + k, j)) if (f[i + k][j] == CHAMPS) somme++;
                }
                for (int k = -1; k < 2; k ++) {
                    if (isInCarte(i, j + k)) if (f[i][j + k] == CHAMPS) somme++;
                }

            }
            if (f[i][j] == CHAMPS) {

                for (int k = -1; k < 2; k ++) {
                    if (isInCarte(i + k, j)) if (f[i + k][j] == EAU) somme++;
                }
                for (int k = -1; k < 2; k ++) {
                    if (isInCarte(i, j + k)) if (f[i][j + k] == EAU) somme++;
                }


            }
        }
        
    }
    return somme;
}

int pointAdjacensce(FeuilleCarte f, int materia1, int material2) {
    int somme = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] == materia1) {
                for (int k = -1; k < 2; k++) {
                    if (isInCarte(i + k, j)) if (f[i + k][j] == material2) somme++;
                }
                for (int k = -1; k < 2; k++) {
                    if (isInCarte(i, j + k)) if (f[i][j + k] == material2) somme++;
                }
            }
        }
    }
    return somme;
}

int calcMageValey(FeuilleCarte f) {

    int somme;
    somme = pointAdjacensce(f, EAU, MONTAGNE) * 2;
	somme += pointAdjacensce(f, CHAMPS, MONTAGNE);

}




