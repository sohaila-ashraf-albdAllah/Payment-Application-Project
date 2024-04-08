#include <stdio.h>
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    char name[25];
    for (uint8_t i = 0; i < 25; i++)
    {
        name[i] = '\0';
    }

    printf("Please Enter Your Name [20-24] characters : ");
    char ch;
    uint8_t i = 0;
    while ((ch = getchar()) != '\n')
    {
        name[i] = ch;
        i++;
    }

    int len = strlen(name);
    if (len == 0 || len > 24 || len < 20)
    {
        return WRONG_NAME;
    }
    else
    {
        strcpy(cardData->cardHolderName, name);
        return CARD_OK;
    }
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{

    char expDate[100];
    uint8_t cardMonth, cardYear;
    for (uint8_t i = 0; i < 100; i++)
    {
        expDate[i] = '\0';
    }

    printf("Please Enter Your Card Expiry Date in [MM/YY] Format : ");
    fseek(stdin, 0, SEEK_END);
    scanf("%s", expDate);

    if (strlen(expDate) != 5)
    {
        return WRONG_EXP_DATE;
    }
    else if (expDate[2] != '/')
    {
        return WRONG_EXP_DATE;
    }

    cardMonth = (expDate[0] - '0') * 10 + (expDate[1] - '0');
    cardYear = (expDate[3] - '0') * 10 + (expDate[4] - '0');

    if ((cardMonth > 12 || cardMonth < 1) || (cardYear < 0 || cardYear > 99))
    {
        return WRONG_EXP_DATE;
    }

    strcpy(cardData->cardExpirationDate, expDate);

    return CARD_OK;
}


int isLuhnValid(const char *number)
{
    int len = strlen(number);
    int sum = 0;
    int alternate = 0;

    for (int i = len - 1; i >= 0; i--)
    {
        int digit = number[i] - '0';

        if (alternate)
        {
            digit *= 2;
            if (digit > 9)
            {
                digit -= 9;
            }
        }

        sum += digit;
        alternate = !alternate;
    }

    return (sum % 10 == 0);
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    char panNumber[30];
    for (uint8_t i = 0; i < 30; i++)
    {
        panNumber[i] = '\0';
    }

    printf("Please Enter Your PAN Number [16-19] characters : ");

    char ch;
    uint8_t i = 0;
    fseek(stdin, 0, SEEK_END);
    while ((ch = getchar()) != '\n')
    {
        panNumber[i] = ch;
        i++;
    }

    int len = strlen(panNumber);
    if (len < 16 || len > 19)
    {
        return WRONG_PAN;
    }
    else if (!isLuhnValid(panNumber))
    {
        return WRONG_PAN;
    }

    strcpy(cardData->primaryAccountNumber, panNumber);
    return CARD_OK;
}

//void setCardState(ST_cardData_t *cardData, EN_cardState_t newState)
//{
//
//}

int main()
{
    ST_cardData_t cardData;
    EN_cardError_t result;

    result = getCardHolderName(&cardData);
    if (result != CARD_OK)
    {
        printf("Error: Invalid card holder name.\n");
        return 1;
    }

    result = getCardExpiryDate(&cardData);
    if (result != CARD_OK)
    {
        printf("Error: Invalid card expiry date.\n");
        return 1;
    }

    result = getCardPAN(&cardData);
    if (result != CARD_OK)
    {
        printf("Error: Invalid PAN.\n");
        return 1;
    }
    return 0;
}
