/* test_src.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlist_queue.h"
#include "stacks.h"
#include "binarySearchTrees.h"

/* visitor to print a tree node */
void printTreeNode(TreeNode *n) {
    printf("%-10s (syn: %-10s ant: %-10s chars: %2d vowels: %d)\n",
           n->word, n->syn, n->ant, n->num_chars, n->num_vowels);
}

int main(void) {
    /* 1. Build a BST directly from a file */
    const char *fname = "words.txt";
    FILE *f = fopen(fname, "r");
    if (!f) {
        perror("Cannot open words.txt");
        return EXIT_FAILURE;
    }
    TTree *fileTree = fillTree(f);
    fclose(f);
    printf("Loaded tree from file (%d nodes):\n", fileTree->size);
    TraversalBSTinOrder(fileTree, printTreeNode);

    /* 2. Push same words onto a stack */
    TStack *stk = initStack();
    // For demo, hard-code a few pushes (word, synonym, antonym)
    push(stk, "happy",   "joyful",   "sad");
    push(stk, "bright",  "luminous", "dim");
    push(stk, "quick",   "fast",     "slow");
    push(stk, "level",   "even",     "uneven"); // palindrome test
    printf("\nStack contents (top→bottom):\n");
    for (SNode *p = stk->top; p; p = p->next)
        printf("  %s (syn=%s ant=%s)\n", p->word, p->syn, p->ant);

    /* 3. Convert stack → BST */
    TTree *stkTree = toTree(stk);
    printf("\nTree from stack (%d nodes) in pre-order:\n", stkTree->size);
    TraversalBSTpreOrder(stkTree, printTreeNode);

    /* 4. Lookup a word */
    const char *lookup = "quick";
    TreeNode *found = getInfWordTree(stkTree, lookup);
    if (found) {
        printf("\nLookup '%s': syn=%s, ant=%s, chars=%d, vowels=%d\n",
               lookup, found->syn, found->ant, found->num_chars, found->num_vowels);
    }

    /* 5. Lowest Common Ancestor */
    TreeNode *lca = LowestCommonAncestor(stkTree, "bright", "quick");
    if (lca) {
        printf("\nLCA of 'bright' & 'quick' is '%s'\n", lca->word);
    }

    /* 6. In-order successor */
    TreeNode *succ = inOrderSuccessor(stkTree, "happy");
    if (succ) {
        printf("In-order successor of 'happy' is '%s'\n", succ->word);
    }

    /* 7. Size & Height & Balance */
    printf("\nTree metrics for stack→tree:\n");
    HighSizeBST(stkTree);
    printf("Balanced? %s\n", isBalancedBST(stkTree) ? "yes" : "no");

    /* 8. Mirror & show in-order */
    BSTMirror(stkTree);
    printf("\nAfter mirror (in-order):\n");
    TraversalBSTinOrder(stkTree, printTreeNode);

    /* 9. Update a word and delete another */
    UpdateWordBST(stkTree, "level", "plane", "uneven");
    deleteWordBST(stkTree, "quick");
    printf("\nAfter update('level') and delete('quick') (in-order):\n");
    TraversalBSTinOrder(stkTree, printTreeNode);

    return EXIT_SUCCESS;
}
