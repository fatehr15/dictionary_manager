/* src.c */

#include "tlist_queue.h"
#include "stacks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* --- Utility functions --- */
int count_vowels(const char *s) {
    int count = 0;
    while (*s) {
        char c = tolower(*s);
        if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u') count++;
        s++;
    }
    return count;
}

int is_palindrome(const char *s) {
    size_t i = 0, j = strlen(s);
    if (j == 0) return 1;
    j--;
    while (i < j) {
        if (tolower(s[i]) != tolower(s[j])) return 0;
        i++; j--;
    }
    return 1;
}

/* --- Stack Operations --- */
static TStack *init_stack(void) {
    TStack *s = malloc(sizeof *s);
    s->top = NULL;
    s->size = 0;
    return s;
}
static SNode *create_snode(const char *w, const char *syn, const char *ant) {
    SNode *n = malloc(sizeof *n);
    n->word = strdup(w);
    n->syn  = strdup(syn);
    n->ant  = strdup(ant);
    n->num_chars = strlen(w);
    n->num_vowels = count_vowels(w);
    n->next = NULL;
    return n;
}

TStack *toStack(TList *merged) {
    TStack *s = init_stack();
    for (TNode *p = merged->head; p; p = p->next) {
        SNode *n = create_snode(p->word, p->syn_or_ant, "");
        n->next = s->top;
        s->top = n;
        s->size++;
    }
    return s;
}

TStack *getInfWordStack(TStack *stk, const char *word) {
    for (SNode *p = stk->top; p; p = p->next) {
        if (!strcmp(p->word, word)) {
            printf("Word: %s, Syn: %s, Ant: %s, Chars: %d, Vowels: %d\n",
                   p->word, p->syn, p->ant, p->num_chars, p->num_vowels);
            return stk;
        }
    }
    printf("%s not found in stack.\n", word);
    return stk;
}

TStack *sortWordStack(TStack *stk) {
    bool swapped;
    do {
        swapped = false;
        for (SNode *p = stk->top; p && p->next; p = p->next) {
            if (strcmp(p->word, p->next->word) > 0) {
                // swap fields
                char *tword = p->word; p->word = p->next->word; p->next->word = tword;
                char *tsyn  = p->syn;  p->syn  = p->next->syn;  p->next->syn  = tsyn;
                char *tant  = p->ant;  p->ant  = p->next->ant;  p->next->ant  = tant;
                int tchar  = p->num_chars; p->num_chars = p->next->num_chars; p->next->num_chars = tchar;
                int tvowel = p->num_vowels; p->num_vowels = p->next->num_vowels; p->next->num_vowels = tvowel;
                swapped = true;
            }
        }
    } while (swapped);
    return stk;
}

TStack *deleteWordStack(TStack *stk, const char *word) {
    TStack *tmp = init_stack();
    while (stk->top) {
        SNode *n = stk->top; stk->top = n->next;
        if (strcmp(n->word, word) != 0) {
            n->next = tmp->top; tmp->top = n; tmp->size++;
        } else {
            free(n->word); free(n->syn); free(n->ant); free(n);
            stk->size--;
        }
    }
    while (tmp->top) {
        SNode *n = tmp->top; tmp->top = n->next;
        n->next = stk->top; stk->top = n;
    }
    free(tmp);
    return stk;
}

TStack *updateWordStack(TStack *stk, const char *word, const char *syne, const char *anton) {
    for (SNode *p = stk->top; p; p = p->next) {
        if (!strcmp(p->word, word)) {
            free(p->syn); free(p->ant);
            p->syn = strdup(syne); p->ant = strdup(anton);
            p->num_vowels = count_vowels(p->word);
            break;
        }
    }
    return stk;
}

TQueue *stackToQueue(TStack *stk) {
    TQueue *Q = init_queue();
    for (SNode *p = stk->top; p; p = p->next) {
        QNode *n = create_qnode(p->word);
        if (!Q->front) Q->front = Q->rear = n;
        else { Q->rear->next = n; Q->rear = n; }
        Q->size++;
    }
    return Q;
}

TList *StacktoList(TStack *stk) {
    TList *L = init_list();
    for (SNode *p = stk->top; p; p = p->next) {
        TNode *n = create_node(p->word, p->syn);
        if (!L->head) L->head = L->tail = n;
        else { L->tail->next = n; n->prev = L->tail; L->tail = n; }
        L->size++;
    }
    return L;
}

TStack *addWordStack(TStack *stk, const char *word, const char *syne, const char *anton) {
    SNode *n = create_snode(word, syne, anton);
    n->next = stk->top; stk->top = n; stk->size++;
    return sortWordStack(stk);
}

TStack *syllableStack(TStack *stk) {
    // Sort by vowel count ascending
    bool swapped;
    do {
        swapped = false;
        for (SNode *p = stk->top; p && p->next; p = p->next) {
            if (p->num_vowels > p->next->num_vowels) {
                // swap fields
                char *tword    = p->word; p->word    = p->next->word; p->next->word    = tword;
                char *tsyn     = p->syn;  p->syn     = p->next->syn;  p->next->syn     = tsyn;
                char *tant     = p->ant;  p->ant     = p->next->ant;  p->next->ant     = tant;
                int tchar      = p->num_chars; p->num_chars = p->next->num_chars; p->next->num_chars = tchar;
                int tvowel     = p->num_vowels; p->num_vowels = p->next->num_vowels; p->next->num_vowels = tvowel;
                swapped = true;
            }
        }
    } while (swapped);
    return stk;
}

TStack *pronounciationStack(TStack *stk) {
    // Grouping stub: just return original
    return stk;
}

char *getSmallest(TStack *stk) {
    if (!stk->top) return NULL;
    char *min = stk->top->word;
    for (SNode *p = stk->top; p; p = p->next) {
        if (strcmp(p->word, min) < 0) min = p->word;
    }
    return min;
}

void cycleSearch(TStack *stk) {
    // Detect simple cycle: if any syn matches another word
    for (SNode *p = stk->top; p; p = p->next) {
        for (SNode *q = stk->top; q; q = q->next) {
            if (strcmp(p->syn, q->word) == 0) {
                printf("Cycle: %s -> %s\n", p->word, q->word);
            }
        }
    }
}

bool isPalyndromeStack(const char *word) {
    size_t n = strlen(word);
    char *rev = malloc(n+1);
    for (size_t i = 0; i < n; i++) rev[i] = word[n-1-i];
    rev[n] = '\0';
    bool res = !strcasecmp(word, rev);
    free(rev);
    return res;
}

TStack *StackRev(TStack *stk) {
    if (!stk->top) return stk;
    SNode *first = stk->top;
    stk->top = first->next;
    TStack *rev = StackRev(stk);
    first->next = NULL;
    if (!rev->top) rev->top = first;
    else {
        SNode *p = rev->top;
        while (p->next) p = p->next;
        p->next = first;
    }
    return rev;
}

int main() {
    // Example: build, merge, stack, operate
    return 0;
}
