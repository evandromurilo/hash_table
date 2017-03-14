#include "hash.h"

unsigned long hash_function(unsigned char *str) {
	return fnv1a(str);
}

unsigned long sdbm(unsigned char *str) {
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

unsigned long djb2(unsigned char *str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

unsigned long fnv1a(unsigned char *str) {
	unsigned long hash = 14695981039346656037;
	int c;

	while (c = *str++) {
		hash = (hash ^ c) * 1099511628211;
	}

	return hash;
}

void hash_init(struct Hashtable *table) {
	table->size = INITIAL_SIZE;
	table->stored = 0;
	table->keys = malloc(sizeof(char*) * INITIAL_SIZE);
	table->values = malloc(sizeof(struct Node*) * INITIAL_SIZE);

	for (int i = 0; i < INITIAL_SIZE; ++i) table->values[i] = NULL;
}

void hash_add(struct Hashtable *table, char *key, double value) {
	double load_factor = (double) table->stored / table->size;
	char* a_key = concat("", key);

	if (DEBUG) printf("load_factor %f\n", load_factor);

	if (load_factor >= 0.7) hash_expand(table);

	struct Node *check = hash_getn(table, key);

	if (check != NULL) check->value = value;
	else {
		struct Node *new = malloc(sizeof(struct Node));
		new->value = value;
		new->key = a_key;

		int i = hash_function(key) % table->size;
		prepend(&(table->values[i]), new);
		table->keys[table->stored] = a_key;
		++(table->stored);
	}
}

int hash_expand(struct Hashtable *table) {
	if (DEBUG) printf("Expanding hashtable.\n");
	int n_size = next_prime(table->size * 2);
	struct Node **n_values = malloc(sizeof(struct Node*) * n_size);
	char **n_keys = malloc(sizeof(char*) * n_size);

	for (int i = 0; i < n_size; ++i) n_values[i] = NULL;

	for (int i = 0; i < table->stored; ++i) {
		int ni = hash_function(table->keys[i]) % n_size;
		struct Node *node = hash_getn(table, table->keys[i]);

		if (node == NULL) {
			if (DEBUG) printf("Could not expand hash table.\n");
			free(n_values);
			free(n_keys);
			return 1;
		}

		prepend(&(n_values[ni]), node);

		n_keys[i] = table->keys[i];
		if (DEBUG) printf("%s from %li to %d\n", table->keys[i], hash_function(table->keys[i])%table->size, ni);
	}

	table->size = n_size;
	table->keys = n_keys;
	table->values = n_values;
	return 0;
}

struct Node *hash_getn(struct Hashtable *table, char *key) {
	if (DEBUG) printf("Getting node %s.\n", key);
	int i = hash_function(key) % table->size;

	struct Node *curr = table->values[i];

	if (DEBUG) if (curr == NULL) printf("there is nothing here at index %d!\n", i);
	while (curr != NULL && strcmp(curr->key, key) != 0) {
		if (DEBUG) printf("its not %s\n", curr->key);
		curr = curr->next;
	}

	return curr;
}

double hash_getv(struct Hashtable *table, char *key) {
	struct Node *curr = hash_getn(table, key);
	return curr == NULL ? 0 : curr->value;
}

void prepend(struct Node **head, struct Node *new) {
	new->next = *head;
	*head = new;
}

int hash_remove(struct Hashtable *table, char *key) {
	if (DEBUG) printf("Removing node %s.\n", key);
	int i = hash_function(key) % table->size;
	struct Node *prev, *curr;

	prev = NULL;
	curr = table->values[i];
	for (; curr != NULL && strcmp(curr->key, key) != 0; prev = curr, curr = curr->next);

	if (curr == NULL) return 1;
	else if (prev == NULL) {
		table->values[i] = curr->next;
	}
	else {
		prev->next = curr->next;
	}
	free(curr);

	return 0;
}

bool is_prime(int n) {
	if (n == 2) return true;
	if (n == 3) return true;
	if (n % 2 == 0) return false;
	if (n % 3 == 0) return false;

	int i = 5;
	int w = 2;

	while (i*i <= n) { // would it be better to calculate sqrt of n?
		if (n % i == 0) return false;
		i += w;
		w = 6-w; // alterns between 2 and 4, skipping multiples of 2 and 3
	}

	return true;
}

int next_prime(int n) {
	if (n%2==0) ++n;
	else n+=2;

	int w = 2;
	while (true) {
		if (is_prime(n)) return n;
		n += w;
		w = 6-w;
	}
}
