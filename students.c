#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256
#define NAME_LEN 10
#define CELL_NUM 10
#define NUM_COURSES 10
#define NUM_CLASSES 10
#define NUM_LEVELS 7

struct student {
    char fname[NAME_LEN];
    char lname[NAME_LEN];
    char cell[CELL_NUM];
    char* courses[NUM_COURSES];
    struct student* next_stud;
};

struct school {
    struct student db[NUM_LEVELS][NUM_CLASSES];
};

static struct school S;


void init_db(FILE* file) {


    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        char *word = strtok(line, " ");
        while (word != NULL) {
            printf("%s\n", word);
            word = strtok(NULL, " ");
        }
    }
}


int main() {
    FILE* file;
    char filename[] = "C:/Users/raedb/Documents/GitHub/checkpoint_raedkbar/students_with_class.txt";

    file = fopen(filename, "r"); // Open the file in write mode
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    init_db(file);

    fclose(file);

    return 0;
};


