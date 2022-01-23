#include <FlexiTimer2.h>

#define time_sw 10
#define start_sw 12
#define sw_on LOW
#define sw_off HIGH
#define beep_pin A0

const int led [5] = {5, 6, 7, 8, 9};

boolean is_count = false;
boolean is_finish = false;

int count_time = 180;
int led_on = HIGH;
int led_num = 0;
int minute = 3;
int cache_minute = 3;
int time_count = 0;
int beep_tone = 1500;

void counter() {
  if (time_count != (count_time * 2) && is_count == true) {
    
    for (int n = 0; n < 5; n++)
      digitalWrite(led[n], HIGH);
    
    led_on = !led_on;
    minute = time_count % 120 == 0 ? --minute : minute;
    
    for (int n = 0; n < minute; ++n)
      digitalWrite(led[n], led_on);
  }
  else
  {
    if (time_count == count_time * 2)
    {
      is_count = false;
      is_finish = true;
    }
  }
  if (is_finish == true && is_count == false)
  {
    for (int a = 0; a < 5; a++)
      digitalWrite(led[a], HIGH);
    
    tone(beep_pin, beep_tone, 400);
    is_finish = led_num > 4 ? false : true;
    led_num = led_num > 4 ? 0 : led_num;
    digitalWrite(led[led_num], LOW);
    ++led_num;
    if (!is_finish)
    {
      led_num = 0;
      for (int n = 0; n < cache_minute; n++)
        digitalWrite(led[n], LOW);
    }

  }
  time_count++;
}


void setup() {
  pinMode(time_sw, INPUT_PULLUP);
  pinMode(start_sw, INPUT_PULLUP);
  pinMode(beep_pin, OUTPUT);
  
  for (int n = 0; n < 5; n++)
    pinMode(led[n], OUTPUT);
    
  for (int n = 0; n < 3; n++)
    digitalWrite(led[n], LOW);
    
  for (int n = 3; n < 5; n++)
    digitalWrite(led[n], HIGH);
    
  tone(beep_pin, beep_tone, 50);
  delay(50);
  tone(beep_pin, beep_tone, 50);
  
  FlexiTimer2::set(250, counter);
  FlexiTimer2::start();
}

void loop() {
  if (digitalRead(time_sw) == sw_on && is_count == false && is_finish == false)
  {
    for (int n = 0; n < 5; n++)
      digitalWrite(led[n], HIGH);
    
    count_time = count_time == 180 ? 300 : 180;
    minute = count_time / 60;
    cache_minute = minute;
    
    for (int n = 0; n < minute; n++)
    {
      digitalWrite(led[n], LOW);
      tone(beep_pin, beep_tone, 50);
      delay(150);
    }
    
    is_count = false;
    delay(300);
  }
  if (digitalRead(start_sw) == sw_on) {
    if (is_count == false && is_finish == false)
    {
      time_count = 0;
      minute = cache_minute;
      count_time = cache_minute * 60;
      minute++;
      is_count = true;
      tone(beep_pin, beep_tone, 50);
      delay(150);
      tone(beep_pin, beep_tone, 50);
      delay(500);
    }
    else
    {
      tone(beep_pin, beep_tone, 250);
      is_count = false;
      time_count = 0;
      count_time = cache_minute * 60;
      minute = cache_minute;
      
      for (int n = 0; n < 5; n++)
        digitalWrite(led[n], HIGH);
      
      for (int n = 0; n < cache_minute; n++)
        digitalWrite(led[n], LOW);
      
      delay(1000);
    }
  }
}
