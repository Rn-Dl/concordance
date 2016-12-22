//hashT.h
#ifndef HASHT_H
#define HASHT_H

#define INITIAL_SIZE (10240)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)
#define MULTIPLIER (97)

typedef struct hashTable *HashT;

typedef struct
{
    char *str;
    int num;
} tuple;

/* create a new empty hash table */
HashT HashTCreate(void);

/* destroy a hash table */
void HashTDestroy(HashT);

/* add a new key-value pair into an existing hashTable */
void HashTAdd(HashT, const char *key, const unsigned int value);

/* return the most recently added value associated with a key
    or 0 if no matching key is present */
const unsigned int HashTSearch(HashT, const char *key);

/* increment value associated with a key */
void HashTIncValue(HashT, const char *key);

/* convert hash table to array of tuples */
unsigned int HashTToArray(HashT hT, tuple **array);

/* destroy array of tuples */
void ArrayDestroy(tuple **array, const unsigned int size);

#endif