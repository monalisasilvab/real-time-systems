#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Definições de pinos e do botão
#define LED_PIN 14
#define LED_PIN_TWO 13
#define BUTTON_PIN 15

SemaphoreHandle_t xCountingSemaphore;

// Função de tratamento de interrupção
void button_isr(uint gpio, uint32_t events) {
    static TickType_t xLastPressTime = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // O pino correto e borda de descida
    if (gpio == BUTTON_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        TickType_t xCurrentTime = xTaskGetTickCountFromISR();
        
        // Debounce: verifica se passaram pelo menos 200ms desde o último acionamento
        if ((xCurrentTime - xLastPressTime) >= pdMS_TO_TICKS(200)) {
            xLastPressTime = xCurrentTime;
            xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
        }
    }
    
    // Força mudança de contexto, se necessário
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void vLedTask(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(xCountingSemaphore, portMAX_DELAY) == pdTRUE) {
            gpio_put(LED_PIN, 1);
            gpio_put(LED_PIN_TWO, 1);
            vTaskDelay(pdMS_TO_TICKS(1000));  // Mantém LEDs acesos por 1s
            gpio_put(LED_PIN, 0);
            gpio_put(LED_PIN_TWO, 0);
        }
    }
}

void app_main(void) {
    stdio_init_all();

    // Configuração dos LEDs
    gpio_init(LED_PIN);
    gpio_init(LED_PIN_TWO);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(LED_PIN_TWO, GPIO_OUT);

    // Configuração do botão com interrupção
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(
        BUTTON_PIN,
        GPIO_IRQ_EDGE_FALL,
        true,
        &button_isr
    );

    xCountingSemaphore = xSemaphoreCreateCounting(10, 0);

    xTaskCreate(vLedTask, "LED Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {}
}

int main() {
    app_main();
    return 0;
}