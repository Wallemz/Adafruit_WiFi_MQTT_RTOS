//*******************************************************************
//****************            Librerias              ****************
//*******************************************************************
float mediahall = 0;
#define SERIAL    SerialUSB
#include <FreeRTOS_SAMD21.h> //samd21
TaskHandle_t Handle_PantallaTask;
TaskHandle_t Handle_MMHallTask;
TaskHandle_t Handle_WifiMQTT;
TaskHandle_t Handle_monitorTask;
#include "Delay.h"
#include "MMHall.h"
#include "Pantalla.h"
#include "WifiMQTT.h"
#include "Monitor.h"


//*******************************************************************
//****************      Definiciones Constantes    ****************//
//*******************************************************************
//#define  ERROR_LED_PIN  13  //Led Pin: Typical Arduino Board
//#define  ERROR_LED_PIN  2 //Led Pin: samd21 xplained board
//#define ERROR_LED_LIGHTUP_STATE  LOW // El estado que hace que el LED se ilumine en su tablero, ya sea bajo o alto



void setup() 
{
  SERIAL.begin(115200);
  vNopDelayMS(1000); // Evita el bloqueo del controlador usb en el inicio, no omita esto
  //while (!SERIAL) ;  // Wait for SERIAL terminal to open port before starting program
  
  PantallaSetup();
  wifisetup();

  SERIAL.println("");
  SERIAL.println("******************************");
  SERIAL.println("            INICIO            ");
  SERIAL.println("******************************");


  // Error Blink Codes:
  //    3 blinks - Fatal Rtos Error, something bad happened. Think really hard about what you just changed.
  //    2 blinks - Malloc Failed, Happens when you couldn't create a rtos object. 
  //               Probably ran out of heap.
  //    1 blink  - Stack overflow, Task needs more bytes defined for its stack! 
  //               Use the taskMonitor thread to help gauge how much more you need
  //vSetErrorLed(ERROR_LED_PIN, ERROR_LED_LIGHTUP_STATE);

  xTaskCreate(threadWifiMQTT, "WifiMQTT",     512, NULL, tskIDLE_PRIORITY + 3, &Handle_WifiMQTT);
  xTaskCreate(threadPantalla, "Pantalla",     512, NULL, tskIDLE_PRIORITY + 2, &Handle_PantallaTask);
  xTaskCreate(threadMMHall,   "MM Hall",      256, NULL, tskIDLE_PRIORITY + 2, &Handle_MMHallTask);
  xTaskCreate(taskMonitor,    "Task Monitor", 256, NULL, tskIDLE_PRIORITY + 1, &Handle_monitorTask);

  // Start the RTOS, this function will never return and will schedule the tasks.
  vTaskStartScheduler();
}

void loop() 
{
  // Optional commands, can comment/uncomment below
  SERIAL.print("."); //print out dots in terminal, we only do this when the RTOS is in the idle state
  vNopDelayMS(100);
}
