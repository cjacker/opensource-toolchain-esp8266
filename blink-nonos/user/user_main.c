#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"
#include "user_config.h"

#define LED_PIN 2 

static os_timer_t blinkTimer;
static int blinkState;

LOCAL void ICACHE_FLASH_ATTR blinkTimerCallback(void *arg) {
	blinkState = (blinkState == 0) ? 1 : 0;

	/* Set GPIO */
	gpio_output_set(
		(blinkState == 0) ? 0 : (1 << LED_PIN),
		(blinkState == 0) ? (1 << LED_PIN) : 0,
		(1 << LED_PIN),
		0
	);
}

void ICACHE_FLASH_ATTR
init_done(void)
{
	os_timer_disarm(&blinkTimer);
	os_timer_setfn(&blinkTimer, &blinkTimerCallback, (void*)0);
	os_timer_arm(&blinkTimer, 1000, 1);
}

void ICACHE_FLASH_ATTR
user_init(void)
{
	blinkState = 0;

	gpio_init();
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2); // Use GPIO4 as GPIO4.
	gpio_output_set((1 << LED_PIN), 0, (1 << LED_PIN), 0);

	system_init_done_cb(init_done);
}
