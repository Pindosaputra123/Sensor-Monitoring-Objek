#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define PIR_PIN A0
#define TRIG_PIN A1
#define ECHO_PIN A2
#define LED_hijau 11
#define LED_kuning 12
#define LED_merah 13

void setup() {
    pinMode(PIR_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_hijau, OUTPUT);
    pinMode(LED_kuning, OUTPUT);
    pinMode(LED_merah, OUTPUT);

    Serial.begin(9600);
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.clearDisplay();
    display.display();
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

void loop() {
    bool pirStatus = digitalRead(PIR_PIN);
    long distance = getDistance();

    // Kontrol LED berdasarkan jarak
    digitalWrite(LED_hijau, distance < 50);
    digitalWrite(LED_kuning, distance >= 50 && distance <= 125);
    digitalWrite(LED_merah, distance > 125);

    // Tampilkan hasil ke OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("PIR: ");
    display.println(pirStatus ? "Terdeteksi" : "Tidak Terdeteksi");
    display.print("Jarak: ");
    display.print(distance);
    display.println(" cm");

    if (distance < 50) {
        display.println("Kondisi: DEKAT");
    } else if (distance >= 50 && distance <= 125) {
        display.println("Kondisi: SEDANG");
    } else {
        display.println("Kondisi: JAUH");
    }
    
    display.display();
    delay(500);
}