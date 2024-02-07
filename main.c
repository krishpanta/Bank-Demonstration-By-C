#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define FILENAME "D:\\Bank C\\accounts\\"

typedef struct {
    char username[20];
    int passcode;
    double balance;
} Account;

Account accounts[MAX_ACCOUNTS];
int numAccounts = 0;

// Function prototypes
void saveAccount(const Account *account);
void loadAccount(const char *username, Account *account);
int findAccountIndex(const char *username);
int findAccountByPasscode(int passcode);
void signup();
void login();
void deposit(int accountIndex, double amount);
void showBalance(int accountIndex);
void transfer(int senderIndex, int recipientIndex, double amount);
void saveAccounts();
void loadAccounts();

// Function to save accounts to the file
void saveAccounts() {
    FILE *file = fopen("D:\\Bank C\\accounts\\account_list.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s\n", accounts[i].username);
    }

    fclose(file);
}

// Function to load accounts from the file
void loadAccounts() {
    FILE *file = fopen("D:\\Bank C\\accounts\\account_list.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%s", accounts[numAccounts].username) == 1) {
        loadAccount(accounts[numAccounts].username, &accounts[numAccounts]);
        numAccounts++;
    }

    fclose(file);
}

// Function to save an account to the file
void saveAccount(const Account *account) {
    char filename[50];
    sprintf(filename, "%s%s.txt", FILENAME, account->username);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d %.2lf\n", account->passcode, account->balance);

    fclose(file);
}

// Function to load an account from the file
void loadAccount(const char *username, Account *account) {
    char filename[50];
    sprintf(filename, "%s%s.txt", FILENAME, username);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d %lf", &account->passcode, &account->balance) != 2) {
        fprintf(stderr, "Error reading file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

// Function to find the index of an account based on the username
int findAccountIndex(const char *username) {
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to find the index of an account based on the passcode
int findAccountByPasscode(int passcode) {
    for (int i = 0; i < numAccounts; i++) {
        if (accounts[i].passcode == passcode) {
            return i;
        }
    }
    return -1;
}

// Function to sign up and create a new account
// Function to sign up and create a new account
void signup() {
    if (numAccounts == MAX_ACCOUNTS) {
        printf("Error: Maximum number of accounts reached.\n");
        return;
    }

    Account newAccount;

    printf("Enter your name: ");
    scanf("%s", newAccount.username);

    printf("Set a passcode (numeric): ");
    scanf("%d", &newAccount.passcode);

    // Check if the passcode already exists
    if (findAccountByPasscode(newAccount.passcode) != -1) {
        printf("Error: Passcode already exists.\n");
        return;
    }

    printf("Enter initial balance: ");
    scanf("%lf", &newAccount.balance);

    // Save the new account to a file
    saveAccount(&newAccount);

    accounts[numAccounts] = newAccount;
    numAccounts++;

    // Save the updated account list
    saveAccounts();

    printf("Account created successfully. Your username is %s.\n", newAccount.username);
}


// Function to log in and perform transactions
void login() {
    int passcode;

    // Prompt the user to enter the passcode directly
    printf("Enter passcode: ");
    scanf("%d", &passcode);

    int index = findAccountByPasscode(passcode);

    if (index == -1) {
        printf("Error: Incorrect passcode.\n");
        return;
    }

    printf("Login successful. Welcome, %s!\n", accounts[index].username);

    int choice;

    do {
        printf("\n------------------------------------------\n");
        printf("1. Deposit\n");
        printf("2. Show Balance\n");
        printf("3. Transfer Money\n");
        printf("4. Logout\n");
        printf("------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Deposit money
                {
                    double depositAmount;
                    printf("Enter the amount to deposit: ");
                    scanf("%lf", &depositAmount);
                    deposit(index, depositAmount);
                }
                break;
            case 2:
                // Show balance
                showBalance(index);
                break;
            case 3:
                // Transfer money
                {
                    int recipientIndex;
                    printf("Enter the recipient's passcode: ");
                    scanf("%d", &passcode);

                    recipientIndex = findAccountByPasscode(passcode);

                    if (recipientIndex == -1) {
                        printf("Error: Recipient account not found.\n");
                    } else {
                        double transferAmount;
                        printf("Enter the amount to transfer: ");
                        scanf("%lf", &transferAmount);

                        transfer(index, recipientIndex, transferAmount);
                    }
                }
                break;
            case 4:
                printf("Logging out.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

// Function to deposit money into the account
void deposit(int accountIndex, double amount) {
    if (amount > 0) {
        accounts[accountIndex].balance += amount;
        printf("Deposit successful. New balance: %.2lf\n", accounts[accountIndex].balance);

        // Save changes to file
        saveAccount(&accounts[accountIndex]);
    } else {
        printf("Invalid deposit amount. Please enter a positive value.\n");
    }
}

// Function to show the account balance
void showBalance(int accountIndex) {
    printf("Your current balance: %.2lf\n", accounts[accountIndex].balance);
}

// Function to transfer money from one account to another
void transfer(int senderIndex, int recipientIndex, double amount) {
    if (amount > 0 && accounts[senderIndex].balance >= amount) {
        accounts[senderIndex].balance -= amount;
        accounts[recipientIndex].balance += amount;

        printf("Transfer successful.\n");
        printf("Your new balance: %.2lf\n", accounts[senderIndex].balance);

        // Save changes to files for both sender and recipient
        saveAccount(&accounts[senderIndex]);
        saveAccount(&accounts[recipientIndex]);
    } else {
        printf("Invalid transfer amount or insufficient funds.\n");
    }
}

// Main function
int main() {
    loadAccounts();

    int choice;

    do {
        printf("\n------------------------------------------\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signup();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}
