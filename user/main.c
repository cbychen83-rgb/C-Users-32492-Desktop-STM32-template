#include "stm32f10x.h"
#include "usart.h"

void App_ADC1_Init(void);
void App_OnBoardLED_Init(void);
void App_USART1_Init(void);
void App_TIM1_Init(void);


int main(void)
{
	App_ADC1_Init();
	App_OnBoardLED_Init();
	App_USART1_Init();
	App_TIM1_Init();
	while(1)
	{
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC) == RESET);

		uint16_t adc_raw = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);

		ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);

		/*
		 * VOFA+ was receiving raw ADC counts and millivolts (for example
		 * 1097,884), while its visible Y range was only about +/-5.  Send
		 * decimal values in a display-friendly range instead: normalized ADC
		 * value (0..1) and measured voltage (0..3.3 V).  Fixed-point output
		 * avoids depending on the C library's floating-point printf support.
		 */
		uint32_t normalized_milli = ((uint32_t)adc_raw * 1000U + 2047U) / 4095U;
		uint32_t voltage_milli = ((uint32_t)adc_raw * 3300U + 2047U) / 4095U;

		My_USART_Printf(USART1, "%lu.%03lu,%lu.%03lu\n",
			(unsigned long)(normalized_milli / 1000U),
			(unsigned long)(normalized_milli % 1000U),
			(unsigned long)(voltage_milli / 1000U),
			(unsigned long)(voltage_milli % 1000U));
	}
}
void App_ADC1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitTypeDef ADC_initStruct = {0};

	ADC_initStruct.ADC_ContinuousConvMode = DISABLE;

	ADC_initStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_initStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_initStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_initStruct.ADC_NbrOfChannel = 1;
	ADC_initStruct.ADC_ScanConvMode = DISABLE;

	ADC_Init(ADC1, &ADC_initStruct);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);

	ADC_ExternalTrigConvCmd(ADC1, DISABLE);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) != RESET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) != RESET);

	ADC_InjectedSequencerLengthConfig(ADC1, 1);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_TRGO);
	ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);





}
void App_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void App_USART1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);
}
void App_TIM1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	/* 100 Hz trigger rate keeps the blocking UART transmitter loss-free. */
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

	TIM_Cmd(TIM1, ENABLE);
}
