#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int num;
    int key[3];             // 一時的に3つ持てるように
    struct Node *parent;
    struct Node *child[4];  // 一時的に4つ持てるように
    bool isleaf;
} Node;

Node* createNode(int key, Node* parent) {
    Node* node = calloc(1, sizeof(Node));
    if(!node) exit(1);
    node->num = 1;
    node->key[0] = key;
    node->key[1] = node->key[2] = 0;
    node -> parent = parent;
    node->child[0] = node->child[1] = node->child[2] = node->child[3] = NULL;
    node->isleaf = true;
    return node;
}

void sortKeys(int a, int b, int c, int* out) {
    int arr[3] = {a, b, c};
    for(int i = 0; i < 2; i++) 
        for(int j = i + 1; j < 3; j++)
            if(arr[i] > arr[j]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
    out[0] = arr[0];
    out[1] = arr[1];
    out[2] = arr[2];
}

// キーが2つの場所に挿入する
// 二分木にした状態で渡す(current)
void fix(Node** root, Node* current) {
    int K[3] = {current->key[0], current->key[1], current->key[2]};
    Node* C[4] = {current->child[0], current->child[1], current->child[2], current->child[3]};
    int mid = K[1];
    
    // 左右ノード生成
    Node* left = createNode(K[0], current->parent);
    Node* right = createNode(K[2], current->parent);
    left->isleaf = right->isleaf = current->isleaf;

    /* 子をleftとrightに分配
     *       [K0|K1|K2]       ->  left->[K0]  right->[K2]
     *      /   /  \   \      ->        /  \         /  \
     *  [C0] [C1]  [C2] [C3]  ->     [C0]  [C1]   [C2]  [C3]
     */
    left->child[0] = C[0];  if(C[0]) C[0]->parent = left;
    left->child[1]  = C[1]; if(C[1]) C[1]->parent = left;
    right->child[0] = C[2]; if(C[2]) C[2]->parent = right;
    right->child[1] = C[3]; if(C[3]) C[3]->parent = right;

    // 根である場合, 根を更新
    if(current->parent == NULL) {
        Node* newRoot = createNode(mid, NULL);
        newRoot->child[0] = left;   left->parent = newRoot;
        newRoot->child[1] = right;  right->parent = newRoot;
        newRoot->isleaf = false;
        *root = newRoot;
        return;
    }
    
    // 親にmidを挿入
    Node* parent = current->parent;
    int index;
    if(mid < parent->key[0])
        index = 0;
    else if(parent->num == 1 || mid < parent->key[1])
        index = 1;
    else
        index = 2;
    
    // parentのkeyとchildをスライド
    for(int i = parent->num; i > index; i--) {
        parent->key[i] = parent->key[i-1];
        parent->child[i+1] = parent->child[i];
    }
    parent->key[index] = mid;
    parent->child[index] = left;
    parent->child[index+1] = right;
    parent->num++;

    // 親もoverflow -> 再帰
    if(parent->num == 3) {
        fix(root, parent);
    }
}

Node* insert(Node* root, int key) {
    if(!root)
        return createNode(key, NULL);

    Node* current = root;
    while(current->isleaf == false) {
        Node* next;
        if(key < current->key[0])
            next = current->child[0];
        else if(current->num == 1 || key < current->key[1])
            next = current->child[1];
        else
            next = current->child[2];
        
        if(next == NULL) break;
        current = next;
    }
    // 重複チェック
    for(int i = 0; i < current->num; i++)
        if(current->key[i] == key) return root;

    // 葉に挿入
    if(current->num == 1) {
        // 1 to 2
        if(key < current->key[0]) {
            current->key[1] = current->key[0];
            current->key[0] = key;
        }
        else
            current->key[1] = key;
        current->num = 2;
        return root;
    }
    // 2 to 3
    current->num = 3;
    int tmp[3] = {current->key[0], current->key[1], key};
    sortKeys(tmp[0], tmp[1], tmp[2], tmp);
    current->key[0] = tmp[0];
    current->key[1] = tmp[1];
    current->key[2] = tmp[2];
    
    fix(&root, current);
    return root;
}

typedef struct QNode {
    Node* treeNode;
    struct QNode* next;
} QNode;

typedef struct Queue {
    QNode *head, *tail;
    int size;
} Queue;

void initQueue(Queue* q) {
    q->head = q->tail = NULL;
    q->size = 0;
}

void enqueue(Queue* q, Node* node) {
    QNode* qn = malloc(sizeof(QNode));
    qn->treeNode = node;
    qn->next = NULL;
    if(q->tail == NULL) q->head = qn;
    else                q->tail->next = qn;
    q->tail = qn;
    q->size++;
}

Node* dequeue(Queue* q) {
    if(q->head == NULL) return NULL;
    QNode* old = q->head;
    Node* ret = old->treeNode;
    q->head = old->next;
    if(!q->head) q->tail = NULL;
    free(old);
    q->size--;
    return ret;
}

// 木全体をレベルごとに出力
void printTree(Node* root) {
    if(!root) return;
    
    Queue currLevel, nextLevel;
    initQueue(&currLevel);
    initQueue(&nextLevel);
    enqueue(&currLevel, root);

    while(currLevel.size > 0) {
        int count = currLevel.size;
        for(int i = 0; i < count; i++) {
            Node* n = dequeue(&currLevel);
            
            // 1キーの場合
            if(n->num == 1) {
                printf("[%d] ", n->key[0]);
            }
            // 2キーの場合
            else {
                printf("[%d, %d] ", n->key[0], n->key[1]);
            }
            // 子ノードを次レベルへ enqueue
            if(!n->isleaf) {
                for(int c = 0; c <= n->num; c++) {
                    if(n->child[c])
                        enqueue(&nextLevel, n->child[c]);
                }
            }
        }
        putchar('\n');
        // レベルを進める
        currLevel = nextLevel;
        initQueue(&nextLevel);
    }
}

int* oddRandArray(int num) {
    int* keys = malloc(sizeof(int) * num);
    
    // 奇数配列
    for(int i = 0, odd = 1; i < num; i++, odd += 2) {
        keys[i] = odd;
    }

    // ランダム入れ替え
    srand((unsigned)time(NULL));
    for(int i = 0; i < num; i++) {
        int randNum = rand() % num; // 0 ~ (num-1) の範囲で乱数生成
        int tmp = keys[i];
        keys[i] = keys[randNum];
        keys[randNum] = tmp;
    }
    return keys;
}

Node* searchTree(Node* root, int key) {
    Node* current = root;
    while(current != NULL) {
        for(int i = 0; i < current->num; i++) {
            if(current->key[i] == key)
                return current;
        }
        if(current->isleaf == true) break;
        
        // 下降⤵️
        if(key < current->key[0])
            current = current->child[0];
        else if(current->num == 1 || key < current-> key[1])
            current = current->child[1];
        else
            current = current->child[2];
    }
    return NULL;
}

int main() {
    int num = 11;
    int* keys = oddRandArray(num);
    Node* root = NULL;
    
    // insert test
    for(int i = 0; i < num; i++) {
        printf("--insert %d--\n", keys[i]);
        root = insert(root, keys[i]);
        printTree(root);
        putchar('\n');
    }
    
    // search test
    int targets[] = {9, 18};
    for(int t = 0; t < 2; t++) {
        int key = targets[t];
        Node* result = searchTree(root, key);
        if(result) {
            printf("Key %d found in node [", key);
            for (int i = 0; i < result->num; i++) {
                printf("%d", result->key[i]);
                if (i + 1 < result->num) printf(", ");
            }
            printf("].\n");
        }
        else
            printf("Key %d not found.\n", key);
    }

    free(keys);
    return 0;
}