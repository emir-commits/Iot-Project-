// Akilli Sandalye - ESP32 IoT Projesi
// SSD1306 OLED + WiFi + Buzzer

#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
#include <math.h>

const char* WIFI_SSID = "WIFI_ADINIZ";
const char* WIFI_PASS = "WIFI_SIFRENIZ";

#define BUZZER_PIN        18
#define SCREEN_WIDTH     128
#define SCREEN_HEIGHT     64
#define OLED_RESET        -1
#define OLED_I2C_ADDR  0x3C

float sapmaEsikDerece = 15.0;
#define KOTU_DURUS_SURE_MS  2000
#define KALIBRASYON_SURE_MS 5000
#define SENSOR_OKUMA_MS      100
#define BUZZER_BEEP_MS       200
#define BUZZER_PAUSE_MS      300

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer server(80);

enum SistemDurumu { BEKLEME, KALIBRASYON, TAKIP };
SistemDurumu durum = BEKLEME;

float refPitch = 0.0, refRoll = 0.0;
float pitch = 0.0, roll = 0.0;
float kalTopPitch = 0.0, kalTopRoll = 0.0;
int kalSayac = 0;

unsigned long kalibrasyonBaslangic = 0;
unsigned long oturmaBaslangic = 0;
unsigned long kotuDurusBaslangic = 0;
unsigned long sonSensorOkuma = 0;
unsigned long sonBuzzerToggle = 0;

bool kotuDurus = false;
bool buzzerAktif = false;
bool buzzerDurumu = false;
bool wifiConnected = false;
bool buzzerMuted = false;
bool remoteRecalib = false;
bool simOturuyor = true;

void sistemBaslat();
void wifiBaslat();
void setupWebServer();
void handleData();
void handleControl();
void handleCORS();
void oledBeklemeEkrani();
void oledKalibrasyonEkrani(int kalan);
void oledTakipEkrani(float p, float r, bool iyi, unsigned long sure);
void buzzerKontrol();

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Sistem baslatiliyor...");

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Wire.begin(21, 22);
  sistemBaslat();
  wifiBaslat();
  setupWebServer();

  Serial.println("Sistem hazir");
}

void loop() {
  server.handleClient();

  unsigned long simdi = millis();
  bool oturuyor = simOturuyor;

  switch (durum) {

    case BEKLEME:
      if (oturuyor) {
        durum = KALIBRASYON;
        kalibrasyonBaslangic = simdi;
        kalTopPitch = 0.0; kalTopRoll = 0.0; kalSayac = 0;
        pitch = 0.0; roll = 0.0;
      } else {
        oledBeklemeEkrani();
        delay(200);
      }
      break;

    case KALIBRASYON:
      if (!oturuyor) {
        durum = BEKLEME;
        break;
      }
      if (simdi - sonSensorOkuma >= SENSOR_OKUMA_MS) {
        sonSensorOkuma = simdi;
        pitch = 0.0; roll = 0.0;
        kalTopPitch += pitch; kalTopRoll += roll; kalSayac++;
      }
      {
        int kalan = (KALIBRASYON_SURE_MS - (simdi - kalibrasyonBaslangic)) / 1000;
        if (kalan < 0) kalan = 0;
        oledKalibrasyonEkrani(kalan);
      }
      if (simdi - kalibrasyonBaslangic >= KALIBRASYON_SURE_MS) {
        if (kalSayac > 0) {
          refPitch = kalTopPitch / kalSayac;
          refRoll = kalTopRoll / kalSayac;
        }
        oturmaBaslangic = simdi;
        kotuDurusBaslangic = 0;
        kotuDurus = false; buzzerAktif = false;
        durum = TAKIP;
      }
      break;

    case TAKIP:
      if (!oturuyor) {
        durum = BEKLEME;
        buzzerAktif = false;
        digitalWrite(BUZZER_PIN, LOW);
        break;
      }
      if (simdi - sonSensorOkuma >= SENSOR_OKUMA_MS) {
        sonSensorOkuma = simdi;

        float t = simdi / 1000.0;
        pitch = sin(t * 0.5) * 20.0;
        roll = cos(t * 0.3) * 10.0;

        float sapmaPitch = fabs(pitch - refPitch);
        float sapmaRoll = fabs(roll - refRoll);
        bool sapmaVar = (sapmaPitch > sapmaEsikDerece) || (sapmaRoll > sapmaEsikDerece);

        if (sapmaVar) {
          if (kotuDurusBaslangic == 0) kotuDurusBaslangic = simdi;
          if (simdi - kotuDurusBaslangic >= KOTU_DURUS_SURE_MS) {
            kotuDurus = true;
            buzzerAktif = !buzzerMuted;
          }
        } else {
          kotuDurusBaslangic = 0;
          kotuDurus = false; buzzerAktif = false;
          digitalWrite(BUZZER_PIN, LOW);
        }

        if (remoteRecalib) {
          remoteRecalib = false;
          durum = KALIBRASYON;
          kalibrasyonBaslangic = simdi;
          kalTopPitch = 0.0; kalTopRoll = 0.0; kalSayac = 0;
          break;
        }

        unsigned long oturmaSuresi = simdi - oturmaBaslangic;
        oledTakipEkrani(pitch, roll, !kotuDurus, oturmaSuresi);
      }
      buzzerKontrol();
      break;
  }
}

void wifiBaslat() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("WiFi Baglaniyor..."));
  display.println(WIFI_SSID);
  display.display();

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 30) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println();
    Serial.println(WiFi.localIP());

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("WiFi Baglandi!"));
    display.setCursor(0, 16);
    display.println(F("IP Adresi:"));
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println(WiFi.localIP());
    display.setCursor(0, 50);
    display.println(F("Dashboard hazir!"));
    display.display();
    delay(3000);
  } else {
    wifiConnected = false;
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println(F("WiFi Baglanamiyor"));
    display.println(F("Offline mod..."));
    display.display();
    delay(2000);
  }
}

void setupWebServer() {
  if (!wifiConnected) return;

  server.on("/data", HTTP_OPTIONS, handleCORS);
  server.on("/control", HTTP_OPTIONS, handleCORS);
  server.on("/data", HTTP_GET, handleData);
  server.on("/control", HTTP_GET, handleControl);

  server.begin();
}

void handleCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  server.sendHeader("Access-Control-Allow-Private-Network", "true");
  server.send(204);
}

void handleData() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Private-Network", "true");

  JsonDocument doc;
  doc["pitch"] = round(pitch * 10.0) / 10.0;
  doc["roll"] = round(roll * 10.0) / 10.0;
  doc["refPitch"] = round(refPitch * 10.0) / 10.0;
  doc["refRoll"] = round(refRoll * 10.0) / 10.0;
  doc["sitting"] = simOturuyor;
  doc["bad"] = kotuDurus;
  doc["buzzer"] = buzzerAktif;
  doc["mode"] = (int)durum;

  if (durum == TAKIP) {
    doc["sittingMs"] = millis() - oturmaBaslangic;
  } else {
    doc["sittingMs"] = 0;
  }

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void handleControl() {
  server.sendHeader("Access-Control-Allow-Origin", "*");

  if (server.hasArg("buzzer")) {
    buzzerMuted = (server.arg("buzzer") == "0");
    if (buzzerMuted) {
      buzzerAktif = false;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  if (server.hasArg("recalibrate")) {
    remoteRecalib = true;
  }

  if (server.hasArg("threshold")) {
    float newThresh = server.arg("threshold").toFloat();
    if (newThresh >= 5 && newThresh <= 30) {
      sapmaEsikDerece = newThresh;
    }
  }

  server.send(200, "application/json", "{\"ok\":true}");
}

void sistemBaslat() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("OLED bulunamadi!");
    while (true) { delay(1000); }
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println(F("Sistem Basliyor..."));
  display.display();

  display.clearDisplay();
  display.setCursor(15, 5);
  display.println(F("Akilli Sandalye"));
  display.setCursor(5, 20);
  display.println(F("Durus Takip Sistemi"));
  display.drawLine(0, 35, 128, 35, SSD1306_WHITE);
  display.setCursor(10, 42);
  display.println(F("v1.0 WiFi+IoT"));
  display.display();
  delay(2000);
}

void buzzerKontrol() {
  if (!buzzerAktif) return;
  unsigned long simdi = millis();
  if (simdi - sonBuzzerToggle >= (buzzerDurumu ? BUZZER_BEEP_MS : BUZZER_PAUSE_MS)) {
    buzzerDurumu = !buzzerDurumu;
    digitalWrite(BUZZER_PIN, buzzerDurumu ? HIGH : LOW);
    sonBuzzerToggle = simdi;
  }
}

void oledBeklemeEkrani() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(5, 0);
  display.println(F("-- UYKU MODU --"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.setCursor(30, 22);
  display.println(F("Oturum"));
  display.setCursor(22, 35);
  display.println(F("Bekleniyor..."));
  if (wifiConnected) {
    display.setCursor(0, 52);
    display.print(F("IP:"));
    display.print(WiFi.localIP());
  }
  display.display();
}

void oledKalibrasyonEkrani(int kalan) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(5, 0);
  display.println(F("-- KALIBRASYON --"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.setCursor(5, 18);
  display.println(F("Dik oturun ve"));
  display.setCursor(5, 30);
  display.println(F("hareket etmeyin!"));
  display.setTextSize(2);
  display.setCursor(55, 45);
  display.print(kalan);
  display.setTextSize(1);
  display.print(F(" sn"));
  display.display();
}

void oledTakipEkrani(float p, float r, bool iyi, unsigned long sure) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("Durus: "));
  display.println(iyi ? F("[  IYI  ]") : F("[ KOTU! ]"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setCursor(0, 14);
  display.print(F("P:")); display.print(p, 1); display.print(F("  "));
  display.print(F("R:")); display.println(r, 1);

  display.drawLine(0, 26, 128, 26, SSD1306_WHITE);
  display.setCursor(0, 29);
  display.print(F("Sapma:"));
  float sapma = max(fabs(p - refPitch), fabs(r - refRoll));
  int barLen = constrain((int)(sapma * 2), 0, 60);
  display.drawRect(45, 29, 62, 8, SSD1306_WHITE);
  display.fillRect(46, 30, barLen, 6, SSD1306_WHITE);

  unsigned long sn = sure / 1000;
  display.setCursor(0, 42);
  display.print(F("Sure: "));
  display.print(sn / 60); display.print(F(":"));
  if (sn % 60 < 10) display.print(F("0"));
  display.print(sn % 60);

  if (wifiConnected) {
    display.setCursor(0, 55);
    display.print(WiFi.localIP());
  }

  if (!iyi) {
    display.setCursor(110, 42);
    display.print(F("(!)"));
  }
  display.display();
}
