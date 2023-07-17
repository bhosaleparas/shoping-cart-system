#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_ITEMS 100
#define HASH_TABLE_SIZE 100
#define MAX_USERS 100
#define MAX_LOGGED_IN_USERS 2

typedef struct
{
    int itemId;
    char itemName[50];
    float itemPrice;
    int itemSize;
} Item;

typedef struct
{
    char username[50];
    char password[50];
} User;

typedef struct
{
    User users[MAX_USERS];
    int userCount;
} UserDatabase;

typedef struct Node
{
    Item item;
    struct Node *next;
} Node;

typedef struct
{
    Node *itemHashTable[HASH_TABLE_SIZE];
    int itemCount;
} OnlineCartSystem;

typedef struct
{
    char lastLoggedInUsers[MAX_LOGGED_IN_USERS][50];
    int top;
} UserStack;

// initialize online cart system
void initializeSystem(OnlineCartSystem *system)
{
    system->itemCount = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        system->itemHashTable[i] = NULL;
    }
}

void initializeUserDatabase(UserDatabase *database)
{
    database->userCount = 0;
}

void registerUser(UserDatabase *database, const char *username, const char *password)
{
    if (database->userCount >= MAX_USERS)
    {
        printf("User database is full. Cannot register more users.\n");
        return;
    }

    // Check if username contains only alphabetic characters
    for (int i = 0; username[i] != '\0'; i++)
    {
        if (!isalpha(username[i]))
        {
            printf("Invalid username. Only alphabetic characters are allowed.\n");
            return;
        }
    }

    User newUser;
    strcpy(newUser.username, username);
    strcpy(newUser.password, password);

    database->users[database->userCount] = newUser;
    database->userCount++;

    printf("User '%s' registered successfully.\n", username);
}

bool loginUser(UserDatabase *database, UserStack *userStack, const char *username, const char *password)
{
    if (userStack->top > 0)
    {
        printf("User '%s' is already logged in. Please log out first.\n", userStack->lastLoggedInUsers[userStack->top - 1]);
        return false;
    }

    for (int i = 0; i < database->userCount; i++)
    {
        User currentUser = database->users[i];
        if (strcmp(currentUser.username, username) == 0 && strcmp(currentUser.password, password) == 0)
        {
            printf("User '%s' logged in successfully.\n", username);
            strcpy(userStack->lastLoggedInUsers[userStack->top], username);
            userStack->top++;
            return true;
        }
    }
    printf("Invalid username or password. Login failed.\n");
    return false;
}

void logoutUser(UserStack *userStack)
{
    if (userStack->top > 0)
    {
        char username[50];
        strcpy(username, userStack->lastLoggedInUsers[userStack->top - 1]);
        userStack->top--;
        printf("User '%s' logged out successfully.\n", username);
    }
    else
    {
        printf("No user logged in.\n");
    }
}

//  add an item to the item hash table
void addItemToHashTable(OnlineCartSystem *system, Item *item)
{
    int index = item->itemId % HASH_TABLE_SIZE;
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->item = *item;
    newNode->next = NULL;

    if (system->itemHashTable[index] == NULL)
    {
        system->itemHashTable[index] = newNode;
    }
    else
    {
        Node *currentNode = system->itemHashTable[index];
        while (currentNode->next != NULL)
        {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }

    system->itemCount++;
}

// delete  item from the item hash table
void deleteItemFromHashTable(OnlineCartSystem *system, int itemId)
{
    int index = itemId % HASH_TABLE_SIZE;
    Node *currentNode = system->itemHashTable[index];
    Node *previousNode = NULL;

    while (currentNode != NULL && currentNode->item.itemId != itemId)
    {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (currentNode != NULL)
    {
        if (previousNode == NULL)
        {
            system->itemHashTable[index] = currentNode->next;
        }
        else
        {
            previousNode->next = currentNode->next;
        }

        free(currentNode);
        system->itemCount--;
        printf("Item with ID %d deleted.\n", itemId);
    }
    else
    {
        printf("Item with ID %d not found.\n", itemId);
    }
}

// display all items in the hash table
void displayItems(OnlineCartSystem *system)
{
    printf("Items in the cart:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        Node *currentNode = system->itemHashTable[i];
        while (currentNode != NULL)
        {
            printf("Item ID: %d, Item Name: %s, Item Price: %.2f, Item Size: %d (Position: %d)\n",
                   currentNode->item.itemId,
                   currentNode->item.itemName,
                   currentNode->item.itemPrice,
                   currentNode->item.itemSize,
                   i);
            currentNode = currentNode->next;
        }
    }
}

// get user input and add item to the cart
void addItemToCart(OnlineCartSystem *system, UserStack *userStack)
{
    if (userStack->top == 0)
    {
        printf("You need to login first.\n");
        return;
    }
    int numItems;
    printf("Enter the number of items to add: ");
    scanf("%d", &numItems);

    for (int i = 0; i < numItems; i++)
    {
        Item newItem;
        printf("-------------------------------------------------\n");
        printf("| Item ID |        Item Name        |   Item Price  |\n");
        printf("-------------------------------------------------\n");
        printf("|    1    |     Nike Air Max        |    $1520.0    |\n");
        printf("|    2    |  Adidas Ultraboost      |    $1280.0    |\n");
        printf("|    3    |     Puma RS-X           |    $1220.0    |\n");
        printf("|    4    |     Jordan Shoes        |    $1520.0    |\n");
        printf("|    5    |     Woodland Shoes      |    $2180.0    |\n");
        printf("|    6    | Puma Sandle RS-X        |   $12220.0    |\n");
        printf("|    7    | Nike Air 1ST copy       |    $1520.0    |\n");
        printf("|    8    |     Sparx Shoes         |    $1820.0    |\n");
        printf("|    9    |     Puma Sandle         |   $12220.0    |\n");
        printf("|   10    |     Sparx Sandle        |    $1520.0    |\n");
        printf("|   11    |     Adidas Chapples     |    $1280.0    |\n");
        printf("|   12    |     Puma R-1            |   $80000.0    |\n");
        printf("|   13    |     Nike Air Max        |    $1520.0    |\n");
        printf("|   14    |     Bata Sandles        |    $1820.0    |\n");
        printf("|   15    |     Bata Shoes          |    $1220.0    |\n");
        printf("|   16    |     Reebok Sandles      |    $12250.0   |\n");
        printf("|   17    |     Reebok Shoes        |    $4580.0    |\n");
        printf("|   18    |     Asks Sandles        |    $1201.0    |\n");
        printf("|   19    |      Asks Shoes         |   $15110.0    |\n");
        printf("|   20    |    Metro Shoes          |    $1810.0    |\n");
        printf("|   21    |    Metro Sandles        |    $1201.0    |\n");
        printf("|   22    |      Metro 1cpy         |    $1150.0    |\n");
        printf("|   101   |     Crocs Shoes         |   $12280.0    |\n");
        printf("|   102   |   Crocs Sandles         |   $11120.0    |\n");
        printf("-------------------------------------------------\n");
        printf("Enter the Item ID:\n ");
        scanf("%d", &newItem.itemId);

        switch (newItem.itemId)
        {
        case 1:
            strcpy(newItem.itemName, "Nike Air Max");
            newItem.itemPrice = 1520.0;
            break;
        case 2:
            strcpy(newItem.itemName, "Adidas Ultraboost");
            newItem.itemPrice = 1280.0;
            break;
        case 3:
            strcpy(newItem.itemName, "Puma RS-X");
            newItem.itemPrice = 1220.0;
            break;
        case 4:
            strcpy(newItem.itemName, "Jordan Shoes");
            newItem.itemPrice = 1520.0;
            break;
        case 5:
            strcpy(newItem.itemName, "Woodland Shoes");
            newItem.itemPrice = 2180.0;
            break;
        case 6:
            strcpy(newItem.itemName, "Puma sandle RS-X");
            newItem.itemPrice = 12220.0;
            break;
        case 7:
            strcpy(newItem.itemName, "Nike Air 1ST copy");
            newItem.itemPrice = 1520.0;
            break;
        case 8:
            strcpy(newItem.itemName, "Sparx Shoes");
            newItem.itemPrice = 1820.0;
            break;
        case 9:
            strcpy(newItem.itemName, "Puma Sandle");
            newItem.itemPrice = 12220.0;
            break;
        case 10:
            strcpy(newItem.itemName, "Sparx Sandle");
            newItem.itemPrice = 1520.0;
            break;
        case 11:
            strcpy(newItem.itemName, "Adidas Chapple");
            newItem.itemPrice = 1280.0;
            break;
        case 12:
            strcpy(newItem.itemName, "Puma R-1");
            newItem.itemPrice = 80000.0;
            break;
        case 13:
            strcpy(newItem.itemName, "Nike Air Max");
            newItem.itemPrice = 1520.0;
            break;
        case 14:
            strcpy(newItem.itemName, "Bata SAndle");
            newItem.itemPrice = 1820.0;
            break;
        case 15:
            strcpy(newItem.itemName, "Bata Shoes");
            newItem.itemPrice = 1220.0;
            break;
        case 16:
            strcpy(newItem.itemName, "Reebok Sandles");
            newItem.itemPrice = 12250.0;
            break;
        case 17:
            strcpy(newItem.itemName, "Reebok Shoes");
            newItem.itemPrice = 4580.0;
            break;
        case 18:
            strcpy(newItem.itemName, "Asks SAndles");
            newItem.itemPrice = 1201.0;
            break;
        case 19:
            strcpy(newItem.itemName, "Asks Shoes");
            newItem.itemPrice = 15110.0;
            break;
        case 20:
            strcpy(newItem.itemName, "Metro Shoes");
            newItem.itemPrice = 1810.0;
            break;
        case 21:
            strcpy(newItem.itemName, "Metro Sandles");
            newItem.itemPrice = 1201.0;
            break;
        case 22:
            strcpy(newItem.itemName, "Metro 1cpy");
            newItem.itemPrice = 1150.0;
            break;
        case 101:
            strcpy(newItem.itemName, "Crocs Shoes");
            newItem.itemPrice = 12280.0;
            break;
        case 102:
            strcpy(newItem.itemName, "Crocs Sandles");
            newItem.itemPrice = 11120.0;
            break;
        default:
            printf("Invalid Item ID. Item not added.\n");
            continue;
        }

        printf("Enter the Item Size: ");
        scanf("%d", &newItem.itemSize);

        addItemToHashTable(system, &newItem);
        printf("Item added to the cart.\n");
    }
}

// calculate the total price of items
float calculateTotalPrice(OnlineCartSystem *system)
{
    float totalPrice = 0.0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        Node *currentNode = system->itemHashTable[i];
        while (currentNode != NULL)
        {
            totalPrice += currentNode->item.itemPrice;
            currentNode = currentNode->next;
        }
    }
    return totalPrice;
}

int main()
{
    OnlineCartSystem system;
    initializeSystem(&system);
    UserStack userStack;
    userStack.top = 0;
    UserDatabase userDB;
    initializeUserDatabase(&userDB);

    bool isAuthenticated = false;
    int choice;
    do
    {
        printf("\n----- Online Cart System -----\n");
        printf("1. Register User\n");
        printf("2. Login\n");
        printf("3. Add Item to Cart\n");
        printf("4. Delete Item from Cart\n");
        printf("5. Calculate Total Cost\n");
        printf("6. Display Items in Cart\n");
        printf("7. Logout\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            char username[50], password[50];
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            registerUser(&userDB, username, password);
            break;
        }
        case 2:
        {
            char username[50], password[50];
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            if (loginUser(&userDB, &userStack, username, password))
            {
                isAuthenticated = true;
            }
            break;
        }
        case 3:
            if (isAuthenticated)
            {
                addItemToCart(&system, &userStack);
            }
            else
            {
                printf("You need to login first.\n");
            }
            break;
        case 4:
        {
            int itemIdToDelete;
            printf("Enter the ID of the item to delete: ");
            scanf("%d", &itemIdToDelete);
            deleteItemFromHashTable(&system, itemIdToDelete);
            break;
        }
        case 5:
        {
            float totalPrice = calculateTotalPrice(&system);
            printf("Total Price: %.2f\n", totalPrice);
            break;
        }
        case 6:
            displayItems(&system);
            break;
        case 7:
            logoutUser(&userStack);
            isAuthenticated = false;
            break;

        case 0:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 0);

    return 0;
}
