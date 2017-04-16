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
#include <unistd.h>
#include <sys/stat.h>
#include "hashT.h"

/* return string (char pointer) from file (file pointer) */
char *getWord(FILE *fp, unsigned int *curLine);

/* output of result to screen */
void printArray(tuple **array, const unsigned int size, const unsigned mode);

/* output of result to file */
void fprintArray(tuple **array, const unsigned int size, const unsigned mode, const char *fout);

/* sort array */
void sortArray(tuple **array, const unsigned int size);

/* check that input is number */
unsigned int isnumber(char *input);

int main(int argc, char *argv[])
{
    opterr = 0;
    int c;
    unsigned mode = 0; /* 0 - only locations of words, 1 - only numbers of words, 2 - locations and numbers of words */
    char *fout = NULL; /* if NULL than output to screen, else output to file */

    while ((c = getopt(argc, argv, "m:o:h")) != -1)
        switch (c)
        {
        case 'm':
            if (isnumber(optarg))
                mode = atoi(optarg);
            break;
        case 'o':
            fout = optarg;
            break;
        case 'h':
            fprintf(stdout, "concordance <input file> -m <mode 0-2> -o <output file>\n");
            exit(0);
            break;
        case '?':
            if (optopt == 'm' || optopt == 'o')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
            exit(1);
        default:
            abort();
        }

    clock_t begin = clock();

    const char *fn = (optind < argc) ? argv[optind] : "input.txt";
    struct stat buf;
    if (stat(fn, &buf) != 0)
    {
        fprintf(stderr, "Unable to open file: %s\n", fn);
        exit(1);
    }
    FILE *fp = fopen(fn, "rt");

    HashT words = HashTCreate();
    unsigned int curLine = 1;

    char *word;
    while ((word = getWord(fp, &curLine)))
        HashTOperation(words, word, curLine);
    fclose(fp);

    tuple *t;
    const unsigned int n = HashTToArray(words, &t);
    sortArray(&t, n);
    if (fout)
        fprintArray(&t, n, mode, fout);
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
    unsigned i = 0;
    int ch;

    while (EOF != (ch = fgetc(fp)) && !isalpha(ch))
        if (ch == '\n')
            ++*curLine;
    if (ch == EOF)
        return NULL;

    do
    {
        buf[i++] = tolower(ch);
    } while (EOF != (ch = fgetc(fp)) &&
             (isalpha(ch) || ch == '\'' || (ch == '-' && buf[i - 1] != '-')));

    (buf[i - 1] == '-' || buf[i - 1] == '\'') ? (buf[i - 1] = '\0') : (buf[i] = '\0');

    ungetc(ch, fp);

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
            printf("[%zu]\n", (*array)[i].locs.used);
        else
            printf("\n");
    }
}

void fprintArray(tuple **array, const unsigned int size, const unsigned mode, const char *fout)
{
    FILE *fp = fopen(fout, "wt");
    if (!fp)
    {
        fprintf(stderr, "Unable to open file: %s\n", fout);
    }
    for (unsigned int i = 0; i < size; i++)
    {
        fprintf(fp, "%s ", (*array)[i].str);
        if (mode == 0 || mode == 2)
            for (unsigned int j = 0; j < (*array)[i].locs.used; j++)
                fprintf(fp, "%u ", (*array)[i].locs.array[j]);
        if (mode == 1 || mode == 2)
            fprintf(fp, "[%zu]\n", (*array)[i].locs.used);
        else
            fprintf(fp, "\n");
    }
    fclose(fp);
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

unsigned int isnumber(char *input)
{
    size_t length = strlen(input);
    for (int i=0; i<length; i++)
        if (!isdigit(input[i]))
        {
            fprintf (stderr, "Entered input is not a number\n");
            exit(1);
        }
    return 1;
}