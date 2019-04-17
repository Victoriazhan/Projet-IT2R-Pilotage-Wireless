#include "LPC17xx.h"                    // Device header
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

void moteur_puissance (void const *argument);                             // thread function
osThreadId ID_moteur_puissance;                                          // thread id
osThreadDef (moteur_puissance, osPriorityNormal, 1, 0);                   // thread object

void moteur_direction (void const *argument);                             // thread function
osThreadId ID_moteur_direction;                                          // thread id
osThreadDef (moteur_direction, osPriorityNormal, 1, 0);                   // thread object

void avance_roue_selon_rapport(double rapport_cyclique);
void recule_roue_selon_rapport(double rapport_cyclique);
void direction_roue(double rapport_cyclique);


int main(void)
{
	osKernelInitialize ();                    // initialize CMSIS-RTOS
  //Création de la tache de gestion des moteur

  ID_moteur_puissance = osThreadCreate (osThread(moteur_puissance), NULL);
	ID_moteur_direction = osThreadCreate (osThread(moteur_direction), NULL);
	
  osKernelStart ();                         // start thread execution 
  
  osDelay(osWaitForever);

}

void moteur_puissance (void const *argument)
{
	int etat_nunchuck_recu=5;
	//etat_nunchuck_recu=(etat_nunchuck_recu&0x0F);

	LPC_GPIO0->FIODIR=((1<<16)|(1<<17));//P0.16 ,P0.17 mise en sortie
	
	while(1)
	{/*
		switch(etat_nunchuck_recu)
		{
			case 1 : 
				recule_roue_selon_rapport(0.5);
				break;
			case 2 :
				recule_roue_selon_rapport(0.4);
				break;
			case 3 :
				avance_roue_selon_rapport(0.1);
				break;
			case 4 :
				avance_roue_selon_rapport(0.4);
				break;
			case 5 :
				avance_roue_selon_rapport(0.5);
				break;
		}*/	
	}
}

void moteur_direction (void const *argument)
{
	int etat_nunchuck_recu;
	etat_nunchuck_recu=1;
	
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
	// valeur de MR0  + 1 = 500 000 cela fait 20 ms, période de la PWM
  // valeur de 50Hz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 499999;    // Ceci ajuste la période de la PWM à 20 ms
	
	LPC_PWM1->MR3 = (int)((11.0/100)*(499999+1)-1);    // ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | 0x00300000; //  P3.26 est la sortie PWM Channel 3 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000009;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit3 = MR3
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
	
	
	while(1)
	{
		
		switch(etat_nunchuck_recu)
		{
			case 1 : 
				direction_roue(11);
				break;
			case 2 :
				direction_roue(8.5);
				break;
			case 3 :
				direction_roue(7.5);
				break;
			case 4 :
				direction_roue(6.3);
				break;
			case 5 :
				direction_roue(4);
				break;
		}		
	}
}


void direction_roue(double rapport_cyclique)
{	
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000002;   //  enable Timer 0

	// init de timer 0, f=20kHz
	
	LPC_TIM0->PR = 0;  // à compléter prescaler pour obtenir une référence de 1 us
	LPC_TIM0->MR0 = 624;    // valeur à compéter pour obtenir une période de comptégae de 25 us
	
	LPC_TIM0->MCR=LPC_TIM0->MCR|(1<<1); // à compléter le compteur est remis à 0 chaque fois qu'il atteint la valeur de MR0 (toutes les 20ms)
	
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7| (2<<20); // P3.25 est la sortie 0 de Timer 0 (MAT0.0) 
	
	
	LPC_TIM0->EMR=0x0030; // Toggle sur MAT0.0 quand le compteur atteint son maximum; 
	LPC_TIM0->TCR = 1;  /*validation de timer 0 et reset counter */
	
	/*
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
	// valeur de MR0  + 1 = 500 000 cela fait 20 ms, période de la PWM
  // valeur de 50Hz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 499999;    // Ceci ajuste la période de la PWM à 20 ms
	
	LPC_PWM1->MR3 = (rapport_cyclique/100)*(499999+1)-1;;    // ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 |(2<<20); //  P3.26 est la sortie TIMER , M0.1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000009;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit3 = MR3
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter */
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
	
	LPC_GPIO0->FIOPIN|=(1<<17); //INB=1, P0.17 avance
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
	
	LPC_GPIO0->FIOPIN|=(1<<16); //INA=1, P0.16 recule
}

