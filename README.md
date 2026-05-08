# Akilli Sandalye - Durus Takip Sistemi (IoT)

Uzun sure masa basinda calisan kisilerin durus bozuklugunu algilayip uyari veren bir IoT projesidir. ESP32 mikrodenetleyici kullanilmistir. Sandalyeye oturuldugunda sistem kalibrasyona gecer, ardindan oturma acisini surekli olcer. Belirlenen esik degerinin ustunde bir sapma olursa buzzer ile sesli uyari verir.

Projede ThingSpeak, Blynk gibi cloud tabanli bir IoT platformu kullanilmamistir. Bunun yerine ESP32 uzerinde dogrudan calisan yerel bir HTTP web sunucusu tercih edilmistir. Bu sayede herhangi bir dis servise veya internet baglantisina gerek kalmadan, ayni WiFi agi uzerinden tarayici ile cihaza baglanilip canli veri takibi ve uzaktan kontrol yapilabilir.

## Kullanilan Donanimlar

### Sensorler
| Sensor | Model | Gorevi |
|--------|-------|--------|
| IMU (Ivmeolcer + Jiroskop) | MPU6050 | Sandalyedeki egilme acisini (pitch/roll) olcmek |
| Limit Switch | Micro Switch | Kullanicinin sandalyeye oturup oturmadigini algilamak |

### Aktuatorler
| Aktuator | Model | Gorevi |
|----------|-------|--------|
| Buzzer | Aktif Buzzer (5V) | Kotu durus tespit edildiginde sesli uyari vermek |
| OLED Ekran | SSD1306 (128x64, I2C) | Aci degerleri, durus durumu ve oturma suresini gostermek |

### Diger Bilesenler
| Bilesen | Aciklama |
|---------|----------|
| ESP32 Dev Module | Ana mikrodenetleyici, WiFi ozelligi var |
| Breadboard + Jumper kablolar | Devre baglantilarini kurmak icin |

## Devre Semasi

```
                          +------------------+
                          |    ESP32 DevKit   |
                          |                  |
    MPU6050               |                  |          OLED SSD1306
    +--------+            |                  |          +--------+
    | VCC    |----3V3-----|  3V3        3V3  |----VCC---| VCC    |
    | GND    |----GND-----|  GND        GND  |----GND---| GND    |
    | SDA    |----GPIO21--|  GPIO21  GPIO21  |----SDA---| SDA    |
    | SCL    |----GPIO22--|  GPIO22  GPIO22  |----SCL---| SCL    |
    +--------+            |                  |          +--------+
                          |                  |
    Limit Switch          |                  |          Buzzer
    +--------+            |                  |          +--------+
    | COM    |----GPIO27--|  GPIO27   GPIO18 |-----(+)--| (+)    |
    | NC     |----GND-----|  GND        GND  |-----(-)--| (-)    |
    +--------+            |                  |          +--------+
                          +------------------+
```

## Pin Baglantilari

| ESP32 Pin | Baglanan Bilesen | Aciklama |
|-----------|-----------------|----------|
| GPIO21 (SDA) | MPU6050 SDA + OLED SDA | I2C veri hatti (ortak bus) |
| GPIO22 (SCL) | MPU6050 SCL + OLED SCL | I2C clock hatti (ortak bus) |
| GPIO27 | Limit Switch (COM) | Oturma algilama (INPUT_PULLUP) |
| GPIO18 | Buzzer (+) | Sesli uyari cikisi |
| 3V3 | MPU6050 VCC + OLED VCC | Guc beslemesi |
| GND | Tum GND baglantilari | Ortak toprak |

> MPU6050 ve OLED ayni I2C hattini paylasiyor. MPU6050 adresi 0x68, OLED adresi 0x3C.

## Calisma Mantigi

Sistem uc modda calisiyor:

1. **Bekleme:** Limit switch acik, kimse oturmuyor. OLED ekranda "Oturum Bekleniyor" yazar.
2. **Kalibrasyon:** Kullanici oturunca sistem 5 saniye boyunca referans acisini olcer. Bu sirada dik oturmak gerekiyor.
3. **Takip:** Kalibrasyon bittikten sonra pitch ve roll acilari surekli olculur. Eger referans acidan 20 dereceden fazla sapma olursa ve bu 2 saniyeden uzun surerse buzzer calmaya baslar.

Aci hesaplamasi icin complementary filter kullaniliyor. Ivmeolcer ve jiroskop verilerini birlestirerek titresime karsi daha kararlı sonuc veriyor (alpha = 0.85).

## Web Sunucusu ve Dashboard

Cloud tabanli bir platform (ThingSpeak, Blynk, AWS IoT vb.) kullanmak yerine ESP32 uzerinde dogrudan HTTP web sunucusu calistirdik. ESP32 WiFi agina baglaninca kendi IP adresi uzerinden istemcilere hizmet veriyor. Veriler disari cikmadan yerel agda kaliyor.

Baglanti kuruldugunda OLED ekranda IP adresi gozukuyor. Tarayiciya bu IP yazilinca dashboard aciliyor.

### API Endpointleri

| Endpoint | Metod | Ne Yapiyor |
|----------|-------|------------|
| `/` | GET | Dashboard HTML sayfasini dondurur |
| `/data` | GET | Anlik sensor verilerini JSON olarak dondurur |
| `/control` | GET | Buzzer, kalibrasyon, esik degeri gibi kontrolleri alir |

### `/data` Yanit Ornegi
```json
{
  "pitch": 5.2,
  "roll": -3.1,
  "refPitch": 2.0,
  "refRoll": -1.5,
  "sitting": true,
  "bad": false,
  "buzzer": false,
  "mode": 2,
  "sittingMs": 45000
}
```

### `/control` Parametreleri
| Parametre | Deger | Aciklama |
|-----------|-------|----------|
| `buzzer` | 0 / 1 | Buzzeri sustur veya ac |
| `recalibrate` | 1 | Yeniden kalibrasyon baslat |
| `threshold` | 5-30 | Sapma esik degerini ayarla (derece) |
| `manual_buzzer` | 0 / 1 | Buzzeri manuel calistir |
| `force_sit` | 0 / 1 | Limit switch olmadan oturma simulasyonu |

### Dashboard Ozellikleri
Dashboard tarayicidan acilip ESP32 IP adresine baglanir. Sayfada sunlar var:
- Pitch ve Roll aci gostergeleri
- Oturma suresi sayaci
- Durus durumu (Iyi / Kotu)
- Buzzer acma/kapama butonu
- Hassasiyet ayari (slider ile)
- Yeniden kalibrasyon butonu
- Canli aci grafigi

## Kutuphane Gereksinimleri

Arduino IDE Library Manager uzerinden yuklenecek kutuphaneler:

| Kutuphane | Ne Icin Kullaniliyor |
|-----------|---------------------|
| Adafruit SSD1306 | OLED ekrani surmek |
| Adafruit GFX Library | Grafik fonksiyonlari (SSD1306 icin gerekli) |
| ArduinoJson | JSON formati olusturmak |
| WiFi (dahili) | ESP32 WiFi baglantisi |
| WebServer (dahili) | HTTP sunucusu |
| Wire (dahili) | I2C haberlesme |

## Kurulum

1. Arduino IDE'de ESP32 board destegini ekleyin (Boards Manager > esp32 by Espressif)
2. Board olarak `ESP32 Dev Module` secin
3. Kutuphaneleri Library Manager'dan yukleyin
4. `akilli_sandalye.ino` dosyasini acin, WiFi bilgileri kodda su sekilde tanimli:
   ```cpp
   const char* WIFI_SSID = "Emir";
   const char* WIFI_PASS = "12345678";
   ```
   Farkli bir ag kullanacaksaniz buralari degistirin.
5. Devreyi kurun ve ESP32'yi USB ile bilgisayara baglayin
6. Kodu upload edin
7. Serial Monitor'u acin (115200 baud), IP adresini ogrenen
8. Tarayicidan `http://<IP_ADRESI>` yazarak dashboard'a ulasin

## Dosya Yapisi

```
Iot-Project-/
├── akilli_sandalye.ino   # Ana Arduino kodu (sensor okuma, buzzer, web sunucusu)
├── dashboard_html.h      # Dashboard HTML'i (ESP32 PROGMEM icine gomulu)
├── dashboard.html        # Dashboard (tarayicida harici acmak icin)
└── README.md
```
