// Pines de entrada
const int pin555 = 2;
const int pinFF  = 3;

// Variables canal 1 (555)
int estadoActual1 = 0;
int estadoAnterior1 = 0;

unsigned long tiempoAnterior1 = 0;
unsigned long periodo1 = 0;
unsigned long ultimoPrint = 0;

unsigned long tiempoCambio1 = 0;
unsigned long tHigh1 = 0;
unsigned long tLow1 = 0;

// Variables canal 2 (Flip-Flop)
int estadoActual2 = 0;
int estadoAnterior2 = 0;

unsigned long tiempoAnterior2 = 0;
unsigned long periodo2 = 0;

unsigned long tiempoCambio2 = 0;
unsigned long tHigh2 = 0;
unsigned long tLow2 = 0;

void setup() {
  pinMode(pin555, INPUT);
  pinMode(pinFF, INPUT);

  Serial.begin(9600);
}

void loop() {
 
  unsigned long tiempoActual = millis();

  
  // CANAL 1 (555)
  estadoActual1 = digitalRead(pin555);

  // Detección de flanco ascendente
  if (estadoActual1 == HIGH && estadoAnterior1 == LOW) {
    periodo1 = tiempoActual - tiempoAnterior1;
    tiempoAnterior1 = tiempoActual;
  }

  // Medición de HIGH y LOW
  if (estadoActual1 != estadoAnterior1) {
    if (estadoActual1 == HIGH) {
      tLow1 = tiempoActual - tiempoCambio1;
    } else {
      tHigh1 = tiempoActual - tiempoCambio1;
    }
    tiempoCambio1 = tiempoActual;
  }

  estadoAnterior1 = estadoActual1;

  
  // 🔹 CANAL 2 (Flip-Flop)
  estadoActual2 = digitalRead(pinFF);

  if (estadoActual2 == HIGH && estadoAnterior2 == LOW) {
    periodo2 = tiempoActual - tiempoAnterior2;
    tiempoAnterior2 = tiempoActual;
  }

  if (estadoActual2 != estadoAnterior2) {
    if (estadoActual2 == HIGH) {
      tLow2 = tiempoActual - tiempoCambio2;
    } else {
      tHigh2 = tiempoActual - tiempoCambio2;
    }
    tiempoCambio2 = tiempoActual;
  }

  estadoAnterior2 = estadoActual2;

  
  // CÁLCULOS
  float frecuencia1 = 0;
  float duty1 = 0;

  if (periodo1 > 0) {
  frecuencia1 = 1000.0 / periodo1;
}

if ((tHigh1 + tLow1) > 0) {
  duty1 = (tHigh1 * 100.0) / (tHigh1 + tLow1);
}

  float frecuencia2 = 0;
  float duty2 = 0;

  if (periodo2 > 0) {
    frecuencia2 = 1000.0 / periodo2;
    duty2 = (tHigh2 * 100.0) / (tHigh2 + tLow2);
  }

  
  //SALIDA SERIAL
  if (millis() - ultimoPrint >= 500) { // cada 0.5 s
  ultimoPrint = millis();

  Serial.print("555 -> Freq: ");
  Serial.print(frecuencia1);
  Serial.print(" Hz | Duty: ");
  Serial.print(duty1);
  Serial.print(" %   ||   ");

  Serial.print("FF -> Freq: ");
  Serial.print(frecuencia2);
  Serial.print(" Hz | Duty: ");
  Serial.print(duty2);
    Serial.println(" %");} 
  //Funcion para la grafica de Serial Plotter
   /*int estado555 = digitalRead(pin555);
  int estadoFF = digitalRead(pinFF);

  
  Serial.print(estado555);
  Serial.print(","); 
  Serial.println(estadoFF + 2);*/
}
