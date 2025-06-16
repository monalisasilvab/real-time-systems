#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Definições de pinos
#define ADC_PIN 26              // Pino do ADC conectado ao potenciômetro
#define BLUE_LED_PIN 16         // LED AZUL (controle via ADC)
#define GREEN_LED_PIN 17       // LED VERDE (controle via ADC)
#define RED_LED_PIN 18          // LED VERMELHO (controle via ADC)
#define BUTTON_PIN 15           // Pino do botão (interrupção)
#define BUTTON_LED_PIN 14       // LED acionado pelo botão (verde)

// Definições de limiares para o ADC
#define X1 3000                 // Limiar superior para LED AZUL
#define X2 1000                 // Limiar inferior para LED VERMELHO
#define ADC_READ_INTERVAL pdMS_TO_TICKS(300)  // Intervalo de leitura do ADC

// Handles para as filas
QueueHandle_t adcQueue;
QueueHandle_t buttonQueue;

// Função de tratamento da interrupção do botão
void gpio_isr_handler(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN) {
        uint8_t buttonPressed = 1;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
        // Envia sinal para a fila do botão
        xQueueSend(buttonQueue, &buttonPressed, portMAX_DELAY);
        
        // Solicita troca de contexto se necessário
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// Tarefa para leitura do ADC
void adcTask(void *pvParameters) {
    uint16_t adcValue;

    while (1) {
        // Lê o valor do ADC
        adcValue = adc_read();
        printf("Valor ADC: %d\n", adcValue);

        // Envia o mesmo valor para a fila três vezes (uma para cada LED)
        for (int i = 0; i < 3; i++) {
            xQueueSend(adcQueue, &adcValue, portMAX_DELAY);
        }

        // Aguarda até o próximo ciclo
        vTaskDelay(ADC_READ_INTERVAL);
    }
}

// Tarefa para controlar o LED AZUL com base no ADC
void blueLedTask(void *pvParameters) {
    uint16_t adcValue;

    while (1) {
        if (xQueueReceive(adcQueue, &adcValue, portMAX_DELAY) == pdTRUE) {
            if (adcValue > X1) {
                gpio_put(BLUE_LED_PIN, 1);
                gpio_put(GREEN_LED_PIN, 0);
                gpio_put(RED_LED_PIN, 0);
            }
        }
    }
}

// Tarefa para controlar o LED AMARELO com base no ADC
void greenLedTask(void *pvParameters) {
    uint16_t adcValue;

    while (1) {
        if (xQueueReceive(adcQueue, &adcValue, portMAX_DELAY) == pdTRUE) {
            if (adcValue >= X2 && adcValue <= X1) {
                gpio_put(GREEN_LED_PIN, 1);
                gpio_put(BLUE_LED_PIN, 0);
                gpio_put(RED_LED_PIN, 0);
            }
        }
    }
}

// Tarefa para controlar o LED VERMELHO com base no ADC
void redLedTask(void *pvParameters) {
    uint16_t adcValue;

    while (1) {
        if (xQueueReceive(adcQueue, &adcValue, portMAX_DELAY) == pdTRUE) {
            if (adcValue < X2) {
                gpio_put(RED_LED_PIN, 1);
                gpio_put(BLUE_LED_PIN, 0);
                gpio_put(GREEN_LED_PIN, 0);
            }
        }
    }
}

// Tarefa para controlar o LED do botão (verde)
void buttonLedTask(void *pvParameters) {
    uint8_t receivedSignal;

    while (1) {
        if (xQueueReceive(buttonQueue, &receivedSignal, portMAX_DELAY) == pdTRUE) {
            // Inverte o estado do LED do botão
            gpio_put(BUTTON_LED_PIN, !gpio_get(BUTTON_LED_PIN));
            printf("LED do botão %s\n", gpio_get(BUTTON_LED_PIN) ? "APAGADO" : "LIGADO");
        }
    }
}

int main() {
    // Inicializa a stdio
    stdio_init_all();

    // Inicializa o ADC
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    // Inicializa os LEDs do ADC
    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);

    // Inicializa o LED do botão
    gpio_init(BUTTON_LED_PIN);
    gpio_set_dir(BUTTON_LED_PIN, GPIO_OUT);
    gpio_put(BUTTON_LED_PIN, 0);  // Garante que o LED esteja apagado inicialmente

    // Configura o botão com pull-up
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  // Botão ativo em nível baixo

    // Cria as filas para comunicação
    adcQueue = xQueueCreate(10, sizeof(uint16_t));
    buttonQueue = xQueueCreate(10, sizeof(uint8_t));

    if (adcQueue == NULL || buttonQueue == NULL) {
        printf("Erro ao criar as filas.\n");
        return 1;
    }

    // usando a função com callback incorporado:
    gpio_set_irq_enabled_with_callback(
    BUTTON_PIN,
    GPIO_IRQ_EDGE_FALL,
    true,                     // habilita
    gpio_isr_handler         // o seu handler (uint gpio, uint32_t events)
    );

    // Cria as tarefas
    xTaskCreate(adcTask, "ADC Task", 256, NULL, 1, NULL);
    xTaskCreate(blueLedTask, "Blue LED Task", 256, NULL, 1, NULL);
    xTaskCreate(greenLedTask, "GREEN LED Task", 256, NULL, 1, NULL);
    xTaskCreate(redLedTask, "Red LED Task", 256, NULL, 1, NULL);
    xTaskCreate(buttonLedTask, "Button LED Task", 256, NULL, 1, NULL);

    // Inicia o scheduler do FreeRTOS
    vTaskStartScheduler();

    // O código não deve chegar aqui
    while (1) {
        // Loop infinito caso o scheduler falhe
    }

    return 0;
}