#include "m_basics.h"

char *concat(char *a, char *b) {
	int len_a = len(a);
	int len_b = len(b);
	char *c = malloc(len_a + len_b + 1);

	if (c == NULL) {
		printf("ERROR: malloc failed in concat\n");
		exit(EXIT_FAILURE);
	}

	while (*a != '\0') *(c++) = *(a++);
	while (*b != '\0') *(c++) = *(b++);
	*c = '\0';

	return c-len_a-len_b;
}

int len(char *s) {
	char *a = s;
	while (*a != '\0') ++a;

	return a-s;
}

int strcmpr_first(char *a, char *b, int n) {
	for (int i = 0; i < n; ++i) 
		if (a[i] != b[i]) return a[i] - b[i];

	return 0;
}

int pushed = EOF-1;

int read_char() {
	char c;

	if (pushed >= EOF) {
		c = pushed;
		pushed = EOF-1;
	}
	else {
		c = getchar();
	}

	return c;
}

int push_char(int c) {
	pushed = c;
	return c;
}

int read(char *arr, int max) {
	int c, i = 0;
	while (i < max && (c = read_char()) != EOF) arr[i++] = c;

	arr[i] = '\0';
	return i;
}

int read_line(char *arr, int size) {
	int c, i = 0;
	while (i < size-1 && (c = read_char()) != EOF && c != '\n') arr[i++] = c;

	arr[i] = '\0';
	return i;
}

int discard_line() {
	int c, n = 0;
	while ((c = read_char()) != '\n' && c != EOF) ++n;

	return n;
}

int discard_until(int goal) {
	int c, n = 0;
	while ((c = read_char()) != goal && c != EOF) ++n;

	push_char(c);
	return n;
}

int discard_while(char *goals) {
	int c, n = 0;
	bool found = true;

	while (found && (c = read_char()) != EOF) {
		found = false;
		for (int i = 0; goals[i] != '\0'; ++i) {
			if (goals[i] == c) {
				found = true;
				++n;
				break;
			}
		}
	}

	push_char(c);
	return n;
}

int read_until(int goal, char* str, int size) {
	int c, i;

	for (i = 0; i < size-1 && (c = read_char()) != goal && c != EOF; ++i, ++str) {
		*str = c;
	}

	*str = '\0';
	return i;
}

#define MAX_WORD_SIZE 100
int split(char** arr, int size, char* str, char goal) {
	char word[MAX_WORD_SIZE+1];
	int i, c, total = 0;

	while (total < size && *str != '\0') {
		while (*str == goal && *str != '\0') ++str;

		if (*str == '\0') break;

		for (i = 0; i < MAX_WORD_SIZE && *str != goal && *str != '\0'; 
				++i, ++str) {
			word[i] = *str;
		}
		word[i] = '\0';

		arr[total] = malloc(i);
		strcpy(arr[total], word);
		printf("Read word: %s\n", arr[total]);
		++total;
	}

	return total;
}
