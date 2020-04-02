#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum { False, True } Boolean;

typedef struct dish {
	char* name;
	int price;
	int quantity;
	int startQuantity;
	char premium;
	struct dish* next;
} dish;

typedef struct dishes {
	dish* head;
	dish* tail;
}dishes;

typedef struct TableOrders {
	int sum;
	int numberOfPremium;
	Boolean isTaken;
	struct dishesOrdered* theDishesThatOrdered;
}TableOrders;

typedef struct dishesOrdered {
	char* dishName;
	int amount; // how many ordered from this dish
	struct dishesOrdered* next;
	struct dishesOrdered* prev;
}dishesOrdered;

Boolean readDishes(dishes* dishesManager, FILE* dishesF);
Boolean printDishes(dishes* dishesManager);
dish* findDish(dishes* dishesManager, char* nameOfDish);
Boolean addItemsToTheKitchen(dishes* dishesManager, char* productName, int quantity);
dish* parameterIntegrityCheck(dishes* dishesManager, int TableNumber, char* ProductName, int Quantity, int numberOfTables);
Boolean makeOrder(dishes* dishesManager, TableOrders* tableOrders, int TableNumber, char* ProductName, int Quantity, int numberOfTables);
Boolean cancelOrder(dishes* dishesManager, TableOrders* tableOrders, int TableNumber, char* ProductName, int Quantity, int numberOfTables);
Boolean removeTable(TableOrders* tableOrders, int TableNumber);
Boolean printTableDishes(TableOrders* tableOrders, int TableNumber, int closeTable);
Boolean showReport(dishes* dishesManager, TableOrders* tableOrders, char para, char* ProductName, int tableNumber);
void Error_Msg(char* msg);