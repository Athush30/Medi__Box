#include <iostream>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <time.h>
#include <DHTesp.h>

#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 0
#define UTC_OFFSET_DST 0

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDERSS 0x3c

#define BUZZER 18
#define LED_1 15
#define LED_2 2
#define CANCEL 34
#define UP 35
#define DOWN 32
#define OK 33
#define DHT 12

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

int n_notes=8;
int c=262;
int d=294;
int e=330;
int f=349;
int g=392;
int a=440;
int b=494;
int c_h =523;
int notes[]={c,d,e,f,g,a,b,c_h};

int days=0;
int hours =0;
int minutes =0;
int seconds =0;

bool alarm_enabled= false;
int n_alarms =2;
int alarm_hours[] = {0,0};
int alarm_minutes[] = {1,10};
int alarm_triggered[] = {false,false};

unsigned long timeNow=0;
unsigned long timeLast=0;

int current_mode =0;
int max_modes = 4;
String options[]={"1 - Set Time", "2 - Set Alarm 1", "3 - Set Alarm 2", "4-Remove Alarm"};

void print_line(String text, int text_size, int row, int column){
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column,row);
  display.println(text);

  display.display();
}

void update_time(void){
  struct tm timeinfor;
  getLocalTime(&timeinfor);

  char day_str[8];
  char hour_str[8];
  char min_str[8];
  char sec_str[8];
  strftime(day_str,8, "%d",&timeinfor);
  strftime(sec_str,8, "%S",&timeinfor);
  strftime(hour_str,8, "%H",&timeinfor);
  strftime(min_str,8, "%M",&timeinfor);
  hours = atoi(hour_str);
  minutes = atoi(min_str);
  days = atoi(day_str);  
  seconds = atoi(sec_str);
}

void print_time_now(void){
  print_line(String(days),2,0,0);
  print_line(":",2,0,30);
  print_line(String(hours),2,0,30);
  print_line(":",2,0,30);
  print_line(String(minutes),2,0,60);
  print_line(":",2,0,30);
  print_line(String(seconds),2,0,90);
}

void ring_alarm(){
  display.clearDisplay();
  print_line("Medicine Time",2,0,0);
  
  digitalWrite(LED_1, HIGH);

  while (digitalRead(CANCEL)==HIGH){
    for (int i=0; i< n_notes; i++){
      if (digitalRead(CANCEL)==LOW){
        delay(200);
        break;
      }
    }
  }
}

void update_time_with_check_alarm(){
  display.clearDisplay();
  update_time();
  print_time_now();
  
  if (alarm_enabled){
    for (int i=0; i<n_alarms; i++){
      if (alarm_triggered[i] ==false && hours == alarm_hours[i] && minutes==alarm_minutes[i]){
        ring_alarm();
        alarm_triggered[i] = true;
      }
    }
  }
}

const char* wait_for_button_press(){
  while (true){
    if (digitalRead(UP) == 0){
      delay(200);
      print_line("10",2,0,0);
      delay(200);
      return "UP";
    }
    else if (digitalRead(DOWN) == 0){
      delay(200);
      print_line("20",2,0,0);
      delay(200);
      return "DOWN";
    }
    else if (digitalRead(CANCEL) == 0){
      delay(200);
      print_line("30",2,0,0);
      delay(200);
      return "CANCEL";
    }
    else if (digitalRead(OK) == 0){
      delay(200);
      print_line("40",2,0,0);
      delay(200);
      return "OK";
    }
    update_time();
  }
}

void set_time(){
  int temp_hour =hours;
  while (true){
    display.clearDisplay();
    print_line("Enter hour:" + String(temp_hour),0,0,2);

    String pressed = wait_for_button_press();
    if (pressed == "UP"){
      delay(200);
      temp_hour +=1;
      temp_hour= temp_hour % 24;
    }
    else if (pressed == "DOWN"){
        delay(200);
        temp_hour -= 1;
        if (temp_hour<0){
           temp_hour=23;
        }
    }
    else if (pressed == "OK"){
        delay(200);
        hours = temp_hour;
        break;
    }
    else if (pressed == "CANCEL"){
        delay(200);
        break;
    }
  }
  int temp_minute = minutes;
  while (true){
    display.clearDisplay();
    print_line("Enter minute:" + String(temp_minute), 0, 0,2);

    String pressed = wait_for_button_press();
    if (pressed == "UP"){
      delay(200);
      temp_minute +=1;
      temp_minute = temp_minute % 60;
    }
    else if (pressed == "DOWN"){
      delay(200);
      temp_minute -= 1;
      if (temp_minute<0){
           temp_minute=59;
      }
    }
    else if (pressed == "OK"){
      delay(200);
      minutes = temp_minute;
      break;
    }
    else if (pressed == "CANCEL"){
      delay(200);
      break;
    }
  }
  display.clearDisplay();
  print_line("Time is set", 0, 0, 2);
  delay(1000);
}

void set_alarm(int alarm){
  int temp_hour = alarm_hours[alarm];
  while (true){
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour), 0, 0, 2);

    String pressed = wait_for_button_press();
    if (pressed == "UP") {
      delay(200);
      temp_hour +=1;
      temp_hour = temp_hour % 24;
    }
    else if (pressed == "DOWN"){
      delay(200);
      temp_hour -=1;
      if (temp_hour < 0) {
        temp_hour =23;
      }
    }
    else if (pressed == "OK"){
      delay(200);
      alarm_hours[alarm] = temp_hour;
      break;
    }
    else if (pressed == "CANCEL"){
      delay(200);
      break;
    }
  }
  int temp_minute = alarm_minutes[alarm];
  while (true){
    display.clearDisplay();
    print_line("Enter minute: " + String(temp_minute),0,0,2);
    String pressed = wait_for_button_press();
    if (pressed == "UP"){
      delay(200);
      temp_minute +=1;
      temp_minute = temp_minute % 60;
    }
    else if (pressed == "DOWN"){
      delay(200);
      temp_minute -= 1;
      if (temp_minute<0){
           temp_minute=59;
      }
    }
    else if (pressed == "OK"){
      delay(200);
      minutes = temp_minute;
      break;
    }
    else if (pressed == "CANCEL"){
      delay(200);
      break;
    }
  }
  display.clearDisplay();
  print_line("Time is set", 0, 0, 2);
  delay(1000);
}

void run_mode(int mode){
  if (mode ==0){
    set_time();
  }
  else if (mode == 1 || mode ==2){
    set_alarm(mode -1);
  }
  else if (mode == 3){
    alarm_enabled = false;
  }
}

void go_to_menu(void){
  while (digitalRead(CANCEL)==0){
    display.clearDisplay();
    for (int i=0; i<5;i++){
      print_line("1-set time",1,0,0);
      print_line("2-set alarm 1",1,8,0);
      print_line("3-set alarm 2",1,16,0);
      print_line("4-remove alarm",1,24,0);
      delay(200);
    }

    String pressed= wait_for_button_press();
    print_line(String(pressed) + "000",1,0,0);
    delay(200);
    display.clearDisplay();
    while(pressed != "OK" & (pressed == "UP" | pressed == "DOWN")){
      print_line(pressed + "000",1,0,0);
      delay(200);
      if (pressed=="UP"){
        current_mode +=1;
        current_mode %= max_modes;
        display.clearDisplay();
        print_line(String(pressed),2,0,0);
        delay(200);
      }
      else if (pressed == "DOWN"){
        current_mode -= 1;
        if (current_mode < 0){
          current_mode = max_modes -1;
        }
        display.clearDisplay();
        print_line(String(current_mode),2,0,0);
        delay(200);
      }
      display.clearDisplay();
      pressed = wait_for_button_press();
      display.clearDisplay();
      print_line(pressed + "00111",1,0,0);
      delay(200);
    }
    Serial.println(current_mode);
    delay(200);
    run_mode(current_mode);
    
  }
}

void check_temp(void){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  bool all_good = true;
  if (data.temperature > 35){
    all_good = false;
    digitalWrite(LED_2, HIGH);
    print_line("TEMP HIGH", 1, 40, 0);
  }
  else if(data.temperature < 25){
    all_good = false;
    digitalWrite(LED_2,HIGH);
    print_line("TEMP LOW", 1, 40, 0);
  }
  if(data.humidity > 85){
    all_good = false;
    digitalWrite(LED_2,HIGH);
    print_line("HUMD HIGH", 1, 50, 0);
  }
  else if(data.humidity < 35){
    all_good = false;
    digitalWrite(LED_2,HIGH);
    print_line("HUMP LOW", 1, 50, 0);
  }
  if (all_good){
    digitalWrite(LED_2, LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_1,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(LED_2,OUTPUT);
  pinMode(CANCEL ,INPUT);
  pinMode(UP ,INPUT);
  pinMode(DOWN ,INPUT);
  pinMode(OK ,INPUT);
  std::cout << "connected";
  dhtSensor.setup(DHT, DHTesp::DHT22);
  std::cout << "connected";
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDERSS)){
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);

  display.clearDisplay();
  print_line("Welcome to Medibox", 2, 4, 0);
  delay(3000);

  WiFi.begin("Wokwi-GUEST");
  while (WiFi.status() != WL_CONNECTED){
    delay(250);
    display.clearDisplay();
    print_line("Connecting to WiFi",2,0,0);
    std::cout<< "connected";
  }
  display.clearDisplay();
  print_line("Connected to wifi",2 ,0, 0);

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  update_time_with_check_alarm();
  if (digitalRead(CANCEL) == LOW){
    delay(1000);
    Serial.println("Menu");
    go_to_menu();
  }
  check_temp();
}
