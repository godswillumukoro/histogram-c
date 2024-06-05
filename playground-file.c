#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

typedef struct
{
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

void to_lowercase(char *str)
{
    for (; *str; ++str)
        *str = tolower(*str);
}

int find_word(WordCount words[], int word_count, const char *word)
{
    for (int i = 0; i < word_count; ++i)
    {
        if (strcmp(words[i].word, word) == 0)
        {
            return i;
        }
    }
    return -1;
}

void print_histogram(WordCount words[], int word_count)
{
    printf("\nWord Frequency Histogram:\n");
    for (int i = 0; i < word_count; ++i)
    {
        printf("%s: ", words[i].word);
        for (int j = 0; j < words[i].count; ++j)
        {
            printf("*");
        }
        printf(" (%d)\n", words[i].count);
    }
}

int main()
{
    char sentence[MAX_TEXT_LEN];
    WordCount words[MAX_WORDS] = {0};
    int word_count = 0;

    FILE *file = fopen("./files/cat-story.txt", "r");
    if (file == NULL)
    {
        perror("Unable to open file");
        return 1;
    }

    // Read the entire file content into the sentence array
    size_t total_len = 0;
    while (fgets(sentence + total_len, sizeof(sentence) - total_len, file) != NULL)
    {
        total_len = strlen(sentence);
        if (total_len >= sizeof(sentence) - 1)
        {
            fprintf(stderr, "File too large to fit in buffer\n");
            fclose(file);
            return 1;
        }
    }
    fclose(file);

    // Print the file content for debugging purposes
    printf("File content:\n%s\n", sentence);

    // Remove trailing newline character if present
    size_t len = strlen(sentence);
    if (len > 0 && sentence[len - 1] == '\n')
    {
        sentence[len - 1] = '\0';
    }

    char *token = strtok(sentence, " ,.-!?\t\n");
    while (token != NULL)
    {
        to_lowercase(token);

        int index = find_word(words, word_count, token);
        if (index != -1)
        {
            words[index].count++;
        }
        else
        {
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
