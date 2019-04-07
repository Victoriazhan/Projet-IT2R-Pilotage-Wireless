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
	int i,l=0;
	char tab[1],fonctionne[20],affichage[50];
	char direction,avance;
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	Init_UART();
	while (1){
		
		l++;
		sprintf(fonctionne, "fonctionne %d",l);	
		GLCD_DrawString(1,20,fonctionne);
		
		Driver_USART1.Receive(tab,2);
		while(Driver_USART1.GetRxCount()<1);
		
		switch(tab[0])
		{
			case 80:
				sprintf(affichage, "test");	
				GLCD_DrawString(100,100,affichage);
				break;
			case 2:
				sprintf(affichage, "ne rien faire");	
				GLCD_DrawString(1,100,affichage);     
				break;
		}
		switch(tab[1])
		{
			case 80:
				sprintf(affichage, "test");	
				GLCD_DrawString(100,100,affichage);
				break;
			case 2:
				sprintf(affichage, "ne rien faire");	
				GLCD_DrawString(1,100,affichage);     
				break;
		}
		for(i=0; i<16000000; i++);
		GLCD_ClearScreen();
	}
	
	return 0;
}

