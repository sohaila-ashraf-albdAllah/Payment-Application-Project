#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include <stdio.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {

    char c;
    int count = 0;
    char dayStr[3], monthStr[3], yearStr[5];
    int day, month, year;
    printf("Please, Enter a valid transaction Date in the format of dd/mm/yyyy: ");
    while ((c = getchar()) != '\n')
    {
        if (count < 11)
        {
            if (c != '/' && (c < '0' || c > '9'))
            {
                return WRONG_DATE;
            }
            termData->transactionDate[count++] = c;
        }
    }


    if (count != 10 || termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/')
    {
        return WRONG_DATE;
    }
    sscanf_s((const char*)termData->transactionDate, "%2s/%2s/%4s", dayStr, sizeof(dayStr), monthStr, sizeof(monthStr), yearStr, sizeof(yearStr));
    //sscanf((const char*)termData->transactionDate, "%2s/%2s/%4s", dayStr, monthStr, yearStr);
    day = atoi(dayStr);
    month = atoi(monthStr);
    year = atoi(yearStr);

    if (day < 1 || day > 31 || month < 1 || month > 12 || year <= 0 || year > 2024) {
        return WRONG_DATE;
    }
    return TERMINAL_OK;
}
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {

    int cardDay, cardMonth;
    sscanf_s(cardData->cardExpirationDate, "%2d/%2d", &cardDay, &cardMonth);

    int transDay, transMonth;
    sscanf_s(termData->transactionDate, "%2d/%2d", &transDay, &transMonth);

    if (cardMonth < transMonth)
        return EXPIRED_CARD;
    else if (cardMonth > transMonth)
        return TERMINAL_OK;
    else {
        if (cardDay < transDay)
            return EXPIRED_CARD;
        else if (cardDay > transDay)
            return TERMINAL_OK;
        else
            return TERMINAL_OK;
    }
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
    printf("Please, Set the Maximum Amount per transaction: ");

    while (scanf_s("%f", &maxAmount) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\nPlease enter a valid Amount: ");
    }
    if (maxAmount <= 0)
        return INVALID_MAX_AMOUNT;
    else
        termData->maxTransAmount = maxAmount;
    return TERMINAL_OK;

}
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    printf("Please, Enter valid Amount Transaction (don't exceed the max amount): ");
    while (scanf_s("%f", &termData->transAmount) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\nPlease enter a valid Amount: ");
    }
    if (termData->transAmount <= 0)
        return INVALID_AMOUNT;
    else
        return TERMINAL_OK;

}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    if (termData->transAmount > termData->maxTransAmount)
        return EXCEED_MAX_AMOUNT;
    else
        return TERMINAL_OK;
}