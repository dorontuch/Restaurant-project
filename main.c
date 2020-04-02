#include "header.h"

void main(char* args) {
	FILE* DishesF, * Instruction;
	dishes dishesManager;
	TableOrders* tableOrders;
	int numberOfTables;

	int func, amountToAdd, tableNumber, temp, error;
	char action, nameOfTheDish[50];

	dishesManager.head = dishesManager.tail = NULL;
	//DishesF = fopen("./files/Manot.txt", "r");
	fopen_s(&DishesF, "./files/Manot.txt", "r");
	fopen_s(&Instruction, "./files/Instructions.txt", "r");

	if (DishesF != 0 && Instruction != 0) {
		printf("Please enter the number of the tables\n");
		if (scanf_s("%d", &numberOfTables) == 0)
			Error_Msg("The input is illegal");
		else {
			tableOrders = (TableOrders*)malloc(sizeof(TableOrders) * numberOfTables);
			if (tableOrders == NULL) {
				Error_Msg("cannot allocate memory for the tables");
				exit(1);
			}
			for (int i = 0; i < numberOfTables; i++) {
				tableOrders[i].theDishesThatOrdered = NULL;
				tableOrders[i].isTaken = False;
				tableOrders[i].sum = 0;
			}

			while (fscanf(Instruction, "%d", &func) != EOF) {
				fseek(Instruction, 1, SEEK_CUR);
				switch (func)
				{
				case 1:
					printf("You ask to read the Dishes\n");
					if (readDishes(&dishesManager, DishesF) == False)
						printf("Couldn't read the Disehs\n");
					else
						printf("The dishes read successfully\n");
					break;
				case 2:

					if (fscanf(Instruction, "%s %d", nameOfTheDish, &amountToAdd) == 2) {
						printf("You ask to add %d the quantity of %s to the kitchen\n", amountToAdd, nameOfTheDish);
						if (addItemsToTheKitchen(&dishesManager, nameOfTheDish, amountToAdd) == False)
							printf("The request has failed\n");
						else
							printf("The quantity has update\n");
					}
					break;
				case 3:
					if (fscanf(Instruction, "%d %s %d", &tableNumber, nameOfTheDish, &amountToAdd) == 3) {
						printf("You ask to make an order of %d of %s to table %d\n",amountToAdd, nameOfTheDish,tableNumber);
						if (makeOrder(&dishesManager, tableOrders, tableNumber - 1, nameOfTheDish, amountToAdd, numberOfTables) == False)
							printf("The request has failed\n");
						else
							printf("The order has been successfully\n");
					}
					break;
				case 4:
					if (fscanf(Instruction, "%d %s %d", &tableNumber, nameOfTheDish, &amountToAdd) == 3) {
						printf("You ask to cancel %d of %s in table %d\n",amountToAdd, nameOfTheDish,tableNumber);
						if (cancelOrder(&dishesManager, tableOrders, tableNumber - 1, nameOfTheDish, amountToAdd, numberOfTables) == False)
							printf("The request to cancel has failed\n");
						else
							printf("The order has been successfully cancelled\n");
					}
					break;
				case 5:
					if (fscanf(Instruction, "%d", &tableNumber) == 1) {
						printf("You ask to colse table no.%d\n",tableNumber);
						if (removeTable(tableOrders, tableNumber - 1) == False)
							printf("The request to colse the table as failes\n");
						else
							printf("The table has been closed\n");
					}
					break;
				case 6:
					if (fscanf(Instruction, "%c %s", &action, nameOfTheDish) == 2) {
						printf("You ask to see a report of ");
						tableNumber = 0;
						switch (action)
						{
						case'A':
							printf("the stock in the kitchen\n");
							break;
						case 'B':
							printf("the amount of order of the dish: %s\n",nameOfTheDish);
							break;
						case 'C':
							printf("the orders of table %d\n",tableNumber);
							tableNumber = nameOfTheDish[0] - '0';
							break;
						default:
							break;
						}
						if (showReport(&dishesManager, tableOrders, action, nameOfTheDish, tableNumber - 1) == False)
							printf("The producing the report has failed\n");
					}
					break;
				default:
					break;
				}
				printf("---------------\n");
				do {
					temp = 1;
					printf("to continue press 0\n");
					if (scanf(" %d", &temp) != 1) printf("iilegal input\n");
					while (getchar() != '\n');
				} while (temp != 0);
				printf("---------------\n\n\n");
			}
		}
	}
	else
		printf("\nCouldn't open the files\n");

	printf("\n\nThe program finished\n\n");
	fclose(DishesF);
	fclose(Instruction);
}