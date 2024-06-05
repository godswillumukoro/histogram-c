#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

void to_lowercase(char *str) {
    for (; *str; ++str) *str = tolower(*str);
}

int find_word(WordCount words[], int word_count, const char *word) {
    for (int i = 0; i < word_count; ++i) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void print_histogram(WordCount words[], int word_count) {
    printf("\nWord Frequency Histogram:\n");
    for (int i = 0; i < word_count; ++i) {
        printf("%s: ", words[i].word);
        for (int j = 0; j < words[i].count; ++j) {
            printf("*");
        }
        printf(" (%d)\n", words[i].count);
    }
}

int main() {
    char sentence[1000];
    WordCount words[MAX_WORDS] = {0};
    int word_count = 0;

    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin);

    // Remove trailing newline character if present
    size_t len = strlen(sentence);
    if (len > 0 && sentence[len-1] == '\n') {
        sentence[len-1] = '\0';
    }

    char *token = strtok(sentence, " ,.-!?\t\n");
    while (token != NULL) {
        to_lowercase(token);

        int index = find_word(words, word_count, token);
        if (index != -1) {
            words[index].count++;
        } else {
            strncpy(words[word_count].word, token, MAX_WORD_LEN);
            words[word_count].word[MAX_WORD_LEN - 1] = '\0'; // Ensure null-termination
            words[word_count].count = 1;
            word_count++;
        }

        token = strtok(NULL, " ,.-!?\t\n");
    }

    print_histogram(words, word_count);

    return 0;
}
