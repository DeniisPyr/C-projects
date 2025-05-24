#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_VERSE_LENGTH 100

// ----------------------------------------------------------------------- 
// ----------------------------------------------------------------------- 

/**
 * @brief Represents a node in a singly-linked list of verses.
 */
typedef struct verse_t {
    char verse[MAX_VERSE_LENGTH];
    struct verse_t *next;
} Verse_t;

/**
 * @brief Creates a new verse node.
 * @param next Pointer to the next node in the list.
 * @param verse String content of the verse.
 * @return Pointer to the newly created node.
 */
Verse_t *createVerse(Verse_t *next, const char *verse) {
    Verse_t *newNode = (Verse_t *) malloc(sizeof(Verse_t));
    strncpy(newNode->verse, verse, MAX_VERSE_LENGTH - 1);
    newNode->verse[MAX_VERSE_LENGTH - 1] = '\0';
    newNode->next = next;
    return newNode;
}

/**
 * @brief Frees all nodes in a linked list.
 */
void freeList(Verse_t *node) {
    while (node) {
        Verse_t *tmp = node->next;
        free(node);
        node = tmp;
    }
}

/**
 * @brief Creates a deep copy of a linked list.
 * @param list Pointer to the list to copy.
 * @return Pointer to the head of the new list.
 */
Verse_t *copyList(const Verse_t *list) {
    if (!list) return NULL;
    const Verse_t *cur = list;
    Verse_t *head = createVerse(NULL, cur->verse);
    Verse_t *tail = head;
    cur = cur->next;

    while (cur) {
        tail->next = createVerse(NULL, cur->verse);
        tail = tail->next;
        cur = cur->next;
    }
    return head;
}

/**
 * @brief Comparison function for qsort to sort only alphabetic characters.
 */
int sortChars(const void *a, const void *b) {
    char c1 = *(char *)a;
    char c2 = *(char *)b;
    if (!isalpha(c1)) return 1;
    if (!isalpha(c2)) return -1;
    return (c1 > c2) - (c1 < c2);
}

/**
 * @brief Extracts only lowercase alphabetic characters and sorts them lexicographically.
 * @return A heap-allocated string of sorted alphabetic characters.
 */
char *lexSort(const char *line) {
    char *copy = strdup(line);
    size_t len = strlen(copy);

    for (size_t i = 0; i < len; ++i)
        if (isupper(copy[i]))
            copy[i] = tolower(copy[i]);

    qsort(copy, len, sizeof(char), sortChars);

    // Remove non-alphabetic characters
    size_t write = 0;
    for (size_t read = 0; read < len; ++read)
        if (isalpha(copy[read]))
            copy[write++] = copy[read];
    copy[write] = '\0';

    return copy;
}

/**
 * @brief Deletes a node from the list, updating the previous node.
 * @return The node following the deleted one.
 */
Verse_t *deleteNode(Verse_t *current, Verse_t *previous) {
    Verse_t *next = current->next;
    free(current);
    previous->next = next;
    return next;
}

/**
 * @brief Removes semantically duplicate verses from a linked list.
 * @return A new list with duplicates removed.
 */
Verse_t *removeCopies(const Verse_t *list) {
    Verse_t *copy = copyList(list);
    Verse_t *base = copy;

    while (base) {
        char *sortedBase = lexSort(base->verse);
        Verse_t *cur = base->next;
        Verse_t *prev = base;

        while (cur) {
            char *sortedCur = lexSort(cur->verse);
            if (strcmp(sortedCur, sortedBase) == 0)
                cur = deleteNode(cur, prev);
            else {
                prev = cur;
                cur = cur->next;
            }
            free(sortedCur);
        }
        free(sortedBase);
        base = base->next;
    }
    return copy;
}

// ----------------------------------------------------------------------- 
// ----------------------------------------------------------------------- 

int main(int argc, char *argv[]) {
    printf("LVL 1\n");
    Verse_t *lvl1_raw = createVerse(createVerse(createVerse(createVerse(createVerse(createVerse(createVerse(createVerse(NULL, "untbugglyOh freddled gr"), "cturations are toThy mi"), "reddled gruntbugglyOh f"), "on a lurgid bee"), "As plurdled gabbleblotchits"), "dled gruntbugglyOh fred"), "Thy micturations are to"), "Oh freddled gruntbuggly");
    Verse_t *lvl1 = removeCopies(lvl1_raw);

    printf("LVL 2\n");
    Verse_t *lvl2_raw = createVerse(createVerse(NULL, "seMASjak*666*AhoJ"), "Ahoj, jak se mas");
    Verse_t *lvl2 = removeCopies(lvl2_raw);
    assert(strcmp(lvl2->verse, "Ahoj, jak se mas") == 0 && lvl2->next == NULL);

    printf("LVL 3\n");
    Verse_t *lvl3_raw = createVerse(createVerse(createVerse(NULL, "cbacba"), "bbb"), "abcabc");
    Verse_t *lvl3 = removeCopies(lvl3_raw);
    assert(strcmp(lvl3->verse, "abcabc") == 0 && strcmp(lvl3->next->verse, "bbb") == 0 && lvl3->next->next == NULL);

    printf("LVL 4\n");
    Verse_t *lvl4_raw = createVerse(createVerse(createVerse(createVerse(createVerse(createVerse(createVerse(NULL, " AA  bbcc"), "bcab  CA"), "abca  BC"), "b aaa"), "aa  ab"), "  *"), "    ");
    Verse_t *lvl4 = removeCopies(lvl4_raw);
    assert(strcmp(lvl4->verse, "    ") == 0 && strcmp(lvl4->next->verse, "aa  ab") == 0 && strcmp(lvl4->next->next->verse, "abca  BC") == 0 && lvl4->next->next->next == NULL);

    printf("LVL 5\n");
    Verse_t *lvl5_raw = createVerse(createVerse(createVerse(createVerse(NULL, "prejtojeee"), "eR88*@8t*@(*(^%#*)o        EP7Je@!e      1J"), "PJEEErToej"), "Prej To JEEE");
    Verse_t *lvl5 = removeCopies(lvl5_raw);
    assert(strcmp(lvl5->verse, "Prej To JEEE") == 0 && lvl5->next == NULL);

    // Free memory
    freeList(lvl1_raw); freeList(lvl2_raw); freeList(lvl3_raw); freeList(lvl4_raw); freeList(lvl5_raw);
    freeList(lvl1);     freeList(lvl2);     freeList(lvl3);     freeList(lvl4);     freeList(lvl5);

    return 0;
}
