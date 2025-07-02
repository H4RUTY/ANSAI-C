#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Node {
    int num;
    int key[2];
    struct Node *parent;
    struct Node *child[3];
    bool isleaf;
} Node;

Node* createNode(int key, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->num = 1;
    node->key[0] = key;
    node->key[1]  = 0;
    node -> parent = parent;
    for(int i = 0; i < 3; i++) node->child[i] = NULL;
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
    Node* parent = current->parent;
    
    // 根の時
    if(parent == NULL) {
        *root = current;
        return;
    }
    
    // 親のキーが1つの時:挿入してreturn
    if(parent->num == 1) {
        if(current->key[0] < parent->key[0]) {
            /*    parent->[a]    ->  parent->[b|a]
             *            / \    ->         /  |  \
             * current->[b] [c]  ->       [d] [e] [c]
             *          / \      ->
             *        [d] [e]    ->
             */
            parent->num = 2;
            parent->key[1] = parent->key[0];
            parent->key[0] = current->key[0];
            parent->child[2] = parent->child[1];
            parent->child[1] = current->child[1];
            parent->child[0] = current->child[0];
            parent->child[1]->parent = parent;
            parent->child[0]->parent = parent;
            return;
        }
        else {
            /* parent->[a]             ->  parent->[a|c]
             *        /  \             ->         /  |  \
             *       [b] [c]<-current  ->       [b] [d] [e]
             *           / \           ->
             *         [d] [e]         ->
             */
            parent->num = 2;
            parent->key[1] = current->key[0];
            parent->child[2] = current->child[1];
            parent->child[1] = current->child[0];
            parent->child[2]->parent = parent;
            parent->child[1]->parent = parent;
            return;
        }
    }

    // 親のキーが2つの時:ノードを追加して再帰
    else {
        if(current->key[0] < parent->key[0]) {
            /*     parent->[a|b]     ->       parent->[a]
             *            /  |  \    ->              /   \
             * current->[c] [d] [e]  ->  current->[c]     [b]<-newnode
             *          / \          ->           / \     / \      
             *        [f] [g]        ->         [f] [g] [d] [e]
             */
            Node* newnode = createNode(parent->key[1], parent);
            newnode->child[1] = parent->child[2];
            newnode->child[0] = parent->child[1];
            newnode->child[1]->parent = newnode;
            newnode->child[0]->parent = newnode;
            newnode->isleaf = false;
            
            parent->num = 1;
            parent->key[1] = 0;
            parent->child[2] = NULL;
            parent->child[1] = newnode;

            fix(root, parent);
        }
        else if(parent->key[0] < current->key[0]) {
            /*       parent->[a|b]      ->       parent->[d]
             *     /           |   \    ->              /   \
             *  [c]  current->[d]  [e]  ->  current->[a]     [b]<-newnode
             *                / \       ->           / \     / \      
             *              [f] [g]     ->         [c] [f] [g] [e]
             */
            Node* newnode = createNode(parent->key[1], parent);
            newnode->child[1] = parent->child[2];
            newnode->child[0] = current->child[1];
            newnode->child[1]->parent = newnode;
            newnode->child[0]->parent = newnode;
            newnode->isleaf = false;

            int tmp = current->key[0];
            current->key[0] = parent->key[0];
            current->child[1] = current->child[0];
            current->child[0] = parent->child[0];
            current->child[0]->parent = current;
            
            parent->num = 1;
            parent->key[1] = 0;
            parent->key[0] = tmp;
            parent->child[2] = NULL;
            parent->child[1] = newnode;
            parent->child[0] = current;
            
            fix(root, parent);
        }
        else {
            /* parent->[a|b]              ->       parent->[b]
             *        /  |  \             ->              /   \
             *      [c] [d] [e]<-current  ->  newnode->[a]     [e]<-current
             *              / \           ->           / \     / \      
             *            [f] [g]         ->         [c] [d] [f] [g]
             */
            Node* newnode = createNode(parent->key[0], parent);
            newnode->child[1] = parent->child[1];
            newnode->child[0] = parent->child[0];
            newnode->child[1]->parent = newnode;
            newnode->child[0]->parent = newnode;
            newnode->isleaf = false;
            
            parent->num = 1;
            parent->key[1] = 0;
            parent->child[2] = NULL;
            parent->child[1] = current;
            parent->child[0] = newnode;

            fix(root, parent);
        }
    }


}

Node* insert(Node* root, int key) {
    if(!root)
        return createNode(key, NULL);

    Node* current = root;
    while (current->isleaf == false) {
        if (key < current->key[0])
            current = current->child[0];
        else if (current->key[0] < key && key < current->key[1])
            current = current->child[1];
        else if (current->key[1] < key)
            current = current->child[2];
        else // 重複
            return root;
    }
    
    // 葉のキーが1つの時:挿入
    if(current->num == 1) {
        current->num = 2;
        if(key < current->key[0]) {
            current->key[1] = current->key[0];
            current->key[0] = key;
        }
        else
            current->key[1] = key;
        return root;
    }
    /* 葉のキーが2つの時: 下の形を作ってfix()
     *   [ ]
     *   / \
     * [ ] [ ]
     */
    else {
        int keys[3];
        sortKeys(current->key[0], current->key[1], key, keys);
        Node* left = createNode(keys[0], current);  // isleaf = true;
        Node* right = createNode(keys[2], current); // isleaf = true;
        current->num = 1;
        current->key[0] = keys[1];
        current->key[1] = 0;
        current->child[0] = left;
        current->child[1] = right;
        current->isleaf = false;

        fix(&root, current);
        
        return root;
    }
}

void printTree(Node* current) {
    if (current == NULL) return;
    
    printTree(current->child[0]);
    printf("%d ", current->key[0]);
    printTree(current->child[1]);
    printf("%d ", current->key[1]);
    printTree(current->child[2]);
}

int main() {
    int keys[10] = {3, 8, 14, 20, 9, 10, 37, 2, 1, 16};
    Node* root = NULL;
    for(int i = 0; i < 10; i++) {
        root = insert(root, keys[i]);
    }
    printTree(root);

    return 0;
}