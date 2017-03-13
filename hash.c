#include <stdio.h>
#include <stdlib.h>
#include "../m/lib/m_basics.h"

struct Hashtable {
	int size;
	int stored;
	double *values;
	char **keys;
};

struct Stack {
	double value;
	struct Stack *next;
};

unsigned long sdbm(unsigned char *str);
void init_hashtable(struct Hashtable *table);
void expand_hashtable(struct Hashtable *table);
void add_to_hashtable(struct Hashtable *table, char *key, double value);
double get_from_hashtable(struct Hashtable *table, char *key);

int main(void) {
	char key[100];
	double value;
	struct Hashtable table;

	init_hashtable(&table);

	for (int i = 0; i < 11; ++i) {
		scanf("%s %lf", key, &value);
		printf("%s = %lu\n", key, sdbm(key)%table.size);
		add_to_hashtable(&table, key, value);
	}

	while (true) {
		scanf("%s", key);
		printf("%s = %f\n", key, get_from_hashtable(&table, key));
	}

	return 0;
}


unsigned long sdbm(unsigned char *str) {
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

#define INITIAL_SIZE 8
void init_hashtable(struct Hashtable *table) {
	table->size = INITIAL_SIZE;
	table->stored = 0;
	table->values = malloc(sizeof(double) * INITIAL_SIZE);
	table->keys = malloc(sizeof(char*) * INITIAL_SIZE);
}

void add_to_hashtable(struct Hashtable *table, char *key, double value) {
	double load_factor = (double) table->stored / table->size;
	printf("load_factor %f\n", load_factor);

	if (load_factor >= 0.7) expand_hashtable(table);

	int i = sdbm(key) % table->size;
	table->values[i] = value;
	table->keys[table->stored] = concat("", key);
	++(table->stored);
}

void expand_hashtable(struct Hashtable *table) {
	printf("Expanding hashtable.\n");
	int n_size = table->size * 2;
	double *n_values = malloc(sizeof(double) * n_size);
	char **n_keys = malloc(sizeof(char*) * n_size);

	for (int i = 0; i < table->stored; ++i) {
		int ni = sdbm(table->keys[i]) % n_size;
		n_values[ni] = get_from_hashtable(table, table->keys[i]);
		n_keys[i] = table->keys[i];
		printf("%s from %li to %d\n", table->keys[i], sdbm(table->keys[i])%table->size, ni);
	}

	table->size = n_size;
	table->keys = n_keys;
	table->values = n_values;
}

double get_from_hashtable(struct Hashtable *table, char *key) {
	int i = sdbm(key) % table->size;
	return table->values[i];
}
