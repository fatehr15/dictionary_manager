/* tlist_queue.c */
#include "queue.h"
#include <string.h>
#include <ctype.h>
/* Utility functions */
int count_vowels(const char *s) {
    int count = 0;
    while (*s){
        char c=tolower(*s);
        if (c=='a'||c=='e'||c =='i'||c =='o'||c=='u') count++ ;
        s++;
    }
    return count;
}

int is_palindrome(const char *s) {
    size_t i = 0, j =strlen(s) ;
    if ( j== 0 ) return 1 ;
    j-- ;
    while (i < j) {
        if (tolower(s[i]) !=tolower(s[j])) return 0 ;
        i++ ; 
        j-- ;
    }
    return 1 ;
}

/* List initialization */
static TList *init_list(void) {
    TList *L = malloc(sizeof *L) ;
    L->head = L->tail = NULL ;
    L-> size = 0 ;
    return L ;
}

static TNode *create_node(const char *w, const char *sa) {
    TNode *n = malloc(sizeof *n) ;
    n -> word = strdup(w) ;
    n ->syn_or_ant = strdup(sa) ;
    n -> num_chars = strlen(w) ;
    n -> num_vowels = count_vowels(w) ;
    n -> prev = n -> next = NULL;
    return n;
}

/* Building lists */
TList * getSynWords(FILE *f) {
    TList *L = init_list();
    char w[256], s[256];
    while (fscanf(f, "%255s %255s", w, s) == 2) {
        TNode *n = create_node(w, s);
        if (!L->head) L->head = L->tail = n;
        else { L->tail->next = n; n->prev = L->tail; L->tail = n; }
        L->size++;
    }
    return L;
}

TList *getAntoWords(FILE *f) {
    return getSynWords(f);
}

/* Lookup by word */
void getInfWord(TList *syn, TList *ant, const char *word) {
    TNode *p;
    for (p = syn->head; p; p = p->next) {
        if (!strcmp(p->word, word)) {
            printf("Word: %s, Synonym: %s, Chars: %d, Vowels: %d\n",
                   p->word, p->syn_or_ant, p->num_chars, p->num_vowels);
            break;
        }
    }
    for (p = ant->head; p; p = p->next) {
        if (!strcmp(p->word, word)) {
            printf("Word: %s, Antonym: %s, Chars: %d, Vowels: %d\n",
                   p->word, p->syn_or_ant, p->num_chars, p->num_vowels);
            break;
        }
    }
}

/* Lookup by synonym/antonym */
void getInfWord2(TList *syn, TList *ant, const char *inf) {
    TNode *p;
    for (p = syn->head; p; p = p->next) {
        if (!strcmp(p->syn_or_ant, inf)) {
            printf("Synonym: %s -> Word: %s, Chars: %d, Vowels: %d\n",
                   inf, p->word, p->num_chars, p->num_vowels);
            return;
        }
    }
    for (p = ant->head; p; p = p->next) {
        if (!strcmp(p->syn_or_ant, inf)) {
            printf("Antonym: %s -> Word: %s, Chars: %d, Vowels: %d\n",
                   inf, p->word, p->num_chars, p->num_vowels);
            return;
        }
    }
    printf("'%s' not found.\n", inf);
}

/* Sorting helpers */
static void swap_nodes(TNode *a, TNode *b) {
    char *w = a->word; a->word = b->word; b->word = w;
    char *sa = a->syn_or_ant; a->syn_or_ant = b->syn_or_ant; b->syn_or_ant = sa;
    int c = a->num_chars; a->num_chars = b->num_chars; b->num_chars = c;
    int v = a->num_vowels; a->num_vowels = b->num_vowels; b->num_vowels = v;
}

TList *sortWord(TList *syn) {
    /* alphabetical bubble sort */
    int swapped;
    do {
        swapped = 0;
        for (TNode *p = syn->head; p && p->next; p = p->next) {
            if (strcmp(p->word, p->next->word) > 0) {
                swap_nodes(p, p->next);
                swapped = 1;
            }
        }
    } while (swapped);
    return syn;
}

TList *sortWord2(TList *syn) {
    /* ascending num_chars */
    int swapped;
    do {
        swapped = 0;
        for (TNode *p = syn->head; p && p->next; p = p->next) {
            if (p->num_chars > p->next->num_chars) {
                swap_nodes(p, p->next);
                swapped = 1;
            }
        }
    } while (swapped);
    return syn;
}

TList *sortWord3(TList *syn) {
    /* descending num_vowels */
    int swapped;
    do {
        swapped = 0;
        for (TNode *p = syn->head; p && p->next; p = p->next) {
            if (p->num_vowels < p->next->num_vowels) {
                swap_nodes(p, p->next);
                swapped = 1;
            }
        }
    } while (swapped);
    return syn;
}

/* File update helpers (stubs) */
TList *deleteWord(FILE *f, TList *syn, TList *ant, const char *word) {
    // TODO: remove from file and both lists
    return syn;
}

TList *updateWord(FILE *f, TList *syn, TList *ant,
                  const char *word, const char *syne, const char *anton) {
    // TODO: update file and lists
    return syn;
}

/* Similarity (stub) */
TList *similarWord(TList *syn, const char *word, double rate) {
    TList *out = init_list();
    // TODO: compute similarity and add if >= rate
    return out;
}

TList *countWord(TList *syn, const char *prt) {
    TList *out = init_list();
    for (TNode *p = syn->head; p; p = p->next) {
        if (strstr(p->word, prt)) {
            TNode *n = create_node(p->word, p->syn_or_ant);
            if (!out->head) out->head = out->tail = n;
            else { out->tail->next = n; n->prev = out->tail; out->tail = n; }
            out->size++;
        }
    }
    return out;
}

TList *palindromWord(TList *syn) {
    TList *out = init_list();
    for (TNode *p = syn->head; p; p = p->next) {
        if (is_palindrome(p->word)) {
            /* insert sorted by alphabetical */
            TNode *n = create_node(p->word, p->syn_or_ant);
            /* simple append; user can sort later */
            if (!out->head) out->head = out->tail = n;
            else { out->tail->next = n; n->prev = out->tail; out->tail = n; }
            out->size++;
        }
    }
    return out;
}

/* Merge into doubly-linked */
TList *merge(TList *syn, TList *ant) {
    TList *out = init_list();
    TNode *p1 = syn->head, *p2 = ant->head;
    while (p1 && p2) {
        TNode *n = create_node(p1->word, p1->syn_or_ant);
        /* reuse syn fields, then append antonym */
        n->next = create_node(p2->word, p2->syn_or_ant);
        /* linkage */
        if (!out->head) out->head = n;
        else {
            out->tail->next = n;
            n->prev = out->tail;
        }
        out->tail = n->next;
        p1 = p1->next; p2 = p2->next;
        out->size += 2;
    }
    return out;
}

/* Merge into circular */
TList *merge2(TList *syn, TList *ant) {
    TList *c = merge(syn, ant);
    if (c->tail && c->head) {
        c->tail->next = c->head;
        c->head->prev = c->tail;
    }
    return c;
}

TList *addWord(FILE *f, TList *syn, TList *ant,
               const char *word, const char *syne, const char *anton) {
    // Append to file
    fprintf(f, "%s %s %s\n", word, syne, anton);
    // Add into lists
    TNode *ns = create_node(word, syne);
    if (!syn->head) syn->head = syn->tail = ns;
    else { syn->tail->next = ns; ns->prev = syn->tail; syn->tail = ns; }
    syn->size++;
    TNode *na = create_node(word, anton);
    if (!ant->head) ant->head = ant->tail = na;
    else { ant->tail->next = na; na->prev = ant->tail; ant->tail = na; }
    ant->size++;
    return syn;
}

/* Queue operations */
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
    /* stub: count vowels as proxy for syllables */
    TQueue *Q = init_queue();
    for (TNode *p = syn->head; p; p = p->next) {
        if (p->num_chars % 2 == 0) { // example split
            QNode *n = create_qnode(p->word);
            if (!Q->front) Q->front = Q->rear = n;
            else { Q->rear->next = n; Q->rear = n; }
            Q->size++;
        }
    }
    return Q;
}

TQueue *pronounciation(TList *syn) {
    TQueue *Q = init_queue();
    for (TNode *p = syn->head; p; p = p->next) {
        QNode *n = create_qnode(p->word);
        if (!Q->front) Q->front = Q->rear = n;
        else { Q->rear->next = n; Q->rear = n; }
        Q->size++;
    }
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
