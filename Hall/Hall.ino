#include <ACROBOTIC_SSD1306.h>
#include "Pantalla.h"
#include "Comunicaciones.h"

float out = 0;

void setup() 
{
  //while (!Serial);
  Wire.begin();  
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  oled.drawBitmap(CDT, 1024);       // 1024 pixels for logo
  delay(3000);
  oled.clearDisplay();

  Serial.begin(115200);

  wifisetup();

}

void loop() 
{
  Encabezado();
  out = 0.0503 * (analogRead(A0)) + 1.6456;
  Imprimir(String(out));
  wifiloop(out);
  delay(600000);
}
