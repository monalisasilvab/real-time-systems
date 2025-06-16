#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>

// LED GPIO EXTERNO
#define LED2_PIN 1
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;

void led1_task(void *pvParameters);
void led2_task(void *pvParameters);

int main() {
    
    gpio_init(LED_PIN);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(LED2_PIN, GPIO_OUT);

    // Create tasks
    xTaskCreate(
        led1_task,          
        "LED Task",        
        128,                
        NULL,               
        1,                  
        &xTask1Handle       
    );

    xTaskCreate(
        led2_task,
        "LED2 Task",
        128,
        NULL,
        1,
        &xTask2Handle
    );

    // Start scheduler
    vTaskStartScheduler();

    while(1);
    return 0;
}

// Task 1: Blink LED every 500ms
void led1_task(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    
    while(1) {
        gpio_put(LED_PIN, 1);
        vTaskDelay(xDelay);
        gpio_put(LED_PIN, 0);
        vTaskDelay(xDelay);
    }
}

// Task 2: Blink LED2 every 500ms
void led2_task(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    
    while(1) {
        gpio_put(LED2_PIN, 1);
        vTaskDelay(xDelay);
        gpio_put(LED2_PIN, 0);
        vTaskDelay(xDelay);
    }
}