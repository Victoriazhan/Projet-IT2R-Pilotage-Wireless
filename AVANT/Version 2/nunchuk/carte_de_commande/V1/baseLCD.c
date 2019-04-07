#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "stdio.h"

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

int main (void){
	uint8_t tab[50];
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	while (1){
		sprintf(tab,"Blabla %d",25);	// du blabla
		GLCD_DrawString(100,100,tab);	// colonne puis ligne en pixel
	}
	
	return 0;
}
