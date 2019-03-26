#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stdio.h"

#define SLAVE_I2C_ADDR       0x52 		//0x52	  // Adresse esclave sur 7 bits
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;
extern ARM_DRIVER_I2C Driver_I2C0;

uint8_t DeviceAddr;

void write1byte (unsigned char composant, unsigned char registre, unsigned char valeur);
void write_without_sub (unsigned char composant, unsigned char valeur);
char read1byte (unsigned char composant, unsigned char registre);
void commande_nunchuk (void);
char receive1byte (unsigned char composant);

void Init_I2C(void){
	Driver_I2C0.Initialize(NULL);
	Driver_I2C0.PowerControl(ARM_POWER_FULL);
	Driver_I2C0.Control(	ARM_I2C_BUS_SPEED,				// 2nd argument = débit
							ARM_I2C_BUS_SPEED_STANDARD  );	// 100 kHz
	Driver_I2C0.Control(	ARM_I2C_BUS_CLEAR,
							0 );
}

int main (void){
	uint8_t tab[10];
	char position_X,position_Y;
	uint8_t chaine[50],chaineY[50];
	
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	Init_I2C();
	//configuration du nunchuk
	write1byte(SLAVE_I2C_ADDR,0xF0,0x55);		// SLAVE : reg[0xf0]=0x55 
	write1byte(SLAVE_I2C_ADDR,0xFB,0x00);   // SLAVE : reg[0xfb]=0x00

	/*
	while (1)
	{
		write1byte(SLAVE_I2C_ADDR,0xF0,0x55);		// SLAVE : reg[0xf0]=0x55 
		write1byte(SLAVE_I2C_ADDR,0xFB,0x00);   // SLAVE : reg[0xfb]=0x00
		position_X = ((read1byte (SLAVE_I2C_ADDR,0x00))^0x17)+0x17; //			Exact data = (Redaing data XOR 0x17) + 0x17
		position_Y = ((read1byte (SLAVE_I2C_ADDR,0x01))^0x17)+0x17;
		
		sprintf(chaine,"position X = %x ",position_X);	// du blabla
		GLCD_DrawString(1,1,(unsigned char*)chaine);	// colonne puis ligne en pixel
		
	  sprintf(chaineY,"position  Y = %x ",position_Y);	// du blabla
		GLCD_DrawString(1,50,(unsigned char*)chaineY);	// colonne puis ligne en pixel
	}
	*/
	
	return 0;
}

char read1byte (unsigned char composant, unsigned char registre)
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

void write1byte (unsigned char composant, unsigned char registre, unsigned char valeur)
{
	unsigned char tab[2]; //déclaration d'un tableau nommé "tab"
	
	tab[0] = registre; //Sous adresse de du registre CTRL5 lié au magnétomètre 
	tab[1] = valeur; //data à écrire dans le registre CTRL5
	
	Driver_I2C0.MasterTransmit (composant, tab, 2, false);		// START + {resigre + W} + Data + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
}

void commande_nunchuk (void)
{
	unsigned char tab[1]; //déclaration d'un tableau nommé "tab"
	
	tab[0] = 0x00; //valeur de la commande de conversion
	
	Driver_I2C0.MasterTransmit (SLAVE_I2C_ADDR, tab, 1, false);		// START + Data + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
}

void write_without_sub (unsigned char composant, unsigned char valeur)
{
	unsigned char tab[1]; //déclaration d'un tableau nommé "tab"
	
	tab[0] = valeur; //data à écrire
	
	Driver_I2C0.MasterTransmit (composant, tab, 1, false);		// START + {resigre + W} + Data + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
}

char receive1byte (unsigned char composant)
{
	unsigned char valeur; //valeur qui va contenir la data que l'esclave veut communiquer au maitre 
	Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &valeur, 1, false); // RESTART + {resigre + R} + Data "valeur" + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
	return valeur;
}