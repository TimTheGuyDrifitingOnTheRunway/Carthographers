#include "Lib3d.h"

void GUIDrawFeuille(FeuilleCarte f) {

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (f[i][j] != 0) {
                float x = i - SIZE / 2.0f + 0.5f;
                float z = j - SIZE / 2.0f + 0.5f;
                float y = 0.0f;   // Half height so cube sits on grid

                DrawCube((Vector3) { x, y, z }, 1.0f, 1.0f, 1.0f, GREEN);
                DrawCubeWires((Vector3) { x, y, z }, 1.0f, 1.0f, 1.0f, MAROON);
            }
        }
    }
}