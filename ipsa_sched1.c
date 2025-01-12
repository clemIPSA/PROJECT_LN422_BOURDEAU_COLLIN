// BOURDEAU Clément
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ipsa_sched.h"


// Déclarations des fonctions de tâches
static void vTaskPeriodic(void *pvParameters);
static void vTaskFahrenheit2degree(void *pvParameters);
static void vTaskMultiply(void *pvParameters);
static void vTaskBinarySearch(void *pvParameters);
static void vTaskAperiodic(void *pvParameters);
static void vTaskKeyboardMonitor(void *pvParameters);

// Fonction pour lire un caractère sans appuyer sur Entrée (linux)
char getChar() {
    struct termios oldt, newt; //on cree des structures pour stocker les parametres du terminal afin de les restaurer apres
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // Sauvegarde des paramètres du terminal 
    newt = oldt; //on copie les parametres du terminal pour pouvoir travailler dessus sans modifier ceux d'origine
    newt.c_lflag &= ~(ICANON | ECHO); // Désactive le mode canonique et l'écho : ca permet de ne pas attendre un appui sur Entree apres un appui sur le clavier
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Applique les nouveaux paramètres : on les active immediatement

    ch = getchar(); // Lit un caractère

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // On restaure les anciens parametres
    return ch; // on renvoie la chaine de caractere saisie par l'utilisateur
}


// Tâche de surveillance clavier avec gestion RESET
static void vTaskKeyboardMonitor(void *pvParameters) {
    char key;
    int resetState = 0; // Variable pour suivre l'état de RESET

    for (;;) {
        key = getChar(); // Attend une entrée clavier via la fonction précédente

        if (key == '1') { // Touche RESET détectée
            resetState = 1;
            printf("RESET Detected: %d\n", resetState);
        } else {
            resetState = 0; // Réinitialisation du paramètre
            printf("RESET State: %d\n", resetState);
        }

        vTaskDelay(pdMS_TO_TICKS(200)); // Pause de 200ms
    }
}


// Tâche périodique
static void vTaskPeriodic(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(1000); // Période de 1 seconde
    for (;;) {
        printf("Working\n");
        vTaskDelay(xDelay);
    }
}

// Tâche de conversion Fahrenheit -> Celsius
static void vTaskFahrenheit2degree(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(700); // Période de 0.7s
    const float fahrenheit = 95.0;
    for (;;) {
        const float degree = (fahrenheit - 32) * 5 / 9;
        printf("%.1f °F is equal to %.1f °C\n", fahrenheit, degree);
        vTaskDelay(xDelay);
    }
}

// Tâche de multiplication
static void vTaskMultiply(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(750); // Période de 0.75s
    unsigned long int num1 = 123456789;
    unsigned long int num2 = 987654321;
    for (;;) {
        unsigned long int result = num1 * num2;
        printf("Multiplication Result: %lu\n", result);
        vTaskDelay(xDelay);
    }
}

// Fonction de recherche binaire
void binary_search(int target) {
    int list[50];
    for (int i = 0; i < 50; i++) {
        list[i] = i + 1;
    }
    int left = 0;
    int right = 49;
    while (left <= right) {
        int middle = (left + right) / 2;
        if (list[middle] == target) {
            printf("The value %d is in the array at position %d\n", target, middle);
            break;
        }
        if (list[middle] < target) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }
}

// Tâche de recherche binaire
static void vTaskBinarySearch(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(700); // Période de 0.7s
    int target = 25;
    for (;;) {
        binary_search(target);
        vTaskDelay(xDelay);
    }
}

/*-----------------------------------------------------------*/

// Fonction principale
void ipsa_sched() {

    // Tâche de surveillance clavier
    xTaskCreate(
        vTaskKeyboardMonitor,
        "KeyboardMonitorTask",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 5,
        NULL
    );


    // Tâche Fahrenheit -> Celsius
    xTaskCreate(
        vTaskFahrenheit2degree,
        "Fahrenheit2degreeTask",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 3,
        NULL
    );

    // Tâche de recherche binaire
    xTaskCreate(
        vTaskBinarySearch,
        "BinarySearchTask",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL
    );

    // Tâche de multiplication
    xTaskCreate(
        vTaskMultiply,
        "MultiplyTask",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL
    );

    // Tâche périodique
    xTaskCreate(
        vTaskPeriodic,
        "PeriodicTask",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL
    );

    vTaskStartScheduler(); // Démarre l'ordonnanceur
}
