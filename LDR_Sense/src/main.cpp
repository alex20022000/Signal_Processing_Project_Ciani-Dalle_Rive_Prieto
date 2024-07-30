#include <Arduino.h>

hw_timer_t *timer = NULL;
hw_timer_t *durationTimer = NULL;

volatile bool lecturaFlag = false; // Flag para switchear en la callback
//volatile bool buttonPressed = false; // Flag para indicar que se presionó el botón
//volatile bool isReading = false; // Flag para indicar si estamos en periodo de lectura

const int ldrPin = 35;
//const int buttonPin = 23;

// Función para calcular la frecuencia del timer a partir del prescaler
unsigned long getTimerFrequency(int prescaler) {
  return APB_CLK_FREQ / prescaler;
}

// Función para convertir una frecuencia en Hz a ticks
unsigned long frequencyToTicks(float frequencyHz, int prescaler) {
  unsigned long timerFreq = getTimerFrequency(prescaler);
  return (unsigned long)(timerFreq / frequencyHz);
}

// Callback del timer de lectura de datos
void IRAM_ATTR lecturaLDR() {
  lecturaFlag = true;
}
/*
// Callback del botón
void IRAM_ATTR onButtonPress() {
  buttonPressed = true;
}

// Callback del temporizador de duración de la lectura de datos
void IRAM_ATTR stopReading() {
  isReading = false;
  timerAlarmDisable(durationTimer); // Deshabilita el temporizador de tiempo de lectura
}
*/
void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // Resolución del ADC

  //pinMode(buttonPin, INPUT_PULLUP); // Resistencia interna pull-up
  //attachInterrupt(digitalPinToInterrupt(buttonPin), onButtonPress, FALLING); // Interrupción de botón, callback y flanco descendente
  
  int prescaler = 80; // Prescaler de 80 para una frecuencia del timer de 1MHz (1 µs por tick)

  // Configuración del timer de lectura
  timer = timerBegin(0, prescaler, true); // Timer 0, prescaler 80, contador ascendente
  timerAttachInterrupt(timer, &lecturaLDR, true); // Vinculo la callback del timer
  int desiredFrequencyHz = 200; // Frecuencia deseada en Hz
  unsigned long ticks = frequencyToTicks(desiredFrequencyHz, prescaler);
  timerAlarmWrite(timer, ticks, true);
  timerAlarmEnable(timer);
  /*
  // Configuración del timer de tiempo de toma de datos
  durationTimer = timerBegin(1, prescaler, true);
  timerAttachInterrupt(durationTimer, &stopReading, true); // Vinculo la callback stopReading
  unsigned long durationTicks = 15000000; // Ticks para 15 segundos (15 segundos * 1 MHz)
  timerAlarmWrite(durationTimer, durationTicks, false); // Configurar para que no se repita
  */
}

void loop() {
  /*
  if (buttonPressed) {
    buttonPressed = false;
    isReading = true;
    timerAlarmEnable(durationTimer); // Habilitar el temporizador de duración
  }
  
  if (isReading && lecturaFlag) {
    lecturaFlag = false;
    int ldrValue = analogRead(ldrPin); // Leer valor del LDR
    Serial.print(">LDR:");
    Serial.println(ldrValue);
  }
  */
  if (lecturaFlag) {
    lecturaFlag = false;
    int ldrValue = analogRead(ldrPin); // Leer valor del LDR
    Serial.print(">LDR:");
    Serial.println(ldrValue);
  }
}
