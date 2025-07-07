#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node* n) {
    return n ? n->height : 0;   // if (n!=NULL) return n->height; else return 0;
}

int getBalance(Node* n) {
    return n ? (height(n->left) - height(n->right)) : 0;
}

Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    // update height
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    // update height
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node* insert(Node* node, int key) {
    if (!node)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;  // duplicate

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node); // balance factor

    if (balance > 1 && key < node->left->key) {
        return rotateRight(node);
    }
    if (balance < -1 && key > node->right->key) {
        return rotateLeft(node);
    }
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

Node* search(Node* node, int key) {
    if (!node) {
        printf("key %d not found.\n", key);
        return NULL;
    }
    if (key < node->key)
        return search(node->left, key);
    else if (key > node->key)
        return search(node->right, key);
    else {
        printf("key %d found.\n", key);
        return node;
    }
}

Node* minValueNode(Node *node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* removeNode(Node* node, int key) {
    if (!node) {
        printf("key %d not found.\n", key);
        return NULL;
    }
    if (key < node->key)
        node->left = removeNode(node->left, key);
    else if (key > node->key)
        node->right = removeNode(node->right, key);

    // found
    else {
        // 子が1つ以下
        if (node->left == NULL || node->right == NULL) {
            Node* temp = node->left ? node->left : node->right;
            free(node);
            return temp;
        }
        // 子が2つ → 右部分木から最小値を探して置き換える
        Node* temp = minValueNode(node->right);
        node->key = temp->key;
        node->right = removeNode(node->right, temp->key);
    }

    // ノードがNULLになった場合（葉だった場合など）そのまま返す
    if (!node)
        return NULL;

    // 高さの更新
    node->height = 1 + max(height(node->left), height(node->right));

    // バランスの取得
    int balance = getBalance(node);

    // バランス調整（4パターン）

    // 左左
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    // 左右
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // 右右
    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    // 右左
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// メモリ解放
void freeTree(Node* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

void printTree(Node* root, int space) {
    if (root == NULL)
        return;

    const int indent = 5; // インデント幅

    // 右部分木を先に表示（上側）
    printTree(root->right, space + indent);

    // 現在のノードを表示
    for (int i = 0; i < space; i++)
        printf(" ");
    printf("%d\n", root->key);

    // 左部分木を表示（下側）
    printTree(root->left, space + indent);
}

int main() {
    Node* root = NULL;

    // insert
    printf("--insert test--\n");
    int insertKeys[] = {7, 4, 3, 1, 5, 2};
    for (int i = 0; i < 6; i++) {
        printf("insert %d:\n", insertKeys[i]);
        root = insert(root, insertKeys[i]);
        printTree(root, 0);
        putchar('\n');
    }

    // search
    printf("--search test--\n");
    int searchKeys[] = {3, 6};
    for (int i = 0; i < 2; i++) {
        search(root, searchKeys[i]);
    }
    putchar('\n');

    // delete
    printf("--delete test--\n");
    int deleteKeys[] = {2, 4, 7, 3, 5, 1};
    for (int i = 0; i < 6; i++) {
        printf("delete %d:\n", deleteKeys[i]);
        root = removeNode(root, deleteKeys[i]);
        printTree(root, 0);
        putchar('\n');
    }
    
    return 0;
}