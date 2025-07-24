#include <Servo.h>

Servo servoX;
Servo servoY;

const int servoXPin = 10;
const int servoYPin = 9;
const int laserPin = 8;

String inputString = "";
boolean stringComplete = false;

void setup() {
  Serial.begin(9600);

  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
  pinMode(laserPin, OUTPUT);

  // Başlangıç pozisyonu
  servoX.write(90);
  servoY.write(90);
  digitalWrite(laserPin, LOW);

  inputString.reserve(30); // Hafızada yer ayır

  Serial.println("🔌 Arduino hazır, veri bekleniyor...");
}

void loop() {
  if (stringComplete) {
    Serial.print("📩 Gelen komut: ");
    Serial.println(inputString);  // <<< Gelen veriyi ekrana yaz

    parseAndExecute(inputString);
    inputString = "";
    stringComplete = false;
  }
}

// Seri porttan veri alma
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
      break;
    } else {
      inputString += inChar;
    }
  }
}

// Komutu ayrıştır ve uygula
void parseAndExecute(String command) {
  int xIndex = command.indexOf('X');
  int yIndex = command.indexOf('Y');
  int lIndex = command.indexOf('L');

  if (xIndex == -1 || yIndex == -1 || lIndex == -1) {
    Serial.println("⚠️ Geçersiz komut formatı!");
    return;
  }

  int xAngle = command.substring(xIndex + 1, yIndex).toInt();
  int yAngle = command.substring(yIndex + 1, lIndex).toInt();
  int laserState = command.substring(lIndex + 1).toInt();

  xAngle = constrain(xAngle, 0, 180);
  yAngle = constrain(yAngle, 0, 180);
  laserState = (laserState != 0) ? HIGH : LOW;

  // Servo ve lazerleri ayarla
  servoX.write(xAngle);
  servoY.write(yAngle);
  digitalWrite(laserPin, laserState);

  Serial.print("✅ Uygulandı: X=");
  Serial.print(xAngle);
  Serial.print(", Y=");
  Serial.print(yAngle);
  Serial.print(", L=");
  Serial.println(laserState == HIGH ? "ON" : "OFF");
}
