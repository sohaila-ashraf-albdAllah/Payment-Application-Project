#include "server.h"
#include "../Terminal/terminal.h"
#define MAX_TRANS 255


ST_accountsDB_t accountsDB[MAX_TRANS] =
{
	{2000.0, RUNNING, "8989374615436851"},
	{100000.0, BLOCKED, "5807007076043875"},
	{ 4000.0, RUNNING, "4903478079245246" },
	{ 4000.0, BLOCKED, "8751876247081216" },
	{ 6000.0, RUNNING, "4719802455979296" },
	{ 6000.0, BLOCKED, "6907940726884180" },
	{ 8000.0, RUNNING, "6263565997807508" },
	{ 8000.0, BLOCKED, "3358065963483406" },
	{ 10000.0, RUNNING, "4068975151313987" },
	{ 10000.0, BLOCKED, "2652623873444840" }
};

// Global array to store transactions
ST_transaction_t transactionDB[MAX_TRANS] = { 0 };
int transactionCount = 0; // Variable to track the number of transactions

/////////////////////////////implement recieveTransactionData function ////////////////////////////////////////////// 
EN_transState_t recieveTransactionData (ST_transaction_t* transData) {
    // 1. Validate card existence
    int accountIndex = isValidAccount(&transData->cardHolderData, accountsDB);
    if (accountIndex == -1) {
        return FRAUD_CARD;
    }

    // 2. Check if account is blocked
    if (accountsDB[accountIndex].state == BLOCKED) {
        return DECLINED_STOLEN_CARD;
    }

    // 3. Check available balance
    if (accountsDB[accountIndex].balance < transData->terminalData.amount) {
        return DECLINED_INSUFFECIENT_FUND;
    }

    // 4. Update balance
    accountsDB[accountIndex].balance -= transData->terminalData.amount;

    // 5. Save transaction
    EN_serverError_t saveResult = saveTransaction(transData);
    if (saveResult != SERVER_OK) {
        return INTERNAL_SERVER_ERROR;
    }

    // 6. Transaction successful
    return APPROVED;
}

// Test function for recieveTransactionData
void recieveTransactionDataTest(void) {
    printf("Tester Name: Amira Hassan\n");
    printf("Function Name: recieveTransactionData\n\n");

    ST_transaction_t transData;  // Variable to hold transaction data

    // Test Case 1: Happy scenario (valid transaction)
    printf("Test Case 1:\n");
    printf("Input Data: Valid transaction data (account exists, sufficient funds, not blocked)\n");
    printf("Expected Result: APPROVED\n");
    strcpy(transData.cardHolderData.primaryAccountNumber, "4068975151313987"); // Existing account number
    transData.terminalData.transactionAmount = 2000.0;  // Valid transaction amount
    EN_transState_t result = recieveTransactionData(&transData);  // Call the function
    printf("Actual Result: %d\n\n", result);  // Print actual result

  

    // Test Case 2: Account does not exist
    printf("Test Case 2:\n");
    printf("Input Data: Non-existent account data\n");
    printf("Expected Result: FRAUD_CARD\n");
    strcpy(transData.cardHolderData.primaryAccountNumber, "1234567890123456"); // Non-existent account number
    transData.terminalData.transactionAmount = 2000.0;  // Valid transaction amount
    result = recieveTransactionData(&transData);  // Call the function
    printf("Actual Result: %d\n\n", result);  // Print actual result

    
}


/////////////////////////////implement isValidAccount function //////////////////////////////////////////////


EN_serverError_t isValidAccount (ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
    // Iterate through accounts to find matching PAN
    for (int i = 0; i < MAX_TRANS; i++) {
        if (strcmp(accountsDB[i].primaryAccountNumber, cardData->primaryAccountNumber) == 0) {
            return SERVER_OK;  // Account found, return success
        }
    }
    return ACCOUNT_NOT_FOUND;  // Account not found
}

// Test function for isValidAccount
void isValidAccountTest(void) {
    printf("Tester Name: Amira Hassan\n");
    printf("Function Name: isValidAccount\n\n");

    // Test Case 1: Happy scenario (existing account)
    printf("Test Case 1:\n");
    printf("Input Data: Valid card data (account exists in DB)\n");
    printf("Expected Result: SERVER_OK\n");
    ST_cardData_t validCard = { "4903478079245246" }; // Account from test cases in main
    EN_serverError_t result = isValidAccount(&validCard, accountsDB);
    printf("Actual Result: %s\n\n", result == SERVER_OK ? "SERVER_OK" : "Error");

    // Test Case 2: Non-existent account
    printf("Test Case 2:\n");
    printf("Input Data: Card data with non-existent account number\n");
    printf("Expected Result: ACCOUNT_NOT_FOUND\n");
    ST_cardData_t nonExistentCard = { "1234567890123456" };
    result = isValidAccount(&nonExistentCard, accountsDB);
    printf("Actual Result: %s\n\n", result == ACCOUNT_NOT_FOUND ? "ACCOUNT_NOT_FOUND" : "Error");
}


/////////////////////////////implement isBlockedAccount function //////////////////////////////////////////////

EN_serverError_t isBlockedAccount (ST_accountsDB_t* accountRefrence) {
    if (accountRefrence->state == RUNNING) {
        return SERVER_OK;  // Account is running
    }
    else {
        return BLOCKED_ACCOUNT;  // Account is blocked
    }
}

void isBlockedAccountTest(void) {

    printf("Tester Name: Amira Hassan\n");
    printf("Function Name: isBlockedAccount\n\n");

    // Test Case 1: Running account
    printf("Test Case 1:\n");
    printf("Input Data: Account reference to a running account\n");
    printf("Expected Result: SERVER_OK\n");
    ST_accountsDB_t runningAccount = { 1000.0, RUNNING, "1234567890123456" };
    EN_serverError_t result = isBlockedAccount(&runningAccount);
    printf("Actual Result: %s\n\n", result == SERVER_OK ? "SERVER_OK" : "Error");

    // Test Case 2: Blocked account
    printf("Test Case 2:\n");
    printf("Input Data: Account reference to a blocked account\n");
    printf("Expected Result: BLOCKED_ACCOUNT\n");
    ST_accountsDB_t blockedAccount = { 5000.0, BLOCKED, "9876543210123456" };
    result = isBlockedAccount(&blockedAccount);
    printf("Actual Result: %s\n\n", result == BLOCKED_ACCOUNT ? "BLOCKED_ACCOUNT" : "Error");
}


/////////////////////////////////////Implement isAmountAvailable function//////////////////////////////////////////

// Function to check if the amount is available in the account
EN_serverError_t isAmountAvailable(ST_terminal_t terminalData, ST_accountsDB_t* accountReference) {
    if (accountReference->balance < terminalData.amount) {
        return LOW_BALANCE;
    } else {
        return SERVER_OK;
    }
}

// Test function to test the isAmountAvailable function
void isAmountAvailableTest(void) {
    printf("Tester Name: Mohamed Ibrahim\n");
    printf("Function Name: isAmountAvailable\n\n");

    ST_terminal_t testTerminal = { 500.0 }; // Example terminal data
    ST_accountsDB_t testAccount = { 1000.0 }; // Example account data

    // Test Case 1: Sufficient balance
    printf("Test Case 1:\n");
    printf("Input Data: Terminal amount = 500.0, Account balance = 1000.0\n");
    printf("Expected Result: SERVER_OK\n");
    EN_serverError_t expectedResult = SERVER_OK;
    EN_serverError_t actualResult = isAmountAvailable(testTerminal, &testAccount);
    printf("Actual Result: %s\n\n", actualResult == SERVER_OK ? "SERVER_OK" : "LOW_BALANCE");

    // Test Case 2: Insufficient balance
    printf("Test Case 2:\n");
    printf("Input Data: Terminal amount = 1500.0, Account balance = 1000.0\n");
    printf("Expected Result: LOW_BALANCE\n");
    testTerminal.amount = 1500.0; // Adjust terminal amount
    expectedResult = LOW_BALANCE;
    actualResult = isAmountAvailable(testTerminal, &testAccount);
    printf("Actual Result: %s\n\n", actualResult == LOW_BALANCE ? "LOW_BALANCE" : "SERVER_OK");
}


///////////////////////////// Implement saveTransaction function //////////////////////////////////////////////

EN_serverError_t saveTransaction(ST_transaction_t* transaction) {
    // Check if there is space available in the transactions database
    if (transactionCount >= MAX_TRANS) {
        return SERVER_ERROR;
    }

    // Assign a sequence number to the transaction
    transaction->sequenceNumber = transactionCount + 1;

    // Save the transaction in the transactions database
    transactionDB[transactionCount] = *transaction;
    transactionCount++;

    return SERVER_OK;
}

///////////////////////////// Implement listSavedTransactions function //////////////////////////////////////////////

void listSavedTransactions(void) {
    printf("List of Saved Transactions:\n");
    for (int i = 0; i < transactionCount; i++) {
        printf("Transaction %d:\n", transactionDB[i].sequenceNumber);
        // Print transaction details (you can adjust this based on your transaction structure)
        printf("  Card Number: %s\n", transactionDB[i].cardHolderData.primaryAccountNumber);
        printf("  Amount: %.2f\n", transactionDB[i].terminalData.amount);
        printf("  Result: %d\n", transactionDB[i].transactionStatus);
        printf("\n");
    }
}

///////////////////////////// Implement saveTransactionTest function //////////////////////////////////////////////

void saveTransactionTest(void) {
    printf("Tester Name: Mohamed Ibrahim\n");
    printf("Function Name: saveTransaction\n\n");

    ST_transaction_t testTransaction; // Example transaction data

    // Test Case 1: Happy scenario (valid transaction)
    printf("Test Case 1:\n");
    printf("Input Data: Valid transaction data\n");
    printf("Expected Result: SERVER_OK\n");
    strcpy(testTransaction.cardHolderData.primaryAccountNumber, "4068975151313987"); // Existing account number
    testTransaction.terminalData.amount = 2000.0;  // Valid transaction amount
    testTransaction.transactionStatus = APPROVED; // Example transaction status
    EN_serverError_t result = saveTransaction(&testTransaction);  // Call the function
    printf("Actual Result: %s\n\n", result == SERVER_OK ? "SERVER_OK" : "SERVER_ERROR");

    // Test Case 2: Maximum transactions reached
    printf("Test Case 2:\n");
    printf("Input Data: Maximum transactions reached\n");
    printf("Expected Result: SERVER_ERROR\n");
    // Add more test cases as needed
}

///////////////////////////// Implement listSavedTransactions function //////////////////////////////////////////////

void listSavedTransactions(void) {
    printf("List of Saved Transactions:\n");
    for (int i = 0; i < transactionCount; i++) {
        printf("#########################\n");
        printf("Transaction Sequence Number: %d\n", transactionDB[i].sequenceNumber);
        // Add more details based on your transaction structure
        printf("Transaction Date: %s\n", transactionDB[i].transactionDate);
        printf("Transaction Amount: %.2f\n", transactionDB[i].transactionAmount);
        printf("Transaction State: %d\n", transactionDB[i].transactionState);
        printf("Terminal Max Amount: %.2f\n", transactionDB[i].terminalMaxAmount);
        printf("Cardholder Name: %s\n", transactionDB[i].cardHolderName);
        printf("PAN: %s\n", transactionDB[i].pan);
        printf("Card Expiration Date: %s\n", transactionDB[i].cardExpirationDate);
        printf("#########################\n");
    }
}

///////////////////////////// Implement listSavedTransactionsTest function //////////////////////////////////////////////

void listSavedTransactionsTest(void) {
    printf("Tester Name: Mohamed Ibrahim\n");
    printf("Function Name: listSavedTransactions\n\n");

    // Add test cases to cover various scenarios
    // Test Case 1: Happy scenario (transactions available)
    printf("Test Case 1:\n");
    printf("Input Data: Transactions available\n");
    printf("Expected Result: List of transactions printed\n");
    // Add transactions to the transactionDB for testing
    // Call listSavedTransactions function
    listSavedTransactions(); // You may want to redirect output to a file for verification

    // Test Case 2: No transactions available
    printf("Test Case 2:\n");
    printf("Input Data: No transactions available\n");
    printf("Expected Result: No transactions printed\n");
    // Clear transactionDB or do not add any transactions
    for (int i = 0; i < 255; i++)
    {
        transactionDB[i].transactionSequenceNumber = 0;
    }
    printf("Expected Result: No transactions should be printed\n");
    printf("Actual Result:\n");
    // Call listSavedTransactions function
    listSavedTransactions(); // You may want to redirect output to a file for verification
}



