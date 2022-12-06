#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* constants */
#define DISCOUNT_PERCENTAGE 0.20
#define MAX_SALES 10
/* menu options as constants */
#define MENU_OPTION_BUY_CAR 'a'
#define MENU_OPTION_VIEW_SALES 'b'
#define MENU_OPTION_EXIT 'x'
#define MENU_OPTION_VIEW_CARS_STOCK_SORT 'c'

/* booleans */
#define TRUE 1
#define FALSE 0

/* Variables */
unsigned int carSold = 0, carsNeeded = 0, discount = FALSE, carsSold = 0, correctInput = FALSE, indexOfSales = 0, carType;
unsigned short carAmountPerSale[MAX_SALES];
unsigned short typeOfCarPerSale[MAX_SALES];
unsigned short discountGivenPerSale[MAX_SALES];
unsigned int customerAgePerSale[MAX_SALES];
float totalSalesValue;
float totalPricePerSale[MAX_SALES];
float carPrices[10] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
char carTypes[10][30] = {"Range Rover", "Audi A8", "BMW7 Series", "Mercedes CLS", "MINI", "Lexus LS",
                         "Mercedes S-Class", "Tesla Model Y", "Mercedes EQS", "Mercedes G-class"};
int availableCars[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
char customerNames[MAX_SALES][201];
int sortedAvailableCars[10];

// file path
char fileName[] = "/Users/mirjalol/sites/study/car-sales/data.csv";


/* Functions */
int returnNumberOfCars() {
    return (int) (sizeof(carTypes) / sizeof(carTypes[0]));
}

int returnTotalAvailableCars() {
    int totalAvailableCars = 0;
    int numberOfCars = returnNumberOfCars();
    for (int i = 0; i < numberOfCars; i++) {
        totalAvailableCars += availableCars[i];
    }
    return totalAvailableCars;
}

// write data to file
void writeToFile() {
    FILE *file;
    file = fopen(fileName, "w");
    if (file != NULL) {
        for (int i = 0; i < indexOfSales; ++i) {
            int typeOfCar = typeOfCarPerSale[i];
            fprintf(file, "%d,%f,%d,%f,%hd,%hu,%s,%d \n", i, totalPricePerSale[i], typeOfCar,
                    carPrices[typeOfCar],
                    carAmountPerSale[i],
                    discountGivenPerSale[i], customerNames[i], customerAgePerSale[i]);
        }
    }
    fclose(file);
}

// read data from file and create if file doesn't exist
void readDataFromFile() {
    FILE *file;

    file = fopen(fileName, "r");
    if (file == NULL) {
        file = fopen(fileName, "wb");
        readDataFromFile();
    } else {
        while (1) {
            unsigned int amount = 0, discount = 0, typeOfCar, age;
            float totalPrice = 0, price = 0;
            char name[201];
            int scanResult = fscanf(file, "%d,%f,%d,%f,%d,%u,%[^,],%d", &indexOfSales,
                                    &totalPrice,
                                    &typeOfCar,
                                    &price,
                                    &amount,
                                    &discount,
                                    &name,
                                    &age);
            if (scanResult == EOF) {
                break;
            }
            totalPricePerSale[indexOfSales] = totalPrice;
            typeOfCarPerSale[indexOfSales] = typeOfCar;
            carAmountPerSale[indexOfSales] = amount;
            discountGivenPerSale[indexOfSales] = discount;
            strcpy(customerNames[indexOfSales], name);
            customerAgePerSale[indexOfSales] = age;
            availableCars[typeOfCarPerSale[indexOfSales]] -= amount;
            indexOfSales++;
        };
    }
    fclose(file);
}

char getCharFromConsole(char message[201]) {
    char userInput;
    printf("%s", message);
    scanf("\n%c", &userInput);
    return userInput;
}

unsigned int getUnsignedIntFromConsole(char message[201]) {
    unsigned int userInput;
    printf("%s", message);
    scanf("%d", &userInput);
    return userInput;
}

// pause app
void pauseProgram(char choice) {
    if (choice == MENU_OPTION_EXIT) {
        printf("\n\nPress Enter to Exit...");
    } else {
        printf("\n\nPress Enter to return to the Menu...");
    }

    getchar();
    getchar();
}

void outputWelcomeTitle() {
    printf("Welcome to car sales!\n\n");
}

void showMenuOptions() {
    printf("menu:\n");
    printf("%c. buy car\n", MENU_OPTION_BUY_CAR);
    printf("%c. view sales\n", MENU_OPTION_VIEW_SALES);
    printf("%c. view cars stock\n", MENU_OPTION_VIEW_CARS_STOCK_SORT);
    printf("%c. exit\n\n\n", MENU_OPTION_EXIT);
}

float applyDiscount(float currentPrice) {
    return currentPrice * (1 - DISCOUNT_PERCENTAGE);
}

unsigned short checkIsUserIsStudent(char answer) {
    /* if user is student change value of discount */
    switch (answer) {
        case 'y':
            return TRUE;
        case 'n':
            return FALSE;
    }
}

// return true if user is student
void printDiscount(unsigned int discount) {
    switch (discount) {
        case TRUE:
            printf("\nYou get a discount!");
            break;
        case FALSE:
            printf("\nNo discount given.");
            break;
    }
}

void showCarTypes() {
    /* loop type of car and get needed type from user */
    printf("Car types:\n");
    printf("%-3s | %-20s | %-15s\n", "ID", "Model", "Price");
    printf("________________________________________\n");
    int numberOfCars = returnNumberOfCars();
    for (int i = 0; i < numberOfCars; i++) {
        printf("%-3d | %-20s | %-15f\n", i, carTypes[i], carPrices[i]);
    }
}

void menuBuyCar() {
/* check if there left available cars */
    if (returnTotalAvailableCars() > 0) {
        printf("There are %u cars available.", returnTotalAvailableCars());
    } else {
        /* output sorry title if there is no available cars */
        printf("\nSorry, there are not available cars now.");
        return;
    }

//    output car types
    showCarTypes();

    carType = getUnsignedIntFromConsole("\nPlease enter type of car you need: ");

//    this validation checks that entered car type is exist.
    int lengthCarPrices = (sizeof(carPrices) / sizeof(int)) - 1;
    if (lengthCarPrices < carType) {
        printf("Please enter correct type of car!");
        return;
    }

//  check that if available entered car type
    if (availableCars[carType] <= 0) {
        printf("This car model doesn't available. Please check later!");
        return;
    }

    /* get amount of cars need */
    carsNeeded = getUnsignedIntFromConsole("How many cars do you need? Amount: ");

//  check that user entered correct type of number. For example negative numbers are incorrect(0,-1,-2,-3...) positive numbers are correct (1,2,3)
    if (0 >= carsNeeded) {
        printf("Wrong type of number!");
        return;
    }

    /* if there is not enough cars, output sorry title */
    if (carsNeeded > availableCars[carType]) {
        printf("Sorry, there are fewer cars remaining than you require ");
        return;
    }

    float totalPrice = 0;
    /* reset discount variable */
    discount = FALSE;

    /* asking is user student */
    char answer = getCharFromConsole("Are you student? Answer 'y' or 'n': ");

    /* get customer age */
    int age = 0;
    printf("\n\nHow old are you? age: ");
    scanf("\n%d", &age);
    if (age == 0) {
        printf("please enter number!");
        return;
    } else if (age < 18) {
        printf("Sorry, we can't sell car under 18 years old!");
        return;
    }


    /* get customer name */

    printf("\n\nWhat is your name? Name: ");
    scanf("\n%s", &customerNames[indexOfSales]);

    /* get carType and write to array */

    typeOfCarPerSale[indexOfSales] = carType;
    /* get value of how many cars user need to carAmountPerSale array */
    carAmountPerSale[indexOfSales] = carsNeeded;
    /* calculating total price by type of car*/
    totalPrice = carsNeeded * carPrices[carType];
    availableCars[carType] -= carsNeeded;
    /* if user is student change value of discount */
    if (checkIsUserIsStudent(answer)) {
        discount = TRUE;
        totalPrice = applyDiscount(totalPrice);
    }
    discountGivenPerSale[indexOfSales] = discount;
    totalPricePerSale[indexOfSales] = totalPrice;
    customerAgePerSale[indexOfSales] = age;
    carsSold += carsNeeded;
    /* output total price and how many cars left */
    printf("\n\nThank you!\n\nYou bought %u cars.\n", carsNeeded);
    printDiscount(discount);
    printf("Total price is %f", totalPricePerSale[indexOfSales]);
    printf("\nThere are %u cars left.\n\nHave a good day!", returnTotalAvailableCars());
    indexOfSales++;
}

void menuViewSales() {
    totalSalesValue = 0;
    carSold = 0;
    printf("All Sales Data:\n\n");
    /* loop number of sales */

    printf("%-3s | %-15s | %-20s | %-15s | %-15s | %-15s | %-15s | %-10s\n", "ID", "Total price", "type of car",
           "car price", "cars amount", "discount given", "name", "age");
    printf("____________________________________________________________________________________________________________________________\n");
    for (int i = 0; i < indexOfSales; i++) {
        int typeOfCar = typeOfCarPerSale[i];
        char discountGivenText[4];
        /* if discount given copy discountGivenText = yes and calculate price with discount */
        if (discountGivenPerSale[i] == TRUE) {
            strcpy(discountGivenText, "Yes");
        } else {
            /* else discountGivenText = No */
            strcpy(discountGivenText, "No");
        }
        printf("%-3d | %-15f | %-20s | %-15f | %-15hd | %-15s | %-15s | %-5d \n",
               i, totalPricePerSale[i], carTypes[typeOfCar], carPrices[typeOfCar], carAmountPerSale[i],
               discountGivenText, customerNames[i], customerAgePerSale[i]);
        totalSalesValue += totalPricePerSale[i];
        carSold += carAmountPerSale[i];
    }
    printf("\n%u cars have sold. Total benefit is %f. There are %u cars left.\n",
           carSold, totalSalesValue, returnTotalAvailableCars());
}

void menuViewCarsStockSort() {
//    I learned this bubble sorting algorithm from this website ↓ ↓ ↓
//  https://www.programmingsimplified.com/c/source-code/c-program-bubble-sort

    int sortedCarsTypeIndex[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int numberOfCars = returnNumberOfCars();
    for (int i = 0; i < numberOfCars; ++i) {
        sortedAvailableCars[i] = availableCars[i];
    }

    for (int i = 0; i < numberOfCars - 1; ++i) {
        int swapped = 0;
        for (int j = 0; j < numberOfCars - i - 1; ++j) {
            if (sortedAvailableCars[j] < sortedAvailableCars[j + 1]) {

                int temp = sortedAvailableCars[j];
                int tempIndex = sortedCarsTypeIndex[j];

                sortedCarsTypeIndex[j] = sortedCarsTypeIndex[j + 1];
                sortedCarsTypeIndex[j + 1] = tempIndex;
                sortedAvailableCars[j] = sortedAvailableCars[j + 1];
                sortedAvailableCars[j + 1] = temp;

                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }

    printf("All available cars data:\n\n");
    printf("%-3s | %-20s | %-10s\n", "ID", "Model", "Amount");
    printf("_______________________________________\n");

    for (int i = 0; i < numberOfCars; ++i) {
        printf("%-3d | %-20s | %-10d\n", i, carTypes[sortedCarsTypeIndex[i]], sortedAvailableCars[i]);
    }
    printf("\n");
}

void menuExit() {
    writeToFile();
    printf("Thank you for using car sales app. Goodbye!");
}

void checkCorrectInput(char choice) {
    if (choice == MENU_OPTION_BUY_CAR || choice == MENU_OPTION_VIEW_SALES ||
        choice == MENU_OPTION_EXIT ||
        choice == MENU_OPTION_VIEW_CARS_STOCK_SORT) {
        correctInput = TRUE;
    } else {
        printf("\n\nPlease enter correct menu option!\n\n");
    }
}

int main() {
    readDataFromFile();
    char choice;
    /* output welcome title and getting needed cars amount */
    outputWelcomeTitle();
    do {
        // menu options
        showMenuOptions();
        correctInput = FALSE;
        do {
            choice = getCharFromConsole("Please choose one: ");
            checkCorrectInput(choice);
        } while (correctInput == FALSE);


        switch (choice) {
            case MENU_OPTION_BUY_CAR:
                menuBuyCar();
                break;
                /* if user choice is b*/
            case MENU_OPTION_VIEW_SALES:
                menuViewSales();
                break;
            case MENU_OPTION_VIEW_CARS_STOCK_SORT:
                menuViewCarsStockSort();
                break;
                /* if user choice is x*/
            case MENU_OPTION_EXIT:
                menuExit();
                break;
        }

        pauseProgram(choice);

    } while (choice != MENU_OPTION_EXIT);
}