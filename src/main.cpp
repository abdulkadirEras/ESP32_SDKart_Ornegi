#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"


void sistem_init();
void liste_dizini(fs::FS &fs, const char * dizAdi, uint8_t seviyeler);
void dizi_olustur(fs::FS &fs, const char * yol);
void dizini_sil(fs::FS &fs, const char * yol);
void dosya_oku(fs::FS &fs, const char * yol);
void dosyaya_yaz(fs::FS &fs, const char * yol, const char * yazilacakVeri);
void dosya_ekle(fs::FS &fs, const char * yol, const char * dosyaya_eklenecekVeri);
void dosyayi_yeniden_adlandir(fs::FS &fs, const char * yol1, const char * yol2);
void dosyayi_sil(fs::FS &fs, const char * yol);
void dosya_test_IO(fs::FS &fs, const char * yol);


void setup()
{
  
  liste_dizini(SD, "/", 0);
  dizi_olustur(SD, "/mydir");
  liste_dizini(SD, "/", 0);
  dizini_sil(SD, "/mydir");
  liste_dizini(SD, "/", 2);
  dosyaya_yaz(SD, "/hello.txt", "Hello ");
  dosya_ekle(SD, "/hello.txt", "World!\n");
  dosya_oku(SD, "/hello.txt");
  dosyayi_sil(SD, "/foo.txt");
  dosyayi_yeniden_adlandir(SD, "/hello.txt", "/foo.txt");
  dosya_oku(SD, "/foo.txt");
  dosya_test_IO(SD, "/test.txt");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop()
{

}


void sistem_init()
{
  Serial.begin(115200);
  if(!SD.begin(5))
  {
    Serial.println("SD kart init edilemedi!");
    return;
  }
  uint8_t kartTuru = SD.cardType();
  
  
  if(kartTuru == CARD_NONE)
  {
    Serial.println("SD kart bulunamadi!");
    return;
  }

  Serial.print("SD kart turu: ");
  if(kartTuru == CARD_MMC)
    Serial.println("MMC");
  else if(kartTuru == CARD_SD)
    Serial.println("SDSC");
  else if(kartTuru == CARD_SDHC)
    Serial.println("SDHC");
  else 
    Serial.println("UNKNOWN");
  

  uint64_t kartBoyutu = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Kart Boyutu: %lluMB\n", kartBoyutu);
}


void liste_dizini(fs::FS &fs, const char * dizAdi, uint8_t seviyeler)
{
  Serial.printf("Liste Dizini: %s\n", dizAdi);

  File root = fs.open(dizAdi);
  if(!root){
    Serial.println("Dizin acilamadi");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Bir dizin değil");
    return;
  }

  File entry = root.openNextFile();
  while(entry)
  {
    if(entry.isDirectory())
    {
      Serial.print("  DIZIN : ");
      Serial.println(entry.name());
      if(seviyeler)
        liste_dizini(fs, entry.name(), seviyeler -1);
      
    } 
    else 
    {
      Serial.print("  DOSYA: ");
      Serial.print(entry.name());
      Serial.print("  BOYUT: ");
      Serial.println(entry.size());
    }
    entry = root.openNextFile();
  }
}

void dizi_olustur(fs::FS &fs, const char * yol)
{
  Serial.printf("Dizin olusturuluyor: %s\n", yol);
  if(fs.mkdir(yol))
  {
    Serial.println("Dizin olusturuldu");
  } else 
  {
    Serial.println("mkdir komutu basarisiz");
  }
}

void dizini_sil(fs::FS &fs, const char * yol)
{
  Serial.printf("Dizin siliniyor: %s\n", yol);
  if(fs.rmdir(yol))
  {
    Serial.println("Dizin silindi");
  } else 
  {
    Serial.println("rmdir komutu basarisiz");
  }
}

void dosya_oku(fs::FS &fs, const char * yol)
{
  Serial.printf("Dosya okunuyor: %s\n", yol);

  fs::File dosya = fs.open(yol);
  if(!dosya){
    Serial.println("Okunacak dosya acilamadi");
    return;
  }

  Serial.print("Dosyadan okunanlar: ");
  while(dosya.available())
  {
    Serial.write(dosya.read());
  }
  dosya.close();
}

void dosyaya_yaz(fs::FS &fs, const char * yol, const char * yazilacakVeri)
{
  Serial.printf("Yazilacak dosya: %s\n", yol);

  fs::File dosya = fs.open(yol, FILE_WRITE);
  if(!dosya)
  {
    Serial.println("Yazmak için dosya acilamadi");
    return;
  }
  if(dosya.print(yazilacakVeri))
  {
    Serial.println("Dosya yazildi");
  } else {
    Serial.println("yazma basarisiz");
  }
  dosya.close();
}

void dosya_ekle(fs::FS &fs, const char * yol, const char * dosyayiEkle)
{
  Serial.printf("Dosya ekleniyor: %s\n", yol);

  fs::File dosya = fs.open(yol, FILE_APPEND);
  if(!dosya)
  {
    Serial.println("Eklemek icin dosya acilamadi");
    return;
  }
  if(dosya.print(dosyayiEkle))
  {
      Serial.println("veri eklendi");
  } else {
    Serial.println("ekleme basarisiz");
  }
  dosya.close();
}

void dosyayi_yeniden_adlandir(fs::FS &fs, const char * yol1, const char * yol2)
{
  Serial.printf("%S'yi %s'ye yeniden adlandiriliyor: ", yol1, yol2);
  if (fs.rename(yol1, yol2)) 
  {
    Serial.println("Dosya yeniden adlandirildi");
  } else 
  {
    Serial.println("dosya yeniden adlandirma basarisiz");
  }
}

void dosyayi_sil(fs::FS &fs, const char * yol)
{
  Serial.printf("dosya siliniyor: %s\n", yol);
  if(fs.remove(yol))
  {
    Serial.println("dosya silindi");
  } else 
  {
    Serial.println("silme basarisiz");
  }
}

void dosya_test_IO(fs::FS &fs, const char * yol)
{
  fs::File dosya = fs.open(yol);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(dosya){
    len = dosya.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      dosya.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    dosya.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  dosya = fs.open(yol, FILE_WRITE);
  if(!dosya){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    dosya.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  dosya.close();
}