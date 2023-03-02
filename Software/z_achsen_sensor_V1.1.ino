#include "HX711.h"

#define HX711_DT_PIN 2
#define HX711_SCK_PIN 3
#define SIGNAL_PIN 4
#define SIGNAL_LED 17
#define UEBERLAST 400 
#define EN_serial

long aktkraft = 0; //latest reading
long letztkraft = 0; //latest-1 reading
long vorletztkraft = 0; //latest-2 reading
long de = 0;
long threshold_Val = 5;
//long const threshold_gap = threshold_Val;
int const threshold_De = 6;

HX711 scale;

void setup()
{
  #ifdef EN_serial
    Serial.begin(115200);
  #endif
  pinMode(SIGNAL_PIN, OUTPUT);
  pinMode(SIGNAL_LED, OUTPUT);
  pinMode(HX711_SCK_PIN, OUTPUT);
  digitalWrite(HX711_SCK_PIN, LOW);
  delay(1000);
  digitalWrite(SIGNAL_PIN, LOW);
  digitalWrite(SIGNAL_LED, LOW);
  scale.begin(HX711_DT_PIN, HX711_SCK_PIN);
  if (!scale.wait_ready_retry(10))
  {
    while (1)
    {
     #ifdef EN_serial 
      Serial.println("HX711 nicht bereit: ");
     #endif;
      digitalWrite(SIGNAL_LED, HIGH);
      delay(50);
      digitalWrite(SIGNAL_LED, LOW);
      delay(50);
    }
  }

  scale.set_scale(-570.f); 
  delay(3000);            // pre-warm up strain gauge, reduce drifting
  scale.tare();           // reset the scale to 0
  digitalWrite(SIGNAL_LED, HIGH);
  delay(500);
  digitalWrite(SIGNAL_LED, LOW);
  delay(200);
  for (int j = 0; j <= 5; j++)
  {
    digitalWrite(SIGNAL_LED, HIGH);
    delay(50);
    digitalWrite(SIGNAL_LED, LOW);
    delay(50);
  }
}

void loop()
{
  vorletztkraft = letztkraft;
  letztkraft = aktkraft;
  aktkraft = scale.get_units(1);
  de = abs(aktkraft + vorletztkraft - (2 * letztkraft));
  if (de >= threshold_De)
  {
    digitalWrite(SIGNAL_PIN, HIGH);
    digitalWrite(SIGNAL_LED, HIGH);
    delay(200);
    digitalWrite(SIGNAL_PIN, LOW);
    digitalWrite(SIGNAL_LED, LOW);
    #ifdef EN_serial 
     Serial.println("Berührung erkannt ");
    #endif;
  }
  else
  {
    digitalWrite(SIGNAL_PIN, LOW);
    digitalWrite(SIGNAL_LED, LOW);
  }

#ifdef EN_serial
  Serial.print("reading: ");
  Serial.println(scale.read());
  Serial.print("Meßwert: ");
  Serial.println(aktkraft);
  Serial.print("De: ");
  Serial.println(de);
#endif
}
