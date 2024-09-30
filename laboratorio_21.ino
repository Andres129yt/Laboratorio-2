const int Pins[3][3] = {
  // Pins del Led 1
  {13, 12, 11},  // Rojo, Azul, Verde
  // Pins del Led 2  
  {9, 8, 7},    // Rojo, Azul, Verde
  // Pins del Led 3 
  {5, 4, 3}     // Rojo, Azul, Verde
};

// Pin del interruptor
const int switchPin = 1;  // Pin al que está conectado el terminal 1 del interruptor
bool lastSwitchState = HIGH; // El pin debería leer HIGH cuando el interruptor está en "Izquierda"
unsigned long previousMillis = 0;
const long sequenceInterval = 1000; // Intervalo de 2 segundos para cambiar secuencias
const long blinkInterval = 250; // Intervalo de parpadeo de 500 ms
int sequenceStep = 0;
bool ledBlinking = false; // Controla el parpadeo de los LEDs

void setup() {
  // Configurar todos los pines como salidas
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(Pins[i][j], OUTPUT);
    }
  }

  // Configurar el pin del interruptor como entrada con resistencia pull-up interna
  pinMode(switchPin, INPUT_PULLUP);

  // Inicializar la comunicación serial
  Serial.begin(9600);

  // Inicializar los LEDs apagados
  initializeLEDs();
}

void loop() {
  // Leer el estado actual del interruptor
  bool currentSwitchState = digitalRead(switchPin);

  // Si el interruptor está activado (conectado a GND)
  if (currentSwitchState == LOW) {
    // Si el estado del interruptor ha cambiado de HIGH a LOW
    if (lastSwitchState == HIGH) {
      // Resetear el temporizador
      previousMillis = millis();
      sequenceStep = 0;
      ledBlinking = false;
    }

    // Ejecutar la secuencia de LEDs basada en el paso actual
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= sequenceInterval) {
      previousMillis = currentMillis; // Resetear el temporizador
      sequenceStep++;
      
      switch (sequenceStep) {
        case 1:
          // Secuencia 1: LED 1 verde, LEDs 2 y 3 rojos
          setColor(Pins[0], LOW, LOW, HIGH); // LED 1: verde
          setColor(Pins[1], HIGH, LOW, LOW); // LED 2: rojo
          setColor(Pins[2], HIGH, LOW, LOW); // LED 3: rojo
          break;
        case 2:
          // Secuencia 2: LED 1 y 3 rojos, LED 2 verde
          setColor(Pins[0], HIGH, LOW, LOW); // LED 1: rojo
          setColor(Pins[1], LOW, LOW, HIGH); // LED 2: verde
          setColor(Pins[2], HIGH, LOW, LOW); // LED 3: rojo
          break;
        case 3:
          // Secuencia 3: LED 1 y 2 rojos, LED 3 verde
          setColor(Pins[0], HIGH, LOW, LOW); // LED 1: rojo
          setColor(Pins[1], HIGH, LOW, LOW); // LED 2: rojo
          setColor(Pins[2], LOW, LOW, HIGH); // LED 3: verde
          break;
        default:
          // Reiniciar la secuencia
          sequenceStep = 0;
          break;
      }
    }
  } else {
    // Si el interruptor está desactivado, hacer parpadear los LEDs en amarillo
    if (!ledBlinking) {
      previousMillis = millis(); // Reiniciar el temporizador de parpadeo
      ledBlinking = true;
    }

    // Parpadeo de LEDs amarillos
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) { // Intervalo de parpadeo de 500 ms
      previousMillis = currentMillis;
      // Alternar entre encender y apagar los LEDs
      static bool ledState = false;
      ledState = !ledState;
      
      if (ledState) {
        setColor(Pins[0], HIGH, LOW, HIGH); // LED 1: amarillo
        setColor(Pins[1], HIGH, LOW, HIGH); // LED 2: amarillo
        setColor(Pins[2], HIGH, LOW, HIGH); // LED 3: amarillo
      } else {
        setColor(Pins[0], LOW, LOW, LOW); // Apagar LED 1
        setColor(Pins[1], LOW, LOW, LOW); // Apagar LED 2
        setColor(Pins[2], LOW, LOW, LOW); // Apagar LED 3
      }
    }
  }

  // Actualizar el estado del interruptor
  lastSwitchState = currentSwitchState;
}

// Función para encender el LED en un color específico
void setColor(const int* ledPins, bool red, bool blue, bool green) {
  digitalWrite(ledPins[0], red ? HIGH : LOW);    // Rojo pin
  digitalWrite(ledPins[1], blue ? HIGH : LOW);   // Azul pin
  digitalWrite(ledPins[2], green ? HIGH : LOW);  // Verde pin
}

// Función para inicializar los LEDs apagados
void initializeLEDs() {
  // Apagar todos los LEDs
  setColor(Pins[0], LOW, LOW, LOW); // LED 1: apagado
  setColor(Pins[1], LOW, LOW, LOW); // LED 2: apagado
  setColor(Pins[2], LOW, LOW, LOW); // LED 3: apagado
}

