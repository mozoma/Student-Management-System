/*
 * project1.c
 *
 *  Created on: Aug 12, 2024
 *      Updated: July 5, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"

struct student {
    int id;
    char name[30];
    int age;
    float gpa;
    struct student *next;
};

struct student *head = NULL;

// Save student list to a binary file
void saveToFile() {
    FILE *file = fopen(FILENAME, "wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct student *temp = head;
    while (temp != NULL) {
        fwrite(temp, sizeof(struct student), 1, file);
        temp = temp->next;
    }

    fclose(file);
}

// Load student list from a binary file
void loadFromFile() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) return;

    struct student temp;
    while (fread(&temp, sizeof(struct student), 1, file)) {
        struct student *new_student = (struct student *)malloc(sizeof(struct student));
        if (!new_student) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return;
        }

        *new_student = temp;
        new_student->next = NULL;

        // Insert into list sorted by ID
        struct student *current = head;
        struct student *previous = NULL;
        while (current != NULL && current->id < new_student->id) {
            previous = current;
            current = current->next;
        }

        if (previous == NULL) {
            new_student->next = head;
            head = new_student;
        } else {
            previous->next = new_student;
            new_student->next = current;
        }
    }

    fclose(file);
}

// Add a new student
void addStudent() {
    struct student *new_student = (struct student*)malloc(sizeof(struct student));
    if (new_student == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &new_student->id);
    getchar();

    // Check for duplicate ID
    struct student *temp = head;
    while (temp != NULL) {
        if (temp->id == new_student->id) {
            printf("Student with this ID already exists.\n");
            free(new_student);
            return;
        }
        temp = temp->next;
    }

    printf("Enter name: ");
    fgets(new_student->name, sizeof(new_student->name), stdin);
    new_student->name[strcspn(new_student->name, "\n")] = '\0';

    printf("Enter age: ");
    scanf("%d", &new_student->age);

    printf("Enter GPA: ");
    scanf("%f", &new_student->gpa);
    getchar();

    // Insert into sorted list
    struct student *current = head;
    struct student *previous = NULL;
    while (current != NULL && current->id < new_student->id) {
        previous = current;
        current = current->next;
    }

    if (previous == NULL) {
        new_student->next = head;
        head = new_student;
    } else {
        previous->next = new_student;
        new_student->next = current;
    }

    saveToFile();
    printf("Student added successfully.\n");
}

// Display all students
void displayStudents() {
    if (head == NULL) {
        printf("No students found.\n");
        return;
    }

    struct student *temp = head;
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Age: %d, GPA: %.2f\n", temp->id, temp->name, temp->age, temp->gpa);
        temp = temp->next;
    }
}

// Search student by ID
void searchStudentByID() {
    int id;
    printf("Enter student ID to search: ");
    scanf("%d", &id);
    getchar();

    struct student *temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("Student found: ID: %d, Name: %s, Age: %d, GPA: %.2f\n", temp->id, temp->name, temp->age, temp->gpa);
            return;
        }
        temp = temp->next;
    }

    printf("Student not found.\n");
}

// Update student information
void updateStudent() {
    int id;
    printf("Enter student ID to update: ");
    scanf("%d", &id);
    getchar();

    struct student *temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("Enter new name: ");
            fgets(temp->name, sizeof(temp->name), stdin);
            temp->name[strcspn(temp->name, "\n")] = '\0';

            printf("Enter new age: ");
            scanf("%d", &temp->age);

            printf("Enter new GPA: ");
            scanf("%f", &temp->gpa);
            getchar();

            saveToFile();
            printf("Student updated successfully.\n");
            return;
        }
        temp = temp->next;
    }

    printf("Student not found.\n");
}

// Delete student by ID
void deleteStudent() {
    int id;
    printf("Enter student ID to delete: ");
    scanf("%d", &id);
    getchar();

    struct student *temp = head;
    struct student *prev = NULL;

    while (temp != NULL) {
        if (temp->id == id) {
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            saveToFile();
            printf("Student deleted successfully.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Student not found.\n");
}

// Calculate average GPA
void calculateAverageGPA() {
    if (head == NULL) {
        printf("No students found.\n");
        return;
    }

    struct student *temp = head;
    float totalGPA = 0.0;
    int count = 0;

    while (temp != NULL) {
        totalGPA += temp->gpa;
        count++;
        temp = temp->next;
    }

    printf("Average GPA: %.2f\n", totalGPA / count);
}

// Find student with highest GPA
void searchHighestGPA() {
    if (head == NULL) {
        printf("No students found.\n");
        return;
    }

    struct student *temp = head;
    struct student *top_student = head;

    while (temp != NULL) {
        if (temp->gpa > top_student->gpa) {
            top_student = temp;
        }
        temp = temp->next;
    }

    printf("Student with the highest GPA: ID: %d, Name: %s, Age: %d, GPA: %.2f\n",
           top_student->id, top_student->name, top_student->age, top_student->gpa);
}

// Main menu
int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    loadFromFile();  // Load students from file on startup

    int choice;

    while (1) {
        printf("\n----- Student Management System -----\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Calculate Average GPA\n");
        printf("7. Search Student with Highest GPA\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudentByID(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: calculateAverageGPA(); break;
            case 7: searchHighestGPA(); break;
            case 8: exit(0);
            default: printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
