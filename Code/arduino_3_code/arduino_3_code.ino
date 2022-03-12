
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

/*
 * recieve data
 */

#define in1 3
#define in2 4
#define in3 5
#define in4 6

#define IR_up   A3
#define IR_down A4

#define front_sw A1
#define back_sw  A2

         /*CE CSN*/
RF24 radio(10, 9); // using pin 7 for the CE pin, and pin 8 for the CSN pin


const uint64_t pipe_rx = 0xE6;
const uint64_t pipe_tx = 0xA7;                  /*  address of nrf24    */
 
uint8_t rx_data = 0;
uint8_t tx_data = 0;

void moveHolder_forward();
void moveHolder_backward();
void stopHolder();
void move_up_holder();
void move_down_holder();
void stoplifting();

void stop_on_frontsw(){
  while((PINC & 0x04) != 0x04);
  stopHolder();
}
void stop_on_backsw(){
  while((PINC & 0x02) != 0x02);
  stopHolder();
}

bool updown_flag = true; /*initialized holder lifting up*/

void stop_on_up(){
  updown_flag = true;
  delay(8000);
  stoplifting();
}

void stop_on_down(){
  updown_flag = false;
  delay(8000);
  stoplifting();
}

uint8_t nrf_send(uint8_t data);
uint8_t nrf_recieve();




void setup() {

  Serial.begin(9600);
 
  radio.begin();
  radio.setPALevel(RF24_PA_LOW); 
  radio.setPayloadSize(sizeof(rx_data));
  radio.openWritingPipe( 0xE6 ); /*A7*/
  radio.openReadingPipe(1,0xE6); /*E6*/
  radio.startListening();

  pinMode(in1,  OUTPUT);
  pinMode(in2,  OUTPUT);
  pinMode(in3,  OUTPUT);
  pinMode(in4,  OUTPUT);
  
  pinMode(front_sw,  INPUT);
  pinMode(back_sw,   INPUT);

  pinMode(IR_up,   INPUT);
  pinMode(IR_down, INPUT);

  
}

void loop() {
  if(radio.available())
  {
    rx_data = nrf_recieve();
    //Serial.println(rx_data);
   
    /*park car*/
    if(rx_data == 0x01)
    {  
      //Serial.println("parking starts");
      moveHolder_forward();
      stop_on_frontsw();
      delay(100);
      //Serial.println("forward done");
      move_down_holder();
      stop_on_down();
      delay(100);
      //Serial.println("down done");
      moveHolder_backward();
      stop_on_backsw();
      delay(100);
      //Serial.println("back done");
      move_up_holder();
      stop_on_up();
      delay(100);
      //Serial.println("up done");
      
      /*tell ARD2 that operation is done*/
      nrf_send(0x04);
      //Serial.println("send done");
      rx_data = 0;
      radio.startListening();
    }
    
    /*GET*/
    else if(rx_data == 0x02)
    {
      //Serial.println("getting car starts");
      move_down_holder();
      stop_on_down();
      delay(100);
      //Serial.println("forward starts");
      moveHolder_forward();
      stop_on_frontsw();
      delay(100);
      //Serial.println("up starts");
      move_up_holder();
      stop_on_up();
      delay(100);
      //Serial.println("backward starts");
      moveHolder_backward();
      stop_on_backsw();
      delay(100);
      //Serial.println("up starts");
      /*tell ARD2 that operation is done*/
      nrf_send(0x06);
     // Serial.println("send done");
      rx_data = 0;
      radio.startListening();
    }
 }
 }




uint8_t nrf_send(uint8_t data)
{
  bool report = false;
  radio.stopListening();  //write
                          /*code to make ARD3 to the specific algorithm*/
  while(report != true)
  {
    report = radio.write(&data, sizeof(data));
    
  }
  return report;
}

uint8_t nrf_recieve()
{
  uint8_t data = 0;
  radio.startListening(); // read
  
  radio.read(&data,1);

  return data;  /*means it is recieved correctly*/
}



/*  FORWARD , BACKWARD , STOP   */
void moveHolder_forward(){
    digitalWrite(in1 , HIGH);
    digitalWrite(in2 , LOW);
}

void moveHolder_backward(){
   digitalWrite(in1 , LOW);
   digitalWrite(in2 , HIGH); 
}
void stopHolder(){
      digitalWrite(in1 , LOW);
      digitalWrite(in2 , LOW); 
}


/* UP , DOWN , STOP  */
void move_up_holder(){
     digitalWrite(in3 , HIGH);
     digitalWrite(in4 , LOW);
}
void move_down_holder(){
    digitalWrite(in3 , LOW);
    digitalWrite(in4 , HIGH); 
}
void stoplifting(){
      digitalWrite(in3 , LOW);
      digitalWrite(in4 , LOW);  
}
