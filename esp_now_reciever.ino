//MUCİT PİLOT 2020
//ESP8266 ESP-NOW KULLANIMI
//Alıcı Kodu

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

//OLED ekran ayarları
#define SCREEN_WIDTH 128 // OLED display genişlik piksel
#define SCREEN_HEIGHT 32 // OLED display yükseklik piksel
#define buton D0
// I2C protokolü ile çalışan bir SSD1306 ekran nesnesi yaratıyoruz (SDA (D2), SCL (D1) pinlerine bağlanacak)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);//wire protokolü belirtir. -1 ise ekranda reset butonu olup olmadığını. -1 ise yok demek
    float t1;
    float h1;
    float t2;
    float h2;
    float t3;
    float h3;
    
int butondurum=0; //butonu takip için

// Alıncak veri yapısı için structure tanımlama
// Gönderici kartlar ile aynı yapı olmalı
typedef struct struct_message {
  int a;//gönderen kart id numarası
  float b; //sıcaklık
  float c; //nem

} struct_message;

// Bu yapıdan myData isimli bir veri oluşturduk
struct_message myData;

// VERİ ALIMI OLDUĞUNDA BU FONKSİYON ÇALIŞACAK
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Byte veri alındı: ");
  Serial.println(len);
  Serial.print("Kart No: ");
  Serial.println(myData.a);
  Serial.print("Sicaklik: ");
  Serial.println(myData.b);
  Serial.print("Nem: ");
  Serial.println(myData.c);

  Serial.println();
  if (myData.a==1)
    {  t1=myData.b;
      h1=myData.c;
    }
 else if (myData.a==2)
  {
      t2=myData.b;
      h2=myData.c;
  }
 else if (myData.a==3)
  {
      t3=myData.b;
      h3=myData.c;
  }
  else {Serial.println("Bağlı kart yok");}

}
 
void setup() {
  
  Serial.begin(115200);
  pinMode(buton, INPUT_PULLUP);
  // Alıcı cihazı WIFI STATION olarak tanımlamamız gerekiyor
  WiFi.mode(WIFI_STA);

  // ESP NOW özelliğini başlatıyoruz
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW başlatılamadı");
    return;
  }
  
  // ESPNOW başlatıldığında 
  // kartın rolünü tanımladık ve veri alındığında çağrılacak fonksiyonu ilişkilendiriyoruz.
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //Eğer ekran başlatılamazsa hata mesajı verecek
    Serial.println(F("SSD1306 ekran hatası"));
    for(;;);//sonsuza kadar döner buradan çıkmaz
  }
  delay(2000);
  display.clearDisplay();//ekranı temizliyoruz.
  display.setTextColor(WHITE);//ekran yazı rengini beyaza ayarladık


   
}

void loop() {
  if (digitalRead(buton)==HIGH)
  {
    butondurum++;
    delay(400);
    if (butondurum>3) {butondurum=0;}
  }
  switch (butondurum) {
    case 0:
      display.clearDisplay();
      display.setTextSize(2);//metin boyutu en küçük
      display.setCursor(0,0);//cursor'u pozisyona konumlandırdık
      display.print("Kart Secin");
      display.display();
    break;

    case 1:
      // sıcaklık verisinin ekrana yazdırılması
      display.clearDisplay();
      display.setTextSize(1);//metin boyutu en küçük
      display.setCursor(0,0);//cursor'u pozisyona konumlandırdık
      display.print("Sicaklik 1: ");
      display.setTextSize(1);
      display.setCursor(65,0);
      display.print(t1);
      display.print(" ");
      display.setTextSize(1);
      display.cp437(true);//yazı karakterleri dışındaki sembolleri yazdırmak için cp437 (DOS) tablosunu kullanıyoruz
      display.write(167);//derece işaretinin karşılığı
      display.setTextSize(1);
      display.print("C");
  
       // rutubeti yazdırıyoruz
      display.setTextSize(1);
      display.setCursor(0, 20);
      display.print("Rutubet 1: ");
      display.setTextSize(1);
      display.setCursor(65, 20);
      display.print(h1);
      display.print(" %"); 

      display.display(); 
    break;
      
    case 2:
      // sıcaklık verisinin ekrana yazdırılması
      display.clearDisplay();
      display.setTextSize(1);//metin boyutu en küçük
      display.setCursor(0,0);//cursor'u pozisyona konumlandırdık
      display.print("Sicaklik 2: ");
      display.setTextSize(1);
      display.setCursor(65,0);
      display.print(t2);
      display.print(" ");
      display.setTextSize(1);
      display.cp437(true);//yazı karakterleri dışındaki sembolleri yazdırmak için cp437 (DOS) tablosunu kullanıyoruz
      display.write(167);//derece işaretinin karşılığı
      display.setTextSize(1);
      display.print("C");
  
       // rutubeti yazdırıyoruz
      display.setTextSize(1);
      display.setCursor(0, 20);
      display.print("Rutubet 2: ");
      display.setTextSize(1);
      display.setCursor(65, 20);
      display.print(h2);
      display.print(" %"); 

      display.display(); 
    break;
      
    case 3:
      // sıcaklık verisinin ekrana yazdırılması
      display.clearDisplay();
      display.setTextSize(1);//metin boyutu en küçük
      display.setCursor(0,0);//cursor'u pozisyona konumlandırdık
      display.print("Sicaklik 3: ");
      display.setTextSize(1);
      display.setCursor(65,0);
      display.print(t3);
      display.print(" ");
      display.setTextSize(1);
      display.cp437(true);//yazı karakterleri dışındaki sembolleri yazdırmak için cp437 (DOS) tablosunu kullanıyoruz
      display.write(167);//derece işaretinin karşılığı
      display.setTextSize(1);
      display.print("C");
  
       // rutubeti yazdırıyoruz
      display.setTextSize(1);
      display.setCursor(0, 20);
      display.print("Rutubet 3: ");
      display.setTextSize(1);
      display.setCursor(65, 20);
      display.print(h3);
      display.print(" %"); 

      display.display(); 
    break;
  }
  
 
  
}
