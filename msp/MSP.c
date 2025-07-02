#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "MSP.h"

/* input data */
extern unsigned num;
extern int data[];

int maxSubarray(unsigned from, unsigned num);

int main() {
    printf("%d\n", maxSubarray(0, num));
}

int maxSubarray(unsigned from, unsigned num) {
    if (num == 1) {
        return data[from];
    } else if (num == 2) {
        int maxdata = data[from];
        if (data[from + 1] > maxdata)
            maxdata = data[from + 1];
        if (data[from] + data[from + 1] > maxdata)
            maxdata = data[from] + data[from + 1];
        return maxdata;
    } else {
        unsigned mid = num / 2;

        int max1 = maxSubarray(from, mid);
        int max2 = maxSubarray(from + mid, num - mid);

        int max3L = INT_MIN;
        int sumL = 0;
        for (int i = (int)(from + mid) - 1; i >= (int)from; i--) {
            sumL += data[i];
            if (sumL > max3L) max3L = sumL;
        }

        int max3R = INT_MIN;
        int sumR = 0;
        for (unsigned i = from + mid; i < from + num; i++) {
            sumR += data[i];
            if (sumR > max3R) max3R = sumR;
        }

        int max3 = max3L + max3R;

        int max = max1;
        if (max2 > max) max = max2;
        if (max3 > max) max = max3;

        return max;
    }
}
