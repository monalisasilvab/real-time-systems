#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "stdio.h"

// Variável global para armazenar os ticks da IDLE Task
static unsigned long int tick_counter = 0;

// Protótipos
void taskCPUusage(void *pvParameters);
void taskUseCPU(void *pvParameters);
void taskUseCPU2(void *pvParameters);
void taskUseCPU3(void *pvParameters);
void taskUseCPU4(void *pvParameters);
void taskLed(void *pvParameters);
void taskLed2(void *pvParameters);

// Hook de Idle: mede o uso da CPU
void vApplicationIdleHook(void) {
    unsigned long int tick = xTaskGetTickCount();
    while (xTaskGetTickCount() == tick);
    tick_counter++;
}

// Task que calcula o uso da CPU
void taskCPUusage(void *pvParameters) {
    unsigned long int idle_tick_last, ticks;
    idle_tick_last = tick_counter = 0;
    for (;;) {
        vTaskDelay(3000 / portTICK_PERIOD_MS);  // Espera 3 segundos

        // Calcula os ticks ociosos desde a última medição
        if (tick_counter > idle_tick_last)
            ticks = tick_counter - idle_tick_last;
        else
            ticks = 0xFFFFFFFF - idle_tick_last + tick_counter; // Overflow

        ticks /= 3;

        printf("%lu idle ticks p/s (de %lu)\n", ticks, configTICK_RATE_HZ);

        unsigned long int cpu_usage = ((configTICK_RATE_HZ - ticks) * 100) / configTICK_RATE_HZ;
        printf("Usage CPU: %lu%%\n", cpu_usage);

        idle_tick_last = tick_counter;
    }
}

// Tasks que consomem CPU
void taskUseCPU(void *pvParameters) {
    unsigned int i, j;
    for (;;) {
        for (i = 0, j = 1; i < 100000; i++) {
            j *= i + 1;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


// Task que pisca um LED
void taskLed(void *pvParameters) {
    const uint led_pin = (uint)pvParameters;
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    for (;;) {
        gpio_xor_mask(1 << led_pin);  // Alterna estado do LED
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// Task para piscar outro LED com frequência diferente
void taskLed2(void *pvParameters) {
    const uint led_pin = (uint)pvParameters;
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    for (;;) {
        gpio_xor_mask(1 << led_pin);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

int main(void) {
    stdio_init_all();

    // LED interno da Raspberry Pi Pico geralmente é o GPIO 25
    const uint LED1 = 10;
    const uint LED2 = 15;  // Outro pino GPIO qualquer

    // Criação das tasks com diferentes prioridades
    xTaskCreate(taskCPUusage, "Task CPU Usage", configMINIMAL_STACK_SIZE*4, NULL, 4, NULL);
    xTaskCreate(taskUseCPU, "TaskUseCPU", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(taskLed, "TaskLed1", configMINIMAL_STACK_SIZE, (void *)LED1, 1, NULL);
    xTaskCreate(taskLed2, "TaskLed2", configMINIMAL_STACK_SIZE, (void *)LED2, 3, NULL);

    vTaskStartScheduler();

    while (true) {}
}
