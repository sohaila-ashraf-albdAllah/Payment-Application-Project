#include "server.h"
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
uint32_t transactionCount = 0; // Variable to track the number of transactions
uint32_t totalNumOfTransactions = 0;

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
    if (accountReference->balance < terminalData->amount) {
        return LOW_BALANCE;
    } else {
        return SERVER_OK;
    }
}

// Test function to test the isAmountAvailable function
void isAmountAvailableTest(void) {
    ST_terminal_t terminalData;
    ST_accountsDB_t accountReference;
    accountReference = accountsDB[0];
    EN_serverError_t resultFlag;
    printf("Tester Name: Mohamed Ibrahim\n");
    printf("Function Name: isAmountAvailable\n\n");
    printf("---------------------------\n");

    // Test Case 1: check if the entered amount is less than the balance
    printf("Test Case 1:\n");
    printf("Input Data: Terminal amount = 500.0, Account balance = 1000.0\n");
    printf("Expected Result: SERVER_OK\n");
    terminalData.amount = 1000.0;
    resultFlag = isAmountAvailable(&terminalData,&accountReference);
    if (resultFlag == SERVER_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

   
    // Test Case 2: check if the entered amount equals the balance
    printf("Test Case 2: Valid Amount(equals the balance)\n");
    printf("Expected Result: Amount is Valid\n");
    terminalData.amount = 2000.0;
    resultFlag = isAmountAvailable(&terminalData,&accountReference);
    if (resultFlag == SERVER_OK)
        printf("Actual Result: Amount is Valid\n");
    else
        printf("Actual Result: Invalid Amount\n");
    printf("---------------------------\n");

    // Test Case 3: check if the entered amount more the balance
    printf("Test Case 3: Invalid Amount(more than the balance)\n");
    printf("Expected Result: Low Balance\n");
    terminalData.amount = 10000.0;
    resultFlag = isAmountAvailable(&terminalData,&accountReference);
    if (resultFlag == LOW_BALANCE)
        printf("Actual Result: Low Balance\n");
    else
        printf("Actual Result: Low Balance\n");
    printf("---------------------------\n");
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
}

///////////////////////////// Implement listSavedTransactions function //////////////////////////////////////////////
void listSavedTransactions(void)
{
if (totalNumOfTransactions==0)
    {
        printf(" --> NO TRANSACTIONS EXIST <-- \n");
    }
    else
    {
        printf("__| List of Saved Transactions:\n");
        printf("___________________________\n");

        for (int i = 0; i < 255; i++)
        {
            if (transactionDB[i].transactionSequenceNumber != 0)
            {
                printf("_______________|\n");
                printf("Transaction Sequence Number: %u\n", transactionDB[i].transactionSequenceNumber);
                printf("Transaction Date: %s\n", transactionDB[i].terminalData.transactionDate);
                printf("Cardholder Name: %s\n", transactionDB[i].cardHolderData.cardHolderName);
                printf("PAN: %s\n", transactionDB[i].cardHolderData.primaryAccountNumber);
                printf("Card Expiration Date: %s\n", transactionDB[i].cardHolderData.cardExpirationDate);
                printf("Transaction Amount: %.2f\n", transactionDB[i].terminalData.transAmount);
                printf("Terminal Max Amount: %.2f\n", transactionDB[i].terminalData.maxTransAmount);

                printf("Transaction State: ");
                switch (transactionDB[i].transState)
                {
                case APPROVED:
                    printf("APPROVED\n");
                    break;
                case DECLINED_INSUFFECIENT_FUND:
                    printf("DECLINED_INSUFFECIENT_FUND\n");
                    break;
                case DECLINED_STOLEN_CARD:
                    printf("DECLINED_STOLEN_CARD\n");
                    break;
                case FRAUD_CARD:
                    printf("FRAUD_CARD\n");
                    break;
                case INTERNAL_SERVER_ERROR:
                    printf("INTERNAL_SERVER_ERROR\n");
                    break;
                default:
                    printf("Unknown state\n");
                }
                printf("_______________|\n");
            }
        }
    }
}    

///////////////////////////// Implement listSavedTransactionsTest function //////////////////////////////////////////////
void listSavedTransactionsTest(void)
{
    printf("Tester Name: Mohamed Ibrahim\n");
    printf("Function Name: listSavedTransactions\n\n");

    // Test Case 1: Happy-case scenario (transactions present in the database)
    printf("Test Case 1:\n");
    printf("Input Data:\n");
    printf("  Transaction database contains transactions\n");
    // Assuming the transaction database has valid transactions
    printf("Expected Result: Transactions should be printed in the specified format\n");
    printf("Actual Result:\n");
    listSavedTransactions();
    printf("\n");

    // Test Case 2: No transactions in the database
    printf("Test Case 2:\n");
    printf("Input Data:\n");
    printf("  Transaction database is empty\n");
    // Clear the transaction database
    for (int i = 0; i < 255; i++)
    {
        transactionDB[i].transactionSequenceNumber = 0;
    }
    printf("Expected Result: No transactions should be printed\n");
    printf("Actual Result:\n");
    listSavedTransactions();
    printf("\n");
}