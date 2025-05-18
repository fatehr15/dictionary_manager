/* test_stack.c */

#include <stdio.h>
#include <stdlib.h>
#include "tlist_queue.h"
#include "stacks.h"

/* Helper to print a TStack */
void printStack(TStack *stk) {
    printf("Stack (top → bottom):\n");
    for (SNode *p = stk->top; p; p = p->next) {
        printf("  %-10s syn=%-10s ant=%-10s chars=%2d vowels=%d\n",
               p->word, p->syn, p->ant, p->num_chars, p->num_vowels);
    }
}

/* Helper to print a TQueue */
void printQueue(TQueue *Q) {
    printf("Queue front → rear:\n");
    QNode *n = Q->front;
    while (n) {
        printf("%s -> ", n->word);
        n = n->next;
    }
    printf("NULL\n");
}

/* Helper to print a TList */
void printList(TList *L) {
    printf("List:\n");
    for (TNode *p = L->head; p; p = p->next) {
        printf("  %-10s syn/ant=%-10s chars=%2d vowels=%d\n",
               p->word, p->syn_or_ant, p->num_chars, p->num_vowels);
    }
}

int main(void) {
    // 1. Prepare two lists and merge
    FILE *fs = fopen("synonyms.txt", "r");
    if (!fs) { perror("synonyms.txt"); return EXIT_FAILURE; }
    TList *syn = getSynWords(fs);
    fclose(fs);

    FILE *fa = fopen("antonyms.txt", "r");
    if (!fa) { perror("antonyms.txt"); return EXIT_FAILURE; }
    TList *ant = getAntoWords(fa);
    fclose(fa);

    printf("Synonyms list size: %d\n", syn->size);
    printf("Antonyms list size: %d\n\n", ant->size);

    TList *merged = merge(syn, ant);
    printf("Merged list size: %d\n", merged->size);
    printList(merged);

    // 2. Convert merged list to stack
    TStack *stk = toStack(merged);
    printStack(stk);

    // 3. Lookup in stack
    printf("\nLookup 'happy':\n");
    getInfWordStack(stk, "happy");

    // 4. Sort stack alphabetically
    stk = sortWordStack(stk);
    printf("\nAfter alphabetical sort:\n");
    printStack(stk);

    // 5. Delete a word
    stk = deleteWordStack(stk, "happy");
    printf("\nAfter deleting 'happy':\n");
    printStack(stk);

    // 6. Update a word
    stk = updateWordStack(stk, "sad", "unhappy", "cheerful");
    printf("\nAfter updating 'sad':\n");
    printStack(stk);

    // 7. Enqueue stack to queue
    TQueue *q = stackToQueue(stk);
    printQueue(q);

    // 8. Convert stack → list
    TList *list2 = StacktoList(stk);
    printf("\nList from stack:\n");
    printList(list2);

    // 9. Add a new word to stack
    stk = addWordStack(stk, "swift", "fast", "slow");
    printf("\nAfter adding 'swift':\n");
    printStack(stk);

    // 10. Sort by syllable (vowel count)
    stk = syllableStack(stk);
    printf("\nAfter sorting by vowel count:\n");
    printStack(stk);

    // 11. Smallest word
    char *min = getSmallest(stk);
    printf("\nSmallest (alphabetically) word: %s\n", min);

    // 12. Cycle search
    printf("\nCycle search:\n");
    cycleSearch(stk);

    // 13. Palindrome test
    const char *testw = "level";
    printf("\nIs '%s' a palindrome? %s\n", testw,
           isPalyndromeStack(testw) ? "yes" : "no");

    // 14. Reverse stack
    stk = StackRev(stk);
    printf("\nAfter reversing the stack:\n");
    printStack(stk);

    return EXIT_SUCCESS;
}
