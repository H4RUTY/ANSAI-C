#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LENGTH 20

typedef struct Node {
    struct Node* parent;    // 親
    struct Node* next[26];  // 子
    int branch;              // ノード下の枝
    bool isEnd;             // 葉
} Node;

Node* createNode(Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->parent = parent;
    for(int i = 0; i < 26; i++) node->next[i] = NULL;
    node->branch = 0;
    node->isEnd = false;
    return node;
}

int alphIndex(char alph) {
    int ascii = alph;   // alphabetのasciiコードを得る
    return ascii - 97;  // 'a'のasciiコードは97なので, aならindex=0
}

Node* insert(Node* root, char* word) {
    if (!root) root = createNode(NULL);

    Node* current = root;
    for(int i = 0; i < strlen(word); i++) {
        int index = alphIndex(word[i]);
        if (current->next[index] == NULL) {
            current->next[index] = createNode(current);
            current->branch++;
        }
        current = current->next[index];
    }
    current->isEnd = true;
    
    return root;
}

bool search(Node* root, char* word) {
    for(int i = 0; i < strlen(word); i++) {
        if (!root) return false;
        
        int index = alphIndex(word[i]);
        if (root->next[index] == NULL)
            return false;
        root = root->next[index];
    }
    if (root->isEnd == true) return true;
    else return false;
}

void printWords(Node* root, char* buffer, int depth) {
    if (!root) return;

    if (root->isEnd) {
        buffer[depth] = '\0';
        printf("    %s\n", buffer);
    }

    for (int i = 0; i < 26; i++) {
        if (root->next[i]) {
            buffer[depth] = 'a' + i;
            printWords(root->next[i], buffer, depth + 1);
        }
    }
}

Node* delete(Node* root, char* word) {
    if(!root) {
        printf("'%s' not found.\n", word);
        return root;
    }

    Node* current = root;

    for(int i = 0; i < strlen(word); i++) {
        int index = alphIndex(word[i]);
        
        // 見つからなかったら終了
        if (current->next[index] == NULL) {
            printf("'%s' not found.\n", word);
            return root;
        }

        current = current->next[index];
    }

    if (!current->isEnd) {
        printf("'%s' not found.\n", word);
            return root;
    }
    
    // ここから削除の始まり👇
    current->isEnd = false;

    // ⚠️末端が葉でない場合は削除しない
    for (int i = 0; i < 26; i++) {
        if(current->next[i] != NULL) return root; // 1個でも子があるなら削除できない
    }
    
    // 子がないなら、上に遡って削除
    for (int i = strlen(word) - 1; i >= 0; i--) {
        Node* parent = current->parent;
        int index = alphIndex(word[i]);

        if(parent) {
            parent->next[index] = NULL;
            parent->branch--;
        }

        free(current);
        
        if(!parent || parent->isEnd || parent->branch > 0) break;
            
        current = parent;
    }

    return root;
}

int main(void) {
    Node* root = NULL;
    char buffer[MAX_LENGTH];

    // insert
    printf("-- insert test --\n");
    char* insertData[] = {"man", "mane", "dry", "dried", "mat"};
    int i_count = sizeof(insertData) / sizeof(insertData[0]);
    
    for(int i = 0; i < i_count; i++)
        root = insert(root, insertData[i]);

    printWords(root, buffer, 0);
    putchar('\n');

    // search
    printf("-- search test --\n");
    char* searchData[] = {"mane", "many", "drie"};
    int s_count = sizeof(searchData) / sizeof(searchData[0]);

    for(int i = 0; i < s_count; i++) {
        if (search(root, searchData[i])) printf("'%s' found.\n", searchData[i]);
        else printf("'%s' not found.\n", searchData[i]);
    }
    putchar('\n');

    // delete
    printf("-- delete test --\n");
    char* deleteData[] = {"dried", "mat", "dry", "man", "mane"};
    int d_count = sizeof(deleteData) / sizeof(deleteData[0]);

    for(int i = 0; i < d_count; i++) {
        root = delete(root, deleteData[i]);
        printf("delete '%s' :\n", deleteData[i]);
        printWords(root, buffer, 0);
        putchar('\n');
    }

    return 0;
}