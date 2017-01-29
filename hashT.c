//hashT.c

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "hashT.h"

struct element
{
    char *key;
    unsigned int value;
    Array locs;
    struct element *next;
};

struct hashTable
{
    unsigned int size; //size of the pointer table
    unsigned int n;    //number of elements stored
    struct element **table;
};

/* Initialization of dynamic array */
void initArray(Array *arr, size_t initialSize)
{
    arr->array = (unsigned int *)malloc(initialSize * sizeof(unsigned int));
    arr->used = 0;
    arr->size = initialSize;
}

/* Add a new integer into an existing dynamic array */
void insertArray(Array *arr, const unsigned int element)
{
    if (arr->used == arr->size)
    {
        arr->size *= 2;
        arr->array = (unsigned int *)realloc(arr->array, arr->size * sizeof(unsigned int));
    }
    arr->array[arr->used++] = element;
}

void freeArray(Array *arr)
{
    free(arr->array);
    arr->array = 0;
    arr->used = arr->size = 0;
}

/* for initialization of hash tables in HashTCreate and grow */
HashT internalHashTCreate(unsigned int size)
{
    HashT hT;
    unsigned int i;

    hT = malloc(sizeof(*hT));

    assert(hT != 0);

    hT->size = size;
    hT->n = 0;
    hT->table = malloc(sizeof(struct element *) * hT->size);

    assert(hT->table != 0);

    for (i = 0; i < hT->size; i++)
        hT->table[i] = 0;

    return hT;
}

HashT HashTCreate(void)
{
    return internalHashTCreate(INITIAL_SIZE);
}

void HashTDestroy(HashT hT)
{
    unsigned int i;
    struct element *e;
    struct element *next;

    for (i = 0; i < hT->size; i++)
    {
        for (e = hT->table[i]; e != 0; e = next)
        {
            next = e->next;

            free(e->key);
            e->key = 0;
            freeArray(&e->locs);
            free(e);
            e = 0;
        }
    }

    free(hT->table);
    hT->table = 0;
    free(hT);
    hT = 0;
}

/* function that generate hash values for table */
static unsigned long hash_function(const char *s)
{
    unsigned const char *us;
    unsigned long h;

    h = 0;

    for (us = (unsigned const char *)s; *us; us++)
    {
        h = h * MULTIPLIER + *us;
    }

    return h;
}

/* increase hash table in GROWTH_FACTOR times */
static void grow(HashT hT, const unsigned mode)
{
    HashT d2;              //new hashTable
    struct hashTable swap; //temporary structure
    unsigned int i;
    struct element *e;

    d2 = internalHashTCreate(hT->size * GROWTH_FACTOR);

    for (i = 0; i < hT->size; i++)
    {
        for (e = hT->table[i]; e != 0; e = e->next)
        {
            if (mode == 1 || mode == 2)
                HashTAddData(d2, e->key, e->value, 0, 1);
            if (mode == 0 || mode == 2)
                HashTCopyArray(d2, e->key, &e->locs);
        }
    }

    swap = *hT;
    *hT = *d2;
    *d2 = swap;

    HashTDestroy(d2);
}

/* add a new data associated with a key into an existing hashTable */
void HashTAddData(HashT hT, const char *key, const unsigned int value, const unsigned int curLoc, const unsigned mode)
{
    struct element *e;
    unsigned long h;

    assert(key);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    e->value = (mode == 1 || mode == 2) ? value : 0;
    if (mode == 0 || mode == 2)
    {
        initArray(&e->locs, 5);
        insertArray(&e->locs, curLoc);
    }

    h = hash_function(key) % hT->size;

    e->next = hT->table[h];
    hT->table[h] = e;

    hT->n++;

    // grow table if there is not enough space
    if (hT->n >= hT->size * MAX_LOAD_FACTOR)
    {
        grow(hT, mode);
    }
}

/* Copy dynamic array to array associated with a key */
void HashTCopyArray(HashT hT, const char *key, const Array *arr)
{
    struct element *e;
    unsigned long h;

    assert(key);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    initArray(&e->locs, arr->size);
    e->locs.used = arr->used;
    memcpy(e->locs.array, arr->array, arr->size * sizeof *e->locs.array);

    h = hash_function(key) % hT->size;

    e->next = hT->table[h];
    hT->table[h] = e;

    hT->n++;
}

/* return the most recently added value associated with a key
    or 0 if no matching key is present */
const unsigned int HashTSearch(HashT hT, const char *key)
{
    struct element *e;

    for (e = hT->table[hash_function(key) % hT->size]; e != 0; e = e->next)
    {
        if (!strcmp(e->key, key))
        {
            // got it
            return e->value;
        }
    }

    return 0;
}

/* increment value and insert new value in dynamic array associated with a key */
void HashTOperation(HashT hT, const char *key, const unsigned int curLoc, const unsigned mode)
{
    struct element *e;

    for (e = hT->table[hash_function(key) % hT->size]; e != 0; e = e->next)
    {
        if (!strcmp(e->key, key))
        {
            // increment it
            if (mode == 1 || mode == 2)
                e->value++;
            if (mode == 0 || mode == 2)
                insertArray(&e->locs, curLoc);
            return;
        }
    }
    HashTAddData(hT, key, 1, curLoc, mode);
}

/* convert hash table to array of tuples */
unsigned int HashTToArray(HashT hT, tuple **array, const unsigned mode)
{
    struct element *e;
    unsigned int counter = 0;
    *array = malloc(hT->n * sizeof(tuple));

    for (unsigned int i = 0; i < hT->size; i++)
        if (hT->table[i] != 0)
            for (e = hT->table[i]; e != 0 && counter != hT->n; e = e->next)
            {
                (*array)[counter].str = strdup(e->key);
                if (mode == 1 || mode == 2)
                    (*array)[counter].num = e->value;
                else
                    (*array)[counter].num = 0;
                if (mode == 0 || mode == 2)
                {
                    initArray(&(*array)[counter].locs, e->locs.size);
                    (*array)[counter].locs.used = e->locs.used;
                    memcpy((*array)[counter].locs.array, e->locs.array, e->locs.size * sizeof *(*array)[counter].locs.array);
                }
                counter++;
            }
    return counter;
}

/* destroy array of tuples */
void ArrayDestroy(tuple **array, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        free((*array)[i].str);
        (*array)[i].str = 0;
        freeArray(&(*array)[i].locs);
    }
    free(*array);
    *array = 0;
}
