#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "stdio.h"
#include "Driver_USART.h"               // ::CMSIS Driver:USART

extern ARM_DRIVER_USART Driver_USART1;

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;
extern ARM_DRIVER_USART Driver_USART1;

void Init_UART(void){
	Driver_USART1.Initialize(NULL);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control(	ARM_USART_MODE_ASYNCHRONOUS |
							ARM_USART_DATA_BITS_8		|
							ARM_USART_STOP_BITS_1		|
							ARM_USART_PARITY_NONE		|
							ARM_USART_FLOW_CONTROL_NONE,
							115200);
	Driver_USART1.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX,1);
}
int main (void){
	int i;
	char tab[1];
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	Init_UART();
	while (1){
		
		
		Driver_USART1.Receive(tab,1);
		while(Driver_USART1.GetRxCount()<1);
		
		
		if(tab[0]=='1')
		{
			sprintf(tab, "Commande OFF");	
			GLCD_DrawString(0,100,tab);        
		}
		else if(tab[0]=='0')
		{
			sprintf(tab, "Commande OFF");	
			GLCD_DrawString(100,100,tab);
		}
		for(i=0; i<160000000; i++);
		GLCD_ClearScreen();
	}
	
	return 0;
}

