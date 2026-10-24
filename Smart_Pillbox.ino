#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_SSD1306.h>

RTC_DS3231 rtc;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

#define BUZZER 23
#define SOS_BTN 19
#define LED_TUE 12
#define SWITCH_TUE 32

int gentleMelody[] = {262, 330, 392, 523};
int missed = 0;

void setup(){
  Serial.begin(115200);
  rtc.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(BUZZER, OUTPUT);
  pinMode(SOS_BTN, INPUT_PULLUP);
  pinMode(SWITCH_TUE, INPUT_PULLUP);
  pinMode(LED_TUE, OUTPUT);
}

void loop(){
  DateTime now = rtc.now();
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.printf("%02d:%02d", now.hour(), now.minute());
  display.setTextSize(1);
  display.setCursor(0,25);
  display.print("NEXT: TUE 8AM");
  display.setCursor(0,40);
  display.print("MISSED: " + String(missed));
  display.display();

  if(now.hour()==8 && now.minute()==0){
    if(digitalRead(SWITCH_TUE)==HIGH){
      digitalWrite(LED_TUE, HIGH);
      playGentleAlarm();
      missed++;
    }
  }
  if(digitalRead(SOS_BTN)==LOW){
    tone(BUZZER, 1000, 2000);
  }
  delay(1000);
}

void playGentleAlarm(){
  for(int i=0; i<4; i++){
    tone(BUZZER, gentleMelody[i], 300);
    delay(400);
    if(digitalRead(SWITCH_TUE)==LOW) break;
  }
  noTone(BUZZER);
}