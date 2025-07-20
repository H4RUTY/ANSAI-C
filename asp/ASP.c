# include <stdlib.h>
# include <stdio.h>
# include "Activity.h"
extern unsigned num;
extern ACT activity[];

int main() {
	sortActivity(activity, num);
	unsigned time = 0;
	int i = 0;
	while ( 1 ) {
		/* find a local optimal solution */
		if (i >= num) break;
		if (activity[i].start >= time) {
			/* print the local optimal solution */
			printf("%s\n", activity[i].name);
			time = activity[i].finish;
		}
		i++;
	}
}

void sortActivity(ACT act[], unsigned n) {
	// bubble sort
	int i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < (n - i - 1); j++) {
			if (act[j].finish > act[j + 1].finish) {
				ACT tmp = act[j];
                act[j] = act[j + 1];
                act[j + 1] = tmp;
			}
		}
	}
}

