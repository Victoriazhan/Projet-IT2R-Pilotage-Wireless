#include "LPC17xx.h"                    // Device header
#include "stdio.h"
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "libraries.h"
#include <stdio.h>
#include "GPIO.h"

//extern ARM_DRIVER_CAN Driver_CAN1;
extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_16x24;
extern GLCD_FONT GLCD_Font_6x8;

//Thread
osThreadId ID_recept_BT;
void recept_BT (void const* argument);
osThreadDef (recept_BT,osPriorityNormal,1,0);

void moteur_puissance (void const *argument);                            // thread function
osThreadId ID_moteur_puissance;                                          // thread id
osThreadDef (moteur_puissance, osPriorityNormal, 1, 0);                  // thread object

//Fonctionsc
void avance_roue_selon_rapport(double rapport_cyclique);
void recule_roue_selon_rapport(double rapport_cyclique);
void fonction_PWM_sens(void);
void fonction_PWM_direction(double rapport_cyclique);
void fonction_recule(void);
void fonction_avance(void);

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
	//Initialise_GPIO ();
	
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
//		switch(direction)
//		{
//			case 1 : 
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_direction,"dir = %x",direction);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,60,affichage_direction);
//				osMutexRelease(ID_mut_lcd);
//				break;
//			case 2 :
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_direction,"dir = %x",direction);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,60,affichage_direction);
//				osMutexRelease(ID_mut_lcd);
//				break;
//			case 3 :
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_direction,"dir = %x",direction);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,60,affichage_direction);
//				osMutexRelease(ID_mut_lcd);
//				break;
//			case 4 :
//	  		osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_direction,"dir = %x",direction);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,60,affichage_direction);
//				osMutexRelease(ID_mut_lcd);
//				break;
//			case 5 :
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_direction,"dir = %x",direction);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,60,affichage_direction);
//				osMutexRelease(ID_mut_lcd);
//				break;
//		}
//		
//		switch(sens)
//		{
//			case 1 : 
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_sens,"sens = %x",sens);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,90,affichage_sens);
//				osMutexRelease(ID_mut_lcd);
//				break;
//			case 2 :
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_sens,"sens = %x",sens);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,90,affichage_sens);
//				osMutexRelease(ID_mut_lcd);
//				break;
//			case 3 :
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_sens,"sens = %x",sens);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,90,affichage_sens);					
//				break;
//			case 4 :
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_sens,"sens = %x",sens);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,90,affichage_sens);
//				break;
//			case 5 :
//				osMutexWait(ID_mut_var,osWaitForever);
//				sprintf(affichage_sens,"sens = %x",sens);
//				osMutexRelease(ID_mut_var);
//				osMutexWait(ID_mut_lcd,osWaitForever);
//				GLCD_DrawString(1,90,affichage_sens);
//				osMutexRelease(ID_mut_lcd);
//				break;
//		}
		switch(direction)
		{
			case 1:
				fonction_PWM_direction(0.11);//11%, tourne extreme gauche 
				break;
			case 2:
				fonction_PWM_direction(0.088);//8.8%, tourne gauche
				break;
			case 3:
				fonction_PWM_direction(0.077);//7.7%, roue droite  
				break;
			case 4:
				fonction_PWM_direction(0.063);//6.3%, tourne droite 
				break;
			case 5:
				fonction_PWM_direction(0.04);//4%, tourne extreme droite
				break;
		}
	
//		switch(sens)
//		{ 
//			case 1: // recule à vitesse rapide
//				fonction_recule();
//				LPC_PWM1->MR2 = 0.6*(1249+1)-1;    // 60% ceci ajuste la duree de l'état haut
//				fonction_PWM_sens();
//				
//				break;
//			case 2: //recule à vitesse lente
//				fonction_recule();
//				LPC_PWM1->MR2 = 0.3*(1249+1)-1;    // 30% ceci ajuste la duree de l'état haut
//				fonction_PWM_sens();
//				break;
//			case 3: //arret des roues
//				fonction_avance();
//				LPC_PWM1->MR2 = 0.1*(1249+1)-1;    // 10% ceci ajuste la duree de l'état haut
//				fonction_PWM_sens();
//				break;
//			case 4: //avance à vitesse lente
//				fonction_avance();
//				LPC_PWM1->MR2 = 0.3*(1249+1)-1;    // 30% ceci ajuste la duree de l'état haut
//				fonction_PWM_sens();
//				break;
//			case 5:	//avance à vitesse rapide
//				fonction_avance();
//				LPC_PWM1->MR2 = 0.6*(1249+1)-1;    // 60% ceci ajuste la duree de l'état haut
//				fonction_PWM_sens();
//				break;
//		}
	}
}

void fonction_PWM_sens(void)
{
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
				
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
				
	// valeur de 20KHz
	// ceci permet de régler facilement le rapport cyclique entre 1 et 100	

	LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
																							 // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
																	
	LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter
}

void fonction_recule(void)
{
	GPIO_SetDir (0, 16, 1);
	GPIO_SetDir (0, 17, 1);
	LPC_GPIO0->FIOPIN=(0<<17); //INB=0, P0.17
	LPC_GPIO0->FIOPIN=(1<<16); //INB=1, P0.16 recule	
	
//	GPIO_PinWrite (0,17,0); //INB=0, P0.17
//	GPIO_PinWrite (0,16,1); //INA=1, P0.16 recule
//	LPC_GPIO0->FIOPIN|=(1<<16); //INA=1, P0.16 recule		
//	LPC_GPIO0->FIOPIN&=(0<<17); //INB=0, P0.17
}

void fonction_avance(void)
{
	GPIO_SetDir (0, 16, 1);
	GPIO_SetDir (0, 17, 1);
	LPC_GPIO0->FIOPIN=(0<<16); //INA=0, P0.16
	LPC_GPIO0->FIOPIN=(1<<17); //INB=1, P0.17 avance			
	
//	GPIO_PinWrite (0,16,0); //INA=0, P0.16
//	GPIO_PinWrite (0,17,1); //INB=1, P0.17 avance	
//	LPC_GPIO0->FIOPIN|=(1<<17); //INB=1, P0.17 avance		
//	LPC_GPIO0->FIOPIN&=(0<<16); //INA=0, P0.16
}

void fonction_PWM_direction(double rapport_cyclique)
{
//	// initialisation des timers
//	LPC_SC->PCONP = LPC_SC->PCONP | 0x00C00000;   //  à compléter enable Timer 0,1,2,3 et PWM1

//	// init de timer 0
//	LPC_TIM0->CTCR = 0;
//	LPC_TIM0->PR = 0;  // à compléter prescaler pour obtenir une référence de 1 us
//  LPC_TIM0->MR0 = 249999;    // valeur à compéter pour obtenir une période de comptégae de 20 ms
//	
//	LPC_TIM0->MCR|=(3<<4); // à compléter le compteur est remis à 0 chaque fois qu'il atteint la valeur de MR0 (toutes les 20 ms)
//	
//	
//	LPC_PINCON->PINSEL7 |= (2<<20); // P3.26 est la sortie 1 de Timer 0 (MAT0.1) 
//	
//	
//	LPC_TIM0->EMR=0x0030; // Toggle sur MAT0.0 quand le compteur atteint son maximum; 
//	LPC_TIM0->TCR = 1;  /*validation de timer 0 et reset counter */

//	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
//	
//	// prescaler+1 = 1 	
//	LPC_PWM1->PR = 0;  // prescaler
//	
//	// valeur de MR0  + 1 = 500 000 cela fait 20 ms, période de la PWM
//  // valeur de 50Hz
//  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

//  LPC_PWM1->MR0 = 499999;    // Ceci ajuste la période de la PWM à 20 ms
//	
//	LPC_PWM1->MR3 = rapport_cyclique*(499999+1)-1;    // ceci ajuste la duree de l'état haut
//	
//	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | 0x00300000; //  P3.26 est la sortie PWM Channel 3 de Timer 1
//	
//	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
//	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000009;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
//	                                             // bit 0 = MR0    bit3 = MR3
//	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
//	                                
//  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 

	LPC_SC->PCONP = LPC_SC->PCONP | 0x00C00000;   //  à compléter enable Timer 0,1,2,3 et PWM1
	LPC_TIM0->CTCR = 0;
	// prescaler+1 = 1 	
	LPC_TIM0->PR = 0;  // prescaler
	
	// valeur de MR0  + 1 = 500 000 cela fait 20 ms, période de la Timer
  // valeur de 50Hz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_TIM0->MR0 = 249999;    // Ceci ajuste la période du Timer à 20 ms
	
//	LPC_TIM0->MR1 = rapport_cyclique*(249999+1)-1;    // ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 |= (2<<20); // P3.26 est la sortie 1 de Timer 0 (MAT0.1) 
	
	LPC_TIM0->MCR|=(1<<1); // le compteur est remis à 0 chaque fois qu'il atteint la valeur de MR0 (toutes les 20 ms)
	LPC_TIM0->EMR|=(3<<4); // Toggle sur MAT0.1 quand le compteur atteint son maximum;
	
  LPC_TIM0->TCR = 1;  //validation de timer 1 et reset counter 
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