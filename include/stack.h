/* tlist_queue.h */
#ifndef TLIST_QUEUE_H
#define TLIST_QUEUE_H

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
    struct TNode *prev; /* for bidirectional/circular merge */
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
TList *updateWord(FILE *f , TList *syn, TList *ant, const char *word,
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

#endif /* TLIST_QUEUE_H */


/* stacks.h */
#ifndef STACKS_H
#define STACKS_H

#include "tlist_queue.h"
#include <stdbool.h>

/* Stack node holding word, synonym, antonym, chars, vowels */
typedef struct SNode {
    char *word;
    char *syn;
    char *ant;
    int num_chars;
    int num_vowels;
    struct SNode *next;
} SNode;

/* Stack wrapper */
typedef struct TStack {
    SNode *top;
    int size;
} TStack;

/* --- Function prototypes --- */
TStack *toStack(TList *merged);
TStack *getInfWordStack(TStack *stk, const char *word);
TStack *sortWordStack(TStack *stk);
TStack *deleteWordStack(TStack *stk, const char *word);
TStack *updateWordStack(TStack *stk, const char *word,
                        const char *syne, const char *anton);
TQueue *stackToQueue(TStack *stk);
TList  *StacktoList(TStack *stk);
TStack *addWordStack(TStack *stk, const char *word,
                     const char *syne, const char *anton);
TStack *syllableStack(TStack *stk);
TStack *pronounciationStack(TStack *stk);
char   *getSmallest(TStack *stk);
void    cycleSearch(TStack *stk);
bool    isPalyndromeStack(const char *word);
TStack *StackRev(TStack *stk);

#endif /* STACKS_H */
