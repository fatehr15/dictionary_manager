/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "stacks.h"
#include "binarySearchTrees.h"
#include "recursion.h"

/* Print functions */
void printList(TList *L) {
    if (!L) { printf("List is empty.\n"); return; }
    printf("List (size=%d):\n", L->size);
    for (TNode *p = L->head; p; p = p->next)
        printf("  %s [%s] (chars=%d, vowels=%d)\n",
               p->word, p->syn_or_ant, p->num_chars, p->num_vowels);
}

void printStack(TStack *stk) {
    if (!stk) { printf("Stack is empty.\n"); return; }
    printf("Stack (size=%d):\n", stk->size);
    for (SNode *p = stk->top; p; p = p->next)
        printf("  %s (syn=%s, ant=%s, chars=%d, vowels=%d)\n",
               p->word, p->syn, p->ant, p->num_chars, p->num_vowels);
}

void printQueue(TQueue *Q) {
    if (!Q) { printf("Queue is empty.\n"); return; }
    printf("Queue (size=%d):\n", Q->size);
    QNode *n = Q->front;
    while (n) {
        printf("%s -> ", n->word);
        n = n->next;
    }
    printf("NULL\n");
}

void printTreeNode(TreeNode *n) {
    if (!n) return;
    printf("%-10s (syn=%-10s ant=%-10s chars=%2d vowels=%d)\n",
           n->word, n->syn, n->ant, n->num_chars, n->num_vowels);
}

int main(void) {
    TList *syn=NULL, *ant=NULL, *merged=NULL;
    TStack *stk=NULL;
    TQueue *que=NULL;
    TTree *tree=NULL;
    int choice;
    char word[256], sym[256], antw[256];
    char fname[256];

    do {
        printf("\n=== Data Management Menu ===\n");
        printf("1. Load synonym list from file\n");
        printf("2. Load antonym list from file\n");
        printf("3. Display synonym list\n");
        printf("4. Display antonym list\n");
        printf("5. Lookup word in lists\n");
        printf("6. Lookup by synonym/antonym in lists\n");
        printf("7. Merge lists\n");
        printf("8. Display merged list\n");
        printf("9. Convert merged list to stack\n");
        printf("10. Display stack\n");
        printf("11. Lookup in stack\n");
        printf("12. Sort stack alphabetically\n");
        printf("13. Delete word from stack\n");
        printf("14. Update word in stack\n");
        printf("15. Convert stack to queue\n");
        printf("16. Display queue\n");
        printf("17. Convert queue to list\n");
        printf("18. Convert stack to BST\n");
        printf("19. Display BST in-order\n");
        printf("20. Lookup word in BST\n");
        printf("21. Add word to BST\n");
        printf("22. Delete word from BST\n");
        printf("23. Update word in BST\n");
        printf("24. BST size, height, balance\n");
        printf("25. BST Lowest Common Ancestor\n");
        printf("26. BST In-order successor\n");
        printf("27. Mirror BST\n");
        printf("28. Merge two BSTs\n");
        printf("29. Count word occurrences in file (recursion)\n");
        printf("30. Remove word occurrences in file (recursion)\n");
        printf("31. Replace word occurrences in file (recursion)\n");
        printf("32. Word permutations (recursion)\n");
        printf("33. Print subsequences of word (recursion)\n");
        printf("34. Longest common subseq (recursion)\n");
        printf("35. Count distinct subseqs (recursion)\n");
        printf("36. Palindrome check (recursion)\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice)!=1) break;

        switch(choice) {
            case 1:
                printf("Filename: "); scanf("%s", fname);
                syn = getSynWords(fopen(fname,"r"));
                printf("Loaded %d synonyms.\n", syn->size);
                break;
            case 2:
                printf("Filename: "); scanf("%s", fname);
                ant = getAntoWords(fopen(fname,"r"));
                printf("Loaded %d antonyms.\n", ant->size);
                break;
            case 3: printList(syn); break;
            case 4: printList(ant); break;
            case 5:
                printf("Word: "); scanf("%s", word);
                getInfWord(syn, ant, word);
                break;
            case 6:
                printf("Syn/Ant: "); scanf("%s", sym);
                getInfWord2(syn, ant, sym);
                break;
            case 7:
                merged = merge(syn, ant);
                printf("Merged list size: %d\n", merged->size);
                break;
            case 8: printList(merged); break;
            case 9:
                stk = toStack(merged);
                printf("Stack created (size=%d).\n", stk->size);
                break;
            case 10: printStack(stk); break;
            case 11:
                printf("Word: "); scanf("%s", word);
                getInfWordStack(stk, word);
                break;
            case 12:
                stk = sortWordStack(stk);
                printf("Stack sorted alphabetically.\n");
                break;
            case 13:
                printf("Word: "); scanf("%s", word);
                stk = deleteWordStack(stk, word);
                printf("Deleted '%s'.\n", word);
                break;
            case 14:
                printf("Word: "); scanf("%s", word);
                printf("New syn: "); scanf("%s", sym);
                printf("New ant: "); scanf("%s", antw);
                stk = updateWordStack(stk, word, sym, antw);
                printf("Updated '%s'.\n", word);
                break;
            case 15:
                que = stackToQueue(stk);
                printf("Queue created (size=%d).\n", que->size);
                break;
            case 16: printQueue(que); break;
            case 17: {
                TList *l2 = init_list();
                for (QNode *n=que->front; n; n=n->next) {
                    TNode *tn = create_node(n->word, "");
                    if (!l2->head) l2->head=l2->tail=tn;
                    else { l2->tail->next=tn; tn->prev=l2->tail; l2->tail=tn; }
                    l2->size++;
                }
                printf("Converted queue to list (size=%d).\n", l2->size);
            } break;
            case 18:
                tree = toTree(stk);
                printf("BST created (size=%d).\n", tree->size);
                break;
            case 19:
                TraversalBSTinOrder(tree, printTreeNode);
                break;
            case 20:
                printf("Word: "); scanf("%s", word);
                {
                    TreeNode *n = getInfWordTree(tree, word);
                    if (n) printTreeNode(n);
                    else printf("Not found.\n");
                }
                break;
            case 21:
                printf("Word: "); scanf("%s", word);
                printf("Syn: "); scanf("%s", sym);
                printf("Ant: "); scanf("%s", antw);
                tree = AddWordBST(tree, word, sym, antw);
                break;
            case 22:
                printf("Word: "); scanf("%s", word);
                tree = deleteWordBST(tree, word);
                break;
            case 23:
                printf("Word: "); scanf("%s", word);
                printf("Syn: "); scanf("%s", sym);
                printf("Ant: "); scanf("%s", antw);
                tree = UpdateWordBST(tree, word, sym, antw);
                break;
            case 24:
                HighSizeBST(tree);
                printf("Balanced? %s\n", isBalancedBST(tree)?"yes":"no");
                break;
            case 25:
                printf("Word1: "); scanf("%s", word);
                printf("Word2: "); scanf("%s", sym);
                {
                    TreeNode *lca = LowestCommonAncestor(tree, word, sym);
                    printf("LCA: %s\n", lca?lca->word:"none");
                }
                break;
            case 26:
                printf("Word: "); scanf("%s", word);
                {
                    TreeNode *s = inOrderSuccessor(tree, word);
                    printf("Successor: %s\n", s?s->word:"none");
                }
                break;
            case 27:
                tree = BSTMirror(tree);
                break;
            case 28: {
                printf("Load second BST from file: "); scanf("%s", fname);
                TTree *t2 = fillTree(fopen(fname,"r"));
                tree = BSTMerge(tree, t2);
            } break;
            case 29:
                printf("Filename: "); scanf("%s", fname);
                printf("Word: "); scanf("%s", word);
                printf("Occurrences: %d\n", countWordOccurrence(fopen(fname,"r"), word));
                break;
            case 30:
                printf("Filename: "); scanf("%s", fname);
                printf("Word to remove: "); scanf("%s", word);
                removeWordOccurrence(fopen(fname,"r"), word);
                break;
            case 31:
                printf("Filename: "); scanf("%s", fname);
                printf("Word to replace: "); scanf("%s", word);
                printf("Replacement: "); scanf("%s", sym);
                replaceWordOccurrence(fopen(fname,"r"), word, sym);
                break;
            case 32:
                printf("Word: "); scanf("%s", word);
                wordPermutation(word);
                break;
            case 33:
                printf("Word: "); scanf("%s", word);
                subseqWord(word);
                break;
            case 34:
                printf("Word1: "); scanf("%s", word);
                printf("Word2: "); scanf("%s", sym);
                printf("Longest subseq length: %d\n", longestSubseqWord(word, sym));
                break;
            case 35:
                printf("Word: "); scanf("%s", word);
                printf("Distinct subseqs: %d\n", distinctSubseqWord(word));
                break;
            case 36:
                printf("Word: "); scanf("%s", word);
                printf("%s\n", isPalindromWord(word) ? "Palindrome" : "Not palindrome");
                break;
            case 0:
                printf("Exiting.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
