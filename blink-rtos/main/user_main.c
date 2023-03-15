/* gpio example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "main";

/**
 * Brief:
 * Blink led connect to GPIO2
 */

#define GPIO_OUTPUT_IO_0    2 
#define GPIO_OUTPUT_IO_1    16
#define GPIO_OUTPUT_PIN_SEL 1ULL<<GPIO_OUTPUT_IO_0

void app_main(void)
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO15/16
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    
	gpio_set_level(GPIO_OUTPUT_IO_0, 1);

    while (1) {
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_LOGI(TAG, "Turn LED on.");
        gpio_set_level(GPIO_OUTPUT_IO_0, 0);
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_LOGI(TAG, "Turn LED off.");
        gpio_set_level(GPIO_OUTPUT_IO_0, 1);
    }
}


