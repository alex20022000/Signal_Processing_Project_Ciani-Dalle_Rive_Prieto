#include <Arduino.h>

hw_timer_t *timer = NULL;

volatile bool lecturaFlag = false; // Flag para switchear en la callback

const int ldrPin = 35;

// Función para calcular la frecuencia del timer a partir del prescaler
unsigned long getTimerFrequency(int prescaler) {
  return APB_CLK_FREQ / prescaler;
}

// Función para convertir una frecuencia en Hz a ticks
unsigned long frequencyToTicks(float frequencyHz, int prescaler) {
  unsigned long timerFreq = getTimerFrequency(prescaler);
  return (unsigned long)(timerFreq / frequencyHz);
}

void IRAM_ATTR lecturaLDR()
{
  lecturaFlag = true;
}

void setup()
{
  Serial.begin(115200);
  analogReadResolution(12);

  int prescaler = 80; // Prescaler de 80 para una frecuencia del timer de 1MHz (1 µs por tick)

  // Configuracion del timer
  timer = timerBegin(0, prescaler, true); // Timer 0, prescaler 80, contador ascendente
  timerAttachInterrupt(timer, &lecturaLDR, true); // Vinculo la callback del timer
  int desiredFrequencyHz = 200; // Frecuencia deseada en Hz
  unsigned long ticks = frequencyToTicks(desiredFrequencyHz, prescaler);
  timerAlarmWrite(timer, ticks, true);
  timerAlarmEnable(timer);
  Serial.print("Timer configurado correctamente a: ");
  Serial.print(desiredFrequencyHz);
}

void loop()
{
  if(lecturaLDR)
  {
    lecturaFlag = false;
    int ldrValue = analogRead(ldrPin); // Leer valor del LDR
    Serial.print(">LDR:");
    Serial.println(ldrValue);
  }
}