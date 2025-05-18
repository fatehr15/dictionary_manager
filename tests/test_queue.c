/* test_tlist_queue.c */
#include <stdio.h>
#include <stdlib.h>
#include "tlist_queue.h"

/* Helper to print a TList */
void printList(TList *L) {
    TNode *p = L->head;
    while (p) {
        printf("Word: %-10s  Syn/Ant: %-10s  Chars: %2d  Vowels: %d\n",
               p->word, p->syn_or_ant, p->num_chars, p->num_vowels);
        p = p->next;
    }
}

/* Helper to print a TQueue */
void printQueue(TQueue *Q) {
    QNode *n = Q->front;
    while (n) {
        printf("%s -> ", n->word);
        n = n->next;
    }
    printf("NULL\n");
}

int main(void) {
    /* Prepare two sample data files:
       Each line: word synonym_or_antonym */
    const char *syn_file = "synonyms.txt";
    const char *ant_file = "antonyms.txt";

    FILE *fsyn = fopen(syn_file, "r");
    if (!fsyn) {
        perror("Failed to open synonyms file");
        return EXIT_FAILURE;
    }
    FILE *fant = fopen(ant_file, "r");
    if (!fant) {
        perror("Failed to open antonyms file");
        fclose(fsyn);
        return EXIT_FAILURE;
    }

    /* Build lists */
    TList *syn = getSynWords(fsyn);
    TList *ant = getAntoWords(fant);

    printf("=== Synonyms List (%d entries) ===\n", syn->size);
    printList(syn);

    printf("\n=== Antonyms List (%d entries) ===\n", ant->size);
    printList(ant);

    /* Lookup tests */
    printf("\n--- Lookup word 'happy' ---\n");
    getInfWord(syn, ant, "happy");

    printf("\n--- Lookup synonym 'joyful' ---\n");
    getInfWord2(syn, ant, "joyful");

    /* Sorting test */
    sortWord(syn);
    printf("\n=== Synonyms Sorted Alphabetically ===\n");
    printList(syn);

    /* Queue test: enqueue words with even length (syllable proxy) */
    TQueue *q = syllable(syn);
    printf("\n=== Queue of even-length words (syllable) ===\n");
    printQueue(q);

    /* Clean up */
    fclose(fsyn);
    fclose(fant);
    return EXIT_SUCCESS;
}
