//concordance.c
/* My implementation of hash tables is not as professional
    like implementation from https://github.com/watmough/jwHash
    And I think time of execution of this task will be better 
    with jwHash */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "hashT.h"

/* return string (char pointer) from file (file pointer) */
char *getWord(FILE *fp);

/* output of result to screen */
void printArray(tuple **array, const unsigned int size);

/* output of result to file */
void fprintArray(tuple **array, const unsigned int size);

/* sort array */
void sortArray(tuple **array, const unsigned int size);

int main(int argc, char *argv[])
{
    //clock_t begin = clock();

    const char *fn = argc > 1 ? argv[1] : "input.txt";
    FILE *fp = fopen(fn, "r");

    HashT words = HashTCreate();

    if (!fp)
    {
        fprintf(stderr, "Error: File open failed.\n");
        return 1;
    }

    char *word;
    while ((word = getWord(fp)))
    {
        HashTIncValue(words, word);
    }
    fclose(fp);

    tuple *t;
    const unsigned int n = HashTToArray(words, &t);
    sortArray(&t, n);
    printArray(&t, n);

    /* if you need output file then uncomment line that is below */
    //fprintArray(&t, n);

    HashTDestroy(words);
    ArrayDestroy(&t, n);

    //clock_t end = clock();

    //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("\n\t%f\n", time_spent);

    return 0;
}

/* ---------- */

char *getWord(FILE *fp)
{
    char buf[100];
    unsigned int ch, i = 0;

    while (EOF != (ch = fgetc(fp)) && !isalpha(ch))
        ;
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

void printArray(tuple **array, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        printf("%s  %u\n", (*array)[i].str, (*array)[i].num);
    }
}

void fprintArray(tuple **array, const unsigned int size)
{
    FILE *f = fopen("output.txt", "w");
    for (unsigned int i = 0; i < size; i++)
    {
        fprintf(f, "%s  %u\n", (*array)[i].str, (*array)[i].num);
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