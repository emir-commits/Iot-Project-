# Akıllı Sandalye - Duruş Takip Sistemi

ESP32 ile duruş bozukluğunu algılayan ve uyarı veren IoT projesi.

## Ne Yapıyor?

Sandalyeye oturduğunuzda eğilme açınızı ölçüp OLED ekranda gösteriyor. Eğer belirli bir süre kötü duruşta kalırsanız buzzer ile uyarı veriyor. WiFi üzerinden web dashboard ile canlı takip yapabiliyorsunuz.

## Donanım

- ESP32 Dev Module
- SSD1306 OLED Ekran (128x64, I2C)
- Aktif Buzzer

## Bağlantılar

```
ESP32 GPIO21 (SDA) -> OLED SDA
ESP32 GPIO22 (SCL) -> OLED SCL
ESP32 3V3          -> OLED VCC
ESP32 GND          -> OLED GND
ESP32 GPIO18       -> Buzzer (+)
ESP32 GND          -> Buzzer (-)
```

## Kutuphane Gereksinimleri

Arduino IDE Library Manager'dan yukleyin:
- Adafruit SSD1306
- Adafruit GFX Library
- ArduinoJson

## Kurulum

1. Arduino IDE'de ESP32 board desteğini ekleyin
2. Board olarak `ESP32 Dev Module` seçin
3. Koddaki WiFi bilgilerini kendi ağınıza göre değiştirin
4. Upload edin
5. Serial Monitor'den IP adresini öğrenin
6. `dashboard.html` dosyasını tarayıcıda açıp IP'yi girin

## Dashboard

Tarayıcıdan ESP32'ye bağlanarak canlı veri izleyebilirsiniz:
- Pitch/Roll açı göstergeleri
- Oturma süresi
- Duruş durumu (iyi/kötü)
- Buzzer kontrolü
- Hassasiyet ayarı
