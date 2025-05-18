/* recursion.h */
#ifndef RECURSION_H
#define RECURSION_H

#include <stdio.h>
#include <stdbool.h>

/* File-based recursive operations */
int countWordOccurrence(FILE *f, const char *word);
FILE *removeWordOccurrence(FILE *f, const char *word);
FILE *replaceWordOccurrence(FILE *f, const char *word, const char *rep);

/* String-based recursive operations */
void wordPermutation(char *word);
void subseqWord(char *word);
int longestSubseqWord(const char *word1, const char *word2);
int distinctSubseqWord(const char *word);
bool isPalindromWord(const char *word);

#endif /* RECURSION_H */

/* recursion.c */
#include "recursion.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* --- File recursion helpers --- */
int countWordOccurrence(FILE *f, const char *word) {
    char w[256];
    if (fscanf(f, "%255s", w) != 1) return 0;
    int count = (strcmp(w, word) == 0);
    return count + countWordOccurrence(f, word);
}

static void removeRecursive(FILE *in, FILE *out, const char *word) {
    char w[256];
    if (fscanf(in, "%255s", w) != 1) return;
    if (strcmp(w, word) != 0) fprintf(out, "%s ", w);
    removeRecursive(in, out, word);
}

FILE *removeWordOccurrence(FILE *f, const char *word) {
    rewind(f);
    FILE *tmp = tmpfile();
    removeRecursive(f, tmp, word);
    rewind(tmp);
    return tmp;
}

static void replaceRecursive(FILE *in, FILE *out, const char *word, const char *rep) {
    char w[256];
    if (fscanf(in, "%255s", w) != 1) return;
    if (strcmp(w, word) == 0) fprintf(out, "%s ", rep);
    else fprintf(out, "%s ", w);
    replaceRecursive(in, out, word, rep);
}

FILE *replaceWordOccurrence(FILE *f, const char *word, const char *rep) {
    rewind(f);
    FILE *tmp = tmpfile();
    replaceRecursive(f, tmp, word, rep);
    rewind(tmp);
    return tmp;
}

/* --- String recursion helpers --- */

void wordPermutation(char *word) {
    size_t n = strlen(word);
    void permute(char *s, int l, int r) {
        if (l == r) { printf("%s\n", s); return; }
        for (int i = l; i <= r; i++) {
            char t = s[l]; s[l] = s[i]; s[i] = t;
            permute(s, l+1, r);
            t = s[l]; s[l] = s[i]; s[i] = t;
        }
    }
    permute(word, 0, n-1);
}

void subseqWord(char *word) {
    size_t n = strlen(word);
    char *buffer = malloc(n+1);
    void subseq(int i, int j) {
        if (i == n) {
            buffer[j] = '\0';
            printf("%s\n", buffer);
            return;
        }
        // include
        buffer[j] = word[i]; subseq(i+1, j+1);
        // exclude
        subseq(i+1, j);
    }
    subseq(0, 0);
    free(buffer);
}

int longestSubseqWord(const char *s1, const char *s2) {
    if (!*s1 || !*s2) return 0;
    if (*s1 == *s2) return 1 + longestSubseqWord(s1+1, s2+1);
    int a = longestSubseqWord(s1+1, s2);
    int b = longestSubseqWord(s1, s2+1);
    return a > b ? a : b;
}

int distinctSubseqWord(const char *word) {
    if (!*word) return 1;
    int count = distinctSubseqWord(word+1) * 2;
    // subtract duplicates
    const char *ch = strchr(word+1, *word);
    if (ch) count -= distinctSubseqWord(ch+1);
    return count;
}

bool isPalindromWord(const char *word) {
    size_t n = strlen(word);
    if (n < 2) return true;
    if (tolower(word[0]) != tolower(word[n-1])) return false;
    char *sub = strndup(word+1, n-2);
    bool res = isPalindromWord(sub);
    free(sub);
    return res;
}
