//BOURDEAU & COLLIN
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "ipsa_sched.h"

/*-----------------------------------------------------------*/

// Task function declarations
static void vTaskPeriodic(void *pvParameters);
static void vTaskFahrenheit2degree(void *pvParameters);
static void vTaskMultiply(void *pvParameters);
static void vTaskBinarySearch(void *pvParameters);
static void vTaskAperiodic(void *pvParameters);

/*-----------------------------------------------------------*/
void ipsa_sched() {

    xTaskCreate(
        vTaskAperiodic,            // Pointer to the task function
        "AperiodicTask",           // Task name for debugging
        configMINIMAL_STACK_SIZE,  // Stack size
        NULL,                      // Task parameter
        tskIDLE_PRIORITY+4,        // Task priority
        NULL                       // Task handle
    );
    
    xTaskCreate(
        vTaskFahrenheit2degree,    // Pointer to the task function
        "Fahrenheit2degreeTask",   // Task name for debugging
        configMINIMAL_STACK_SIZE,  // Stack size
        NULL,                      // Task parameter
        tskIDLE_PRIORITY + 3,      // Task priority
        NULL                       // Task handle
    );
    
    xTaskCreate(
        vTaskBinarySearch,         // Pointer to the task function
        "BinarySearchTask",        // Task name for debugging
        configMINIMAL_STACK_SIZE,  // Stack size
        NULL,                      // Task parameter
        tskIDLE_PRIORITY + 2,      // Task priority
        NULL                       // Task handle
    );
    
    xTaskCreate(
        vTaskMultiply,             // Pointer to the task function
        "MultiplyTask",            // Task name for debugging
        configMINIMAL_STACK_SIZE,  // Stack size
        NULL,                      // Task parameter
        tskIDLE_PRIORITY + 1,      // Task priority
        NULL                       // Task handle
    );
    
    xTaskCreate(
        vTaskPeriodic,             // Pointer to the task function
        "PeriodicTask",            // Task name
        configMINIMAL_STACK_SIZE,  // Stack size
        NULL,                      // Task parameter
        tskIDLE_PRIORITY + 0,      // Task priority
        NULL                       // Task handle
    );
    
    vTaskStartScheduler();
}

// Implementation of the status display task function
static void vTaskPeriodic(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(1000); // Period of 1s
    for( ; ; ) {
        printf("Working\n");
        vTaskDelay(xDelay); // Wait for the next execution
    }
}

// Implementation of the temperature conversion task function
static void vTaskFahrenheit2degree(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(700); // Period of 0.7s
    const float fahrenheit = 95.0;
    for( ; ; ) {
        const float degree = (fahrenheit - 32) * 5/9;
        printf("%.1f °F is equal to %.1f °C\n", fahrenheit, degree);
        vTaskDelay(xDelay); // Wait for the next execution
    }
}

// Implementation of the multiplication task function
static void vTaskMultiply(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(750); // Period of 0.75s
    unsigned long int num1 = 123456789;
    unsigned long int num2 = 987654321;
    unsigned long int result;
    for( ; ; ) {
        result = num1 * num2;
        printf("Multiplication Result: %lu\n", result);
        vTaskDelay(xDelay); // Wait for the next execution
    }
}

// Binary search function
void binary_search(int target) {
    int list[50];
    
    for(int i = 0; i < 50; i++) {
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

// Implementation of the binary search task function
static void vTaskBinarySearch(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(700); // Period of 0.7 s
    int list[50]; 
    int searchElement = 25; 
    
    // Initialize the list with values (the list must be sorted)
    for(int i = 0; i < 50; i++) {
        list[i] = i + 1; 
    }

    for( ; ; ) {
        int target =25;
        binary_search(target);
        vTaskDelay(xDelay); // Wait for the next execution
    }
}

// Implementation of the aperiodic task function
static void vTaskAperiodic(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(100); // Convert 100 ms to ticks
    for(;;) {
        vTaskDelay(xDelay); // Wait for 100 ms
        printf("Aperiodic Task finished\n");
    }
}

