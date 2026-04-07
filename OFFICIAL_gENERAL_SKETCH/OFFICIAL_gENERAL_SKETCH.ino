#include <LiquidCrystal_I2C.h>

//токег бота 7748537788:AAGcm8kG79DPWloQALM3fMct7xWcHRz8090
//id бота 7302470355

//в закрытом состоянии 10
//в открытом состоянии 1600
#include <Servo.h>
Servo servo;
const int movPin = 7;



LiquidCrystal_I2C LCD(0x27, 16, 2);
// Подключаем необходимые библиотеки и задаем параметры
#include <Wire.h>
#include <HX711.h>

// MPU6050
#define MPU6050_ADDRESS 0x68
#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0x3B
#define MEASUREMENT_COUNT 20
#define GRE 8
#define RED 9
#define BUZ 2
#define PIN_POT     A3



// Ультразвуковой датчик
const int Tright = 11;
const int Echo = 12;

// Датчик влажности почвы
const int AirValue = 620;
const int WaterValue = 310;
int soilMoistureValue = 0;
int soilmoisturepercent = 0;

// Гибкий датчик
const int flexSensorPin = 35;
const int ledPin = 13;
const float thresholdResistance = 38000.0;
const float knownResistor = 10000.0;

// HX711
#define DT 5  // Пин для данных (DT)
#define SCK 4  // Пин для тактирования (SCK)
HX711 scale;
float calibration_factor = -100;
float samples[100];
int sample_index = 0;
const float gravity = 9.81;

void setup() {
  
  pinMode(movPin, INPUT);
  servo.attach(6);
  pinMode(13, OUTPUT);
  pinMode(PIN_POT, INPUT);
  delay(3000);
  pinMode(BUZ, OUTPUT);
  noTone(BUZ);
  LCD.init();
  LCD.backlight();
  pinMode(GRE, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(GRE, HIGH);
  // Инициализация I2C для MPU6050
  Wire.begin();

  // Инициализация последовательного соединения
  Serial.begin(9600);

  // Инициализация MPU6050
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  // Настройка пинов для ультразвукового датчика
  pinMode(Tright, OUTPUT);
  pinMode(Echo, INPUT);

  // Настройка пина для светодиода
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Настройка HX711
  scale.begin(DT, SCK);
  scale.set_scale(calibration_factor);
  scale.tare();

  // Калибровка гибкого датчика
  Serial.println("Waiting...");
  delay(1000);
  Serial.println("Arduino готов. Жду команды от ESP32...");
}

void loop() {
  int val111 = digitalRead(movPin);
  int rotat = analogRead(PIN_POT);
  // --- MPU6050 ---
  long accelXSum = 0, accelYSum = 0, accelZSum = 0;

  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    int16_t ax, ay, az;
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDRESS, 6, true);

    ax = (Wire.read() << 8) | Wire.read();
    ay = (Wire.read() << 8) | Wire.read();
    az = (Wire.read() << 8) | Wire.read();

    accelXSum += ax;
    accelYSum += ay;
    accelZSum += az;

    delay(10);
  }

  float accelXAvg = (accelXSum / MEASUREMENT_COUNT) / 16384.0 * 9.81;
  float accelYAvg = (accelYSum / MEASUREMENT_COUNT) / 16384.0 * 9.81;
  float accelZAvg = (accelZSum / MEASUREMENT_COUNT) / 16384.0 * 9.81 - 10.7;

  // --- Ультразвуковой датчик ---
  digitalWrite(Tright, LOW);
  delayMicroseconds(2);
  digitalWrite(Tright, HIGH);
  delayMicroseconds(10);
  digitalWrite(Tright, LOW);
  long duration = pulseIn(Echo, HIGH);
  float distance = duration * 0.034 / 2;

  // --- Датчик влажности почвы ---
  soilMoistureValue = analogRead(A1);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if (soilmoisturepercent > 100) {
    soilmoisturepercent = 100;
  } else if (soilmoisturepercent < 0) {
    soilmoisturepercent = 0;
  }

  // --- Гибкий датчик ---
  int sensorValue = analogRead(flexSensorPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float resistance = (5.0 / voltage - 1.0) * knownResistor;

  if (resistance > thresholdResistance) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // --- HX711 ---
  float current_reading = scale.get_units();
  samples[sample_index] = current_reading;
  sample_index = (sample_index + 1) % 100;

  float sum = 0;
  for (int i = 0; i < 100; i++) {
    sum += samples[i];
  }
  float avg_weight = sum / 100.0;
  float mass_kg = avg_weight / 1000.0;
  float force_newtons = mass_kg * gravity;


  // --- Вывод данных ---

  if(soilmoisturepercent >= 40 or distance <= 11.70 ){
    Serial.println("критический уровень");
    digitalWrite(RED, HIGH);
    digitalWrite(GRE, LOW);
    LCD.setCursor(0,1);
    LCD.print("WARNING");
    tone(BUZ, 263, 300);
    delay(100);
    noTone(BUZ);
    delay(100);
    
    if(val111 == 1){
      delay(5000);
      servo.write(1400);
      servo.write(-30);
      
      delay(5000);  
    }
    else{
      noTone(BUZ);
    }
    
  }
  else{
    noTone(BUZ);
    digitalWrite(RED, LOW);
    digitalWrite(GRE, HIGH);
    LCD.clear();
    LCD.print("Most Control M-1");
    servo.write(1400);
  }
  if(distance <= 12.30){
    Serial.println("предкритический уровень");
    digitalWrite(RED, LOW);
    digitalWrite(GRE, LOW);
    digitalWrite(13, HIGH);
    LCD.setCursor(0,1);
    LCD.print("WARNING");
    tone(BUZ, 600, 300);
    delay(100);
    noTone(BUZ);
    delay(100);
  }
  else{
    noTone(BUZ);
    digitalWrite(RED, LOW);
    digitalWrite(GRE, HIGH);
    digitalWrite(13, LOW);
    LCD.clear();
    LCD.print("Most Control M-1");
  }
  
  
  if (rotat >= 950 && rotat <= 1005){
    LCD.setCursor(0,0);
    LCD.print("Most Control M-1");
  }
  if (rotat >= 800 && rotat <= 950){
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Distance:");
    LCD.print(distance);
  }
  if (rotat >= 650 && rotat <= 800){
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Accel:");
    LCD.setCursor(7,0);
    LCD.print("X:");
    LCD.print(accelXAvg);
    LCD.setCursor(0,1);
    LCD.print("Y:");
    LCD.print(accelYAvg);
    LCD.setCursor(7,1);
    LCD.print("Z:");
    LCD.print(accelZAvg);
  } 
  if(rotat >= 500 && rotat <= 650){
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Water level:");
    LCD.print(soilmoisturepercent);
    LCD.print("%");
  }
  if(rotat >= 350 && rotat <= 500){
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Flex:");
    LCD.print(resistance);
  }

  if (rotat >= 200 && rotat <= 350){
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Weight:");
    LCD.print(mass_kg);
    LCD.print("kg");
    LCD.setCursor(0,1);
    LCD.print("Force:");
    LCD.print(force_newtons);
    LCD.print("N");
  }
  if(rotat >= 0 && rotat <= 200){
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Movement:");
    LCD.print(val111);
  }
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    Serial.print("Получена команда: ");
    Serial.println(command);

    if (command == "/barrier") {
      servo.write(5);
      delay(3000);
      Serial.println("Шлагбаум опущен");
      
    }
    if (command == "/critical_light") {
      digitalWrite(RED, HIGH);
      digitalWrite(GRE, LOW);
      LCD.setCursor(0,1);
      LCD.print("WARNING");
      tone(BUZ, 263, 300);
      delay(100);
      noTone(BUZ);
      delay(100);
      delay(3000);
    }
    if (command == "/get_data") {
  // Отправляем данные поочередно
  Serial.println("Distance: " + String(distance) + " cm");
  delay(1000);  // Задержка 1 секунда

  Serial.println("Soil Moisture: " + String(soilmoisturepercent) + "%");
  delay(1000);  // Задержка 1 секунда

  Serial.println("Acceleration X: " + String(accelXAvg) + " m/s²");
  delay(1000);  // Задержка 1 секунда

  Serial.println("Acceleration Y: " + String(accelYAvg) + " m/s²");
  delay(1000);  // Задержка 1 секунда

  Serial.println("Acceleration Z: " + String(accelZAvg) + " m/s²");
  delay(1000);  // Задержка 1 секунда

  Serial.println("Flex Resistance: " + String(resistance) + " Ohms");
  delay(1000);  // Задержка 1 секунда

  Serial.println("Weight: " + String(mass_kg) + " kg");
  delay(1000);  // Задержка 1 секунда

  Serial.println("Force: " + String(force_newtons) + " N");
}
}
}