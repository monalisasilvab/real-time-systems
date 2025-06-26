#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define LED_PIN  22
#define LED_PINTWO 20
#define LED_PINTREE  19
#define LED_PINFOUR 2
#define LED_PINFIVE 3
#define LED_PINSIX 4

// Tarefa de monitoramento de heap
void heapMonitorTask(void *pvParameters) {          
    const size_t totalHeapSize = configTOTAL_HEAP_SIZE;  // Tamanho total do heap conforme definido nas configurações do FreeRTOS
    const size_t threshold = totalHeapSize / 10;  // 10% do tamanho total do heap
    size_t freeHeapSize;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (1) {
        // Obtém o tamanho livre do heap
        freeHeapSize = xPortGetFreeHeapSize();
        
        // Verifica se o tamanho livre do heap é menor que 10%
        if (freeHeapSize < threshold) {
            gpio_put(LED_PIN, 1);  
        } else {
            gpio_put(LED_PIN, 0);  
        }
        
        // Aguarda 1 segundo antes de verificar outra vez
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Função para alocar memória
void allocateMemory(const char *taskName) {
    void *allocatedMemory = NULL;
    size_t sizeToAllocate = 1024;

    // Tenta alocar memória
    allocatedMemory = pvPortMalloc(sizeToAllocate);
    if (allocatedMemory != NULL) {
        printf("[%s] Valor de memória alocada: %zu bytes\n", taskName, sizeToAllocate);
    } else {
        printf("[%s] Ocorreu falha na alocação de memória!\n", taskName);
    }

    // Exibe o tamanho do heap livre após a alocação
    size_t freeHeapSize = xPortGetFreeHeapSize();
    printf("[%s] Valor de memória livre restante: %zu bytes\n", taskName, freeHeapSize);
}

// Tarefa que controla LEDs e alocação de memória
void taskone(void *pvParameters) {
    gpio_init(LED_PINTWO);
    gpio_init(LED_PINTREE);
    gpio_set_dir(LED_PINTWO, GPIO_OUT);
    gpio_set_dir(LED_PINTREE, GPIO_OUT);

    while (1) {
        gpio_put(LED_PINTWO, 1);
        gpio_put(LED_PINTREE, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        allocateMemory("TaskOne");  // Aloca memória e mostra o heap livre

        gpio_put(LED_PINTWO, 0);
        gpio_put(LED_PINTREE, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void tasktwo(void *pvParameters) {
    gpio_init(LED_PINFOUR);
    gpio_set_dir(LED_PINFOUR, GPIO_OUT);

    while (1) {
        gpio_put(LED_PINFOUR, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        allocateMemory("TaskTwo");  

        gpio_put(LED_PINFOUR, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void tasktree(void *pvParameters) {
    gpio_init(LED_PINFIVE);
    gpio_set_dir(LED_PINFIVE, GPIO_OUT);
  
    while (1) {
        gpio_put(LED_PINFIVE, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        allocateMemory("TaskTree"); 

        gpio_put(LED_PINFIVE, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void taskfour(void *pvParameters) {
    gpio_init(LED_PINSIX);
    gpio_set_dir(LED_PINSIX, GPIO_OUT);

    while (1) {
        gpio_put(LED_PINSIX, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        allocateMemory("TaskFour"); 

        gpio_put(LED_PINSIX, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();

    // Inicializa as tarefas
    xTaskCreate(heapMonitorTask, "Heap Monitor", 256, NULL, 2, NULL);
    xTaskCreate(taskone, "TaskOne", 256, NULL, 1, NULL);
    xTaskCreate(tasktwo, "TaskTwo", 256, NULL, 1, NULL);
    xTaskCreate(tasktree, "TaskTree", 256, NULL, 1, NULL);
    xTaskCreate(taskfour, "TaskFour", 256, NULL, 1, NULL);

    // Inicia o agendador do FreeRTOS
    vTaskStartScheduler();  

    while (1) {
        // O loop principal deve estar vazio quando o agendador do FreeRTOS está em execução
    }

    return 0;
}