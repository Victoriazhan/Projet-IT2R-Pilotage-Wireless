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
	
int main (void)
{	int rapport_cyclique;
	
	rapport_cyclique=tourne_extreme_gauche();
	
	direction_roue(rapport_cyclique);
	
	return 0;
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
	
	LPC_PWM1->MR2 = 0.2*(1249+1)-1;    // 50% ceci ajuste la duree de l'état haut
	
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