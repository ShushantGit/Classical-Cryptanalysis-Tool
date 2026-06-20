#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT_SIZE 5000
#define ALPHABET_SIZE 26
#define PLAINTEXT_FILE "plain_text.txt"
#define MAPPING_FILE "substitution_mapping.txt"
#define OUTPUT_FILE "encrypted_message.txt"

/*
    Loads substitution mappings from a file.

    Mapping format:
    PlainLetter EncryptedLetter

    Example:
    A E
    B F
    C G
*/
void load_encryption_mapping(char encryption_map[]) {
    FILE *file = fopen(MAPPING_FILE, "r");

    if (!file) {
        printf("Error: Could not open mapping file (%s)\n", MAPPING_FILE);
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        encryption_map[i] = 'A' + i;
    }

    char plain, encrypted;

    while (fscanf(file, " %c %c", &plain, &encrypted) == 2) {
        plain = toupper(plain);
        encrypted = toupper(encrypted);

        if (plain >= 'A' && plain <= 'Z' && encrypted >= 'A' && encrypted <= 'Z') {
            encryption_map[plain - 'A'] = encrypted;
        }
    }

    fclose(file);
}

void encrypt_text(FILE *inputFile, FILE *outputFile, char encryption_map[]) {
    char ch;

    while ((ch = fgetc(inputFile)) != EOF) {
        if (isalpha(ch)) {
            char upper = toupper(ch);
            fputc(encryption_map[upper - 'A'], outputFile);
        } else {
            fputc(ch, outputFile);
        }
    }
}

int main() {
    FILE *inputFile = fopen(PLAINTEXT_FILE, "r");
    FILE *outputFile = fopen(OUTPUT_FILE, "w");
    char encryption_map[ALPHABET_SIZE];

    if (!inputFile) {
        printf("Error: Could not open plaintext file (%s)\n", PLAINTEXT_FILE);
        return 1;
    }

    if (!outputFile) {
        printf("Error: Could not create encrypted output file (%s)\n", OUTPUT_FILE);
        fclose(inputFile);
        return 1;
    }

    load_encryption_mapping(encryption_map);
    encrypt_text(inputFile, outputFile, encryption_map);

    fclose(inputFile);
    fclose(outputFile);

    printf("Encryption completed successfully.\n");
    printf("Encrypted text saved to %s\n", OUTPUT_FILE);

    return 0;
}