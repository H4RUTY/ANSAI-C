# include <stdlib.h>
# include <stdio.h>
# include "Heap.h"
# include "Dijkstra.h"
extern unsigned dist[NMAX];

void changeHeap(HEAP *heap, unsigned pos) {
    while (pos > 0) {
        unsigned parent = (pos - 1) / 2;
        if (dist[heap->val[parent]] <= dist[heap->val[pos]]) break;
        // ヒープ条件を満たさない場合、交換
        unsigned tmp = heap->val[parent];
        heap->val[parent] = heap->val[pos];
        heap->val[pos] = tmp;
        pos = parent;
    }
}

static void heapify(HEAP *heap, unsigned pos) {
    unsigned smallest = pos;
    while (1) {
        unsigned left = 2 * pos + 1;
        unsigned right = 2 * pos + 2;

        if (left < heap->num && dist[heap->val[left]] < dist[heap->val[smallest]]) {
            smallest = left;
        }
        if (right < heap->num && dist[heap->val[right]] < dist[heap->val[smallest]]) {
            smallest = right;
        }

        if (smallest == pos) break;

        // ヒープ条件を満たさないので交換
        unsigned tmp = heap->val[pos];
        heap->val[pos] = heap->val[smallest];
        heap->val[smallest] = tmp;

        pos = smallest;
    }
}

unsigned removeRoot(HEAP *heap) {
	unsigned root = heap->val[0];
	heap->val[0] = heap->val[-- heap->num];
	heapify(heap, 0);
	return root;
}

HEAP *newHeap(unsigned num) {
	HEAP *new;
	if ( (new = (HEAP *)malloc(sizeof(HEAP))) == NULL ) {
		perror("no more memory");
		exit(EXIT_FAILURE);
	}
	new->num = num;
	if ( (new->val = (unsigned *)calloc(num, sizeof(unsigned))) == NULL ) {
		perror("too large");
	}
	return new;
}

void freeHeap(HEAP *heap) {
	free(heap->val);
	free(heap);
}

