/* Test function for the hash table, kind-of half-assed, maybe I will improve it later
 * loads the file hash_inserts_100.txt before giving control to the user */

#include <unistd.h>
#include "hash.h"

enum option {ADD, CHECK, REMOVE, STATUS, EXIT};

int main(void) {
	char key[100];
	double value;
	int choice;
	struct Hashtable table;
	int stdin_n = fileno(stdin);
	int stdin_copy = dup(stdin_n);

	if ((freopen("hash_inserts_100.txt", "r", stdin)) == NULL) {
		perror("Could not open file");
		return 1;
	}

	hash_init(&table);

	while (true) {
		printf("0. add\n");
		printf("1. check\n");
		printf("2. remove\n");
		printf("3. status\n");
		printf("4. exit\n");

		scanf("%d", &choice);

		switch (choice) {
			case ADD:
				printf("Enter KEY VALUE: \n");
				if (scanf("%s %lf", key, &value) <= 0) dup2(stdin_copy, stdin_n);
				double *n_value = malloc(sizeof(double));
				*n_value = value;
				hash_add(&table, key, n_value);
				break;
			case CHECK:
				printf("Enter KEY: \n");
				scanf("%s", key);
				printf("%f\n", *((double *) hash_getv(&table, key)));
				break;
			case REMOVE:
				printf("Enter KEY: \n");
				scanf("%s", key);
				hash_remove(&table, key);
				break;
			case STATUS:
				printf(" "); // who knows why
				int biggest_chain = 0;

				for (int i = 0; i < table.size; ++i) {
					printf("%d: ", i);
					int chain = 0;
					for (struct Node *node = table.values[i]; node != NULL; node = node->next) {
						printf("%.2f (%s), ", *((double *)(node->value)), node->key);
						++chain;
					}

					if (chain > biggest_chain) biggest_chain = chain;
					putchar('\n');
				}

				printf("\nSize: %d\n", table.size);
				printf("Stored: %d\n", table.stored);
				printf("Load factor: %f\n", (double)table.stored/table.size);
				printf("Biggest chain: %d\n", biggest_chain);

				break;
			case EXIT:
				return 0;
		}
		putchar('\n');
	}

	return 0;
}

