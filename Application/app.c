#include "app.h"
#include "../Card/card.h"
#include "../Server/server.h"
#include "../Terminal/terminal.h"
#include <stdio.h>
#include <stdbool.h>

void appStart()
{
	ST_cardData_t cardData;
	ST_terminalData_t terminalData;
	ST_transaction_t transactionData;
	EN_terminalError_t terminalReturn;
	EN_serverError_t serverReturn;
	EN_cardError_t cardReturn;
	bool exitFlag = true;


    printf("Welcome to our Payment Application\n");
    printf("-----------------------------------------------------------------------------\n");
	while (exitFlag != false)
	{
		int userChoice = -1;
		printf("Enter 0 to Exit and 1 to enter your data\n");
		printf("-----------------------------------------------------------------------------\n");
		
		printf("Thanks to use our Payment application\n");
	}
}