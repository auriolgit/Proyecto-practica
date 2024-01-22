const int trigPin = 2;    // Pin de trig del sensor de ultrasonido
const int echoPin = 3;    // Pin de echo del sensor de ultrasonido
const int motorPin1 = 9;  // Pin 1 del puente H para controlar el motor 1
const int motorPin2 = 10; // Pin 2 del puente H para controlar el motor 1
const int motorPin3 = 5;  // Pin 1 del puente H para controlar el motor 2
const int motorPin4 = 6;  // Pin 2 del puente H para controlar el motor 2

// Constantes para el controlador PD
float Kp = 2;  // Constante proporcional (ajústala según sea necesario)
float Kd = 5;  // Constante derivativa (ajústala según sea necesario)
int lastError = 0;

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

  // Valor de referencia (distancia deseada)
  int setpoint = 10;

  // Error proporcional
  int error = setpoint - distance;

  // Derivada del error
  int derivative = error - lastError;

  // Control PD
  int motorSpeed = Kp * error + Kd * derivative;

  // Limita la velocidad de los motores
  motorSpeed = constrain(motorSpeed, -255, 255);

  // Aplica la velocidad a los motores
  if (error > 0) {
    analogWrite(motorPin1, motorSpeed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, motorSpeed);
    analogWrite(motorPin4, 0);
  } else {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, abs(motorSpeed));
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, abs(motorSpeed));
  }

  // Muestra información en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.print(" cm, Error: ");
  Serial.print(error);
  Serial.print(", Derivada: ");
  Serial.println(derivative);

  // Actualiza el valor del error anterior
  lastError = error;

  delay(100); // Ajusta el retardo según sea necesario
}