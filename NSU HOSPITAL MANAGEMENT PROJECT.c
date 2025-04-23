#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h> // Only works on Windows

#define MAX_PATIENTS 100
#define MAX_DOCTORS 6

typedef struct {
    char username[20];
    int doctorIndex;
    char date[20];
    int isPaid;
    char paymentMethod[20];
} Appointment;

typedef struct {
    char name[50];
    char specialization[30];
    char availability[20];
    int fee;
} Doctor;

Doctor doctors[MAX_DOCTORS] = {
    {"Dr. Alam", "Cardiologist", "Mon-Wed", 1000},
    {"Dr. Sara", "Dermatologist", "Tue-Thu", 800},
    {"Dr. Reza", "Neurologist", "Mon-Fri", 1200},
    {"Dr. Nila", "Pediatrician", "Wed-Fri", 900},
    {"Dr. Khan", "General", "Everyday", 500},
    {"Dr. Zaman", "Orthopedic", "Tue-Sat", 1100}
};

Appointment appointments[MAX_PATIENTS];
int appointmentCount = 0;

void welcomeScreen();
int login(char username[]);
void patientPortal(char username[]);
void viewDoctors();
void newAppointment(char username[]);
void checkAppointments(char username[]);
void processPayment(int index);

int main() {
    welcomeScreen();
    char username[20];
    if (login(username)) {
        patientPortal(username);
    }
    return 0;
}

void welcomeScreen() {
    printf("=========================================\n");
    printf("   Welcome to NSU Hospital Management System\n");
    printf("=========================================\n");
    printf("Providing Easy Appointment & Payment Solutions\n");
    printf("-----------------------------------------\n");
    printf("Press Enter to proceed to Login...\n");
    getchar();
    getchar();
}

int login(char username[]) {
    char inputUsername[20], inputPassword[20];
    char fileUsername[20], filePassword[20];
    int attempts = 3;

    while (attempts--) {
        FILE *file = fopen("users.txt", "r");
        if (file == NULL) {
            printf("Error: Cannot open users.txt\n");
            return 0;
        }

        printf("\n--- Patient Login Portal ---\n");
        printf("Username: ");
        scanf(" %s", inputUsername);
        printf("Password: ");

        // Hide password input
        int i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 19) { // Enter key
            if (ch == '\b' && i > 0) {
                i--;
                printf("\b \b");
            } else if (ch != '\b') {
                inputPassword[i++] = ch;
                printf("*");
            }
        }
        inputPassword[i] = '\0';
        printf("\n");

        int found = 0;
        while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
            if (strcmp(inputUsername, fileUsername) == 0) {
                found = 1;
                if (strcmp(inputPassword, filePassword) == 0) {
                    fclose(file);
                    strcpy(username, inputUsername);
                    printf("Login successful! Welcome %s.\n", username);
                    return 1;
                } else {
                    printf("Incorrect password.\n");
                }
                break;
            }
        }

        if (!found) printf("Username not found.\n");
        fclose(file);
        if (attempts) printf("Attempts remaining: %d\n", attempts);
        else printf("Login failed after 3 attempts.\n");
    }

    return 0;
}

void patientPortal(char username[]) {
    int choice;
    while (1) {
        printf("\n--- %s's Portal ---\n", username);
        printf("1. Add New Appointment\n");
        printf("2. Check Old Appointments\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (choice == 1) newAppointment(username);
        else if (choice == 2) checkAppointments(username);
        else if (choice == 3) {
            printf("Logged out successfully.\n");
            break;
        }
        else printf("Invalid choice.\n");
    }
}

void viewDoctors() {
    printf("\n--- Doctor List ---\n");
    for (int i = 0; i < MAX_DOCTORS; i++) {
        printf("%d. %s (%s) - Available: %s - Fee: %d\n",
            i + 1, doctors[i].name, doctors[i].specialization,
            doctors[i].availability, doctors[i].fee);
    }
}

void newAppointment(char username[]) {
    viewDoctors();
    int docIndex;
    printf("Choose doctor index: ");
    scanf("%d", &docIndex);
    if (docIndex < 1 || docIndex > MAX_DOCTORS) {
        printf("Invalid doctor selection.\n");
        return;
    }
    docIndex--;

    printf("Enter appointment date (YYYY-MM-DD): ");
    scanf(" %s", appointments[appointmentCount].date);

    strcpy(appointments[appointmentCount].username, username);
    appointments[appointmentCount].doctorIndex = docIndex;
    appointments[appointmentCount].isPaid = 0;
    strcpy(appointments[appointmentCount].paymentMethod, "Pending");

    printf("Appointment booked. Please pay after visiting the doctor.\n");
    appointmentCount++;
}

void checkAppointments(char username[]) {
    int found = 0;
    printf("\n--- Your Appointments ---\n");
    for (int i = 0; i < appointmentCount; i++) {
        if (strcmp(appointments[i].username, username) == 0) {
            int docIndex = appointments[i].doctorIndex;
            printf("Doctor: %s | Date: %s | Payment: %s | Status: %s\n",
                   doctors[docIndex].name,
                   appointments[i].date,
                   appointments[i].paymentMethod,
                   appointments[i].isPaid ? "Paid" : "Unpaid");
            if (!appointments[i].isPaid) {
                int payChoice;
                printf("Do you want to pay now? (1 = Yes / 0 = No): ");
                scanf("%d", &payChoice);
                if (payChoice == 1) {
                    processPayment(i);
                }
            }
            found = 1;
        }
    }
    if (!found) {
        printf("No appointments found.\n");
    }
}

void processPayment(int index) {
    printf("Select Payment Method (Bkash/Nagad/Card): ");
    scanf(" %s", appointments[index].paymentMethod);
    appointments[index].isPaid = 1;
    printf("Payment via %s successful! Thank you.\n", appointments[index].paymentMethod);
}