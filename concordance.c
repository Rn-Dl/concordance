//concordance.c
/* My implementation of hash tables is not as professional
    like implementation from https://github.com/watmough/jwHash
    And I think time of execution of this task will be better 
    with jwHash */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "hashT.h"

/* return string (char pointer) from file (file pointer) */
char *getWord(FILE *fp, unsigned int *curLine);

/* output of result to screen */
void printArray(tuple **array, const unsigned int size, const unsigned mode);

/* output of result to file */
void fprintArray(tuple **array, const unsigned int size, const unsigned mode);

/* sort array */
void sortArray(tuple **array, const unsigned int size);

int main(int argc, char *argv[])
{
    //opterr = 0;
    /* 0 - only locations of words, 1 - only numbers of words, 2 - locations and numbers of words */
    int c;
    unsigned mode = 0;
    bool fout = true;

    while ((c = getopt(argc, argv, "m:o")) != -1)
        switch (c)
        {
        case 'm':
            mode = atoi(optarg);
            break;
        case 'o':
            fout = false;
            break;
        case '?':
            if (optopt == 'm')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
            return 1;
        default:
            abort();
        }

    clock_t begin = clock();

    const char *fn = (optind < argc) ? argv[optind] : "input.txt";
    FILE *fp = fopen(fn, "r");

    HashT words = HashTCreate();
    unsigned int curLine = 1;

    if (!fp)
    {
        fprintf(stderr, "Error: File open failed.\n");
        return 1;
    }

    char *word;
    while ((word = getWord(fp, &curLine)))
        HashTOperation(words, word, curLine, mode);
    fclose(fp);

    tuple *t;
    const unsigned int n = HashTToArray(words, &t, mode);
    sortArray(&t, n);
    if (fout)
        fprintArray(&t, n, mode);
    else
        printArray(&t, n, mode);

    HashTDestroy(words);
    ArrayDestroy(&t, n);

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nExecution time:\t%f\n", time_spent);

    return 0;
}

/* ---------- */

char *getWord(FILE *fp, unsigned int *curLine)
{
    char buf[100];
    unsigned int ch, i = 0;

    while (EOF != (ch = fgetc(fp)) && !isalpha(ch))
        if (ch == '\n')
            ++*curLine;
    if (ch == EOF)
        return NULL;

    do
    {
        buf[i++] = tolower(ch);
    } while (EOF != (ch = fgetc(fp)) &&
             (isalpha(ch) || ch == '\'' || ch == '-'));

    buf[i] = '\0';

    return strdup(buf);
}

void printArray(tuple **array, const unsigned int size, const unsigned mode)
{
    for (unsigned int i = 0; i < size; i++)
    {
        printf("%s ", (*array)[i].str);
        if (mode == 0 || mode == 2)
            for (unsigned int j = 0; j < (*array)[i].locs.used; j++)
                printf("%u ", (*array)[i].locs.array[j]);
        if (mode == 1 || mode == 2)
            printf("[%u]\n", (*array)[i].num);
        else
            printf("\n");
    }
}

void fprintArray(tuple **array, const unsigned int size, const unsigned mode)
{
    FILE *f = fopen("output.txt", "w");
    for (unsigned int i = 0; i < size; i++)
    {
        fprintf(f, "%s ", (*array)[i].str);
        if (mode == 0 || mode == 2)
            for (unsigned int j = 0; j < (*array)[i].locs.used; j++)
                fprintf(f, "%u ", (*array)[i].locs.array[j]);
        if (mode == 1 || mode == 2)
            fprintf(f, "[%u]\n", (*array)[i].num);
        else
            fprintf(f, "\n");
    }
    fclose(f);
}

void sortArray(tuple **array, const unsigned int size)
{
    tuple temp;
    for (unsigned int i = 0; i < size; i++)
        for (unsigned int j = 0; j < size - 1; j++)
            if (strcmp((*array)[j].str, (*array)[j + 1].str) > 0)
            {
                temp = (*array)[j];
                (*array)[j] = (*array)[j + 1];
                (*array)[j + 1] = temp;
            }
}