#include <stdio.h>
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    char name[25] = {0};
    printf("Please Enter Your Name [20-24] characters : ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Removing newline character

    int len = strlen(name);
    if (len < 20 || len > 24)
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
    char expDate[6] = {0};
    printf("Please Enter Your Card Expiry Date in [MM/YY] Format : ");
    fgets(expDate, sizeof(expDate), stdin);
    expDate[strcspn(expDate, "\n")] = '\0'; // Removing newline character

    if (strlen(expDate) != 5 || expDate[2] != '/')
    {
        return WRONG_EXP_DATE;
    }

    int cardMonth = atoi(expDate);
    int cardYear = atoi(expDate + 3);

    if ((cardMonth > 12 || cardMonth < 1) || (cardYear < 0 || cardYear > 99))
    {
        return WRONG_EXP_DATE;
    }

    strcpy(cardData->cardExpirationDate, expDate);

    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char panNumber[30];
	for (int i = 0; i < 30; i++)
	{
		panNumber[i] = '\0';
	}

	printf("Please Enter Your PAN Number [16-19] characters : ");

	char ch;
	int i = 0;
	fseek(stdin, 0, SEEK_END);
	while ((ch = getchar()) != '\n')
	{
		panNumber[i] = ch;
		i++;
	}

	int len = strlen(panNumber);
	if (!panNumber) return WRONG_PAN;
	if (len > 19 || len < 16)
	{
		return WRONG_PAN;
	}
	else
	{
			for (int i = 0; i <= len; i++)
			{
				cardData->primaryAccountNumber[i] = panNumber[i];
			}
			return CARD_OK;
	}
}
