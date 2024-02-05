const int trigPin = 8;    // Pin de trig del sensor de ultrasonido
const int echoPin = 9;    // Pin de echo del sensor de ultrasonido
const int motorPin1 = 6;  // Pin 1 del puente H para controlar el motor 1
const int motorPin2 = 7;  // Pin 2 del puente H para controlar el motor 1
const int motorPin3 = 4;  // Pin 1 del puente H para controlar el motor 2
const int motorPin4 = 5;  // Pin 2 del puente H para controlar el motor 2

// Constante proporcional (ajústala según sea necesario)
float Kp = 5;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  // Información
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.print(" cm, Error: ");
  int setpoint = 10;
  int error = setpoint - distance;
  Serial.println(error);

  // Control proporcional
  int motorSpeed = Kp * error;

  // Limita la velocidad de los motores
  motorSpeed = constrain(motorSpeed, -255, 255);

  // Aplica la velocidad a los motores con lógica de evasión
  if (distance < 10) {
    // Si hay un obstáculo cercano, gira hacia la derecha
    Serial.println("Obstáculo detectado. Gira hacia la derecha.");
    analogWrite(motorPin1, motorSpeed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, motorSpeed);
    analogWrite(motorPin4, 0);
  } else {
    // Si no hay obstáculo, sigue adelante
    Serial.println("Sin obstáculos. Avanza.");
    analogWrite(motorPin1, motorSpeed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, motorSpeed);
    analogWrite(motorPin4, 0);
  }

  delay(100); // Ajusta el retardo según sea necesario
}
