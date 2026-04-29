#include "LibCarthographie.h"


/*****************************************CONSTANTES PIECES*************************************************/

const Piece POINT = { {0,0,0},{0,1,0},{0,0,0} };
const Piece NOTHING = { {0,0,0},{0,0,0},{0,0,0} };

const Piece U = { {0, 1, 1},{0, 1, 0},{0, 1, 1} };

const Piece L = { {0, 1, 0},{0, 1, 0},{0, 1, 1} };
const Piece L_L = { {0,1,0},{0,1,1},{0,0,0} };
const Piece B_L = { {1,0,0},{1,0,0},{1,1,1} };

const Piece LINE = { {0,0,0},{1,1,1},{0,0,0} };
const Piece L_LINE = { {0,0,0},{1,1,0},{0,0,0} };

const Piece T = { {1,1,1},{0,1,0},{0,1,0} };
const Piece L_T = { {0,1,0},{0,1,1},{0,1,0} };

const Piece CUBE = { {1,1,0},{1,1,0},{0,0,0} };
const Piece CUBE_WITH_POINT = { {1,1,0},{1,1,0},{1,0,0} };
const Piece RECT = { {1,1,1},{1,1,1},{0,0,0} };
const Piece RECT_WITH_HOLE = { {1,0,1},{1,0,1},{0,0,0} };

const Piece DIAG = { {1,0,0},{0,1,0},{0,0,1} };
const Piece L_DIAG = { {1,0,0},{0,1,0},{0,0,0} };

const Piece CROSS = { {0,1,0},{1,1,1},{0,1,0} };
const Piece STAIRS = { {1,1,0},{0,1,1},{0,0,1} };
const Piece STRANGE = {
	{1,0,0},
	{1,1,1},
	{0,1,0} };


const Piece Z = { {0,0,1},{0,1,1},{0,1,0} };
const Piece B_Z = { {0,0,1},{1,1,1},{1,0,0} };



/******************************FONCTIONS SOUS PROGRAMMES************************/
// Initialise une carte vide, optionnellement avec des montagnes (déprécié)
void initCarte(FeuilleCarte f, int montagneActive) {

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			f[i][j] = 0;
		}

	}

	if (montagneActive) {
		int posMontagne[NOMBREMONTAGNE][2];
		setupMontagnePosition(posMontagne);
		for (int i = 0; i < NOMBREMONTAGNE; i++) {
			f[posMontagne[i][0]][posMontagne[i][1]] = MONTAGNE;
		}
	}

}

// Initialise une carte vide avec optionnellement des montagnes et des ruines
void initCarte2(FeuilleCarte f, int montagneActive, int ruinsActive) {

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			f[i][j] = 0;
		}

	}

	if (montagneActive) {
		int posMontagne[NOMBREMONTAGNE][2];
		setupMontagnePosition(posMontagne);
		for (int i = 0; i < NOMBREMONTAGNE; i++) {
			f[posMontagne[i][0]][posMontagne[i][1]] = MONTAGNE;
		}
	}

	if (ruinsActive) {
		int posRuins[NOMBRERUINE][2];
		setupRuinsPosition(f, posRuins);

		for (int i = 0; i < NOMBRERUINE; i++) {
			f[posRuins[i][0]][posRuins[i][1]] = RUINE;
		}
	}


}


// Génère les positions aléatoires des montagnes en respectant la distance minimale
void setupMontagnePosition(int posMontage[NOMBREMONTAGNE][2]) {
	Position* op = emptyPositionList(NOMBREMONTAGNE);



	for (int i = 0; i < NOMBREMONTAGNE; i++) {
		Position np;
		np.x = randInt(1, SIZE - 2);
		np.y = randInt(1, SIZE - 2);



		int validPos = 1;
		for (int j = 0; j < NOMBREMONTAGNE; j++) {

			if (distPos(np, op[j]) < MONTAGNEDIST && isInCarte(np.x, np.y)) validPos = 0;
		}
		if (validPos) {
			posMontage[i][0] = np.x;
			posMontage[i][1] = np.y;
			op[i] = np;
			printf("Mountain %d pos : %d, %d \n", i, np.x, np.y);
		}
		else i--;

	}
	free(op);
}

// Inverse horizontalement une pièce (effet miroir)
void flipShape(Piece shape) {
	for (int i = 0; i < PIECESIZE; i++) {
		for (int j = 0; j < PIECESIZE / 2; j++) {
			int temp = shape[i][j];
			shape[i][j] = shape[i][PIECESIZE - 1 - j];
			shape[i][PIECESIZE - 1 - j] = temp;
		}
	}

}

// Génère les positions aléatoires des ruines en respectant la distance minimale et sans chevaucher les montagnes
void setupRuinsPosition(FeuilleCarte f, int posRuins[NOMBRERUINE][2]) {
	Position* op = emptyPositionList(NOMBRERUINE);
	int a = 0;

	for (int i = 0; i < NOMBRERUINE; i++) {
		Position np;
		do {
			np.x = randInt(1, SIZE - 2);
			np.y = randInt(1, SIZE - 2);
		} while (getMaterialAtPos(f, np) == MONTAGNE);


		int validPos = 1;

		for (int j = 0; j < NOMBRERUINE; j++) {
			if (distPos(np, op[j]) < RUINEDIST && isInCarte(np.x, np.y)) { validPos = 0; a++; break; }
		}
		if (validPos) {
			posRuins[i][0] = np.x;
			posRuins[i][1] = np.y;
			op[i] = np;
			printf("Ruins %d pos : %d, %d \n", i, np.x, np.y);
		}
		else if (a > 1000) {
			i = 0;
			a = 0;
			printf("Too much failed attempts to place ruins, retrying whole placement\n");
			// implémenter un truc WTF si jamais ça arrive, 1 chance sur des milliards, mais bon, on sait jamais
		}
		else i--;
	}
	free(op);
}


// Affiche la carte en mode texte (débogage)
void displayCarte(FeuilleCarte f) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			printf("%d ", f[j][i]);

		}
		printf("\n");
	}
}

// Génère un nombre aléatoire entre min et max (inclus)
int randInt(int min, int max) {
	return (rand() % (max + 1 - min)) + min;
}

// Calcule la distance euclidienne entre deux positions
float distPos(Position a, Position b) {

	float val = (float)((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
	return sqrtf(val);
}


// Vérifie si une position est dans les limites de la carte
int isPositionInCarte(Position pos) {
	if ((pos.x < SIZE) && (pos.x >= 0) && (pos.y < SIZE) && (pos.y >= 0)) return 1;
	return 0;
}

// Vérifie si des coordonnées sont dans les limites de la carte
int isInCarte(int x, int y) {
	if ((x < SIZE) && (x >= 0) && (y < SIZE) && (y >= 0)) return 1;
	return 0;
}

// Copie une pièce vers une autre
void copyPiece(const Piece pieceFrom, Piece pieceTo) {
	for (int i = 0; i < PIECESIZE; i++) {
		for (int j = 0; j < PIECESIZE; j++) {
			pieceTo[i][j] = pieceFrom[i][j];

		}
	}
}

// Copie une feuille de carte vers une autre
void copyCarte(FeuilleCarte Ffrom, FeuilleCarte Fto) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			Fto[i][j] = Ffrom[i][j];
		}
	}
}

// Récupère le matériau à une position donnée (ignorant les ruines)
int getMaterialAtPos(FeuilleCarte f, Position pos) {
	if (isInCarte(pos.x, pos.y)) { return (f[pos.x][pos.y] % RUINE); }
	return OUTOFBOUND;
}

// Récupère le matériau aux coordonnées données
int getMaterialAt(FeuilleCarte f, int x, int y) {
	if (isInCarte(x, y)) { return (f[x][y] % RUINE); }
	return OUTOFBOUND;
}


// Vérifie si une position contient un matériau spécifique
int isPosMaterial(FeuilleCarte f, Position pos, int material) {
	return getMaterialAtPos(f, pos) == material;
}

// Récupère les voisins adjacents d'une position ayant un matériau spécifique
void getVoisinMaterialPos(FeuilleCarte f, Position pos, int material, Position listeVoisins[4]) {
	Position posCible;
	int k = 0;
	/*for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if ((i != 0) && (j != 0)) {
				posCible.x = pos.x + i;
				posCible.y = pos.y + j;
				if (isPosMaterial(f, posCible, material)) {
					listeVoisins[k] = posCible;
					k++;
				}
			}
		}
	}*/

	Position d[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
	for (int i = 0; i < 4; i++) {
		posCible.x = pos.x + d[i].x;
		posCible.y = pos.y + d[i].y;
		if (isPosMaterial(f, posCible, material)) {
			listeVoisins[k] = posCible;
			k++;
		}
	}
}

// Compte le nombre de cases vides dans la carte
int getEmptySpots(FeuilleCarte f) {
	int count = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == 0)count++;
		}
	}
	return count;
}

// Compte le nombre de cases contenant un matériau spécifique
int getOccurencesOf(FeuilleCarte f, int material) {
	int count = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == material)count++;
		}
	}
	return count;
}

// Retourne array des positions contenant un matériau spécifique
Position* getPositionsOfMaterial(FeuilleCarte f, int material) {
	int occurences = getOccurencesOf(f, material);
	if (occurences == 0) return NULL;

	Position* positions = malloc(sizeof(Position) * occurences);
	if (positions == NULL) return NULL; // allocation failed

	int k = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == material) {
				positions[k].x = i;
				positions[k].y = j;
				k++;
			}
		}
	}
	return positions;
}

// Copie une feuille de carte vers une autre
void copyFeuilleCarte(FeuilleCarte Ffrom, FeuilleCarte Fto) {

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			Fto[i][j] = Ffrom[i][j];
		}
	}
}


// Crée et initialise une liste vide de positions
Position* emptyPositionList(int size) {
	Position* pos = malloc(sizeof(Position) * size);
	Position p;
	p.x = -10;
	p.y = -10;

	for (int i = 0; i < size; i++) {
		pos[i] = p;
	}
	return pos;
}

// Vérifie si un groupe est adjacent à un matériau spécifique ou à la bordure
int isGroupAtPosNeighborWithMaterial(FeuilleCarte f, Position pos, int material, int includeBorder) {
	FeuilleCarte temp;
	int groupMaterial = f[pos.x][pos.y];
	printf("metirial cible : %d \n", groupMaterial);
	copyFeuilleCarte(f, temp);
	temp[pos.x][pos.y] = ACTUAL;
	if (temp[pos.x][pos.y] == 0) return 0;
	int finish = 0;
	int result = 1;
	while (result && !finish) {
		displayCarte(temp);
		printf("\n");

		result = GroupNextStep(temp, groupMaterial, material, includeBorder);
		finish = isAllProcessed(temp);
		displayCarte(temp);
		printf("\n");

	}
	return !result;

}

// Étape récursive du calcul d'adjacence de groupe
int GroupNextStep(FeuilleCarte temp, int material, int materialToAvoid, int includeBorder) {
	int retour = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(temp, i, j) == ACTUAL) {
				retour = 1;
				temp[i][j] = 0;
				for (int k = -1; k < 2; k++) {

					if (getMaterialAt(temp, i + k, j) == material) temp[i + k][j] = NEXT;
					else if (getMaterialAt(temp, i + k, j) == materialToAvoid) return 0;
					else if (!isInCarte(i + k, j) && includeBorder) return 0;

					if (getMaterialAt(temp, i, j + k) == material) temp[i + k][j] = NEXT;
					else if (getMaterialAt(temp, i, j + k) == materialToAvoid) return 0;
					else if (!isInCarte(i + k, j) && includeBorder) return 0;

				}
			}
		}
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (getMaterialAt(temp, i, j) == NEXT) temp[i][j] = ACTUAL;
			}
		}

	}
	return 1;
}

// Vérifie si toutes les positions d'un groupe ont été traitées
int isAllProcessed(FeuilleCarte f) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == ACTUAL) return 0;
		}
	}
	return 1;
}


// Explore récursivement un groupe connecté de terrains pour calculer ses statistiques
void exploreGroup(FeuilleCarte f, int x, int y, FeuilleCarte visited, FeuilleCarte voisinsVisited, InfoGroupe* info) {
	int material = getMaterialAt(f, x, y);

	if (material == -1) {
		info->materialVoisin[1] = 1;
		return;
	}

	if (material != info->material) {
		if (voisinsVisited[x][y] == 0 && material < 10) {
			info->materialVoisin[material] += 1;
			voisinsVisited[x][y] = 1;
		}
		return;
	}

	if (visited[x][y] == 1) return;

	visited[x][y] = 1;
	info->taille++;

	exploreGroup(f, x + 1, y, visited, voisinsVisited, info);
	exploreGroup(f, x - 1, y, visited, voisinsVisited, info);
	exploreGroup(f, x, y + 1, visited, voisinsVisited, info);
	exploreGroup(f, x, y - 1, visited, voisinsVisited, info);
}

int RecenseEveryGroups(FeuilleCarte f, InfoGroupe listeGroupes[SIZE * SIZE]) {
	int start = clock();
	FeuilleCarte visited;
	initCarte2(visited, FALSE, FALSE);

	int nbGroupes = 0;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {

			int mat = getMaterialAt(f, i, j);
			if (visited[i][j] == 0 && mat != 0 && mat != -1) {

				listeGroupes[nbGroupes].taille = 0;
				listeGroupes[nbGroupes].material = mat;
				for (int k = 0; k < 10; k++) {
					listeGroupes[nbGroupes].materialVoisin[k] = 0;
				}
				FeuilleCarte voisinsVisited;
				initCarte(voisinsVisited, FALSE);

				exploreGroup(f, i, j, visited, voisinsVisited, &listeGroupes[nbGroupes]);

				nbGroupes++;
			}
		}
	}
	printf("Recensement des groupes effectué en %f secondes\n", (float)(clock() - start));
	return nbGroupes;
}

int compareShape(Piece shape1, Piece shape2) {// compare 2 shape et renvoi 1 si c'est les m$êmes

	for (int i = 0; i < PIECESIZE; i++) for (int j = 0; j < PIECESIZE; j++) if (shape1[i][j] != shape2[i][j]) return 0;
	return 1;
}


/*********************************FONCTIONS DE DESSIN DE FEUILLE********************************/

int isDrawable(FeuilleCarte f, FeuilleCarte feuilleVide) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (feuilleVide[i][j] != 0 && getMaterialAt(f, i, j) != 0) return 0;
		}
	}
	return 1;
}


void draw(FeuilleCarte f, FeuilleCarte feuilleVide) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (feuilleVide[i][j] != 0) {
				if (f[i][j] == RUINE) f[i][j] = feuilleVide[i][j] + RUINE;
				else
					f[i][j] = feuilleVide[i][j];
			}
		}
	}
}


void tryDraw(FeuilleCarte f, FeuilleCarte feuilleVide, FeuilleCarte sortie) {// remplie la grille sortie en fusionant les valeurs de f et feuillevide. si 2 valeurs sont au même endroit : ecrit COnflictValue à la place
	initCarte(sortie, FALSE);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if ((feuilleVide[i][j] != 0) && (f[i][j] != 0)) {
				if (f[i][j] == RUINE) sortie[i][j] = feuilleVide[i][j] + RUINE;
				else sortie[i][j] = CONFLICTVALUE;
			}
			else sortie[i][j] = max(feuilleVide[i][j], f[i][j]);

		}


	}
}






/*********************************FONCTIONS DE DESSIN DE FORMES*********************************/

int drawU(FeuilleCarte f, Position pos, int rotation, int material) {
	int rot = rotation % 4;
	if (!(isInCarte(pos.x, pos.y))) return 0;
	f[pos.x][pos.y] = material;

	switch (rot) {
	case(0):


		if (!(isInCarte(pos.x - 1, pos.y) & isInCarte(pos.x + 1, pos.y) & isInCarte(pos.x + 1, pos.y + 1) & isInCarte(pos.x - 1, pos.y + 1))) return 0;
		f[pos.x - 1][pos.y] = material;
		f[pos.x + 1][pos.y] = material;
		f[pos.x + 1][pos.y + 1] = material;
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
		if (!(isInCarte(pos.x, pos.y + 1) & isInCarte(pos.x, pos.y - 1) & isInCarte(pos.x + 1, pos.y + 1) & isInCarte(pos.x + 1, pos.y - 1))) return 0;
		f[pos.x][pos.y + 1] = material;
		f[pos.x][pos.y - 1] = material;
		f[pos.x + 1][pos.y + 1] = material;
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
	for (int i = -1; i < PIECESIZE - 1; i++) {
		for (int j = -1; j < PIECESIZE - 1; j++) {
			if (pieceOrientee[i + 1][j + 1] == 1) {
				if (!isInCarte(pos.x + i, pos.y + j)) return 0;
				buff[pos.x + i][pos.y + j] = material;
			}
		}
	}
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (buff[i][j] != 0) f[i][j] = buff[i][j];
		}
	}
	return 1;

}

void rotateShape(Piece piece, int rotation) {
	for (int k = 0; k < rotation % 4; k++) {
		int buff[PIECESIZE][PIECESIZE] = { {0,0} };

		for (int i = 0; i < PIECESIZE; i++) {
			for (int j = 0; j < PIECESIZE; j++) {
				buff[j][i] = piece[i][PIECESIZE - j - 1];
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
			//scanf("%d %d %d", &pos.x, &pos.y, &rotation);
			printf("\n");


		} while ((pos.x > -1) || (pos.y > -1) || !(drawable));
		draw(f, feuilleVide);
		return 1;
	}
	else {
		return placementDefault(f, material);
	}


}


int placementDefault(FeuilleCarte f, int  material) {
	if (getEmptySpots(f) == 0) return 0;
	placementShape(f, POINT, material);
	return 1;

}

int placementShape(FeuilleCarte f, const Piece shape, int material) {
	if (checkShape(f, shape)) {
		Piece shapeCopy;
		copyPiece(shape, shapeCopy);
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
			drawable = drawShape(feuilleVide, shapeCopy, pos, rotation, material);
			tryDraw(f, feuilleVide, temp);
			displayCarte(temp);
			drawable = drawable && isDrawable(f, feuilleVide);
			int flip = 0;
			printf(" position actuelle : (%d , %d , %d ) valide : %d \n  entrez la nouvelle position (X Y ROTATION FLIP) : ", pos.x, pos.y, rotation, drawable);
			//scanf("%d %d %d %d", &pos.x, &pos.y, &rotation, &flip);
			if (flip) flipShape(shapeCopy);
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
	/// vérifie si il y a la place de placer un u quelque part sur la carte, et retourne 1 si c'est le cas, 0 sinon
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

int checkShape(FeuilleCarte f, const Piece shape) {/// vérifie si il y a la place de placer une piece quelque part sur la carte, et retourne 1 si c'est le cas, 0 sinon
	FeuilleCarte feuilleVide, temp;
	int drawable = 0;
	Position pos;
	for (int i = -1; i < SIZE + 1; i++) {
		for (int j = -1; j < SIZE + 1; j++) {
			for (int r = 0; r < 4; r++) {
				for (int flip = 0; flip < 2; flip++) {
					Piece shapeCopy;
					copyPiece(shape, shapeCopy);
					if (flip) flipShape(shapeCopy);
					initCarte(feuilleVide, FALSE);
					pos.x = i;
					pos.y = j;


					drawable = drawShape(feuilleVide, shapeCopy, pos, r, 4);
					tryDraw(f, feuilleVide, temp); // utile ?

					drawable = min(drawable, isDrawable(f, feuilleVide));

					if (drawable == 1) return 1;
				}
			}
		}
	}
	return 0;
}

int coversRuin(FeuilleCarte f, FeuilleCarte feuilleVide) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			// Si la case de la forme est pleine ET que la case correspondante sur la map est une RUINE non couverte
			if (feuilleVide[i][j] != 0 && f[i][j] == RUINE) {
				return 1;
			}
		}
	}
	return 0;
}

int checkShapeOnRuin(FeuilleCarte f, const Piece shape) {
	FeuilleCarte feuilleVide, temp;
	int drawable = 0;
	Position pos;

	for (int i = -1; i < SIZE + 1; i++) {
		for (int j = -1; j < SIZE + 1; j++) {
			for (int r = 0; r < 4; r++) {
				for (int flip = 0; flip < 2; flip++) {
					Piece shapeCopy;
					copyPiece(shape, shapeCopy);
					if (flip) flipShape(shapeCopy);

					initCarte(feuilleVide, FALSE);
					pos.x = i;
					pos.y = j;

					drawable = drawShape(feuilleVide, shapeCopy, pos, r, 4);
					tryDraw(f, feuilleVide, temp); // utile ?

					// Si on peut la dessiner, on vérifie si elle est jouable ET si elle couvre une ruine
					if (drawable) {
						if (isDrawable(f, feuilleVide) && coversRuin(f, feuilleVide)) {
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}


/************************FONCTIONS de points*******************/



/*******FORESTIERE********/
int calcSentinelWood(FeuilleCarte f) {
	int somme = 0;
	for (int i = 0; i < SIZE - 1; i++) if (f[0][i] == FORET) somme++;
	for (int i = 0; i < SIZE - 1; i++) if (f[SIZE - 1][i] == FORET) somme++;
	for (int i = 1; i < SIZE - 1; i++) if (f[i][0] == FORET) somme++;
	for (int i = 0; i < SIZE; i++) if (f[i][SIZE - 1] == FORET) somme++;
	return somme;

}

int calcTreeTower(FeuilleCarte f) {
	int somme = 0;
	int valid = 0;
	Position d[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == FORET) {
				valid = 1;
				for (int k = 0; k < 4; k++) {
					if (getMaterialAt(f, i + d[k].x, j + d[k].y) == 0) valid = 0;
				}
				if (valid) somme++;
			}
		}
	}
	return somme;

}

int calcGreenBough(FeuilleCarte f) {
	int somme = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == FORET) {
				somme++;
				break;
			}
		}
	}
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (f[j][i] == FORET) {
				somme++;
				break;
			}
		}
	}

	return somme;
}

///Calcul des chemins etc...
int calcStoneSideQuest(FeuilleCarte f) {
	int mountainCount = getOccurencesOf(f, MONTAGNE);
	if (mountainCount == 0) return 0;

	Position* mountainList = getPositionsOfMaterial(f, MONTAGNE);
	if (mountainList == NULL) return 0; // allocation failed or no mountains

	FeuilleCarte temp;
	int somme = 0;


	for (int i = 0; i < mountainCount; i++) {
		copyFeuilleCarte(f, temp);
		temp[mountainList[i].x][mountainList[i].y] = ACTUALFOREST;
		int buff = 0;
		int step = 0;
		int valid = 0;
		do {
			// displayCarte(temp);
			//printf("\n\n\n");
			buff = nextForestStep(temp);
			//displayCarte(temp);
			//printf("\n\n%d\n", buff);
			if (buff > 0) { valid = 1; break; }
			step++;
		} while ((buff > -1) && (step < 200) && !(isForestAllProcessed(temp)));
		if (valid) somme += 1; // on augmente que d'un par montagne, peut importe le nombre de montagnes connectées
	}
	free(mountainList);
	return somme * 3;
}


int nextForestStep(FeuilleCarte f) {
	int total = 0;
	int nbO = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAtPos(f, (Position) { i, j }) == ACTUALFOREST) {
				f[i][j] = 0;
				for (int k = -1; k < 2; k++) {

					if (isInCarte(i + k, j)) {

						if (getMaterialAtPos(f, (Position) { i + k, j }) == FORET) {
							f[i + k][j] = NEXTFOREST;
						}
						else if (getMaterialAtPos(f, (Position) { i + k, j }) == MONTAGNE) {
							f[i + k][j] = 0;
							total++;
						}
					}

					if (isInCarte(i, j + k)) {

						if (getMaterialAtPos(f, (Position) {
							i, j + k
						}) == FORET) {
							f[i][j + k] = NEXTFOREST;
						}
						else if (getMaterialAtPos(f, (Position) {
							i, j + k
						}) == MONTAGNE) {
							f[i][j + k] = 0;
							total++;
						}

					}
				}
			}
			else if (getMaterialAtPos(f, (Position) { i, j }) == 0) nbO++;
		}
	}
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == NEXTFOREST) f[i][j] = ACTUALFOREST;
		}
	}

	if (nbO == 11 * 11) return -1;
	return total;


}

int isForestAllProcessed(FeuilleCarte f) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == ACTUALFOREST) return 0;
		}
	}
	return 1;
}

/************aquatiques**********/

int calcCanalLake(FeuilleCarte f) {
	int somme = 0;
	int valid = 0;
	Position d[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			valid = 0;
			if ((getMaterialAt(f, i, j)) == EAU) {
				for (int k = 0; k < 4; k++) {
					if (getMaterialAt(f, i + d[k].x, j + d[k].y) == CHAMPS) { valid = 1; break; }
				}
				if (valid) somme++;
			}
			else if (getMaterialAt(f, i, j) == CHAMPS) {
				for (int k = 0; k < 4; k++) {
					if (getMaterialAt(f, i + d[k].x, j + d[k].y) == EAU) { valid = 1; break; }
				}
				if (valid) somme++;
			}
		}

	}
	return somme;
}

int pointAdjacensce(FeuilleCarte f, int material1, int material2) {
	int somme = 0;
	int valid = 0;
	Position d[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			valid = 0;
			if (getMaterialAt(f, i, j) == material1) {
				for (int k = 0; k < 4; k++) {
					if (getMaterialAt(f, i + d[k].x, j + d[k].y)) { valid = 1; break; }
				}
			}
			if (valid) somme++;
		}
	}
	return somme;
}


int calcGoldenGranary(FeuilleCarte f) {
	int somme = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (f[i][j] >= RUINE) {
				Position d[] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
				for (int k = 0; k < 4; k++) {
					if (getMaterialAt(f, i + d[k].x, j + d[k].y) == EAU) somme++;
				}
				if (f[i][j] == RUINE + CHAMPS) {
					somme += 3;
				}
			}
		}
	}
	return somme;
}


int calcMagesValley(FeuilleCarte f) {

	int somme;
	somme = pointAdjacensce(f, EAU, MONTAGNE) * 2;
	somme += pointAdjacensce(f, CHAMPS, MONTAGNE);
	return somme;
}

int calcShoreSideExpanse(FeuilleCarte f) {
	int somme = 0;

	InfoGroupe allGroups[SIZE * SIZE];
	int total = RecenseEveryGroups(f, allGroups);

	for (int i = 0; i < total; i++) {
		InfoGroupe aG = allGroups[i];
		if (aG.material == EAU && aG.materialVoisin[CHAMPS] == 0 && aG.materialVoisin[1] == 0) somme++;
		if (aG.material == CHAMPS && aG.materialVoisin[EAU] == 0 && aG.materialVoisin[1] == 0) somme++;
	}
	return somme * 3;
}

// ancient calcShoreSideExpanse
/*
int calcShoreSideExpanse(FeuilleCarte f) {
	int somme = 0;
	somme += CalcShoreSidePart1(f, CHAMPS);


	return somme;
}

int CalcShoreSidePart1(FeuilleCarte f, int material) {
	Position* posOfMaterial = getPositionsOfMaterial(f, material);
	Position* alreadyChecked = emptyPositionList(getOccurencesOf(f, material));

	int occurences = getOccurencesOf(f, material);
	int somme = 0;


	for (int i = 0; i < occurences; i++) {
		int retour = 1;

		FeuilleCarte temp;
		copyFeuilleCarte(f, temp);
		temp[posOfMaterial[i].x][posOfMaterial[i].y] = ACTUAL;
		int fullprocess = 0;
		while (retour && !fullprocess) {
			retour = ShoreSide1NextStep(temp, material, alreadyChecked, occurences);
			fullprocess = isAllProcessed(temp);
			displayCarte(temp);
			printf("\n");
			for (int i = 0; i < occurences; i++)printf("(%d, %d) ", alreadyChecked[i].x, alreadyChecked[i].y);

		}
		if (retour) {
			somme += 3;

			printf("\n\n valeur de somme actuelle : %d \n\n", somme);
		}
	}

	free(posOfMaterial);
	return somme;

}

int ShoreSide1NextStep(FeuilleCarte temp, int material, Position* alreadyChecked, int occurences) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (temp[i][j] == ACTUAL) {

				temp[i][j] = 0;
				for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						if (isInCarte(i + k, j + l)) {
							if (getMaterialAtPos(temp, (Position){ i + k , j + k}) == material) {

								for (int m = 0; m < occurences; m++) {
									if ((alreadyChecked[m].x == i + k) && (alreadyChecked[m].y == j + l)) {

										return 0;
									}
								}

								temp[i + k][j + l] = NEXT;
								printf(" debug : \n ");
								displayCarte(temp);
								printf("\n");
								int n = 0;
								while (alreadyChecked[n].x > -1)n++;
								alreadyChecked[n].x = i + k;
								alreadyChecked[n].y = j + l;


							}
							else if (temp[i + k][j + l] == EAU) return 0;
						}
						else {
							return 0;
						}
					}
				}
			}
		}
		nextStepShoreside(temp);
	}
	return 1;
}

void nextStepShoreside(FeuilleCarte temp) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (temp[i][j] == NEXT) temp[i][j] = ACTUAL;
		}
	}
}
*/

// Village

int calcWildholds(FeuilleCarte f) {
	int somme = 0;

	InfoGroupe allGroups[SIZE * SIZE];
	int total = RecenseEveryGroups(f, allGroups);

	for (int i = 0; i < total; i++) {
		if (allGroups[i].material == VILLAGE && allGroups[i].taille >= 6) somme++;
	}
	return somme * 8;
}

int calcGreengoldPlains(FeuilleCarte f) {
	int somme = 0;
	int voisins = 0;
	InfoGroupe allGroups[SIZE * SIZE];
	int total = RecenseEveryGroups(f, allGroups);

	for (int i = 0; i < total; i++) {
		InfoGroupe aG = allGroups[i];
		if (aG.material == VILLAGE && aG.taille >= 6) {
			voisins = 0;
			for (int j = 0; j < 10; j++) {
				if (aG.materialVoisin[j] >= 1) {
					voisins++;
				}
			}
			if (voisins >= 3) somme++;
		}
	}
	return somme * 3;
}

int calcGreatCity(FeuilleCarte f) {
	int length = 0;

	InfoGroupe allGroups[SIZE * SIZE];
	int total = RecenseEveryGroups(f, allGroups);

	for (int i = 0; i < total; i++) {
		InfoGroupe aG = allGroups[i];
		if (aG.material == VILLAGE && aG.taille > length && aG.materialVoisin[MONTAGNE] == 0) length = aG.taille;
	}
	return length;
}

int calcShieldgate(FeuilleCarte f) {
	int bigger = 0;
	int second = 0;

	InfoGroupe allGroups[SIZE * SIZE];
	int total = RecenseEveryGroups(f, allGroups);

	for (int i = 0; i < total; i++) {
		InfoGroupe aG = allGroups[i];
		if (aG.material == VILLAGE && aG.taille > second) {
			if (aG.taille > bigger) {
				second = bigger;
				bigger = aG.taille;
			}
			else second = aG.taille;
		}
	}
	return second * 2;

}

// Map

int calcBorderlands(FeuilleCarte f) {
	int somme = 0;
	int full = 1;

	for (int i = 0; i < SIZE; i++) {
		full = 1;
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == 0) {
				full = 0;
				break;
			}
		}
		if (full) somme++;
	}
	for (int i = 0; i < SIZE; i++) {
		full = 1;
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, j, i) == 0) {
				full = 0;
				break;
			}
		}
		if (full) somme++;
	}


	return somme * 6;
}


int calcBrokenRoad(FeuilleCarte f) {
	int somme = 0;
	int valid;

	for (int i = 0; i < SIZE; i++) {
		valid = 1;
		for (int j = 0; j <= i; j++) {
			if (getMaterialAt(f, j, SIZE - i - 1 + j) == 0) {
				valid = 0;
			}
		}
		if (valid) somme++;
	}
	return somme * 3;
}


int calcLostBarony(FeuilleCarte f) {
	int length = 2;
	for (int i = 0; i <= SIZE - length; i++) {
		for (int j = 0; j <= SIZE - length; j++) {
			if (IsASquare(f, i, j, length)) {
				length++;
				j--;
			}
		}
	}
	return (length - 1) * 3;
}

int IsASquare(FeuilleCarte f, int x, int y, int length) {
	for (int i = x; i < x + length; i++) {
		for (int j = y; j < y + length; j++) {
			if (getMaterialAt(f, i, j) == 0) return FALSE;
		}
	}
	return TRUE;
}


int calcTheCauldrons(FeuilleCarte f) {
	int somme = 0;
	int isVoisin = 1;
	int valid = 1;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == 0) {
				valid = 1;
				Position d[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
				for (int k = 0; k < 4; k++) {
					isVoisin = getMaterialAt(f, i + d[k].x, j + d[k].y) != 0; // voisin ou bordure
					if (!isVoisin) {
						valid = 0;
						break;
					}
				}
				if (valid) somme++;
			}
		}
	}
	return somme;
}


/*décompte des points dus aux ennemis*/

int calcEnenmyPoints(FeuilleCarte f) {
	int somme = 0;
	int valid = 1;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == 0) {
				valid = 0;
				Position d[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
				for (int k = 0; k < 4; k++) {
					if (getMaterialAt(f, i + d[k].x, j + d[k].y) == MONSTRE) { // Si un enemy est voisin
						valid = 1;
						printf("Case vide en (%d,%d) perd un point a cause du monstre en (%d,%d)\n", i, j, i + d[k].x, j + d[k].y);
						break;
					}
				}
				if (valid) somme++;
			}
		}
	}
	printf("\npoints perdus par les ennemis : %d\n\n", somme);
	return somme;
}


// Calcluer le nombre de montagnes "complétées"
int countSurroundedMountains(FeuilleCarte f) {
	int count = 0;
	Position d[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (getMaterialAt(f, i, j) == MONTAGNE) {
				int isSurrounded = 1;
				for (int k = 0; k < 4; k++) {
					int mat = getMaterialAt(f, i + d[k].x, j + d[k].y);
					if (mat == 0) {
						isSurrounded = 0;
						break;
					}
				}
				if (isSurrounded) {
					count++;
				}
			}
		}
	}
	return count;
}




