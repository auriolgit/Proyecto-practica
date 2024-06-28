const int trigPin = 8;    // Pin de trig del sensor de ultrasonido
const int echoPin = 9;    // Pin de echo del sensor de ultrasonido
const int motorPin1 = 6;  // Pin 1 del puente H para controlar el motor 1 (IN1)
const int motorPin2 = 7;  // Pin 2 del puente H para controlar el motor 1 (IN2)
const int motorPin3 = 4;  // Pin 1 del puente H para controlar el motor 2 (IN3)
const int motorPin4 = 5;  // Pin 2 del puente H para controlar el motor 2 (IN4)

const int distanceThreshold = 10; // Umbral de distancia para detectar obstáculos
const int clearThreshold = 15;    // Umbral de distancia para dejar de girar

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
  long distance = getDistance();

  if (distance < distanceThreshold) {
    // Detener el carrito por 2 segundos
    detenerCarrito();
    delay(2000);
    
    // Girar en su propio eje (solo rueda derecha hacia atrás)
    girarEnPropioEje();
    delay(1000); // Gira por 1 segundo
    
    // Detener el giro en su propio eje
    detenerCarrito();
    
    // Esperar hasta que no haya obstáculo o se supere el umbral de distancia para dejar de girar
    while (distance < distanceThreshold && distance > clearThreshold) {
      distance = getDistance();
    }
    
    // Continuar avanzando si no hay obstáculo
    avanzarSimple();
  } else {
    // Continuar avanzando si no hay obstáculo
    avanzarSimple();
  }

  // Muestra información en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100); // Ajusta el retardo según sea necesario
}

long getDistance() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}

void detenerCarrito() {
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}

void girarEnPropioEje() {
  // Gira el carrito en su propio eje (solo rueda derecha hacia atrás)
  analogWrite(motorPin1, 0);    // Avanzar rueda izquierda
  analogWrite(motorPin2, 0);    // Detener rueda derecha
  analogWrite(motorPin3, 255);    // Detener rueda izquierda
  analogWrite(motorPin4, 0);  // Retroceder rueda derecha
}

void avanzarSimple() {
  // Activa los motores para avanzar
  analogWrite(motorPin1, 0);  // Avanzar rueda izquierda
  analogWrite(motorPin2, 255);    // Detener retroceso rueda izquierda
  analogWrite(motorPin3, 0);  // Avanzar rueda derecha
  analogWrite(motorPin4, 255);    // Detener retroceso rueda derecha
}
