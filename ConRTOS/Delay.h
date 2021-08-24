//**************************************************************************
// Puede usar esta función para retrasos RTOS
// Ten en cuenta la velocidad del procesador
//**************************************************************************

//************* MicroSegundos *************//
void myDelayUs(int us)
{
  vTaskDelay( us / portTICK_PERIOD_US );  
}


//************* MiliSegundos *************//
void myDelayMs(int ms)
{
  vTaskDelay( (ms * 1000) / portTICK_PERIOD_US );  
}


//************* Delay MiliSegundos *************//
//Ignora el tiempo que dura la tarea, se ejecuta siempre en el tiempo especificado 
void myDelayMsUntil(TickType_t *previousWakeTime, int ms)
{
  vTaskDelayUntil( previousWakeTime, (ms * 1000) / portTICK_PERIOD_US );  
}
