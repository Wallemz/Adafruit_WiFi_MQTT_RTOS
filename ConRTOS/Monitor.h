//*****************************************************************
// Task will periodically print out useful information about the tasks running
// Is a useful tool to help figure out stack sizes being used
// Run time stats are generated from all task timing collected since startup
// No easy way yet to clear the run time stats yet
//*****************************************************************
static char ptrTaskList[400]; //temporary string bufer for task stats

void taskMonitor(void *pvParameters)
{
    int x;
    int measurement;
    
    SERIAL.println("Task Monitor: Started");

    // run this task afew times before exiting forever
    while(1)
    {
      myDelayMs(10000); // print every 10 seconds

      SERIAL.println("****************************************************");
      SERIAL.print("Free Heap: ");
      SERIAL.print(xPortGetFreeHeapSize());
      SERIAL.println(" bytes");

      SERIAL.print("Min Heap: ");
      SERIAL.print(xPortGetMinimumEverFreeHeapSize());
      SERIAL.println(" bytes");

      SERIAL.println("****************************************************");
      SERIAL.println("Task            ABS             %Util");
      SERIAL.println("****************************************************");

      vTaskGetRunTimeStats(ptrTaskList); //save stats to char array
      SERIAL.println(ptrTaskList); //prints out already formatted stats

    SERIAL.println("****************************************************");
    SERIAL.println("Task            State   Prio    Stack   Num     Core" );
    SERIAL.println("****************************************************");

    vTaskList(ptrTaskList); //save stats to char array
    SERIAL.println(ptrTaskList); //prints out already formatted stats

    SERIAL.println("****************************************************");
    SERIAL.println("[Stacks Free Bytes Remaining] ");

    measurement = uxTaskGetStackHighWaterMark( Handle_PantallaTask );
    SERIAL.print("Pantalla: ");
    SERIAL.println(measurement);

    measurement = uxTaskGetStackHighWaterMark( Handle_MMHallTask );
    SERIAL.print("Media Movil Hall: ");
    SERIAL.println(measurement);

    measurement = uxTaskGetStackHighWaterMark( Handle_WifiMQTT );
    SERIAL.print("Wifi MQTT: ");
    SERIAL.println(measurement);

    measurement = uxTaskGetStackHighWaterMark( Handle_monitorTask );
    SERIAL.print("Monitor Stack: ");
    SERIAL.println(measurement);

    SERIAL.println("****************************************************");

    }

    // delete ourselves.
    // Have to call this or the system crashes when you reach the end bracket and then get scheduled.
    SERIAL.println("Task Monitor: Deleting");
    vTaskDelete( NULL );

}
