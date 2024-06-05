#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORDS 1000
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 10000

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

void write_histogram_to_csv(WordCount words[], int word_count, const char *output_file)
{
    FILE *csv_file = fopen(output_file, "w");
    if (csv_file == NULL)
    {
        perror("Error: Unable to open output file");
        return;
    }

    fprintf(csv_file, "Word,Count\n");
    for (int i = 0; i < word_count; ++i)
    {
        fprintf(csv_file, "%s,%d\n", words[i].word, words[i].count);
    }

    fclose(csv_file);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <output.csv> <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }

    const char *output_file = argv[1];
    char sentence[MAX_TEXT_LEN];
    WordCount words[MAX_WORDS] = {0};
    int word_count = 0;

    for (int file_index = 2; file_index < argc; ++file_index)
    {
        FILE *file = fopen(argv[file_index], "r");
        if (file == NULL)
        {
            perror("Error: Unable to open file");
            continue;
        }

        // Read the entire file content into the sentence array
        size_t total_len = 0;
        while (fgets(sentence + total_len, sizeof(sentence) - total_len, file) != NULL)
        {
            total_len = strlen(sentence);
            if (total_len >= sizeof(sentence) - 1)
            {
                fprintf(stderr, "Error: File too large to fit in buffer. Maximum allowed size is %d characters.\n", MAX_TEXT_LEN - 1);
                fclose(file);
                return 1;
            }
        }
        fclose(file);

        // Remove trailing newline character if present
        size_t len = strlen(sentence);
        if (len > 0 && sentence[len - 1] == '\n')
        {
            sentence[len - 1] = '\0';
        }

        // Tokenize the file content and count words
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
                if (word_count >= MAX_WORDS)
                {
                    fprintf(stderr, "Error: Too many unique words. Increase MAX_WORDS.\n");
                    return 1;
                }
                strncpy(words[word_count].word, token, MAX_WORD_LEN);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0'; // Ensure null-termination
                words[word_count].count = 1;
                word_count++;
            }

            token = strtok(NULL, " ,.-!?\t\n");
        }
    }

    write_histogram_to_csv(words, word_count, output_file);

    return 0;
}
