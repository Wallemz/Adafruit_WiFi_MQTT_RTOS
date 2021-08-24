//*****************************************************************
// Create a thread that prints out B to the screen every second
// this task will run forever
//*****************************************************************

int tam = 50, i = 0;
float Medicion[50], ValorAnt = 0.0, ValorAct = 0.0, val = 0.0, media = 0.0;

static void threadMMHall( void *pvParameters ) 
{
  SERIAL.println("Media Movil Hall: Iniciada");

  while(1)
  {
    // MUEVE UNA POSICION TODO EL VECTOR
    media = 0.0;
    for (int i = tam-1; i >= 1; i--)
    {
      if (i == tam-1)
      {
        ValorAnt = Medicion[i-1];
        Medicion[i] = ValorAnt;
      }
      else
      {
        ValorAnt = Medicion[i-1];
        ValorAct = Medicion[i];
        Medicion[i] = ValorAnt;
        Medicion[i+1] = ValorAct;
      }
      
    }
    // ACTUALIZA EL PRIMER VALOR DEL VECTOR
    val = 0.0503 * (analogRead(A0)) + 1.6456;;
    Medicion[0] = val;
  
    // VECTOR FINAL
    for (int i = 0; i <= tam-1; i++)
    {
      SERIAL.print(Medicion[i]);
      SERIAL.print(" ");
    }
    SERIAL.println();
  
  
    
    for (int i=0; i<tam; i++)
    {
      media = media + Medicion[i];
    }
    mediahall = media / tam;
    SERIAL.print("Media: ");
    SERIAL.println(mediahall);

    
    myDelayMs(250);
  }
}
