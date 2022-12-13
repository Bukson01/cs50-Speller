// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 4002;

// Hash table
node *table[N];

// word counts
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int i = hash(word);
    node *head = table[i];

    while (head) {
        //compares the words case -insensitive
        if (strcasecmp(head->word, word) == 0)
            return true;
        head = head->next;
    }

    return false;
}

// Hash Function
unsigned int hash(const char *word)
{
    int number = 0;

    while (*word) {
        number += toupper(*word++);
        if (*word)
            number += ((int) toupper(*word++)) << 8;
    }

    return number % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Opens the dictionary as a readable file
    FILE *file;
    if ((file = fopen(dictionary, "r")) == NULL)
        return false;
    //Creates a new node by allocating memory for the node
    char *word = malloc((LENGTH + 1) * sizeof(char));
    if (word == NULL)
        return false;
    //Prints the words from the file into the word variable
    while (fscanf(file, "%s", word) != EOF) {
        unsigned int key = hash(word);
        node **head = &table[key];
        node *n = malloc(sizeof(node));

        strcpy(n->word, word);

        if (*head)
            n->next = *head;
        else
            n->next = NULL;

        *head = n;

        word_count++;
    }

    free(word);
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    int free_counter = 0;

    for (int i = N - 1; i >= 0; i--) {
        //creats a template node to store the node to be freed
        node *head = table[i], *temp = head;

        while (head) {
            //Points to the next node
            head = head->next;
            free(temp);
            temp = head;
            free_counter++;
        }
    }

    // checks if the number of freed node = total amount of words
    if (free_counter == word_count)
        return true;
    else
        return false;
}
