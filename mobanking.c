#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define TRANS_PIN 4
#define SIZE_OF_CHAR_IN_BYTES 8
#define ACCOUNTS_FILENAME "accounts.txt"
#define BALANCE_FILENAME "amount.txt"
#define MAX_BALANCE_LENGTH 6

typedef struct signInUserInfo
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char transactionPin[TRANS_PIN];
};
struct signInUserInfo getSignInUserInfo();

void updateBalance(int amount)
{
    //printf("i am updarting balance");
    FILE* balance_file = fopen(BALANCE_FILENAME, "a+");

    ftruncate(fileno(balance_file), 0);
    fprintf(balance_file, "%d",amount);

    printf("\nYour balance has been updated.");
    fclose(balance_file);
}

int getTotalBalance()
{

    char line[MAX_BALANCE_LENGTH]; // Max length of line in accounts file is username + password + ':' + '\n'

    // Open accounts file for reading
    FILE* balance_file = fopen(BALANCE_FILENAME, "r");
    if (balance_file == NULL)
    {
        printf("Error: could not open accounts file\n");
        exit(1);
    }

    int amount = atoi(fgets(line, sizeof(line), balance_file));
    //printf("%d",amount);
    fclose(balance_file);
    return amount;

}




void loadBalance()
{
    int accountBalance;
    int amount;

    accountBalance = getTotalBalance();
    printf("Money you wanted to add on this account:");

    scanf("%d", &amount);
    //printf("\n%d",accountBalance);
    int newBalance = accountBalance + amount;
    printf("Total balanced after loading: %d",newBalance);
    updateBalance(newBalance);
    afterSignIn();
}




void withdrawBalance()
{

    int accountBalance;
    int amount;

    accountBalance = getTotalBalance();
    printf("Money you wanted to transfer:");
    scanf("%d", &amount);

    //check for sufficent balance

    //printf("\n%d",accountBalance);
    if(amount<accountBalance){
        int newBalance = accountBalance - amount;
        printf("Total balanced after loading: %d",newBalance);
        updateBalance(newBalance);
    } else {
        printf("Insufficient Balance");
    }
    afterSignIn();
}

char* getTransactionPin()
{
    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + TRANS_PIN + 2]; // Max length of line in accounts file is username + password + ':' + '\n'
    FILE* accounts_file = fopen(ACCOUNTS_FILENAME, "r");
    if (accounts_file == NULL)
    {
        printf("Error: could not open accounts file\n");
        exit(1);
    }


    while (fgets(line, sizeof(line), accounts_file) != NULL)
    {
        // Strip newline character from line
        strtok(line, "\n");


        // Split line into username and password and transaction pin
        strtok(line, ":");
        strtok(NULL, ":");
        char* saved_transactionPin = strtok(NULL, ":");

        return saved_transactionPin;
    }
}
int validateTransactionPin(char tempPin[])
{
    char* savedTransactionPin = getTransactionPin();
    return strcasecmp(tempPin,savedTransactionPin);
}


void balanceInquiry()
{

    char tempPin[TRANS_PIN];
    do {
        printf("\nEnter your transcation PIN :");
        scanf("%s",&tempPin);

    } while(validateTransactionPin(tempPin) != 0);
    int balance = getTotalBalance();
    printf("Your total balance is: %d", balance);

    afterSignIn();
}

void logout()
{
    int choice;
    printf("1. Create an account\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        create_account();
        break;
    case 2:
        login();
        break;
    case 3:
        exit(0);
    default:
        printf("Invalid choice\n");
        break;
    }
}

void create_account()
{
    struct signInUserInfo info = getSignInUserInfo();
    FILE* accounts_file = fopen(ACCOUNTS_FILENAME, "a+");

    if (accounts_file == NULL)
    {
        printf("Error: could not open accounts file\n");
        exit(1);
    }

    ftruncate(fileno(accounts_file), 0);

    //printf("%s", password);
    //printf("%s", username);
    //printf("%s", transactionPin);
    // Write username, password and trans pin to accounts file
    fprintf(accounts_file, "%s:", info.username);
    fprintf(accounts_file, "%s:", info.password);
    fprintf(accounts_file, "%s:", info.transactionPin);
    printf("Account created successfully!\n");
    FILE* balance_file = fopen(BALANCE_FILENAME, "a+");
    if (balance_file == NULL)
    {
        printf("Error: could not open accounts file\n");
        exit(1);
    }
    ftruncate(fileno(balance_file), 0);
    fprintf(balance_file, "%d", 0);


    // Close accounts file
    fclose(accounts_file);
    fclose(balance_file);
}


struct signInUserInfo getSignInUserInfo()
{
    struct signInUserInfo info;
    printf("Enter user name: ");
    scanf("%s", info.username);
    printf("Enter password: ");
    scanf("%s", info.password);
    printf("Enter transaction pin(must be exactly 4 digits): ");
    scanf("%s", info.transactionPin);
    return info;
}

void afterSignIn(){
    int choice1;
            printf("\n\n1.Load Balance\n");
            printf("2. Withdraw\n");
            printf("3. Balance Inquiry\n");
            printf("4. Logout\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice1);

            switch (choice1)
            {
            case 1:
                loadBalance();
                break;
            case 2:
                withdrawBalance();
                break;
            case 3:
                balanceInquiry();
                break;
            case 4:
                logout();
                break;
            case 5:
                printf("THANK YOU FOR YOUR VISIT");
                exit(0);
            default:
                printf("Invalid choice\n");
                break;
            }


            //return 1;

}


void login()
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char transcationPin[TRANS_PIN];
    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + TRANS_PIN + 2]; // Max length of line in accounts file is username + password + ':' + '\n'

    // Get username and password from user
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    // Open accounts file for reading
    FILE* accounts_file = fopen(ACCOUNTS_FILENAME, "r");
    if (accounts_file == NULL)
    {
        printf("Error: could not open accounts file\n");
        exit(1);
    }

    // Check if username and password match a line in the accounts file
    while (fgets(line, sizeof(line), accounts_file) != NULL)
    {
        // Strip newline character from line
        //strtok(line, "\n");
        //printf("%s", line);

        // Split line into username and password
        char* saved_username = strtok(line, ":");
        char* saved_password = strtok(NULL, ":");

        // Check if username and password match
        if (strcmp(username, saved_username) == 0 && strcmp(password, saved_password) == 0)
        {
            printf("Login successful!\n");
            for(int i = 0; i<83; i++)
                printf("=");
            // Mobile banking features
            afterSignIn();
        }
    }

    // Close accounts file
    fclose(accounts_file);

    printf("Invalid username or password\n");
    login();
}

void main()
{


    int choice;
    printf("1. Create an account\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        create_account();
        login();
        break;
    case 2:
        login();
        break;
    case 3:
        exit(0);
    default:
        printf("Invalid choice\n");
        break;
    }

    getch();
}
