#include "LPC17xx.h"                    // Device header
#include "stdio.h"
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "libraries.h"
#include <stdio.h>

//extern ARM_DRIVER_CAN Driver_CAN1;
extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_16x24;
extern GLCD_FONT GLCD_Font_6x8;

//Thread
osThreadId ID_recept_BT;
void recept_BT (void const* argument);
osThreadDef (recept_BT,osPriorityNormal,1,0);

void moteur_puissance (void const *argument);                             // thread function
osThreadId ID_moteur_puissance;                                          // thread id
osThreadDef (moteur_puissance, osPriorityNormal, 1, 0);                   // thread object

//Fonctionsc
void avance_roue_selon_rapport(double rapport_cyclique);
void recule_roue_selon_rapport(double rapport_cyclique);

//Mutex
osMutexId ID_mut_lcd;
osMutexDef(mut_lcd);
osMutexId ID_mut_var;
osMutexDef(mut_var);

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
	ID_moteur_puissance = osThreadCreate (osThread(moteur_puissance), NULL);
	
	ID_mut_lcd=osMutexCreate(osMutex(mut_lcd));
	ID_mut_var=osMutexCreate(osMutex(mut_var));
	
	osKernelStart();
	osDelay(osWaitForever);
	return 0;
}

void recept_BT (void const* argument)
{
	int number_msg=0;
	char tab[3],fonctionne[20],affichage[50],affichage1[20],affichage2[20],data=0;
	
	while(1)
	{
	//numeros de msg
//	number_msg++;
//	sprintf(fonctionne, "fonctionne %d",number_msg);	
//	osMutexWait(ID_mut_lcd,osWaitForever);
//	GLCD_DrawString(1,1,fonctionne);
//	osMutexRelease(ID_mut_lcd);
	
	//reception BT
	Driver_USART1.Receive(tab,1);
	while(Driver_USART1.GetRxCount()<1);
	
	//affichage de la valeur recu
	sprintf(affichage,"data = %c",tab[0]);
	osMutexWait(ID_mut_lcd,osWaitForever);
	GLCD_DrawString(1,25,affichage);
	osMutexRelease(ID_mut_lcd);
			
	data=tab[0];
	osMutexWait(ID_mut_var,osWaitForever);
	sens = data & 0x0F;
	direction = data>>4;
	osMutexRelease(ID_mut_var);
	
//	sprintf(affichage1,"sens = %x",sens);
//	GLCD_DrawString(1,60,affichage1);
//	sprintf(affichage2,"direction = %x",direction);
//	GLCD_DrawString(1,100,affichage2);
	}
}

void moteur_puissance (void const *argument)
{
	char affichage_direction[10],affichage_sens[10];
//	etat_nunchuck_recu=(etat_nunchuck_recu&0x0F);
	
	while(1)
	{
//			osMutexWait(ID_mut_var,osWaitForever);
//			sprintf(affichage_sens,"sens = %x",sens);
//			sprintf(affichage_direction,"dir = %x",direction);
//			osMutexRelease(ID_mut_var);
//			osMutexWait(ID_mut_lcd,osWaitForever);
//			GLCD_DrawString(1,90,affichage_sens);	
//			GLCD_DrawString(1,60,affichage_direction);		
//			osMutexRelease(ID_mut_lcd);
		switch(direction)
		{
			case 1 : 
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_direction,"dir = %x",direction);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,60,affichage_direction);
				osMutexRelease(ID_mut_lcd);
				break;
			case 2 :
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_direction,"dir = %x",direction);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,60,affichage_direction);
				osMutexRelease(ID_mut_lcd);
				break;
			case 3 :
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_direction,"dir = %x",direction);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,60,affichage_direction);
				osMutexRelease(ID_mut_lcd);
				break;
			case 4 :
	  		osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_direction,"dir = %x",direction);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,60,affichage_direction);
				osMutexRelease(ID_mut_lcd);
				break;
			case 5 :
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_direction,"dir = %x",direction);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,60,affichage_direction);
				osMutexRelease(ID_mut_lcd);
				break;
		}
				switch(sens)
		{
			case 1 : 
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_sens,"sens = %x",sens);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,90,affichage_sens);
				osMutexRelease(ID_mut_lcd);
				break;
			case 2 :
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_sens,"sens = %x",sens);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,90,affichage_sens);
				osMutexRelease(ID_mut_lcd);

				break;
			case 3 :
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_sens,"sens = %x",sens);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,90,affichage_sens);
				osMutexRelease(ID_mut_lcd);
				break;
			case 4 :
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_sens,"sens = %x",sens);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,90,affichage_sens);
				osMutexRelease(ID_mut_lcd);
				break;
			case 5 :
				osMutexWait(ID_mut_var,osWaitForever);
				sprintf(affichage_sens,"sens = %x",sens);
				osMutexRelease(ID_mut_var);
				osMutexWait(ID_mut_lcd,osWaitForever);
				GLCD_DrawString(1,90,affichage_sens);
				osMutexRelease(ID_mut_lcd);
				break;
		}
		switch(direction)
		{
			case 1:
				recule_roue_selon_rapport(0.6);
				break;
			case 2:
				recule_roue_selon_rapport(0.4);
				break;
			case 3:
				avance_roue_selon_rapport(0.1);
				break;
			case 4:
				avance_roue_selon_rapport(0.4);
				break;
			case 5:
				avance_roue_selon_rapport(0.6);
				break;
		}
//		switch(sens)
//		{
//			case 1:
//				recule_roue_selon_rapport(0.6);
//				break;
//			case 2:
//				recule_roue_selon_rapport(0.4);
//				break;
//			case 3:
//				avance_roue_selon_rapport(0.1);
//				break;
//			case 4:
//				avance_roue_selon_rapport(0.4);
//				break;
//			case 5:
//				avance_roue_selon_rapport(0.6);
//				break;
//		}
	}
}

void avance_roue_selon_rapport(double rapport_cyclique)
{
	LPC_GPIO0->FIODIR=((1<<16)|(1<<17));//P0.16 ,P0.17 mise en sortie
	
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
  // valeur de 20KHz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PWM1->MR2 = rapport_cyclique*(1249+1)-1;    // la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
	
	LPC_GPIO0->FIOPIN|=(1<<17); //INB=1, P0.17 avance, rouge
}
void recule_roue_selon_rapport(double rapport_cyclique)
{
	LPC_GPIO0->FIODIR=((1<<16)|(1<<17));//P0.16 ,P0.17 mise en sortie
	
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
  // valeur de 20KHz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PWM1->MR2 = rapport_cyclique*(1249+1)-1;    // la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
	
	LPC_GPIO0->FIOPIN|=(1<<16); //INA=1, P0.16 recule, vert
}



