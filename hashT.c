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
    struct element *next;
};

struct hashTable
{
    unsigned int size; //size of the pointer table
    unsigned int n;    //number of elements stored
    struct element **table;
};

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
            free(e);
        }
    }

    free(hT->table);
    free(hT);
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
static void grow(HashT hT)
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
            HashTAdd(d2, e->key, e->value);
        }
    }

    swap = *hT;
    *hT = *d2;
    *d2 = swap;

    HashTDestroy(d2);
}

/* add a new key-value pair into an existing hashTable */
void HashTAdd(HashT hT, const char *key, const unsigned int value)
{
    struct element *e;
    unsigned long h;

    assert(key);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    e->value = value;

    h = hash_function(key) % hT->size;

    e->next = hT->table[h];
    hT->table[h] = e;

    hT->n++;

    // grow table if there is not enough space
    if (hT->n >= hT->size * MAX_LOAD_FACTOR)
    {
        grow(hT);
    }
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

/* increment value associated with a key */
void HashTIncValue(HashT hT, const char *key)
{
    struct element *e;

    for (e = hT->table[hash_function(key) % hT->size]; e != 0; e = e->next)
    {
        if (!strcmp(e->key, key))
        {
            // increment it
            e->value++;
            return;
        }
    }
    HashTAdd(hT, key, 1);
}

/* convert hash table to array of tuples */
unsigned int HashTToArray(HashT hT, tuple **array)
{
    struct element *e;
    unsigned int counter = 0;
    *array = malloc(hT->n * sizeof(tuple));

    for (unsigned int i = 0; i < hT->size; i++)
        if (hT->table[i] != 0)
            for (e = hT->table[i]; e != 0 && counter != hT->n; e = e->next)
            {
                (*array)[counter].str = strdup(e->key);
                (*array)[counter].num = e->value;
                counter++;
            }
    return counter;
}

/* destroy array of tuples */
void ArrayDestroy(tuple **array, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        free((*array)[i].str);
    free(*array);
}