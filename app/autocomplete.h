#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie Node Structure
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;

TrieNode *createNode();
void autocomplete(TrieNode *root, const char *prefix);