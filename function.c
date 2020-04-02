#include "header.h"

Boolean readDishes(dishes* dishesManager, FILE* dishesF) {
	char nameOfDishe[50], premium;
	int price, quantity, lengthOfDishName;
	dish* newDish;
	while (fscanf(dishesF, "%s %d %d %c", nameOfDishe, &quantity, &price, &premium) != EOF) {
		//check if the dish has inputted once before
		if (findDish(dishesManager, nameOfDishe) != NULL) {
			printf("the dish %s already entered\n", nameOfDishe);
			return False;
		}
		else {
			//if the dish is new
			//check the if the quantity of the dish is legal
			if (quantity < 0) {
				printf("the emount of dish %s is illegal\n", nameOfDishe);
				return False;
			}
			//if not exist and amount is legal create new dish
			else {
				newDish = (dish*)malloc(sizeof(struct dish));
				if (newDish == NULL) {
					Error_Msg("can not allocate memory for dish object");
					exit(1);
				}
				lengthOfDishName = strlen(nameOfDishe) + 1;
				newDish->name = (char*)malloc(sizeof(char) * lengthOfDishName);
				if (newDish->name == NULL) {
					Error_Msg("can not allocate memory for name of dish");
					exit(1);
				}
				strcpy(newDish->name, nameOfDishe);
				newDish->quantity = quantity;
				newDish->startQuantity = quantity;
				newDish->price = price;
				newDish->premium = premium;
				newDish->next = NULL;
				if (dishesManager->head == NULL) {
					dishesManager->head = dishesManager->tail = newDish;
				}
				else {
					dishesManager->tail->next = newDish;
					dishesManager->tail = newDish;
				}
			}
		}
	}
	return True;
}

dish* findDish(dishes* dishesManager, char* nameOfDish) {
	dish* tempDishForLookUp;
	tempDishForLookUp = dishesManager->head;
	while (tempDishForLookUp) {
		if (!strcmp(tempDishForLookUp->name, nameOfDish))
			return tempDishForLookUp;
		tempDishForLookUp = tempDishForLookUp->next;
	}
	return NULL;
}

Boolean printDishes(dishes* dishesManager) {
	dish* tempForPrint;
	if (dishesManager->head == NULL)
		return False;
	tempForPrint = dishesManager->head;
	printf("----- The dishes in the kitchen -----\n");
	while (tempForPrint) {
		printf("%s %d %d %c\n", tempForPrint->name, tempForPrint->quantity, tempForPrint->price, tempForPrint->premium);
		tempForPrint = tempForPrint->next;
	}
	return True;
}

Boolean addItemsToTheKitchen(dishes* dishesManager, char* productName, int quantity) {
	dish* tempDishForAddQuantityTo;
	if (quantity < 0) {
		printf("\nthe quantity you asked to add to \"%s\" is illegal\n\n", productName);
	}
	else {
		tempDishForAddQuantityTo = findDish(dishesManager, productName);
		if (tempDishForAddQuantityTo == NULL) {
			printf(1, "the dish %s doesn't exist in the kitchen");
			return False;
		}
		else {
			tempDishForAddQuantityTo->quantity += quantity;
		}
	}
	return True;
}

Boolean makeOrder(dishes* dishesManager, TableOrders* tableOrders, int TableNumber, char* ProductName, int Quantity, int numberOfTables) {
	dish* orderedDish;
	dishesOrdered* newDishForTable, * temp;
	int lengthOfProductName;
	Boolean doesTableOrderedDish = False;
	orderedDish = parameterIntegrityCheck(dishesManager, TableNumber, ProductName, Quantity, numberOfTables);
	if (orderedDish != NULL) {
		if (orderedDish->quantity < Quantity) {
			printf("there isn't enough dishes in the kitchen");
			return False;
		}
		//check if the table already ordered this dish
		temp = tableOrders[TableNumber].theDishesThatOrdered;

		for (; temp != NULL; temp = temp->next) {
			if (strcmp(temp->dishName, ProductName) == 0) {
				doesTableOrderedDish = True;
				break;
			}
		}
		if (temp != NULL) {
			tableOrders[TableNumber].sum += (orderedDish->price * Quantity);

			temp->amount += Quantity;
			//save the number of premium dishes the table ordered
			if (orderedDish->premium == 'Y') {
				tableOrders[TableNumber].numberOfPremium += Quantity;
			}

			//if he does update the quantity and exit the function
			if (doesTableOrderedDish == True)
				return True;
		}

		//otherwise create new dish for this table
		newDishForTable = (dishesOrdered*)malloc(sizeof(dishesOrdered));
		if (newDishForTable == NULL) {
			Error_Msg("couldn't able to allocate memory for new dish on reservetion");
			exit(1);
		}
		tableOrders[TableNumber].sum += orderedDish->price * Quantity;
		tableOrders[TableNumber].numberOfPremium = 0;


		lengthOfProductName = strlen(orderedDish->name) + 1;
		newDishForTable->dishName = (char*)malloc(sizeof(char) * lengthOfProductName);
		strcpy(newDishForTable->dishName, orderedDish->name);
		newDishForTable->amount = Quantity;


		newDishForTable->next = newDishForTable->prev = NULL;

		//update the quatntity in the kitchen
		orderedDish->quantity -= Quantity;

		//insert the dish into the linked list of the table
		if (tableOrders[TableNumber].theDishesThatOrdered == NULL) {
			tableOrders[TableNumber].theDishesThatOrdered = newDishForTable;
		}
		else {
			newDishForTable->next = tableOrders[TableNumber].theDishesThatOrdered;
			tableOrders[TableNumber].theDishesThatOrdered->prev = newDishForTable;
			tableOrders[TableNumber].theDishesThatOrdered = newDishForTable;
		}
		if (tableOrders[TableNumber].isTaken == False)
			tableOrders[TableNumber].isTaken = True;
	}
	return True;
}

Boolean cancelOrder(dishes* dishesManager, TableOrders* tableOrders, int TableNumber, char* ProductName, int Quantity, int numberOfTables) {
	dish* orderedDish;
	dishesOrdered* tempForLookup;
	orderedDish = parameterIntegrityCheck(dishesManager, TableNumber, ProductName, Quantity, numberOfTables);
	if (orderedDish == NULL)
		return False;
	tempForLookup = tableOrders[TableNumber].theDishesThatOrdered;
	while (tableOrders[TableNumber].isTaken == True) {
		if (strcmp(tempForLookup->dishName, ProductName) == 0) {
			if (tempForLookup->amount < Quantity) {
				printf("You request to cancel %d dishes of %s \nbut the order was %d dishes, the request is cancelled", Quantity, ProductName, tempForLookup->amount);
				return False;
			}
			if (orderedDish->premium == 'Y') {
				tableOrders[TableNumber].numberOfPremium -= Quantity;
			}
			tempForLookup->amount -= Quantity;
			tableOrders[TableNumber].sum -= (orderedDish->price * Quantity);
			if (tempForLookup->amount == 0) {
				if ((tempForLookup->next == NULL) && (tempForLookup->prev == NULL));
				else if (tempForLookup->next == NULL)
					tempForLookup->prev->next = tempForLookup->next;
				else if (tempForLookup->prev == NULL)
					tempForLookup->next->prev = tempForLookup->prev;
				else {
					tempForLookup->prev->next = tempForLookup->next;
					tempForLookup->next->prev = tempForLookup->prev;
				}
				free(tempForLookup->dishName);
				free(tempForLookup);
			}
			break;
		}
		tempForLookup = tempForLookup->next;
	}
	return True;

}

dish* parameterIntegrityCheck(dishes* dishesManager, int TableNumber, char* ProductName, int Quantity, int numberOfTables) {
	dish* orderedDish;
	orderedDish = findDish(dishesManager, ProductName);

	if (orderedDish == NULL)
		printf("the dish %s that table #%d ordered is not valid", ProductName, TableNumber);

	if (TableNumber > numberOfTables) {
		printf("the number of the table is illegal");
		orderedDish = NULL;
	}
	if (Quantity <= 0) {
		printf("The requested quatntity is invalid");
		orderedDish = NULL;
	}
	return orderedDish;
}

Boolean removeTable(TableOrders* tableOrders, int TableNumber) {
	int totalToPay;
	if (printTableDishes(tableOrders, TableNumber, 1) == False)
		return False;
	totalToPay = tableOrders[TableNumber].sum;
	if (tableOrders[TableNumber].numberOfPremium > 0) {
		printf("Your table is a primum\n");
		totalToPay *= 1.1;
	}
	printf("The total amount payable is: %d\n", totalToPay);
	tableOrders[TableNumber].isTaken = False;
	return True;
}

Boolean printTableDishes(TableOrders* tableOrders, int TableNumber, int closeTable) {
	dishesOrdered* tempDishesOrderedForPrinting, * toDelete;
	if (tableOrders[TableNumber].isTaken == False) {
		printf("The table %d doens't order anything\n", TableNumber);
		return False;
	}
	tempDishesOrderedForPrinting = tableOrders[TableNumber].theDishesThatOrdered;
	toDelete = tempDishesOrderedForPrinting;
	printf("The orders for table %d:\n", TableNumber);
	while (tempDishesOrderedForPrinting) {
		printf("amount:%d of:%s\n", tempDishesOrderedForPrinting->amount, tempDishesOrderedForPrinting->dishName);
		tempDishesOrderedForPrinting = tempDishesOrderedForPrinting->next;
		if (closeTable) {
			free(toDelete->dishName);
			free(toDelete);
		}
		toDelete = tempDishesOrderedForPrinting;
	}
	return True;
}

Boolean showReport(dishes*dishesManager, TableOrders* tableOrders, char para, char* ProductName, int tableNumber) {
	dish* orderedDish;
	dishesOrdered* theDishesThatOrdered;
	int sum = 0;
	switch (para)
	{
	case 'A':
		printDishes(dishesManager);
		break;
	case 'B':
		orderedDish = findDish(dishesManager, ProductName);
		printf("The dish '%s' has been ordered today %d times\n", ProductName, (orderedDish->startQuantity - orderedDish->quantity));
		break;
	case 'C':
		if (tableOrders[tableNumber].isTaken == True)
			printf("The table %d is Free\n", tableNumber);
		else
			printTableDishes(tableOrders, tableNumber, 0);
		break;
	default:
		return False;
		break;
	}
	return True;
}

void Error_Msg(char* msg)
{
	printf("\n%s\n", msg);
	exit(1);
}