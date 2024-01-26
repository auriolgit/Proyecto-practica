const int trigPin = 8;    // Pin de trig del sensor de ultrasonido
const int echoPin = 9;    // Pin de echo del sensor de ultrasonido
const int motorPin1 = 6;  // Pin 1 del puente H para controlar el motor 1
const int motorPin2 = 7; // Pin 2 del puente H para controlar el motor 1
const int motorPin3 = 4;  // Pin 1 del puente H para controlar el motor 2
const int motorPin4 = 5;  // Pin 2 del puente H para controlar el motor 2

// Constantes para el controlador PID
float Kp = 2;  // Constante proporcional (ajústala según sea necesario)
float Ki = 0.1;  // Constante integral (ajústala según sea necesario)
float Kd = 5;  // Constante derivativa (ajústala según sea necesario)
int lastError = 0;
int integral = 0;

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

  // Integral del error
  integral += error;

  // Derivada del error
  int derivative = error - lastError;

  // Control PID
  int motorSpeed = Kp * error + Ki * integral + Kd * derivative;

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
  Serial.print(", Integral: ");
  Serial.print(integral);
  Serial.print(", Derivada: ");
  Serial.println(derivative);

  // Actualiza los valores del error anterior
  lastError = error;

  delay(100); // Ajusta el retardo según sea necesario
}
