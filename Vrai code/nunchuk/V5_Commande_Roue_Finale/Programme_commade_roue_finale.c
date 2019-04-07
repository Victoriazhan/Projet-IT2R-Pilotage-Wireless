#include "LPC17xx.h"                    // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO


void train1 (void const *argument);                             // thread function
osThreadId tid_train1;                                          // thread id
osThreadDef (train1, osPriorityNormal, 1, 0);                   // thread object

void train2 (void const *argument);                             // thread function
osThreadId tid_train2;                                          // thread id
osThreadDef (train2, osPriorityNormal, 1, 0);                   // thread object

void train4 (void const *argument);                             // thread function
osThreadId tid_train4;                                          // thread id
osThreadDef (train4, osPriorityNormal, 1, 0);                   // thread object

int main(void)
{
	osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here

  //Création de la tâche de gestion du train1
  tid_train1 = osThreadCreate (osThread(train1), NULL);
	tid_train2 = osThreadCreate (osThread(train2), NULL);
	tid_train4 = osThreadCreate (osThread(train4), NULL);
	
  osKernelStart ();                         // start thread execution 
  
  osDelay(osWaitForever);

}
	