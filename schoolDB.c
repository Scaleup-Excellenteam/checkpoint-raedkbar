/********************************** includes ********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************** consts **********************************/

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

/********************************** structs *********************************/

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

/********************************** statics *********************************/

static struct school S;

/********************************* functions ********************************/

// Function to insert a new student into the database
void insert_student(int level, int class, struct student* new_student) {
    // Get the head student of the specified level and class
    struct student* head_student = S.db[level][class];

    // If there are no students in the specified level and class, make the new student the head
    if (head_student == NULL) {
        S.db[level][class] = new_student;
    } else {
        // Otherwise, insert the new student at the beginning of the linked list
        struct student* temp = head_student->next_stud;
        new_student->next_stud = head_student->next_stud;
        head_student->next_stud = new_student;
        new_student->next_stud = temp;
    }
}

/****************************************************************************/

// Function to create a new student and initialize its data
struct student* make_student(const char* fname, const char* lname, const char* cell, int grades[]) {
    // Allocate memory for the new student
    struct student* new_student = (struct student*) malloc(sizeof(struct student));

    // Check if memory allocation was successful
    if (new_student == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Copy the first name, last name, and cell number of the student
    strcpy(new_student->fname, fname);
    strcpy(new_student->lname, lname);
    strcpy(new_student->cell, cell);

    // Copy the grades for each course
    for (int course = 0; course < NUM_COURSES; course++) {
        new_student->grades[course] = *grades++;
    }

    // Set the next student pointer to NULL, as this is a new student
    new_student->next_stud = NULL;

    // Return the newly created student
    return new_student;
}


/****************************************************************************/

// Function to initialize the database from a file
void init_db(FILE* file) {
    char line[MAX_LINE_LEN];

    // Read each line from the file and create a new student based on the data
    while (fgets(line, sizeof(line), file)) {

        // Variables to store student information
        char fname[NAME_LEN], lname[NAME_LEN], cell[CELL_NUM];
        int grades[NUM_COURSES] = {0};
        int level, class;

        // Parse the line to extract student information
        int num_read = sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
                              fname, lname, cell, &level, &class,
                              &grades[0], &grades[1], &grades[2], &grades[3], &grades[4],
                              &grades[5], &grades[6], &grades[7], &grades[8], &grades[9]);

        // Check if the line contains the correct number of elements
        if (num_read != 15) {
            printf("Error reading line: %s\n", line);
            continue;
        }

        // Create a new student based on the extracted data
        struct student* new_student = make_student(fname, lname, cell, grades);

        // If the student creation is successful, adjust the level and class indices (zero-based)
        if (new_student != NULL) {
            level--;
            class--;

            // Insert the new student into the database
            insert_student(level, class, new_student);
        }
    }
}


/****************************************************************************/

// Function to print the data of all students in the database
void print_data() {
    int level;
    // Iterate through each level
    for (level = 0; level < NUM_LEVELS; level++) {
        int class;
        // Iterate through each class in the level
        for (class = 0; class < NUM_CLASSES; class++) {
            // Get the first student in the current level and class
            struct student* current_student = S.db[level][class];

            // Loop through all students in the linked list for the current level and class
            while (current_student != NULL) {
                printf("Level: %d, Class: %d\n", level + 1, class + 1);
                printf("Name: %s %s\n", current_student->fname, current_student->lname);
                printf("Cell: %s\n", current_student->cell);
                printf("Grades:\n");
                int course;
                // Print the grades for each course
                for (course = 0; course < NUM_COURSES; course++) {
                    printf("%s: %d\n", course_names[course], current_student->grades[course]);
                }

                printf("\n");
                // Move to the next student in the linked list
                current_student = current_student->next_stud;
            }
        }
    }
}


/****************************************************************************/

// Function to find a student in the database by first name and last name
struct student* find_student_by_name(const char* fname, const char* lname) {
    int level, class;

    // Iterate through each level and class in the school database
    for (level = 0; level < NUM_LEVELS; level++) {
        for (class = 0; class < NUM_CLASSES; class++) {
            // Start with the head student of the current level and class
            struct student* current_student = S.db[level][class];

            // Traverse the linked list of students at the current level and class
            while (current_student != NULL) {
                // Check if the first name and last name match the input
                if (strcmp(current_student->fname, fname) == 0 && strcmp(current_student->lname, lname) == 0) {
                    // If a matching student is found, return the pointer to that student
                    return current_student;
                }

                // Move to the next student in the linked list
                current_student = current_student->next_stud;
            }
        }
    }

    // Return NULL if no matching student is found in the database
    return NULL;
}


/****************************************************************************/

// Function to insert a new student into the database
void insert_new_student() {
    char fname[NAME_LEN], lname[NAME_LEN], cell[CELL_NUM];
    int grades[NUM_COURSES];

    // Read student's first name and last name from the user
    printf("Enter first name: ");
    scanf("%s", fname);

    printf("Enter last name: ");
    scanf("%s", lname);

    // Check if the student already exists in the database
    struct student* existing_student = find_student_by_name(fname, lname);
    if (existing_student != NULL) {
        printf("Student %s %s already exists in the database.\n\n", fname, lname);
        return;
    }

    // Read student's cell number from the user
    printf("Enter cell number: ");
    scanf("%s", cell);

    // Read student's level and class from the user
    int level, class;
    printf("Enter level and class (e.g., level class): ");
    if (scanf("%d %d", &level, &class) != 2 || level < 1 || level > NUM_LEVELS || class < 1 || class > NUM_CLASSES) {
        printf("Invalid level or class number. Please try again.\n\n");
        return;
    }

    // Read student's grades for each course
    printf("Enter grades for %d courses:\n", NUM_COURSES);
    for (int course = 0; course < NUM_COURSES; course++) {
        printf("Enter grade for %s: ", course_names[course]);
        scanf("%d", &grades[course]);
    }

    // Create a new student
    struct student* new_student = make_student(fname, lname, cell, grades);
    if (new_student != NULL) {
        level--;
        class--;

        // Insert the new student into the database
        insert_student(level, class, new_student);
        printf("Student %s %s has been inserted into Level %d, Class %d.\n\n", fname, lname, level + 1, class + 1);
    }
}


/****************************************************************************/

// Function to delete a student from the database by first name and last name
void delete_student() {
    char fname[NAME_LEN], lname[NAME_LEN];

    // Get the first name of the student to delete
    printf("Enter first name of the student to delete: ");
    scanf("%s", fname);

    // Get the last name of the student to delete
    printf("Enter last name of the student to delete: ");
    scanf("%s", lname);

    // Initialize a pointer to the previous student (to track the node before the current student in the linked list)
    struct student* prev_student = NULL;

    int level, class;
    for (level = 0; level < NUM_LEVELS; level++) {
        for (class = 0; class < NUM_CLASSES; class++) {
            // Get the head student of the current level and class
            struct student* current_student = S.db[level][class];

            while (current_student != NULL) {
                // Check if the current student's first name and last name match the input
                if (strcmp(current_student->fname, fname) == 0 && strcmp(current_student->lname, lname) == 0) {
                    // If the student to delete is the head student, update the head pointer
                    if (prev_student == NULL) {
                        S.db[level][class] = current_student->next_stud;
                    } else {
                        // Otherwise, update the next pointer of the previous student to skip the current student
                        prev_student->next_stud = current_student->next_stud;
                    }

                    // Free the memory occupied by the current student
                    free(current_student);
                    printf("Student %s %s has been deleted from the database.\n\n", fname, lname);
                    return;
                }

                // Move to the next student in the linked list
                prev_student = current_student;
                current_student = current_student->next_stud;
            }
        }
    }

    // If the student with the given first name and last name is not found in the database
    printf("Student %s %s not found in the database.\n\n", fname, lname);
}


/****************************************************************************/

// Function to update student information
void update_student() {
    char fname[NAME_LEN], lname[NAME_LEN];

    // Get the first name and last name of the student to update
    printf("Enter first name: ");
    scanf("%s", fname);

    printf("Enter last name: ");
    scanf("%s", lname);

    // Find the student in the database by name
    struct student* found_student = find_student_by_name(fname, lname);
    if (found_student == NULL) {
        printf("Student %s %s not found in the database.\n", fname, lname);
        return;
    }

    char choice;
    // Display the options for updating student information
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
            // Get and update the new first name
            printf("Enter new first name: ");
            scanf("%s", new_fname);
            strncpy(found_student->fname, new_fname, NAME_LEN - 1);
            found_student->fname[NAME_LEN - 1] = '\0';
            printf("First name updated.\n");
            break;
        }
        case '2': {
            char new_lname[NAME_LEN];
            // Get and update the new last name
            printf("Enter new last name: ");
            scanf("%s", new_lname);
            strncpy(found_student->lname, new_lname, NAME_LEN - 1);
            found_student->lname[NAME_LEN - 1] = '\0';
            printf("Last name updated.\n");
            break;
        }
        case '3': {
            char new_cell[CELL_NUM];
            // Get and update the new cell number
            printf("Enter new cell number: ");
            scanf("%s", new_cell);
            strncpy(found_student->cell, new_cell, CELL_NUM - 1);
            found_student->cell[CELL_NUM - 1] = '\0';
            printf("Cell number updated.\n");
            break;
        }
        case '4': {
            int grade_choice;
            // Get the course to update the grade
            printf("Select the grade to update (1-%d): ", NUM_COURSES);
            scanf("%d", &grade_choice);

            if (grade_choice < 1 || grade_choice > NUM_COURSES) {
                printf("Invalid grade choice. No grades updated.\n\n");
                break;
            }

            // Get and update the new grade for the selected course
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


/****************************************************************************/

// Function to search for a student by first name and last name
void search_student_by_name() {
    char fname[NAME_LEN], lname[NAME_LEN];

    // Get the first name from the user
    printf("Enter first name: ");
    scanf("%s", fname);

    // Get the last name from the user
    printf("Enter last name: ");
    scanf("%s", lname);

    // Iterate over all levels and classes to find the student
    int level, class;
    for (level = 0; level < NUM_LEVELS; level++) {
        for (class = 0; class < NUM_CLASSES; class++) {
            // Get the current student in the database
            struct student* current_student = S.db[level][class];

            // If the current student is not NULL (i.e., there are students in this level and class)
            if (current_student != NULL) {
                // Compare the first name and last name of the current student with the input names
                if (strcmp(current_student->fname, fname) == 0 && strcmp(current_student->lname, lname) == 0) {
                    // Student found, print their information and return
                    printf("\nStudent Found:\n");
                    printf("Level: %d, Class: %d\n", level + 1, class + 1);
                    printf("Name: %s %s\n", current_student->fname, current_student->lname);
                    printf("Cell: %s\n", current_student->cell);
                    printf("Grades:\n");

                    // Print the grades of the current student for all courses
                    int course;
                    for (course = 0; course < NUM_COURSES; course++) {
                        printf("%s: %d\n", course_names[course], current_student->grades[course]);
                    }
                    return; // Student found, no need to continue searching
                }
            }
        }
    }

    // If the function reaches this point, the student was not found in the database
    printf("\nStudent %s %s not found in the database.\n", fname, lname);
}


/****************************************************************************/

// Function to present the top ten students in a specific subject
void present_top_students_by_subject(const char* subject_name) {
    printf("\nTop Ten Students in %s by Grade:\n", subject_name);

    // Create a 3D array to store the top ten students for each level and class
    struct student* top_students[NUM_LEVELS][NUM_CLASSES][10] = {0};

    // Iterate over each level and class to find the top students
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            struct student* current_student = S.db[level][class_num];

            while (current_student != NULL) {
                int total_grade = 0;

                // Calculate the total grade for the specified subject
                int course;
                for (course = 0; course < NUM_COURSES; course++) {
                    if (strcmp(course_names[course], subject_name) == 0) {
                        total_grade += current_student->grades[course];
                        break;
                    }
                }

                // Find the minimum grade index in the top_students array
                int min_grade_index = 0;
                int grade;
                for (grade = 1; grade < 10; grade++) {
                    if (top_students[level][class_num][grade] != NULL &&
                        top_students[level][class_num][grade]->grades[NUM_COURSES - 1] <
                        top_students[level][class_num][min_grade_index]->grades[NUM_COURSES - 1]) {
                        min_grade_index = grade;
                    }
                }

                // Replace the minimum grade student with the current student if their total grade is higher
                if (top_students[level][class_num][min_grade_index] == NULL ||
                    total_grade > top_students[level][class_num][min_grade_index]->grades[NUM_COURSES - 1]) {
                    top_students[level][class_num][min_grade_index] = current_student;
                }

                current_student = current_student->next_stud;
            }
        }
    }

    // Print the top ten students for each level and class
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            int has_top_students = 0;

            // Check if there are top students in this level and class
            for (int student = 0; student < 10; student++) {
                if (top_students[level][class_num][student] != NULL) {
                    has_top_students = 1;
                    break;
                }
            }

            // If there are top students, print their details
            if (has_top_students) {
                printf("\nLevel: %d, Class: %d\n", level + 1, class_num + 1);
                printf("====================================\n");
                printf("Top Ten Students in %s:\n", subject_name);

                for (int student = 0; student < 10; student++) {
                    struct student* current_student = top_students[level][class_num][student];
                    if (current_student != NULL) {
                        printf("%d. %s %s\n", student + 1, current_student->fname, current_student->lname);
                        printf("   Total Grade: %d\n", current_student->grades[NUM_COURSES - 1]);
                    }
                }
            }
        }
    }
}

/****************************************************************************/

// Function to present the top ten students in a particular subject
void present_top_ten() {
    // Display the list of available subjects
    printf("Available Subjects:\n");
    int course;
    for (course = 0; course < NUM_COURSES; course++) {
        printf("%d. %s\n", course + 1, course_names[course]);
    }

    // Prompt the user to enter the number corresponding to the subject
    int choice;
    printf("Enter the number corresponding to the subject: ");
    // Read the user's choice and validate it
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > NUM_COURSES) {
        printf("Invalid choice. Please try again.\n\n");
        return;
    }

    // Call the function to present the top students in the selected subject
    present_top_students_by_subject(course_names[choice - 1]);
}


/****************************************************************************/

// Function to check if a student is eligible for departure
int is_eligible_for_departure(struct student* s) {
    // If the student is NULL (not found in the database), they are not eligible
    if (s == NULL) {
        return 0;
    }

    // If the student's final grade (last element in grades array) is less than 60, they are not eligible
    if (s->grades[NUM_COURSES - 1] < 60) {
        return 0;
    }

    // Check each course grade to ensure they are all 60 or above, excluding the last element (level)
    for (int course = 0; course < NUM_COURSES - 1; course++) {
        if (s->grades[course] < 60) {
            return 0;
        }
    }

    // If all conditions are met, the student is eligible for departure
    return 1;
}


/****************************************************************************/

// Function to present students eligible for departure (Level 12, Grades >= 60 in All Subjects)
void present_eligible_for_departure() {
    printf("\nStudents Eligible for Departure (Level 12, Grades >= 60 in All Subjects):\n");

    // Loop through all classes in Level 12
    for (int class = 0; class < NUM_CLASSES; class++) {
        // Get the head student of the current class in Level 12
        struct student* current_student = S.db[NUM_LEVELS - 1][class];

        // Loop through the students in the current class
        while (current_student != NULL) {
            // Check if the student is eligible for departure based on their grades
            if (is_eligible_for_departure(current_student)) {
                printf("Name: %s %s\n", current_student->fname, current_student->lname);
                printf("Cell: %s\n", current_student->cell);
                printf("Grades:\n");

                for (int course = 0; course < NUM_COURSES; course++) {
                    printf("%s: %d\n", course_names[course], current_student->grades[course]);
                }
                printf("\n");
            }

            current_student = current_student->next_stud;
        }
    }
}


/****************************************************************************/

// Function to present the average grade per course per level
void present_avg_course_level() {
    printf("\nAverage Grade per Course per Level:\n");

    // Arrays to store total grades and the number of students for each course and level
    int total_grades[NUM_COURSES][NUM_LEVELS] = {0};
    int num_students[NUM_COURSES][NUM_LEVELS] = {0};

    // Iterate over each level and class to calculate the total grades and number of students for each course
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            struct student* current_student = S.db[level][class_num];

            // Traverse the linked list of students for each level and class
            while (current_student != NULL) {
                // Update the total grades and count the number of students for each course
                for (int course = 0; course < NUM_COURSES; course++) {
                    total_grades[course][level] += current_student->grades[course];
                    if (current_student->grades[course] > 0) {
                        num_students[course][level]++;
                    }
                }

                current_student = current_student->next_stud;
            }
        }
    }

    // Print the average grade for each course at each level
    for (int level = 0; level < NUM_LEVELS; level++) {
        printf("\nLevel %d\n", level + 1);
        printf("----------------------------\n");

        for (int course = 0; course < NUM_COURSES; course++) {
            if (num_students[course][level] > 0) {
                // Calculate the average grade and print it
                double average = (double)total_grades[course][level] / num_students[course][level];
                printf("%s: %.2lf\n", course_names[course], average);
            } else {
                // If no students are enrolled in the course at this level, print N/A
                printf("%s: N/A (No students enrolled)\n", course_names[course]);
            }
        }
    }
}


/****************************************************************************/

// Function to export the database to a file
void export_db_file() {
    char filename[] = "C:/Users/brr/Documents/GitHub/checkpoint_raedkbar/school_db.txt";

    // Open the file in write mode
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening the file for export.\n");
        return;
    }

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_num = 0; class_num < NUM_CLASSES; class_num++) {
            struct student* current_student = S.db[level][class_num];

            // Write student information to the file
            while (current_student != NULL) {
                fprintf(file, "%s %s %s %d %d ", current_student->fname, current_student->lname,
                        current_student->cell, level + 1, class_num + 1);

                for (int course = 0; course < NUM_COURSES; course++) {
                    fprintf(file, "%d ", current_student->grades[course]);
                }

                fprintf(file, "\n");
                current_student = current_student->next_stud;
            }
        }
    }

    fclose(file);
    printf("Database exported to %s.\n", filename);
}


/****************************************************************************/

// Function to display the database access menu and handle user choices
void db_access_menu() {
    char choice;

    // Display the menu options
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

    // Prompt the user for their choice
    printf("Enter your choice: ");
    scanf("%s", &choice);

    // Process the user's choice using a switch statement
    switch (choice) {
        case '1':
            // Option 1: Insert a new student
            insert_new_student();
            break;
        case '2':
            // Option 2: Delete a student
            delete_student();
            break;
        case '3':
            // Option 3: Update student information
            update_student();
            break;
        case '4':
            // Option 4: Search for a student by name
            search_student_by_name();
            break;
        case '5':
            // Option 5: Present top ten students in a subject
            present_top_ten();
            break;
        case '6':
            // Option 6: Present students eligible for departure
            present_eligible_for_departure();
            break;
        case '7':
            // Option 7: Present average per course per level
            present_avg_course_level();
            break;
        case '8':
            // Option 8: Export the database to a file
            export_db_file();
            break;
        case '0':
            // Option 0: Exit the program
            printf("Exiting the system.\n\n");
            exit(0);
        default:
            // Invalid choice: Display an error message and prompt again
            printf("Invalid choice. Please try again.\n\n");
            break;
    }

    // After processing the choice, show the menu again
    db_access_menu();
}


/****************************************************************************/

// main function
int main() {
    FILE* file;
    char filename[] = "C:/Users/brr/Documents/GitHub/checkpoint_raedkbar/students_with_class.txt";

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    init_db(file);

    fclose(file);

    //print_data();

    db_access_menu();

    return 0;
}