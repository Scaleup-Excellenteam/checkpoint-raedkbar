#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256
#define NAME_LEN 256
#define CELL_NUM 10
#define NUM_COURSES 10
#define NUM_CLASSES 10
#define NUM_LEVELS 12

const char* course_names[NUM_COURSES] = {
        "Mathematics",
        "Physics",
        "Chemistry",
        "Biology",
        "Robotics",
        "Geology",
        "History",
        "Civics",
        "English",
        "Hebrew"
};

struct student {
    char fname[NAME_LEN];
    char lname[NAME_LEN];
    char cell[CELL_NUM];
    int grades[NUM_COURSES];
    struct student* next_stud;
};

struct school {
    struct student* db[NUM_LEVELS][NUM_CLASSES];
};

static struct school S;

void insert_student(int level, int class, struct student* new_student) {
    struct student* head_student = S.db[level][class];
    if (head_student == NULL) {
        S.db[level][class] = new_student;
    } else {
        struct student* temp = head_student->next_stud;
        new_student->next_stud = head_student->next_stud;
        head_student->next_stud = new_student;
        new_student->next_stud = temp;
    }
}

struct student* make_student(const char* fname, const char* lname, const char* cell, int grades[]) {
    struct student* new_student = (struct student*) malloc(sizeof(struct student));
    if (new_student == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    strcpy(new_student->fname, fname);
    strcpy(new_student->lname, lname);
    strcpy(new_student->cell, cell);

    int i;
    for(i = 0; i < NUM_COURSES; i++) {
        new_student->grades[i] = *grades++;
    }

    new_student->next_stud = NULL;
    return new_student;
}


void parse_data(FILE* file) {
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {

        char fname[NAME_LEN], lname[NAME_LEN], cell[CELL_NUM];
        int grades[NUM_COURSES] = {0};

        int level, class;
        int num_read = sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
                              fname, lname, cell, &level, &class,
                              &grades[0], &grades[1], &grades[2], &grades[3], &grades[4],
                              &grades[5], &grades[6], &grades[7], &grades[8], &grades[9]);

        if (num_read != 15) {
            printf("Error reading line: %s\n", line);
            continue;
        }

        struct student* new_student = make_student(fname, lname, cell, grades);
        if (new_student != NULL) {
            level--;
            class--;
            insert_student(level, class, new_student);
        }
    }
}

void print_data() {
    int level;
    for (level = 0; level < NUM_LEVELS; level++) {
        int class;
        for (class = 0; class < NUM_CLASSES; class++) {
            struct student* current_student = S.db[level][class];

            while (current_student != NULL) {
                printf("Level: %d, Class: %d\n", level + 1, class + 1);
                printf("Name: %s %s\n", current_student->fname, current_student->lname);
                printf("Cell: %s\n", current_student->cell);
                printf("Grades:\n");
                int i;
                for (i = 0; i < NUM_COURSES; i++) {
                    printf("%s: %d\n", course_names[i], current_student->grades[i]);
                }

                printf("\n");
                current_student = current_student->next_stud;
            }
        }
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

    print_data();

    return 0;
}