#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "stdio.h"
#include <string.h>
#include "Driver_USART.h" 

#define SLAVE_I2C_ADDR       0x52 		//0x52	  // Adresse esclave sur 7 bits
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;
extern ARM_DRIVER_I2C Driver_I2C0;
extern ARM_DRIVER_USART Driver_USART1;

uint8_t DeviceAddr;

void write1byte (unsigned char composant, unsigned char registre, unsigned char valeur);
char read1byte (unsigned char composant, unsigned char registre);
void write_without_sub (unsigned char composant, unsigned char valeur);
char valeur_converti_pos_x (char pos_X);
char valeur_converti_pos_y (char pos_Y);
	
void Init_I2C(void){
	Driver_I2C0.Initialize(NULL);
	Driver_I2C0.PowerControl(ARM_POWER_FULL);
	Driver_I2C0.Control(	ARM_I2C_BUS_SPEED,				// 2nd argument = d�bit
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
	uint8_t tab[10];
	char  Position_X_conv, Position_Y_conv, donnee_envoie,bl;
	uint8_t chaine[50], chaine_conv[50], chaine_bl[3],chaine_blX[1],chaine_blY[1];
	unsigned char x,y,accx,accy,accz,reste;
	
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	Init_UART();
	Init_I2C();
	
	//configuration du nunchuk
	write1byte(SLAVE_I2C_ADDR,0xF0,0x55);		// SLAVE : reg[0xf0]=0x55 
	write1byte(SLAVE_I2C_ADDR,0xFB,0x00);   // SLAVE : reg[0xfb]=0x00

	
	while (1)
	{
		write1byte(SLAVE_I2C_ADDR,0x40,0x00);		//demande d'une nouvelle conversion des capteurs
		write_without_sub (SLAVE_I2C_ADDR, 0x00);	//debut de la demande d'info
		
		Driver_I2C0.MasterTransmit (SLAVE_I2C_ADDR,tab, 0, true);		// demande de valeurs
		while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
		Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &x, 1, false); // r�cup�ration de la valeur de x
		while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
		Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &y, 1, false); // r�cup�ration de la valeur de y
		while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
//	code � activer pour recevoir les diff�rentes valeurs des autres capteurs
//	Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &accx, 1, false); // r�cup�ration de la valeur de acc�l�rometre x
//	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
//	
//	Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &accy, 1, false); // r�cup�ration de la valeur de acc�l�rometre y
//	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
//	
//	Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &accz, 1, false); // r�cup�ration de la valeur de acc�l�rometre z
//	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
//	
//	Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &reste, 1, false); // info autres voir doc
//	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
		
		//affichage sur le LCD
		sprintf(chaine,"X= %3d, Y= %3d",x, y);	// du blabla
		GLCD_DrawString(1,1,(unsigned char*)chaine);	// colonne puis ligne en pixel
		
		//utilisation de variable passerelle
		Position_X_conv = valeur_converti_pos_x (x);
		Position_Y_conv = valeur_converti_pos_y (y);
		
		//donnee X et Y mise sur 1 octet
		donnee_envoie = ((Position_X_conv<<4)| (Position_Y_conv)); 
		
		//affichage sur le LCD
		sprintf(chaine_conv,"X_c=%1d Y_c=%1d o=%x",Position_X_conv, Position_Y_conv, donnee_envoie);	// du blabla
		GLCD_DrawString(1,50,(unsigned char*)chaine_conv);	// colonne puis ligne en pixel
		
		//trame_bluetooth
		sprintf(chaine_bl,"%c",donnee_envoie);//octet full
		GLCD_DrawString(100,200,chaine_bl);
		
		//envoie bluetooth
			//test

		while(Driver_USART1.GetStatus().tx_busy==1);
		Driver_USART1.Send(chaine_bl,strlen(chaine_bl));
	
		sprintf(chaine_bl,"%c",donnee_envoie);
		GLCD_DrawString(1,100,(unsigned char*)chaine_bl);	// colonne puis ligne en pixel
			}
	return 0;
}

char read1byte (unsigned char composant, unsigned char registre)		//non utiliser
{
	unsigned char tab[1]; //d�claration d'un tableau nomm� "tab"
	unsigned char valeur; //valeur qui va contenir la data que l'esclave veut communiquer au maitre 
	
	tab[0] = registre; //Sous adresse de du registre CTRL5 li� au magn�tom�tre 
	
	Driver_I2C0.MasterTransmit (composant, tab, 1, true);	// START + {resigre + W} + true = pas de STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
	Driver_I2C0.MasterReceive (SLAVE_I2C_ADDR, &valeur, 1, false); // RESTART + {resigre + R} + Data "valeur" + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
	return valeur;
}

void write1byte (unsigned char composant, unsigned char registre, unsigned char valeur)		//ecriture d'un octet sur le registre d'une adresse
{
	unsigned char tab[2]; //d�claration d'un tableau nomm� "tab"
	
	tab[0] = registre; //Sous adresse de du registre CTRL5 li� au magn�tom�tre 
	tab[1] = valeur; //data � �crire dans le registre CTRL5
	
	Driver_I2C0.MasterTransmit (composant, tab, 2, false);		// START + {resigre + W} + Data + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
}

void write_without_sub (unsigned char composant, unsigned char valeur)		//envoie d'un octet � esclave sans sous adresse
{
	unsigned char tab[1]; //d�claration d'un tableau nomm� "tab"
	
	tab[0] = valeur; //data � �crire
	
	Driver_I2C0.MasterTransmit (composant, tab, 1, false);		// START + {resigre + W} + Data + false = avec STOP
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
}

char valeur_converti_pos_x (char pos_X)
	
{
	char valeur_converti_pos_x;
	
	if ( (pos_X>=0) && (pos_X<=30) ) valeur_converti_pos_x = 1; //gauche extr�me
	else if ( (pos_X>30) && (pos_X<133) ) valeur_converti_pos_x = 2; //gauche
	else if ( (pos_X>=133) && (pos_X<=134)) valeur_converti_pos_x = 3; // normal
	else if ( (pos_X>134) && (pos_X<=230) ) valeur_converti_pos_x = 4; //droite
	else if ( (pos_X>230) && (pos_X<=254) ) valeur_converti_pos_x = 5; //droite extr�me
	
	
	return valeur_converti_pos_x;
}

char valeur_converti_pos_y (char pos_Y)
	
{
	char valeur_conv_pos_y;
	
	if ( (pos_Y>=0) && (pos_Y<=65) ) valeur_conv_pos_y = 1; //recule vite
	else if ( (pos_Y>65) && (pos_Y<130) ) valeur_conv_pos_y = 2; //recule
	else if (pos_Y==130) valeur_conv_pos_y = 3; // normal
	else if ( (pos_Y>130) && (pos_Y<=192) ) valeur_conv_pos_y = 4; //avance
	else if ( (pos_Y>192) && (pos_Y<=255) ) valeur_conv_pos_y = 5; //avance vite
	
	
	return valeur_conv_pos_y;
}
