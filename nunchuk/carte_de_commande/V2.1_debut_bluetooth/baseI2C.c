#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "stdio.h"
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD

#define SLAVE_I2C_ADDR       0x52 		//0x52	  // Adresse esclave sur 7 bits

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;
extern ARM_DRIVER_I2C Driver_I2C0;
extern ARM_DRIVER_USART Driver_USART1;

uint8_t DeviceAddr;

void write1byte (unsigned char composant, unsigned char registre, unsigned char valeur);
char read1byte (unsigned char composant, unsigned char registre);
void write_without_sub (unsigned char composant, unsigned char valeur);

void Init_I2C(void){
	Driver_I2C0.Initialize(NULL);
	Driver_I2C0.PowerControl(ARM_POWER_FULL);
	Driver_I2C0.Control(	ARM_I2C_BUS_SPEED,				// 2nd argument = débit
							ARM_I2C_BUS_SPEED_STANDARD  );	// 100 kHz
	Driver_I2C0.Control(	ARM_I2C_BUS_CLEAR,
							0 );
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

int main (void){
	uint8_t tab[50];
	char position_X,position_Y;
	uint8_t chaine[50], chaineU[50];
	unsigned char x,y,accx,accy,accz,reste;
	
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	Init_I2C();
	Init_UART();
	
	//configuration du nunchuk
	write1byte(SLAVE_I2C_ADDR,0xF0,0x55);		// SLAVE : reg[0xf0]=0x55 
	write1byte(SLAVE_I2C_ADDR,0xFB,0x00);   // SLAVE : reg[0xfb]=0x00

	
	while (1)
	{		
		write1byte(SLAVE_I2C_ADDR,0x40,0x00);		//demande d'une nouvelle conversion des capteurs
		write_without_sub (SLAVE_I2C_ADDR, 0x00);	//debut de la demande d'info
		
		Driver_I2C0.MasterTransmit (SLAVE_I2C_ADDR,tab, 0, true);		// demande de valeurs
		while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
		Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &x, 1, false); // récupération de la valeur de x
		while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
		Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &y, 1, false); // récupération de la valeur de y
		while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
		position_X=x;			//(x ^ 0x17) + 0x17;	pour décoder la donnée
		position_Y=y;			//(y ^ 0x17) + 0x17;
		
		//affichage sur le LCD
		sprintf(chaine,"X= %x, Y= %x ",position_X, position_Y);	// du blabla
		GLCD_DrawString(1,1,(unsigned char*)chaine);	// colonne puis ligne en pixel
		
		
		Driver_USART1.Receive(tab,1);
		if (tab[0]=1)
		{
			sprintf(chaineU,"Commande ON\n\r ");	// du blabla
			GLCD_DrawString(1,50,(unsigned char*)chaineU);	// colonne puis ligne en pixel
		}
		else
		{
			sprintf(chaineU,"Commande OFF\n\r ");	// du blabla
			GLCD_DrawString(1,50,(unsigned char*)chaineU);	// colonne puis ligne en pixel
		}
		while (Driver_USART1.GetRxCount()<1);
		
	}
	return 0;
}

char read1byte (unsigned char composant, unsigned char registre)		//non utiliser
{
	unsigned char tab[1]; //déclaration d'un tableau nommé "tab"
	unsigned char valeur; //valeur qui va contenir la data que l'esclave veut communiquer au maitre 
	
	tab[0] = registre; //Sous adresse de du registre CTRL5 lié au magnétomètre 
	
	Driver_I2C0.MasterTransmit (composant, tab, 1, true);	// START + {resigre + W} + true = pas de STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
	Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &valeur, 1, false); // RESTART + {resigre + R} + Data "valeur" + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
	return valeur;
}

void write1byte (unsigned char composant, unsigned char registre, unsigned char valeur)		//ecriture d'un octet sur le registre d'une adresse
{
	unsigned char tab[2]; //déclaration d'un tableau nommé "tab"
	
	tab[0] = registre; //Sous adresse de du registre CTRL5 lié au magnétomètre 
	tab[1] = valeur; //data à écrire dans le registre CTRL5
	
	Driver_I2C0.MasterTransmit (composant, tab, 2, false);		// START + {resigre + W} + Data + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
}

void write_without_sub (unsigned char composant, unsigned char valeur)		//envoie d'un octet à esclave sans sous adresse
{
	unsigned char tab[1]; //déclaration d'un tableau nommé "tab"
	
	tab[0] = valeur; //data à écrire
	
	Driver_I2C0.MasterTransmit (composant, tab, 1, false);		// START + {resigre + W} + Data + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
}
