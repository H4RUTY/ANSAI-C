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

int maxSubarray(unsigned from, unsigned num) {
    /* stop recursive call */
    if (num == 1) return data[from];
    if (num == 2) return max_3(data[from], data[from+1], data[from]+data[from+1]);

    /* divide */
    unsigned mid = num / 2;
    unsigned numA1 = mid;
    unsigned fromA1 = from;
    unsigned numA2 = num - mid;
    unsigned fromA2 = from + mid;

    /* conquer */
    int max1 = maxSubarray(fromA1, numA1);
    int max2 = maxSubarray(fromA2, numA2);
    
    int max3L = data[from + mid - 1];
    int sum = 0;
    for (int i = from + mid - 1; i >= (int)from; i--) {
        sum += data[i];
        if (sum > max3L) max3L = sum;
    }

    int max3R = data[from + mid];
    sum = 0;
    for (int i = from + mid; i < from + num; i++) {
        sum += data[i];
        if (sum > max3R) max3R = sum;
    }

    int max3 = max3L + max3R;

    /* combine */
    return max_3(max1, max2, max3);
}