#include "pico/stdlib.h"

int main() {
    // Inicializa o LED no GPIO 25
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(300);
        gpio_put(LED_PIN, 0);  
        sleep_ms(300);
    }
}