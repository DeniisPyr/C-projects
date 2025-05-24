#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1000

// ----------------------------------------------------------------------- 
// ----------------------------------------------------------------------- 


/**
 * @brief Checks if a string contains only digits.
 * @param str Pointer to the string.
 * @return 1 if the string is numeric, 0 otherwise.
 */
int isNumeric(const char *str){
    for(size_t i = 0; str[i] != '\0'; i++){
        if(!isdigit(str[i])) 
            return 0;
    }
    return 1;
}

/**
 * @brief Prints a valid partition.
 * @param parts Array of string parts.
 * @param count Number of parts in the array.
 */
void printPartition(char **parts, int count){
    printf("* ");
    for(int i = 0; i < count; i++){
        if(i > 0) 
            printf(",");
        printf("%s", parts[i]);
    }
    printf("\n");
}

/**
 * @brief Recursively partitions a string of digits into valid increasing sequences under given rules.
 * @param sequence Pointer to the remaining input string.
 * @param current Array to hold current partition.
 * @param depth Current depth of recursion / index in parts.
 * @param count Pointer to total valid partitions counter.
 * @param skipPrint If non-zero, skip printing and only count.
 */
void findPartitions(const char *sequence, char **current, int depth, int *count, int skipPrint){
    size_t len = strlen(sequence);
    if(len == 0){
        if(!skipPrint) 
            printPartition(current, depth);
        (*count)++;
        return;
    }

    for(size_t i = 1; i <= len; i++){
        char part[SIZE];
        strncpy(part, sequence, i);
        part[i] = '\0';

        int valid = 1;

        if(depth > 0){
            long prev = atol(current[depth - 1]);
            long curr = atol(part);
            if(prev % 2 == 0 && curr < prev) 
                valid = 0;
        }

        if(valid){
            current[depth] = strdup(part);
            findPartitions(sequence + i, current, depth + 1, count, skipPrint);
            free(current[depth]);
        }
    }
}

// ----------------------------------------------------------------------- 
// ----------------------------------------------------------------------- 

int main(int argc, char *argv[]) {
    char line[SIZE];
    char *currentParts[SIZE];
    
    printf("Sequence:\n");
    while(fgets(line, sizeof(line), stdin)){
        line[strcspn(line, "\n")] = '\0'; // Remove newline character

        if(line[0] != '?' && line[0] != '#'){
            printf("Invalid input.\n");
            return 1;
        }

        char *iter = line + 1;
        while(isspace(*iter)) iter++;

        if(*iter == '\0'){
            printf("Invalid input.\n");
            return 1;
        }

        if(!isNumeric(iter)){
            printf("Invalid input.\n");
            return 1;
        }

        int count = 0;
        if(line[0] == '?'){
            findPartitions(iter, currentParts, 0, &count, 0);
        } 
        else if(line[0] == '#'){
            findPartitions(iter, currentParts, 0, &count, 1);
        }
        printf("Total: %d\n", count);
    }

    return 0;
}
