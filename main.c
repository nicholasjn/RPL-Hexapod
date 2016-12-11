#include "main.h"

GPIO_InitTypeDef gpio_init;

int main(void)
{
	uint8_t time = 100;
	char *getByte;
	char a = {"0"};
	uint8_t value;

	SystemInit();
	init();
    while(1)
    {
    	TM_USART_ClearBuffer(USART1);
//    	while (TM_USART_BufferEmpty(USART1));
//    	while (TM_USART_Getc(USART1) != '<');
    	while (TM_USART_BufferEmpty(USART1));
    	value = TM_USART_Getc(USART1);
    	if(value > 1){
    		GPIO_SetBits(GPIOD, GPIO_Pin_12);
    	}
    }
}

void init(){
	delayInit();
//	servoInit();
//	normalInit(time);
//	normal(time);
	i2c2Init();
	uartInit();
	ledInit();
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 9600);
}

void ledInit(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //Mengaktifkan reset dan clock controller untuk port A
	gpio_init.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //Hanya mengkonfigurasikan PIN0, bisa diganti dengan gpio_init.GPIO_Pin = 0x0001;
	gpio_init.GPIO_Mode = GPIO_Mode_OUT; //Mode input
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz; //clock 100MHz
	gpio_init.GPIO_OType = GPIO_OType_PP; //Type PUSH PULL
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN; //Memakai pull down resistor
	GPIO_Init(GPIOD, &gpio_init); //pass semua parameter setting untuk GPIOA
}
