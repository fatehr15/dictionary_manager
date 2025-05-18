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
