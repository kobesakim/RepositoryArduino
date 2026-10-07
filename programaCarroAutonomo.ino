// General: https://www.youtube.com/watch?v=43hHO71O89g
// Ultrasónico (Medición de Distancia): https://www.youtube.com/watch?v=RKyypt_7aSU
// Motores DC: https://www.youtube.com/watch?v=1DPPf7Y72CA
// Ultrasónico (Obstaculos): https://www.youtube.com/watch?v=5ByBdEY9mUI
// Detector de Colores: https://www.youtube.com/watch?v=LpgKJOS8c20
// Detector de Colores 2: https://www.youtube.com/watch?v=n18FmtGQdJ8
// Servo (Garra): https://robotuno.com/proyecto-brazo-robotico-con-arduino/
// HC-05 (Bluetooth): https://www.youtube.com/watch?v=SUZJAiCuUxA
// Seguidor de Línea: https://www.youtube.com/watch?v=TUIy8t4UU8s

// LIBRERÍAS:
// - - - - - TCS34725 (Detector de Color) - - - - - COMPLETADO
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// - - - - - Servo - - - - - COMPLETADO
#include <Servo.h>

// CONFIGURACIONES:
// - - - - - TCS34725 (Detector de Color) - - - - - COMPLETADO
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_60MS, TCS34725_GAIN_1X);

// - - - - - Servo (Garra) - - - - -
Servo servomotor1;
int SRV = 8;

// - - - - - Pines de Motores - - - - - COMPLETADO
// Motor A (Izquierda)
int ENA = 2;
int IN1 = 3;
int IN2 = 4;

// Motor B (Derecha)
int IN3 = 5;
int IN4 = 6;
int ENB = 7;

// - - - - - Pines para Untrasónico - - - - - COMPLETADO
int TRIG = A1; // Envío de Señal
int ECHO = A2; // Respuesta

long Tiempo1;
long Duracion, Distancia;

// - - - - - Seguidor de Línea (QTR-8A) - - - - - PARCIAL
int sensores[8] = {A3,A4,A5,A6,A7,A8,A9,A10};
int UMBRAL = 100;



void setup() {
  Serial.begin(9600);
  // - - - - - Iniciación de Bluetooth - - - - - COMPLETADO
  Serial1.begin(38400);

  // - - - - - Seguidor de Línea - - - - -
  for(int i=0; i<8; i++){
    pinMode(sensores[i], INPUT);
  }

  // - - - - - Pines de Motores - - - - - COMPLETADO
  // Motor A
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Motor B
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // - - - - - Pin Servomotor - - - - -
  servomotor1.attach(8);

  // - - - - - Pines Ultrasónico - - - - - COMPLETADO
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Tiempo1 = micros();

  // - - - - - Verificación del Detector de Color - - - - - COMPLETADO
  if (!tcs.begin()) {
    Serial.println("Color Sensor not Detected");
    while (1) delay(500);
  }
  Serial.println("Color Sensor Ready");
}




void loop() {
  // - - - - - Lector Bluetooth - - - - - COMPLETADO
  if (Serial1.available())
  Serial.write(Serial1.read());

    if(Serial.available())
    Serial1.write(Serial.read());

  // - - - - - Movimientos de Motor - - - - - COMPLETADO
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH); // ADELANTE
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  // - - - - - Seguidor de Línea - - - - - PARCIAL
  int valores[8];

  for(int i=0; i<8; i++){
    valores[i] = digitalRead(sensores[i]);
  }

  for(int i=0; i<8; i++){
    Serial.print(valores[i]);
    Serial.print(" ");
  }

  Serial.print(" | Estado: ");

  if (valores[3] == 0 || valores[4] == 0) {
    Serial.println("CENTRO");
  }
  else if (valores[0] == 0 || valores[1] == 0 || valores[2] == 0) {
    Serial.println("IZQUIERDA");
  }
  else if (valores[5] == 0 || valores[6] == 0 || valores[7] == 0) {
    Serial.println("DERECHA");
  }
  else {
    Serial.println("SIN LINEA");
  }

  delay(200);

  bool izquierda = false;
  bool centro = false;
  bool derecha = false;

  if (valores[0] > UMBRAL ||
      valores[1] > UMBRAL ||
      valores[2] > UMBRAL) {

    izquierda = true;
  }
  else if (valores[3] > UMBRAL ||
      valores[4] > UMBRAL) {

    centro = true;
  }
  else if (valores[5] > UMBRAL ||
      valores[6] > UMBRAL ||
      valores[7] > UMBRAL) {

    derecha = true;
  }

  if (centro) {
    analogWrite(ENA, HIGH);
    analogWrite(ENB, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); // ADELANTE
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(500);
  }

  else if (izquierda) {
    analogWrite(ENA, LOW);
    analogWrite(ENB, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH); // GIRO IZQUIERDA
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(600);
  }

  else if (derecha) {
    analogWrite(ENA, LOW);
    analogWrite(ENB, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); // GIRO DERECHA
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(600);
  }

  else {
    analogWrite(ENA, LOW);
    analogWrite(ENB, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW); // DETIENE
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(200);
  }

  // - - - - - Función para ultrasónico - - - - - COMPLETADO
  do {
    digitalWrite(TRIG, HIGH);
    if (micros() - Tiempo1 >= 10){
      digitalWrite(TRIG, LOW);
    }
  } while (micros() - Tiempo1 <= 10);

  Duracion = pulseIn(ECHO, HIGH);
  Distancia = Duracion / 59; // Medir distancia en centímetros

  if(Distancia >= 500 || Distancia <= 0){ // Evitar obstáculos.
    Serial.println("Moving Forward");

  } else{
    Serial.println("Obstacle Ahead");
    analogWrite(ENA, LOW);
    analogWrite(ENB, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW); // DETIENE
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(200);
  }
  if (Distancia <= 15 && Distancia >= 1){
    analogWrite(ENA, HIGH);
    analogWrite(ENB, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH); // REVERSA
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(500);

    analogWrite(ENA, LOW);
    analogWrite(ENB, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW); // GIRO
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(500);

    analogWrite(ENA, HIGH);
    analogWrite(ENB, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); // ADELANTE
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(500);
  }

  delay(500);
  
  // - - - - - Función Servo - - - - -
  servomotor1.write(0);
  delay(3000);

  for (int i=0; i<=45; i++){
    servomotor1.write(i);
    delay(25);
  }
  delay(1000);
    for (int i=45; i>=0; i--){
      servomotor1.write(i);
      delay(25);
    }
  delay(10000);
  for (int i=0; i<=45; i++){
    servomotor1.write(i);
    delay(25);
  }
  delay(1000);

  // - - - - - Función para Detector de Colores - - - - - PARCIAL
  float red, green, blue;
  tcs.getRGB(&red, &green, &blue);

  int R = int(red);
  int G = int(green);
  int B = int(blue);

  String color = ""; // Solo son estimaciones
    if(R>G+30 && R>B+20 && B>G){
      color = "Rosa";
      analogWrite(ENA, HIGH);
      analogWrite(ENB, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH); // RETROCEDER
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(550);
    }
    else if(R>B+30 && G>B+30){
      color = "Amarillo";
      analogWrite(ENA, LOW);
      analogWrite(ENB, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH); // GIRO IZQUIERDA
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(600);

      analogWrite(ENA, HIGH);
      analogWrite(ENB, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW); // ADELANTE
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(550);

      analogWrite(ENA, LOW);
      analogWrite(ENB, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW); // GIRO DERECHA
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(600);
    }
    else if(R>G+20 && G>B+20){
      color = "Naranja";
      analogWrite(ENA, HIGH);
      analogWrite(ENB, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW); // ADELANTE
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(550);
    }
    else if(G>R+30 && B>R+30){
      color = "Cian";
      analogWrite(ENA, LOW);
      analogWrite(ENB, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW); // GIRO DERECHA
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(600);

      analogWrite(ENA, HIGH);
      analogWrite(ENB, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW); // ADELANTE
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(550);

      analogWrite(ENA, LOW);
      analogWrite(ENB, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH); // GIRO IZQUIERDA
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(600);
    }

  Serial.print("R: "); Serial.print(int(red));
  Serial.print("  G: "); Serial.print(int(green));
  Serial.print("  B: "); Serial.print(int(blue)); // VALORAR COLORES
  Serial.print("  Color: "); Serial.print(color);
  Serial.println();

}
