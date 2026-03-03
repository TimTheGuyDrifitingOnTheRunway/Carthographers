#define _CRT_SECURE_NO_WARNINGS
#include "LibCarthographie.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>





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
    placementShape(f, L, CHAMPS);
    placementShape(f, L, EAU);

    printf("\n nombre of montagnes :%d\n", getOccurencesOf(f, MONTAGNE));





    printf("\n\n\n\n canal shoreside expanse : %d\n", calcShoreSideExpanse(f));



}