/* binarySearchTrees.h */
#ifndef BINARY_SEARCH_TREES_H
#define BINARY_SEARCH_TREES_H

#include "stacks.h"
#include <stdio.h>
#include <stdbool.h>

/* Tree node holding word data */
typedef struct TreeNode {
    char *word;
    char *syn;
    char *ant;
    int num_chars;
    int num_vowels;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

/* Tree wrapper */
typedef struct TTree {
    TreeNode *root;
    int size;
} TTree;

/* --- Function prototypes --- */

/* Build a BST from stack */
TTree *toTree(TStack *stk);

/* Build a BST by reading from file (word synonym antonym per line) */
TTree *fillTree(FILE *f);

/* Lookup in tree */
TreeNode *getInfWordTree(TTree *tr, const char *word);

/* Add, delete, update */
TTree *AddWordBST(TTree *tr, const char *word, const char *syne, const char *anton);
TTree *deleteWordBST(TTree *tr, const char *word);
TTree *UpdateWordBST(TTree *tr, const char *word, const char *syne, const char *anton);

/* Traversals */
void TraversalBSTinOrder(TTree *tr, void (*visit)(TreeNode*));
void TraversalBSTpreOrder(TTree *tr, void (*visit)(TreeNode*));
void TraversalBSTpostOrder(TTree *tr, void (*visit)(TreeNode*));

/* Utilities */
void HighSizeBST(TTree *tr);
TreeNode *LowestCommonAncestor(TTree *tr, const char *word1, const char *word2);
int CountNodesRanges(TTree *tr, int l, int h);
TreeNode *inOrderSuccessor(TTree *tr, const char *word);
TTree *BSTMirror(TTree *tr);
bool isBalancedBST(TTree *tr);
TTree *BSTMerge(TTree *tr1, TTree *tr2);

#endif /* BINARY_SEARCH_TREES_H */
