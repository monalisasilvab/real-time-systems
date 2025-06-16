#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "stdio.h"

SemaphoreHandle_t mutex;
int16_t elementax, elementay, elementaz;

// Função de leitura do ADC
void read_adc_values(int16_t *ax, int16_t *ay, int16_t *az) {
    uint16_t raw_value = adc_read(); // Lê o valor bruto do ADC (0-4095)
    *ax = (int16_t)(raw_value - 2048); // Centraliza o valor em 0
    *ay = *ax; // Exemplo: Y e Z copiam X
    *az = *ax;
}

// Tarefa de leitura do potenciômetro (50ms)
void vTaskSimulateAccelerometer(void *pvParameters) {
    while (1) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        read_adc_values(&elementax, &elementay, &elementaz);
        xSemaphoreGive(mutex);  
        vTaskDelay(pdMS_TO_TICKS(50)); // 50ms
    }
}

// Tarefa de envio serial (1 segundo)
void vTaskProcessData(void *pvParameters) {
    while (1) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        printf("Eixo X: %d, Eixo Y: %d, Eixo Z: %d\n", elementax, elementay, elementaz);
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1s
    }
}

int main() {
    stdio_init_all();
    adc_init(); // Inicializa o ADC
    adc_gpio_init(26); // Configura GPIO26 como entrada analógica
    adc_select_input(0); // Seleciona o canal 0

    mutex = xSemaphoreCreateMutex();

    // Cria tarefas     
    xTaskCreate(vTaskSimulateAccelerometer, "SimulateAccelerometer", 256, NULL, 2, NULL);
    xTaskCreate(vTaskProcessData, "ProcessData", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {}
}