// <制約条件>
// 豚鶏:x1 鶏特:x2 DX:x3
// 150x1 + 100x2 +  50x3 <= 7000
// 150x1 + 250x2 +  50x3 <= 8000
//                 150x3 <= 3000

// <目的関数>
// 850x1 + 950x2 + 1100x3 = z

// <初期化>
// z - 850x1 - 950x2 -1100x3                =    0
//     150x1 + 100x2 +  50x3 + x4           = 7000
//     150x1 + 250x2 +  50x3      + x5      = 8000
//                     150x3           + x6 = 3000

#include <stdio.h>
#include <float.h>
#include <math.h>
#define ROWS 4
#define COLS 7

double tableau[ROWS][COLS] = {
    //       x1    x2    x3   x4  x5  x6  const
    /*z*/  {-850, -950,-1100,  0,  0,  0,     0},
    /*x4*/ { 150,  100,   50,  1,  0,  0,  7000},
    /*x5*/ { 150,  250,   50,  0,  1,  0,  8000},
    /*x6*/ {   0,    0,  150,  0,  0,  1,  3000}
};

void printTableau() {
    printf("          x1        x2        x3        x4        x5        x6     const\n");
    for (int i = 0; i < ROWS; i++) {
        switch (i) {
        case 0:  printf(" z"); break;
        case 1:  printf("x4"); break;
        case 2:  printf("x5"); break;
        case 3:  printf("x6"); break;
        default: break;
        }
        for (int j = 0; j < COLS; j++) {
            printf("%10.2lf", tableau[i][j]);
        }
        putchar('\n');
    }
}

int findCol() {
    int pivotCol = 0;
    double min = 0;
    for (int i = 0; i < (COLS - 1); i++) {
        if (tableau[0][i] < min) {
            min = tableau[0][i];
            pivotCol = i;
        }
    }
    return (min < 0) ? pivotCol : -1;  // -1のとき: 終了
}

int findRow(int pivotCol) {
    int pivotRow = 1;
    double minDev = DBL_MAX;
    for (int i = 1; i < ROWS; i++) {
        // tableau[][6] ... 定数項
        double dev;
        if (tableau[i][pivotCol] == 0) dev = DBL_MAX;       // ゼロ除算のとき
        else dev = tableau[i][6] / tableau[i][pivotCol];
        if (minDev > dev) {
            minDev = dev;
            pivotRow = i;
        }
    }
    return pivotRow;
}

void rowReduction(int pivotRow, int pivotCol) {
    double pivotVal = tableau[pivotRow][pivotCol];
    // pivot row
    for (int i = 0; i < COLS; i++) {
        tableau[pivotRow][i] /= pivotVal;
    }
    // other row
    for (int i = 0; i < ROWS; i++) {
        if (i == pivotRow) continue;
        double factor = tableau[i][pivotCol];
        for (int j = 0; j < COLS; j++) {
            tableau[i][j] -= (tableau[pivotRow][j] * factor);
        }
    }
}

void symplex() {
    for (;;) {
        int pivotCol = findCol();
        if (pivotCol == -1) break;
        int pivotRow = findRow(pivotCol);
        
        rowReduction(pivotRow, pivotCol);
        printTableau();
        putchar('\n');
    }
    return;
}

int main(void) {
    printf("--initial tableau--\n");
    printTableau();
    putchar('\n');
    symplex();
    
    printf("-- optimal solution--\n");

    double x1 = floor(tableau[1][COLS-1]);
    double x2 = floor(tableau[2][COLS-1]);
    double x3 = floor(tableau[3][COLS-1]);

    printf("豚鶏Wつけ麺: %.0f個\n", x1);
    printf("鶏特盛つけ麺: %.0f個\n", x2);
    printf("DXつけ麺: %.0f個\n", x3);

    double maxPrft = 850 * x1 + 950 * x2 + 110 * x3;
    printf("最大利益: %.0f円\n", maxPrft);
    
    return 0;
}