#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Node {
    int num;
    int key[3];             // 一時的に3つ持てるように
    struct Node *parent;
    struct Node *child[4];  // 一時的に4つ持てるように
    bool isleaf;
} Node;

Node* createNode(int key, Node* parent) {
    Node* node = calloc(1, sizeof(Node));
    if (!node) exit(1);
    node->num = 1;
    node->key[0] = key;
    node -> parent = parent;
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
    else if (parent->num == 1 || mid < parent->key[1])
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
    if (parent->num == 3) {
        fix(root, parent);
    }
}

Node* insert(Node* root, int key) {
    if(!root)
        return createNode(key, NULL);

    Node* current = root;
    while (current->isleaf == false) {
        Node* next;
        if (key < current->key[0])
            next = current->child[0];
        else if (current->num == 1 || key < current->key[1])
            next = current->child[1];
        else
            next = current->child[2];
        
        if(next == NULL) break;
        current = next;
    }
    // 重複チェック
    for (int i = 0; i < current->num; i++)
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

int getHeight(Node* root) {
    int height = 0;
    Node* now = root;
    while(now->isleaf == false) {
        now = now->child[0];
        height++;
    }
    return height;
}

typedef struct Data {
    int key0;
    int key1;
    struct Data* next;
} Data;

void printNode(int key0, int key1) {
    printf("[");
    if(key0 == 0) printf(" *");
    else printf("%2d", key0);
    printf(",");
    if(key1 == 0) printf(" *");
    else printf("%2d", key1);
    printf("]");
}

void printTree(Node* current, int height) {
    if (current == NULL) return;
    printTree(current->child[0], height - 1);
    printf("height:[%d], key:[%d]\n", height, current->key[0]);
    
    printTree(current->child[1], height - 1);
    printf("height:[%d], key:[%d]\n", height, current->key[1]);

    printTree(current->child[2], height - 1);
}

int main() {
    int keys[10] = {3, 8, 14, 20, 9, 10, 37, 2, 1, 16};
    Node* root = NULL;
    for(int i = 0; i < 10; i++) 
        root = insert(root, keys[i]);
    
    int height = getHeight(root);
    printTree(root, height);
    
    putchar('\n');
    return 0;
}