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
    for (i = 0; i < NUM_COURSES; i++) {
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
    int level, class;
    for (level = 0; level < NUM_LEVELS; level++) {
        for (class = 0; class < NUM_CLASSES; class++) {
            struct student* current_student = S.db[level][class];

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
        printf("Student %s %s already exists in the database.\n\n", fname, lname);
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
        printf("Student %s %s has been inserted into Level %d, Class %d.\n\n", fname, lname, level + 1, class + 1);
    }
}

void delete_student() {
    char fname[NAME_LEN], lname[NAME_LEN];

    printf("Enter first name of the student to delete: ");
    scanf("%s", fname);

    printf("Enter last name of the student to delete: ");
    scanf("%s", lname);

    struct student* prev_student = NULL;
    int level, class;
    for (level = 0; level < NUM_LEVELS; level++) {
        for (class = 0; class < NUM_CLASSES; class++) {
            struct student* current_student = S.db[level][class];

            while (current_student != NULL) {
                if (strcmp(current_student->fname, fname) == 0 && strcmp(current_student->lname, lname) == 0) {
                    if (prev_student == NULL) {
                        S.db[level][class] = current_student->next_stud;
                    } else {
                        prev_student->next_stud = current_student->next_stud;
                    }

                    free(current_student);
                    printf("Student %s %s has been deleted from the database.\n\n", fname, lname);
                    return;
                }

                prev_student = current_student;
                current_student = current_student->next_stud;
            }
        }
    }

    printf("Student %s %s not found in the database.\n\n", fname, lname);
}

void update_student() {
    char fname[NAME_LEN], lname[NAME_LEN];

    printf("Enter first name: ");
    scanf("%s", fname);

    printf("Enter last name: ");
    scanf("%s", lname);

    struct student* found_student = find_student_by_name(fname, lname);
    if (found_student == NULL) {
        printf("Student %s %s not found in the database.\n", fname, lname);
        return;
    }

    char choice;
    printf("Select information to update:\n");
    printf("1. First Name\n");
    printf("2. Last Name\n");
    printf("3. Cell Number\n");
    printf("4. Grade\n");
    printf("Enter your choice: ");
    scanf("%s", &choice);

    switch (choice) {
        case '1': {
            char new_fname[NAME_LEN];
            printf("Enter new first name: ");
            scanf("%s", new_fname);
            strncpy(found_student->fname, new_fname, NAME_LEN - 1);
            found_student->fname[NAME_LEN - 1] = '\0';
            printf("First name updated.\n");
            break;
        }
        case '2': {
            char new_lname[NAME_LEN];
            printf("Enter new last name: ");
            scanf("%s", new_lname);
            strncpy(found_student->lname, new_lname, NAME_LEN - 1);
            found_student->lname[NAME_LEN - 1] = '\0';
            printf("Last name updated.\n");
            break;
        }
        case '3': {
            char new_cell[CELL_NUM];
            printf("Enter new cell number: ");
            scanf("%s", new_cell);
            strncpy(found_student->cell, new_cell, CELL_NUM - 1);
            found_student->cell[CELL_NUM - 1] = '\0';
            printf("Cell number updated.\n");
            break;
        }
        case '4': {
            int grade_choice;
            printf("Select the grade to update (1-%d): ", NUM_COURSES);
            scanf("%d", &grade_choice);

            if (grade_choice < 1 || grade_choice > NUM_COURSES) {
                printf("Invalid grade choice. No grades updated.\n\n");
                break;
            }

            printf("Enter new grade for %s: ", course_names[grade_choice - 1]);
            scanf("%d", &found_student->grades[grade_choice - 1]);
            printf("Grade for %s updated.\n", course_names[grade_choice - 1]);
            break;
        }
        default:
            printf("Invalid choice. No information updated.\n\n");
            break;
    }
}


void search_student_by_name() {
    char fname[NAME_LEN], lname[NAME_LEN];

    printf("Enter first name: ");
    scanf("%s", fname);

    printf("Enter last name: ");
    scanf("%s", lname);

    int level, class;
    for (level = 0; level < NUM_LEVELS; level++) {
        for (class = 0; class < NUM_CLASSES; class++) {
            struct student *current_student = S.db[level][class];
            if (S.db[level][class] == current_student) {
                printf("\nStudent Found:\n");
                printf("Level: %d, Class: %d\n", level + 1, class + 1);
                printf("Name: %s %s\n", current_student->fname, current_student->lname);
                printf("Cell: %s\n", current_student->cell);
                printf("Grades:\n");

                int i;
                for (i = 0; i < NUM_COURSES; i++) {
                    printf("%s: %d\n", course_names[i], current_student->grades[i]);
                }
                return;
            }
        }
    }

    printf("\nStudent %s %s not found in the database.\n", fname, lname);
}


void present_top_students_by_subject(const char* subject_name) {
    printf("\nTop Ten Students in %s by Grade:\n", subject_name);

    struct student* top_students[NUM_LEVELS][NUM_CLASSES][10] = {0};

    int level;
    for (level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            struct student* current_student = S.db[level][class_num];

            while (current_student != NULL) {
                int total_grade = 0;

                int i;
                for (i = 0; i < NUM_COURSES; i++) {
                    if (strcmp(course_names[i], subject_name) == 0) {
                        total_grade += current_student->grades[i];
                        break;
                    }
                }

                int min_grade_index = 0;
                for (int i = 1; i < 10; i++) {
                    if (top_students[level][class_num][i] != NULL &&
                        top_students[level][class_num][i]->grades[NUM_COURSES - 1] <
                        top_students[level][class_num][min_grade_index]->grades[NUM_COURSES - 1]) {
                        min_grade_index = i;
                    }
                }

                if (top_students[level][class_num][min_grade_index] == NULL ||
                    total_grade > top_students[level][class_num][min_grade_index]->grades[NUM_COURSES - 1]) {
                    top_students[level][class_num][min_grade_index] = current_student;
                }

                current_student = current_student->next_stud;
            }
        }
    }

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class = 0; class < NUM_CLASSES; class++) {
            int has_top_students = 0;

            int i;
            for (i = 0; i < 10; i++) {
                if (top_students[level][class][i] != NULL) {
                    has_top_students = 1;
                    break;
                }
            }

            if (has_top_students) {
                printf("\nLevel: %d, Class: %d\n", level + 1, class + 1);
                printf("====================================\n");
                printf("Top Ten Students in %s:\n", subject_name);

                for (i = 0; i < 10; i++) {
                    struct student* current_student = top_students[level][class][i];
                    if (current_student != NULL) {
                        printf("%d. %s %s\n", i + 1, current_student->fname, current_student->lname);
                        printf("   Total Grade: %d\n", current_student->grades[NUM_COURSES - 1]);
                    }
                }
            }
        }
    }
}

void present_top_ten() {
    printf("Available Subjects:\n");
    int i;
    for (i = 0; i < NUM_COURSES; i++) {
        printf("%d. %s\n", i + 1, course_names[i]);
    }

    int choice;
    printf("Enter the number corresponding to the subject: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > NUM_COURSES) {
        printf("Invalid choice. Please try again.\n\n");
        return;
    }

    present_top_students_by_subject(course_names[choice - 1]);
}

int is_eligible_for_departure(struct student* s) {
    if (s == NULL) {
        return 0;
    }

    if (s->grades[NUM_COURSES - 1] < 60) {
        return 0;
    }

    int i;
    for (i = 0; i < NUM_COURSES - 1; i++) { // Exclude the last element (level) from the loop
        if (s->grades[i] < 60) {
            return 0;
        }
    }

    return 1;
}

void present_eligible_for_departure() {
    printf("\nStudents Eligible for Departure (Level 12, Grades >= 60 in All Subjects):\n");

    int class;
    for (class = 0; class < NUM_CLASSES; class++) {
        struct student* current_student = S.db[NUM_LEVELS - 1][class];

        while (current_student != NULL) {
            if (is_eligible_for_departure(current_student)) {
                printf("Name: %s %s\n", current_student->fname, current_student->lname);
                printf("Cell: %s\n", current_student->cell);
                printf("Grades:\n");

                int i;
                for (i = 0; i < NUM_COURSES; i++) {
                    printf("%s: %d\n", course_names[i], current_student->grades[i]);
                }
                printf("\n");
            }

            current_student = current_student->next_stud;
        }
    }
}

void present_avg_course_level() {
    printf("\nAverage Grade per Course per Level:\n");

    int total_grades[NUM_COURSES][NUM_LEVELS] = {0};
    int num_students[NUM_COURSES][NUM_LEVELS] = {0};

    int level;
    for (level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            struct student* current_student = S.db[level][class_num];

            while (current_student != NULL) {
                int course;
                for (course = 0; course < NUM_COURSES; course++) {
                    total_grades[course][level] += current_student->grades[course];
                    if (current_student->grades[course] > 0) {
                        num_students[course][level]++;
                    }
                }

                current_student = current_student->next_stud;
            }
        }
    }


    for (level = 0; level < NUM_LEVELS; level++) {
        printf("\nLevel %d\n", level + 1);
        printf("----------------------------\n");
        int course;
        for (course = 0; course < NUM_COURSES; course++) {
            if (num_students[course][level] > 0) {
                double average = (double)total_grades[course][level] / num_students[course][level];
                printf("%s: %.2lf\n", course_names[course], average);
            } else {
                printf("%s: N/A (No students enrolled)\n", course_names[course]);
            }
        }
    }
}



void db_access_menu() {
    char choice;
    printf("\n-- Database Access Menu --\n");
    printf("*======================================*\n");
    printf("1. Insert student\n");
    printf("2. Delete student\n");
    printf("3. Update student\n");
    printf("4. Search for a student by first name and last name\n");
    printf("5. Present the top ten students in each grade in a particular subject\n");
    printf("6. Present the students who are candidates for departure\n");
    printf("7. Present average per course per level\n");
    printf("8. Export DB\n");
    printf("0. Exit\n");
    printf("*======================================*\n\n");
    printf("Enter your choice: ");
    scanf("%s", &choice);

    switch (choice) {
        case '1':
            insert_new_student();
            break;
        case '2':
            delete_student();
            break;
        case '3':
            update_student();
            break;
        case '4':
            search_student_by_name();
            break;
        case '5':
            present_top_ten();
            break;
        case '6':
            present_eligible_for_departure();
            break;
        case '7':
            present_avg_course_level();
            break;
        case '8':

            break;
        case '0':
            printf("Exiting the system.\n\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n\n");
            break;
    }

    db_access_menu();
}

int main() {
    FILE* file;
    char filename[] = "students_with_class.txt";

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