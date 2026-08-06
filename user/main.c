#include "stm32f10x.h"

#define LED_GPIO        GPIOA
#define LED_PIN         GPIO_Pin_0
#define KEY_GPIO        GPIOA
#define KEY_PIN         GPIO_Pin_1

int main(void)
{
	GPIO_InitTypeDef gpioInit = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* PA0 drives the LED: PA0 -> resistor -> LED anode, LED cathode -> GND. */
	gpioInit.GPIO_Pin = LED_PIN;
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_GPIO, &gpioInit);
	GPIO_ResetBits(LED_GPIO, LED_PIN);

	/* PA1 is pulled high internally and becomes low when the button is pressed. */
	gpioInit.GPIO_Pin = KEY_PIN;
	gpioInit.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY_GPIO, &gpioInit);

	while (1)
	{
		if (GPIO_ReadInputDataBit(KEY_GPIO, KEY_PIN) == Bit_RESET)
		{
			GPIO_SetBits(LED_GPIO, LED_PIN);
		}
		else
		{
			GPIO_ResetBits(LED_GPIO, LED_PIN);
		}
	}
}
