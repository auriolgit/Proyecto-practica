#include <Servo.h>
#include <NewPing.h>

// Definir pines del motor
const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 5;
const int RightMotorBackward = 4;

// Definir pines del sensor ultrasónico
const int trigPin = 8;
const int echoPin = 9;

Servo miServo;
NewPing sonar(trigPin, echoPin);

void setup() {
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  miServo.attach(10); // Asigna un pin disponible para el servo
}

void loop() {
  // Medir la distancia con el sensor ultrasónico
  int distancia = medirDistancia();

  // Si no hay obstáculos cercanos, avanzar
  if (distancia > 10) {
    moveForward();
  } else {
    // Si hay un obstáculo, realizar maniobra de evasión
    avoidObstacle();
  }
}

int medirDistancia() {
  return sonar.ping_cm();
}

void moveStop() {
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

void moveForward() {
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
}

void moveRight() {
  // Girar a la derecha
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

void moveLeft() {
  // Girar a la izquierda
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
}

void avoidObstacle() {
  // Detener el movimiento
  moveStop();

  // Realizar un escaneo del entorno girando el servo
  for (int angulo = 0; angulo <= 180; angulo += 45) {
    miServo.write(15);
    delay(500); // Ajusta según sea necesario
    int distancia = medirDistancia();

    // Ajusta el umbral para detectar obstáculos a mayor distancia
    int umbral = 10; // Ajusta según sea necesario

    // Si no hay obstáculos en la dirección actual, girar hacia esa dirección y avanzar
    if (distancia > umbral) {
      if (angulo < 90) {
        // Girar a la derecha
        moveRight();
      } else {
        // Girar a la izquierda
        moveLeft();
      }
      delay(500); // Ajusta según sea necesario
      moveForward();
      break; // Salir del bucle de escaneo
    }
  }

  // Volver el servo a la posición frontal
  miServo.write(90);
  delay(500); // Ajusta según sea necesario
}
