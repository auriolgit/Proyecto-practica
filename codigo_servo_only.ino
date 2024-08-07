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

unsigned long tiempoAnterior = 0;
const unsigned long tiempoRetraso = 500; // 0.5 segundos

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  miServo.attach(10); // Asigna un pin disponible para el servo
}

void loop() {
  // Medir la distancia con el sensor ultrasónico
  int distancia = medirDistancia();
  Serial.print("Distancia: ");
  Serial.println(distancia);

  // Ajusta el umbral para detectar obstáculos
  int umbral = 10; // Ajusta según sea necesario

  // Si no hay obstáculos cercanos, avanzar
  if (distancia > umbral) {
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

void moveBackward() {
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
}

void turnLeft() {
  // Girar a la izquierda
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
}

void turnRight() {
  // Girar a la derecha
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
}

void avoidObstacle() {
  // Detener el movimiento
  moveStop();

  // Retroceder levemente
  moveBackward();
  if (millis() - tiempoAnterior >= tiempoRetraso) {
    moveStop();
    tiempoAnterior = millis();
  }


}
