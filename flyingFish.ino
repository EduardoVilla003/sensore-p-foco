
//libreria para control remoto
#include <IRremote.h>

//señal de apagado en hexadecimal
#define btn 0x7F80

// pin de relay
#define relayPin 2

// pin control infrarrojo
#define BUTTON_PIN 6

// pin sensor iman
#define iman 3

//variable para alternar el valor de led
bool ledState = false;

// variable para guardar valor de sensor
int sensorLightSignal;

//variable que guardará el valor de sensor iman
int sensorImanSignal;

// variable que guardará el valor de sensor infrarrojo
int sensorInfraSignal;

int imanFlag = LOW;  //bandera para indicar cambios en el estado de sensor de iman

//función para invertir valor de relay
void invertirRelay() {
  if (ledState) {                 // si led = true
    digitalWrite(relayPin, LOW);  // encender
    ledState = false;             // invertir valor de led
  } else {
    digitalWrite(relayPin, HIGH);  //sino pagar
    ledState = true;               // invertir valor de led
  }
}

void setup() {

  Serial.begin(9600);

  IrReceiver.begin(BUTTON_PIN, DISABLE_LED_FEEDBACK);  // inicializar el puerto

  pinMode(A0, INPUT);  // entrada sensor luz

  pinMode(relayPin, OUTPUT);  // salida al relay

  pinMode(iman, INPUT);  //entrada sensor magentismo

  pinMode(BUTTON_PIN, INPUT_PULLUP);  // entrada sensor infrarrojo

  digitalWrite(relayPin, LOW);  // poner relay como bajo
}

void loop() {

  //leer los sensores
  sensorLightSignal = analogRead(A0);
  sensorImanSignal = digitalRead(iman);
  sensorInfraSignal = digitalRead(BUTTON_PIN);

  if (sensorLightSignal <= 500) {  // si recibe luz
    Serial.println("función luz");
    invertirRelay();                          //funcion para invertir el estado del foco
  } else if (sensorImanSignal != imanFlag) {  // si acerca iman
    Serial.println("función iman");
    invertirRelay();  //funcion para invertir el estado del foco
    imanFlag = sensorImanSignal;
  } else if (IrReceiver.decode()) {  //si recibe algo el sensor
    Serial.println("función control");
    int senal = IrReceiver.decodedIRData.decodedRawData;  // guardar el valor recibido en una variable
    if (senal == btn) {                                   //si la señal es = a la señal de apagado en hexadecimal
      invertirRelay();                                    //funcion para invertir el estado del foco
    }
    IrReceiver.resume();  // borrar el valor recibido del sensor
  }
  delay(500);
}
