#include "autocomplete.h"

// Create a New Trie Node
TrieNode *createNode() {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node) {
        node->isEndOfWord = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            node->children[i] = NULL;
    }
    return node;
}

// Insert a Word into the Trie
void insert(TrieNode *root, const char *word) {
    TrieNode *pCrawl = root;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = createNode();
        pCrawl = pCrawl->children[index];
        word++;
    }
    pCrawl->isEndOfWord = 1;
}

// Perform a Depth-First Search (DFS) to Get Suggestions
void dfs(TrieNode *node, char *prefix, int level) {
    if (node->isEndOfWord) {
        prefix[level] = '\0';
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[level] = 'a' + i;
            dfs(node->children[i], prefix, level + 1);
        }
    }
}

// Find and Print Autocomplete Suggestions
void autocomplete(TrieNode *root, const char *prefix) {
    TrieNode *pCrawl = root;
    while (*prefix) {
        int index = *prefix - 'a';
        if (!pCrawl->children[index]) {
            write(STDOUT_FILENO, "\a", 1);// bell sound
            return;
        }
        pCrawl = pCrawl->children[index];
        prefix++;
    }

    char buffer[100];
    strcpy(buffer, prefix);
    dfs(pCrawl, buffer, strlen(buffer));
    write(STDOUT_FILENO, buffer, 5); // Print other characters normally
    strcat(buffer, " ");
    strcat(prefix, buffer);
}
