
#import <Arduino.h>
#include "Ultrasonic_header.h"

long duration;

void ultrasonic_init()
{
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
}

long ultrasonic_read()
{
  
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);

  digitalWrite(trigpin,HIGH);
    delayMicroseconds(10);
  digitalWrite(trigpin,HIGH);
  
  duration = pulseIn(echopin, HIGH);
  
  return (duration * 0.034 /2);
}
