#include "LPC17xx.h"                    // Device header
#include "stdio.h"
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include <stdio.h>


extern GLCD_FONT GLCD_Font_6x8;


#define LIDAR_STOP              0x25        // byte d'arret du scan
#define LIDAR_RESET             0x40        // byte de reinitialisation 
#define LIDAR_SCAN              0x20        // byte de demarage du scan
#define EXPRESS_SCAN            0x82        // byte de demarage du scan express
#define GET_INFO                0x50        // byte de reception des infos 
#define GET_HEALTH              0x52        // byte de reception de l'état de santé
#define GET_SAMPLERATE          0x29        // byte de réception de la fréquence d'échantillonage 
#define START_FLAG              0xA5        // byte necessaire a l'envoi des trames


extern ARM_DRIVER_USART Driver_USART0;
extern ARM_DRIVER_USART Driver_USART1;


//initialisation
void Init_UART0(void);
void Init_PWM(void);


//fonctions d'envoi
void debutScan(void);
void stopScan(void);
void receive(void);

//tache
osThreadId ID_recup_trame;
void recup_trame (void const* argument);
osThreadDef (recup_trame,osPriorityNormal,1,0);


void Init_PWM(void);
void Init_UART0(void);


int main ()
{
	osKernelInitialize();
	Init_PWM();
	Init_UART0();
	
  ID_recup_trame = osThreadCreate(osThread(recup_trame),NULL);

	osKernelStart();
	osDelay(osWaitForever);
	return 0;
}

void recup_trame (void const* argument)
{
	char tab[100];
	//char texte[100];
	while(1)
	{
	stopScan();
	debutScan();
	Driver_USART0.Receive(tab,100);
	//sprintf(texte,"%0.2x\n\r",tab[0]);
	Driver_USART1.Send(tab,100);
	}
}	

void Init_PWM(void){
	LPC_SC->PCONP |= (1<<6);                          //Activation PWM
	LPC_PINCON->PINSEL4 |= (1<<10);                   //P2.5 en PWM6
	LPC_PWM1->CTCR = 0;                               //Mode timer
	LPC_PWM1->PR = 0;                                 //Pas de prédivision
	LPC_PWM1->MR0 = 999;                              //Valeur de N
	LPC_PWM1->MR6 = 409;
	LPC_PWM1->MCR |= (1<<1);                          //RAZ du compteur si = MR0
	LPC_PWM1->PCR |= (1<<14);                       	//Active PWM6
	LPC_PWM1->TCR = 1;
}

void Init_UART0(void){
	Driver_USART0.Initialize(NULL);
	Driver_USART0.PowerControl(ARM_POWER_FULL);
	Driver_USART0.Control( ARM_USART_MODE_ASYNCHRONOUS |
	ARM_USART_DATA_BITS_8 |
	ARM_USART_STOP_BITS_1 |
	ARM_USART_PARITY_NONE |
	ARM_USART_FLOW_CONTROL_NONE,
	115200);
	Driver_USART0.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART0.Control(ARM_USART_CONTROL_RX,1);

	Driver_USART1.Initialize(NULL);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control( ARM_USART_MODE_ASYNCHRONOUS |
	ARM_USART_DATA_BITS_8 |
	ARM_USART_STOP_BITS_1 |
	ARM_USART_PARITY_NONE |
	ARM_USART_FLOW_CONTROL_NONE,
	115200);
	Driver_USART1.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX,1);
}

void debutScan(void){	
	char tab[2];
	tab[0] = START_FLAG;
	tab[1] = LIDAR_SCAN;

	while (Driver_USART0.GetStatus().tx_busy==1);
	Driver_USART0.Send(tab,2);//envoi du flag et start scan
}

void stopScan(void){	
	char tab[2];
	tab[0] = START_FLAG;
	tab[1] = LIDAR_STOP;
	
	while (Driver_USART0.GetStatus().tx_busy==1);
	Driver_USART0.Send(tab,2);//envoi du flag et stop scan
}

void receive(void){
	char tab[100];
	Driver_USART0.Receive(tab,100);
	while(Driver_USART0.GetRxCount()<1);
}

