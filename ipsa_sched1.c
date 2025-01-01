//BOURDEAU & COLLIN
#include <stdio.h>
#include <conio.h>  // Pour getch() pour la détection clavier

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ipsa_sched.h"

/*-----------------------------------------------------------*/

// Déclarations des fonctions de tâches
static void vTaskPeriodic(void *pvParameters);
static void vTaskFahrenheit2degree(void *pvParameters);
static void vTaskMultiply(void *pvParameters);
static void vTaskBinarySearch(void *pvParameters);
static void vTaskAperiodic(void *pvParameters);
static void vTaskKeyboardMonitor(void *pvParameters);

/*-----------------------------------------------------------*/

// Déclaration de la file d'attente pour les événements clavier
static QueueHandle_t xKeyEventQueue = NULL;

/*-----------------------------------------------------------*/

// Fonction principale
void ipsa_sched() {
    // Création de la file d'attente
    xKeyEventQueue = xQueueCreate(5, sizeof(char));
    if (xKeyEventQueue == NULL) {
        printf("Erreur : Impossible de créer la file d'attente\n");
        return;
    }

    // Tâche de surveillance clavier
    xTaskCreate(
        vTaskKeyboardMonitor,     
        "KeyboardMonitorTask",    
        configMINIMAL_STACK_SIZE, 
        NULL,                     
        tskIDLE_PRIORITY + 5,     
        NULL                      
    );

    // Tâche apériodique déclenchée par l'appui sur Espace
    xTaskCreate(
        vTaskAperiodic,           
        "AperiodicTask",          
        configMINIMAL_STACK_SIZE, 
        NULL,                     
        tskIDLE_PRIORITY + 4,     
        NULL                      
    );

    // Tâche de conversion Fahrenheit -> Celsius
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

/*-----------------------------------------------------------*/

// Tâche Périodique
static void vTaskPeriodic(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(1000); // Période de 1 seconde
    for (;;) {
        printf("Working\n");
        vTaskDelay(xDelay);
    }
}

// Tâche de Conversion Fahrenheit -> Celsius
static void vTaskFahrenheit2degree(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(700); // Période de 0.7s
    const float fahrenheit = 95.0;
    for (;;) {
        const float degree = (fahrenheit - 32) * 5 / 9;
        printf("%.1f °F is equal to %.1f °C\n", fahrenheit, degree);
        vTaskDelay(xDelay);
    }
}

// Tâche de Multiplication
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

// Fonction de Recherche Binaire
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

// Tâche de Recherche Binaire
static void vTaskBinarySearch(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(700); // Période de 0.7s
    int target = 25;
    for (;;) {
        binary_search(target);
        vTaskDelay(xDelay);
    }
}

// Tâche Apériodique (déclenchée par la touche Espace)
static void vTaskAperiodic(void *pvParameters) {
    char receivedKey;
    for (;;) {
        if (xQueueReceive(xKeyEventQueue, &receivedKey, portMAX_DELAY) == pdTRUE) {
            printf("Aperiodic Task Triggered by Space Key\n");
        }
    }
}

// Tâche de Surveillance Clavier
static void vTaskKeyboardMonitor(void *pvParameters) {
    char key;
    for (;;) {
        key = getch(); // Attend une entrée clavier
        if (key == ' ') { // Touche Espace détectée
            xQueueSend(xKeyEventQueue, &key, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // Pause pour réduire la charge CPU
    }
}