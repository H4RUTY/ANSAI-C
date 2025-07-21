#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum {タテ, ヨコ, ナナメ} Vector;
typedef struct Data {
    int value;
    Vector vector;
} Data;

void to1base(char* src, char* dst) {
    int len = strlen(src);
    dst[0] = 0; // dummy
    for (int i = 0; i < len; i++) dst[i+1] = src[i];
    dst[len+1] = '\0';
}

void lcs(char *x0, char *y0){
    int lx0 = (int)strlen(x0);
    int ly0 = (int)strlen(y0);

    char x[lx0 + 2], y[ly0 + 2];

    to1base(x0, x);
    to1base(y0, y);
    
    int lx = lx0 + 1;
    int ly = ly0 + 1;
    Data c[lx][ly];

    // initialize
    for (int i = 0; i < lx; i++) c[i][0].value = 0;
    for (int j = 0; j < ly; j++) c[0][j].value = 0;

    for (int i = 1; i < lx; i++) {
        for (int j = 1; j < ly; j++) {
            if (x[i] == y[j]) {
                c[i][j].value = c[i-1][j-1].value + 1;
                c[i][j].vector = ナナメ;
            }
            else if (c[i][j-1].value > c[i-1][j].value) {
                c[i][j].value = c[i][j-1].value;
                c[i][j].vector = ヨコ;
            }
            else {
                c[i][j].value = c[i-1][j].value;
                c[i][j].vector = タテ;
            }
        }
    }

    // search common words
    bool common_x[lx];
    for (int k = 0; k < lx; k++) common_x[k] = false; // init

    int i = lx - 1, j = ly - 1;
    while(i > 0 && j > 0) {
        if (c[i][j].vector == ナナメ) {
            common_x[i] = true;
            i--; j--;
        }
        else if (c[i][j].vector == ヨコ) j--;
        else i--;
    }

    // print
    printf("再長共通部分:\n");
    for (int i = 1; i < lx; i++) {
        if (common_x[i]) printf("%c", x[i]);
    }
    printf("\n");

    return;
}

int main() {
    char x[] = "if (flag == 1) doSomething();";
    char y[] = "if (flag) doAnything();";

    lcs(x,y);

    return 0;
}