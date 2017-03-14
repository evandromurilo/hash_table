#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../m/lib/m_basics.h"

#define DEBUG 0
#define INITIAL_SIZE 13

struct Hashtable {
	int size;
	int stored;
	struct Node **values;
	char **keys;
};

struct Node {
	double value;
	char* key;
	struct Node *next;
};

unsigned long sdbm(unsigned char *str);
unsigned long djb2(unsigned char *str);
unsigned long fnv1a(unsigned char *str);
unsigned long hash_function(unsigned char *str);
void hash_init(struct Hashtable *table);
int hash_expand(struct Hashtable *table);
void hash_add(struct Hashtable *table, char *key, double value);
struct Node *hash_getn(struct Hashtable *table, char *key);
double hash_getv(struct Hashtable *table, char *key);
void prepend(struct Node **head, struct Node *new);
int hash_remove(struct Hashtable *table, char *key);
bool is_prime(int n);
int next_prime(int n);
