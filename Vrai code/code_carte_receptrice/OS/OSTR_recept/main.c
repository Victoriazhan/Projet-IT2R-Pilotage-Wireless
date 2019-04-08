#include "LPC17xx.h"                    // Device header
#include "stdio.h"
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include <stdio.h>

extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_16x24;
extern GLCD_FONT GLCD_Font_6x8;

//prototype
void Init_UART(void);

//Thread
osThreadId ID_recept_BT;
void recept_BT (void const* argument);
osThreadDef (recept_BT,osPriorityNormal,1,0);

//direction
char direction;
//sens
char sens;

int main (void)
{
	osKernelInitialize();
	Init_UART();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	ID_recept_BT=osThreadCreate(osThread(recept_BT),NULL);
	
	osKernelStart();
	osDelay(osWaitForever);
	return 0;
}

void recept_BT (void const* argument)
{
	int number_msg=0;
	char tab[3],fonctionne[20],affichage[50],affichage1[50],affichage2[50],data;
	
	while(1)
	{
	//numeros de msg
	number_msg++;
//	sprintf(fonctionne, "fonctionne %d",number_msg);	
//	GLCD_DrawString(1,1,fonctionne);
		
	//reception BT
	Driver_USART1.Receive(tab,1);
	while(Driver_USART1.GetRxCount()<1);
	
	//affichage de la valeur recu
//	sprintf(affichage,"data = %c",tab[0]);
//	GLCD_DrawString(1,25,affichage);
	data=tab[0];
	sens = data & 0x0F;
	direction = data>>4;
//	sprintf(affichage1,"sens = %x",sens);
//	GLCD_DrawString(1,60,affichage1);
//	sprintf(affichage2,"direction = %x",direction);
//	GLCD_DrawString(1,100,affichage2);
	}
}

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