//Header guard 
#ifndef CARD_H
#define CARD_H

#include <stdint.h>
typedef struct cardData
{
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[6];
} cardData;

typedef enum cardError
{
    cardOk = 0,
    WRONG_NAME,
    WRONG_EXP_DATE,
    WRONG_PAN
} cardError;

typedef enum cardState
{
    BLOCKED = 0,
    RUNNING
} cardState;


cardError getCardHolderName(cardData* CardData);
cardError getCardExpiryDate(cardData* CardData);
cardError getCardPAN(cardData* CardData);

//End of the header guard
#endif
