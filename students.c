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

struct student* make_student(const char* fname, const char* lname, const char* cell, char* courses[]) {
    struct student* new_student = malloc(sizeof(struct student));
    if (new_student == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    strncpy(new_student->fname, fname, NAME_LEN - 1);
    new_student->fname[NAME_LEN - 1] = '\0';

    strncpy(new_student->lname, lname, NAME_LEN - 1);
    new_student->lname[NAME_LEN - 1] = '\0';

    strncpy(new_student->cell, cell, CELL_NUM - 1);
    new_student->cell[CELL_NUM - 1] = '\0';

    for (int i = 0; i < NUM_COURSES; i++) {
        if (courses[i] != NULL) {
            new_student->courses[i] = strdup(courses[i]);
        } else {
            new_student->courses[i] = NULL;
        }
    }

    new_student->next_stud = NULL;
    return new_student;
}


void parse_data(FILE* file) {
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        char fname[NAME_LEN], lname[NAME_LEN], cell[CELL_NUM];
        char* courses[NUM_COURSES] = {0};

        int level, class;
        sscanf(line, "%s %s %s %d %d %s %s %s %s %s %s %s %s %s %s",
               fname, lname, cell, &level, &class,
               courses[0], courses[1], courses[2], courses[3], courses[4],
               courses[5], courses[6], courses[7], courses[8], courses[9]);

        struct student* new_student = make_student(fname, lname, cell, courses);
    }
}


int main() {
    FILE* file;
    char filename[] = "C:/Users/raedb/Documents/GitHub/checkpoint_raedkbar/students_with_class.txt";

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    parse_data(file);

    fclose(file);

    return 0;
};


