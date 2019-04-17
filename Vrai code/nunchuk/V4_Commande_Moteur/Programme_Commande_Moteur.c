#include "LPC17xx.h"                    // Device header
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "GPIO.c"


int roue_droite (void);

int tourne_gauche (void);
int tourne_extreme_gauche(void);

int tourne_droite(void);
int tourne_extreme_droite(void);

void direction_roue(int rapport_cyclique);

void avance_roue_lent(void);
void avance_roue_vite(void);

void recule_roue_lent(void);
void recule_roue_vite(void);

void avance_roue_selon_rapport(int rapport_cyclique);
void recule_roue_salon_rapport(int rapport_cyclique);
void fonction_PWM(void);
	void TIMER0_IRQHandler(void)
{
	LPC_TIM1->IR = LPC_TIM1->IR | (1<<0); //baisse le drapeau dû à MR0
	LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN ^ (1<<0); //inverse l’état de P2.0
}

int main (void)
{	
//	Initialise_GPIO ();
//	GPIO_SetDir (0, 16, 1);
//	GPIO_SetDir (0, 17, 1);
//	LPC_GPIO0->FIODIR=((1<<16)|(1<<17));//P0.17 mise en sortie
//	LPC_GPIO0->FIOPIN&=(0<<16); //INA=0, P0.17 avance	
//	LPC_GPIO0->FIOPIN|=(1<<17); //INB=1, P0.17 avance	
//	GPIO_SetDir (0, 16, 1);
//	GPIO_SetDir (0, 17, 1);
//	LPC_GPIO0->FIOPIN&=~(1<<16); //INA=0, P0.16
//	LPC_GPIO0->FIOPIN|=(1<<17); //INA=0, P0.16
//	
//	LPC_PWM1->MR2 = 0.3*(1249+1)-1;    // 10% ceci ajuste la duree de l'état haut
//	fonction_PWM();
//	

//	LPC_GPIO2->FIODIR = LPC_GPIO2->FIODIR | (1<<0); //P2.0 configuré en sortie
//	LPC_TIM0->CTCR = 0; //mode Timer (signaux carrés)
//	LPC_TIM0->PR = 0; //Prescaler à 0
//	LPC_TIM0->MR0 = 25000000 - 1; //valeur de N
//	LPC_TIM0->MCR = LPC_TIM0->MCR | (3<<0);
//	//RAZ du compteur si correspondance avec MR0, et interruption
//	NVIC_SetPriority(1,0); //TIMER0 : interruption de priorité 0
//	NVIC_EnableIRQ(1); //active les interruption TIMER0
	
//	while(1);

	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000002;   //  enable Timer 0

	// init de timer 0, f=20kHz
	
	LPC_TIM0->CTCR = 0;
	LPC_TIM0->PR = 0;  // à compléter prescaler pour obtenir une référence de 1 us
	LPC_TIM0->MR0 = 624;    // valeur à compéter pour obtenir une période de comptégae de 50 µs
	LPC_TIM0->MR1 = 0.1*(624+1)-1; 
	
	LPC_TIM0->MCR=LPC_TIM0->MCR|(1<<1); // le compteur est remis à 0 chaque fois qu'il atteint la valeur de MR0 (toutes les 50µs)
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7| (2<<18); // P3.25	est la sortie 0 de Timer 0 (MAT0.0) 
	
	LPC_TIM0->EMR=0x0030; // Toggle sur MAT0.0 quand le compteur atteint son maximum; 
	LPC_TIM0->TCR = 1;  //validation de timer 0 et reset counter */
	
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00C00000;   //  à compléter enable Timer 0,1,2,3 et PWM1
	LPC_TIM1->CTCR = 0;
	LPC_TIM1->PR = 0;  // à compléter prescaler pour obtenir une référence de 1 us
	LPC_TIM1->MR0 = 0.1*(624+1)-1;    // valeur à compéter pour obtenir une période de 10% de 50 µs
	
	LPC_TIM1->MCR|=(1<<0); // Interruption dès que le compteur atteint la valeur de MR0 (toutes les 10% de 50µs)
	
	NVIC_SetPriority(TIMER1_IRQn,3); //TIMER1 : interruption de priorité 3
	NVIC_EnableIRQ(TIMER1_IRQn); //active les interruption TIMER1
	
	LPC_TIM0->EMR=0x0030; // Toggle sur MAT0.0 quand le compteur atteint son maximum; 
	LPC_TIM0->TCR = 1;  //validation de timer 0 et reset counter */
	
	
	
	
//	int etat =0;
//	while (1)
//	{
//		if (etat==0)
//		{
//			LPC_SC->PCONP = LPC_SC->PCONP | 0x00000002;   //  enable Timer 0

//			// init de timer 0, f=20kHz
//			
//			LPC_TIM0->CTCR = 0;
//			LPC_TIM0->PR = 0;  // à compléter prescaler pour obtenir une référence de 1 us
//			LPC_TIM0->MR0 = 624;    // valeur à compéter pour obtenir une période de comptégae de 50 µs
//			
//			LPC_TIM0->MCR=LPC_TIM0->MCR|(1<<1); // le compteur est remis à 0 chaque fois qu'il atteint la valeur de MR0 (toutes les 50µs)
//			
//			
//			LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7| (2<<18); // P3.25	est la sortie 0 de Timer 0 (MAT0.0) 
//			
//			
//			LPC_TIM0->EMR=0x0030; // Toggle sur MAT0.0 quand le compteur atteint son maximum; 
//			LPC_TIM0->TCR = 1;  //validation de timer 0 et reset counter */
//			etat=1;
//		}
//		else
//		{
//			LPC_SC->PCONP = LPC_SC->PCONP | 0x00000002;   //  enable Timer 0

//			// init de timer 0, f=20kHz
//			
//			LPC_TIM0->CTCR = 0;
//			LPC_TIM0->PR = 0;  // à compléter prescaler pour obtenir une référence de 1 us
//			LPC_TIM0->MR0 = 600;    // valeur à compéter pour obtenir une période de comptégae de 50 µs
//			
//			LPC_TIM0->MCR=LPC_TIM0->MCR|(1<<1); // le compteur est remis à 0 chaque fois qu'il atteint la valeur de MR0 (toutes les 50µs)
//			
//			
//			LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7| (2<<18); // P3.25	est la sortie 0 de Timer 0 (MAT0.0) 
//			
//			
//			LPC_TIM0->EMR=0x0030; // Toggle sur MAT0.0 quand le compteur atteint son maximum
//			LPC_TIM0->TCR = 1;  //validation de timer 0 et reset counter */
//			etat=0;
//		}
//	}
	return 0;
}

void fonction_PWM(void)
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

int roue_droite()
{
	int valeur;
	valeur = (7.5/100)*(499999+1)-1; //7.5%
	
	return valeur;
}           
int tourne_gauche()
{
	int valeur;
	valeur = (8.5/100)*(499999+1)-1; //8.5%
	
	return valeur;
}
int tourne_extreme_gauche()
{
	int valeur;
	valeur = (11.0/100)*(499999+1)-1;//11%
	
	return valeur;
}
int tourne_droite()
{
	int valeur;
	valeur = (6.3/100)*(499999+1)-1;//6.3%
	
	return valeur;
}
int tourne_extreme_droite()
{
	int valeur;
	valeur = (4.0/100)*(499999+1)-1;//4%
	
	return valeur;
}

void direction_roue(int rapport_cyclique)
{	
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
	// valeur de MR0  + 1 = 500 000 cela fait 20 ms, période de la PWM
  // valeur de 50Hz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 499999;    // Ceci ajuste la période de la PWM à 20 ms
	
	LPC_PWM1->MR3 = rapport_cyclique;    // ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | 0x00300000; //  P3.26 est la sortie PWM Channel 3 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000009;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit3 = MR3
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
}

void avance_roue(int rapport_cyclique)
{
		LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
  // valeur de 20KHz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PWM1->MR2 = rapport_cyclique*(1249+1)-1;    // 50% ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
	
	LPC_GPIO0->FIOPIN|=(1<<17); //INB=1, P0.17 avance
}
void recule_roue(int rapport_cyclique)
{
		LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
  // valeur de 20KHz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PWM1->MR2 = rapport_cyclique*(1249+1)-1;    // 50% ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
	
	LPC_GPIO0->FIOPIN|=(1<<16); //INA=1, P0.16 recule
}

void avance_roue_lent(void)
{
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
  // valeur de 20KHz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PWM1->MR2 = 0.3*(1249+1)-1;    // 50% ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
	
	LPC_GPIO0->FIOPIN|=(1<<17); //INB=1, P0.17 avance
}
void avance_roue_vite(void)
{
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
  // valeur de 20KHz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PWM1->MR2 = 0.5*(1249+1)-1;    // 50% ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
	
	LPC_GPIO0->FIOPIN|=(1<<17); //INB=1, P0.17 avance
}

void recule_roue_lent(void)
{
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
  // valeur de 20KHz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PWM1->MR2 = 0.2*(1249+1)-1;    // 50% ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 
	
	LPC_GPIO0->FIOPIN|=(1<<16); //INA=1, P0.16 recule
}
void recule_roue_vite(void)
{
	LPC_SC->PCONP = LPC_SC->PCONP | 0x00000040;   // enable PWM1
	
	// prescaler+1 = 1 	
	LPC_PWM1->PR = 0;  // prescaler
	
  // valeur de 20KHz
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 1249;    // Ceci ajuste la période de la PWM à 50 µs
	
	LPC_PWM1->MR2 = 0.5*(1249+1)-1;    // 50% ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | (3<<18); //  P3.25 est la sortie PWM Channel 2 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000005;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit2 = MR2
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter
	
	LPC_GPIO0->FIOPIN|=(1<<16); //INA=1, P0.16 recule
}

/*
	
	LPC_PWM1->PR = 11;  // prescaler
	
	// valeur de MR0  + 1 = 100 cela fait 48 us, période de la PWM
  // valeur proche des 20 KHz de la gamelle !
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 99;    // Ceci ajuste la période de la PWM à 48 us
	LPC_PWM1->MR3 = rapport_cyclique-1;    // ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | 0x00300000; //  P3.26 est la sortie PWM Channel 3 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000009;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit3 = MR3
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter 


LPC_SC->PCONP = LPC_SC->PCONP | (1<<6); //active le périphérique PWM1
LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4 | (1<<4); //P2.2 en mode PWM1.3
LPC_PWM1->CTCR = 0; //mode Timer
LPC_PWM1->PR = 0; //Prescaler à 0
LPC_PWM1->MR0 = 1249; //valeur de N
LPC_PWM1->MR3 = 312; //pour le rapport cyclique de 25%
LPC_PWM1->MCR = LPC_PWM1->MCR | (1<<1); //RAZ du compteur si correspondance avec MR0
LPC_PWM1->PCR = LPC_PWM1->PCR | (1<<11); //active PWM3
LPC_PWM1->TCR = 1; //démarre le comptage du Timer
return 0 ;



	LPC_PWM1->PR = 11;  // prescaler
	
	// valeur de MR0  + 1 = 100 cela fait 48 us, période de la PWM
  // valeur proche des 20 KHz de la gamelle !
  // ceci permet de régler facilement le rapport cyclique entre 1 et 100	

  LPC_PWM1->MR0 = 99;    // Ceci ajuste la période de la PWM à 48 us
	LPC_PWM1->MR3 = rapport_cyclique-1;    // ceci ajuste la duree de l'état haut
	
	LPC_PINCON->PINSEL7 = LPC_PINCON->PINSEL7 | 0x00300000; //  P3.26 est la sortie PWM Channel 3 de Timer 1
	
	LPC_PWM1->MCR = LPC_PWM1->MCR | 0x00000002; // Timer relancé quand MR0 repasse à 0
	LPC_PWM1->LER = LPC_PWM1->LER | 0x00000009;  // ceci donne le droit de modifier dynamiquement la valeur du rapport cyclique
	                                             // bit 0 = MR0    bit3 = MR3
	LPC_PWM1->PCR = LPC_PWM1->PCR | 0x00000e00;  // autorise la sortie PWM
	                                
  LPC_PWM1->TCR = 1;  //validation de timer 1 et reset counter
	*/