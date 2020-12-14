//MUCİT PİLOT 2020
//ESP8266 ESP-NOW KULLANIMI
//Verici Kodu

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DHT.h>
// Alıcının MAC Adresini buraya yazalım
uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0xDA, 0xF0, 0x41}; 

// gönderilecek veriler için bir veri yapısı tanımlamamız lazım
// Bu veri yapısı alıcı tarafında da aynı olmalı
typedef struct struct_message {
  int a; //kart numarası
  float b; //sıcaklık
  float c; //nem

} struct_message;

// tanımladığımız veri yapısı türünde myData isimli bir veri oluşturduk
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // veriler ne kadar sürede bir gönderilecek

// Veri gönderme yapıldığında bu fonk çalışacak
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Son mesaj paketinin gönderim durumu: ");
  if (sendStatus == 0){
    Serial.println("Gönderme Başarılı");
  }
  else{
    Serial.println("Gönderme Hatası");
  }
}


 #define DHTPIN D5     // DHT sensör D5 pinine bağlı

// DHT sensör seçeneği için comment kaldırın
  #define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);//dht nesnesi oluşturduk.


void setup() {

  Serial.begin(115200);
 dht.begin();//dht yi başlatıyoruz
  // cihazı WIFI STATION olarak çalıştırmamız gerekiyor
  WiFi.mode(WIFI_STA);

  // ESP-NOW özelliğini başlatıyoruz
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Başlatılamadı!!!");
    return;
  }

  // Kartın rolünü tanımlıyoruz
  // send cb fonksiyonu ile de veri gittiğinde onDataSent fonksiyonunun çağrılmasını sağlıyoruz.
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // yukarıdaki mac adresli alıcıyı eşleştiriyoruz.
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // belirlediğimiz aralıklarla gönderme yapmak için
     

  //sıcaklık ve rutubet bilgilerini okuyoruz.
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(h) || isnan(t)) { //veriler alınamadıysa hata mesajı verdiriyoruz
    Serial.println("Veriler DHT Sensörden okunamadı!!!");
  }
  Serial.println(t);
  Serial.println(h);

  
    myData.a = 3;//her kart için farklı bir numara tanımlarsak gönderenleri ayırt edebiliriz.
    myData.b = t;
    myData.c = h;


    // mesajımızı ESP NOW ile gönderiyoruz.
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();//zaman sayacını güncellemek için
  }
}
