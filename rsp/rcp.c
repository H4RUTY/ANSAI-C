#include <stdio.h>
#include <limits.h>

#define NUM 10
int table[3][NUM+1] = {
    {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30}, // price
    {0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0}, // maximun price
    {0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0}, // first cut
};

void cut() {
    for (int i = 1; i <= NUM; i++) {
        int max = 0;
        for (int j = 1; j <= i; j++) {
            if (max < table[0][j] + table[1][i-j]) {
                max = table[0][j] + table[1][i-j];
                table[2][i] = j;
            }
        }
        table[1][i] = max;
    }
}

void print() {
    printf("### result\n");
    printf("| n |");
    for (int i = 1; i <= NUM; i++) printf(" %d |", i);
    printf("\n");
    for (int i = 0; i <= NUM; i++) printf("| --- ");
    printf("|\n");

    printf("| 最大値 |");
    for (int i = 1; i <= NUM; i++) printf(" %d |", table[1][i]);
    printf("\n");
    
    printf("| 切り方 | ");
    for (int i = 1; i <= NUM; i++) {
        int length = i;
        int x = 0;
        while(length > 0) {
            if (x > 0) printf("+");
            printf("%d", table[2][length]);
            x++;
            length -= table[2][length];
        }
        printf(" | ");
    }
    printf("\n");
}

int main() {
    cut();
    print();
    return 0;
}