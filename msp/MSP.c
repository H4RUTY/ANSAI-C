#include <stdlib.h>
#include <stdio.h>

#include "MSP.h"

/* input data */
extern unsigned num;
extern int data[];

int main() {
    printf("%d\n", maxSubarray(0, num));
}

int max_3(int a, int b, int c) {
    int max = a;
    if(b > max) max = b;
    if(c > max) max = c;
    return max;
}

int max_array(int* array, int num) {
    int max = 0;
    for (int i = 0; i < num; i++) {
        if (max < array[i]) max = array[i];
    }
    return max;
}

int maxSubarray(unsigned from, unsigned num) {
    /* stop recursive call */
    if (num == 1) return data[from];
    if (num == 2) return max_3(data[from], data[from+1], data[from]+data[from+1]);

    /* divide */
    unsigned mid = num / 2;
    unsigned numA1 = mid;
    int fromA1 = from;
    unsigned numA2 = num - numA1;
    int fromA2 = from + numA1;

    /* conquer */
    int max1 = maxSubarray(fromA1, numA1);
    int max2 = maxSubarray(fromA2, numA2);
    
    int array3L[numA1];
    for (int i = 0; i < numA1; i++) array3L[i] = 0;
    for (int i = from; i < mid; i++) {
        for (int j = i; j < from + mid; j++) {
            array3L[i - from] += data[j];
        }
    }
    int max3L = max_array(array3L, numA1);

    int array3R[numA2];
    for (int i = 0; i < numA2; i++) array3R[i] = 0;
    for (int i = mid; i < num-1; i++) { // 2 3 4
        for (int j = mid; j <= i; j++) {
            array3R[num-i-1] += data[j];
        }
    }
    int max3R = max_array(array3R, numA2);

    int max3 = max3L + max3R;

    /* combine */
    int max = max_3(max1, max2, max3);
    return max;
}