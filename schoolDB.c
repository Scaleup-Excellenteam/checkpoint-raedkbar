#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256
#define NAME_LEN 256
#define CELL_NUM 128
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
                int course;
                for (course = 0; course < NUM_COURSES; course++) {
                    printf("%s: %d\n", course_names[course], current_student->grades[course]);
                }

                printf("\n");
                current_student = current_student->next_stud;
            }
        }
    }
}

struct student* find_student_by_name(const char* fname, const char* lname) {
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            struct student* current_student = S.db[level][class_num];

            while (current_student != NULL) {
                if (strcmp(current_student->fname, fname) == 0 && strcmp(current_student->lname, lname) == 0) {
                    return current_student;
                }

                current_student = current_student->next_stud;
            }
        }
    }

    return NULL;
}

void insert_new_student() {
    char fname[NAME_LEN], lname[NAME_LEN], cell[CELL_NUM];
    int grades[NUM_COURSES];

    printf("Enter first name: ");
    scanf("%s", fname);

    printf("Enter last name: ");
    scanf("%s", lname);

    struct student* existing_student = find_student_by_name(fname, lname);
    if (existing_student != NULL) {
        printf("Student %s %s already exists in the database.\n", fname, lname);
        return;
    }

    printf("Enter cell number: ");
    scanf("%s", cell);

    int level, class;
    printf("Enter level and class (e.g., level class): ");
    scanf("%d %d", &level, &class);

    printf("Enter grades for %d courses:\n", NUM_COURSES);
    int i;
    for (i = 0; i < NUM_COURSES; i++) {
        printf("Enter grade for %s: ", course_names[i]);
        scanf("%d", &grades[i]);
    }

    struct student* new_student = make_student(fname, lname, cell, grades);
    if (new_student != NULL) {
        level--;
        class--;
        insert_student(level, class, new_student);
        printf("Student %s %s has been inserted into Level %d, Class %d.\n", fname, lname, level + 1, class + 1);
    }
}

void db_access_menu() {
    int choice;
    printf("1. Insert student\n");
    printf("2. Delete student\n");
    printf("3. Update student\n");
    printf("4. Search for a student by first name and last name\n");
    printf("5. Present the top ten students in each grade in a particular subject\n");
    printf("6. Present the students who are candidates for departure, according to parameters of your choice.\n");
    printf("7. Present average per course per layer\n");
    printf("8. Export DB\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            insert_new_student();
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        case 6:

            break;
        case 7:

            break;
        case 8:

            break;
        case 0:
            printf("Exiting the system.\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }

    db_access_menu();
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
    db_access_menu();

    return 0;
}