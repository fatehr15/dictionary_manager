/* tlist_queue.h */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Node for synonyms/antonyms linked list */
typedef struct TNode {
    char *word;
    char *syn_or_ant; /* synonym or antonym */
    int num_chars;
    int num_vowels;
    struct TNode *prev; /* for bidirectional merge */
    struct TNode *next;
} TNode;

/* Linked list wrapper */
typedef struct TList {
    TNode *head;
    TNode *tail;
    int size;
} TList;

/* Queue node */
typedef struct QNode {
    char *word;
    struct QNode *next;
} QNode;

/* Queue wrapper */
typedef struct TQueue {
    QNode *front;
    QNode *rear;
    int size;
} TQueue;

/* --- Function prototypes --- */

/* Build lists from file */
TList *getSynWords(FILE *f);
TList *getAntoWords(FILE *f);

/* Lookup */
void getInfWord(TList *syn, TList *ant, const char *word);
void getInfWord2(TList *syn, TList *ant, const char *inf);

/* Sorting */
TList *sortWord(TList *syn);
TList *sortWord2(TList *syn);
TList *sortWord3(TList *syn);

/* File updates */
TList *deleteWord(FILE *f, TList *syn, TList *ant, const char *word);
TList *updateWord(FILE *f, TList *syn, TList *ant, const char *word,
                  const char *syne, const char *anton);

/* Search and filters */
TList *similarWord(TList *syn, const char *word, double rate);
TList *countWord(TList *syn, const char *prt);
TList *palindromWord(TList *syn);

/* Merge lists */
TList *merge(TList *syn, TList *ant);
TList *merge2(TList *syn, TList *ant);

/* Add word */
TList *addWord(FILE *f, TList *syn, TList *ant,
               const char *word, const char *syne, const char *anton);

/* Queue operations */
TQueue *syllable(TList *syn);
TQueue *pronounciation(TList *syn);
TQueue *toQueue(TList *merged);

/* Utility */
int count_vowels(const char *s);
int is_palindrome(const char *s);

#endif // QUEUE_H

/* list_ops.c */
#include "tlist_queue.h"

/* Helper: create new node */
static TNode *create_node(const char *w, const char *sa) {
    TNode *node = malloc(sizeof *node);
    node->word = strdup(w);
    node->syn_or_ant = strdup(sa);
    node->num_chars = strlen(w);
    node->num_vowels = count_vowels(w);
    node->prev = node->next = NULL;
    return node;
}

/* Initialize empty list */
static TList *init_list(void) {
    TList *L = malloc(sizeof *L);
    L->head = L->tail = NULL;
    L->size = 0;
    return L;
}

TList *getSynWords(FILE *f) {
    TList *L = init_list();
    char w[128], s[128];
    while (fscanf(f, "%127s %127s", w, s) == 2) {
        TNode *n = create_node(w, s);
        if (!L->head) L->head = L->tail = n;
        else { L->tail->next = n; n->prev = L->tail; L->tail = n; }
        L->size++;
    }
    return L;
}

TList *getAntoWords(FILE *f) {
    /* identical to getSynWords, reading word and antonym */
    return getSynWords(f);
}

void getInfWord(TList *syn, TList *ant, const char *word) {
    for (TList *lists[] = {syn, ant}; ; ) {
        /* find and print info */
        break;
    }
}

void getInfWord2(TList *syn, TList *ant, const char *inf) {
    /* similar to above */
}

/* stubs for sorting etc. */
TList *sortWord(TList *syn) { return syn; }
TList *sortWord2(TList *syn) { return syn; }
TList *sortWord3(TList *syn) { return syn; }

TList *deleteWord(FILE *f, TList *syn, TList *ant, const char *word) { return syn; }
TList *updateWord(FILE *f, TList *syn, TList *ant,
                  const char *word, const char *syne, const char *anton) { return syn; }

TList *similarWord(TList *syn, const char *word, double rate) { return init_list(); }
TList *countWord(TList *syn, const char *prt) { return init_list(); }
TList *palindromWord(TList *syn) { return init_list(); }
TList *merge(TList *syn, TList *ant) { return init_list(); }
TList *merge2(TList *syn, TList *ant) { return init_list(); }
TList *addWord(FILE *f, TList *syn, TList *ant,
               const char *word, const char *syne, const char *anton) { return syn; }

int count_vowels(const char *s) {
    int c = 0;
    for (; *s; ++s) if (strchr("aeiouAEIOU", *s)) c++;
    return c;
}

int is_palindrome(const char *s) {
    size_t i = 0, j = strlen(s);
    if (!j) return 1;
    j--;
    while (i < j) {
        if (tolower(s[i]) != tolower(s[j])) return 0;
        i++; j--;
    }
    return 1;
}


/* queue_ops.c */
#include "tlist_queue.h"

static TQueue *init_queue(void) {
    TQueue *Q = malloc(sizeof *Q);
    Q->front = Q->rear = NULL;
    Q->size = 0;
    return Q;
}

static QNode *create_qnode(const char *w) {
    QNode *n = malloc(sizeof *n);
    n->word = strdup(w);
    n->next = NULL;
    return n;
}

TQueue *syllable(TList *syn) {
    TQueue *Q = init_queue();
    /* stub: enqueue based on syllable count */
    return Q;
}

TQueue *pronounciation(TList *syn) {
    TQueue *Q = init_queue();
    /* stub: enqueue by pronunciation type */
    return Q;
}

TQueue *toQueue(TList *merged) {
    TQueue *Q = init_queue();
    for (TNode *p = merged->head; p; p = p->next) {
        QNode *n = create_qnode(p->word);
        if (!Q->front) Q->front = Q->rear = n;
        else { Q->rear->next = n; Q->rear = n; }
        Q->size++;
    }
    return Q;
}




