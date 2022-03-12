/*modified*/
/*  go to floor street to wait the user to put his car*/
  /*  wait here untill you recieve a data from UART to initialize the project*/ /*(while(mySerial.available()==0);*/
  /*  get data from uart Ard1 and """decide""" you want to "park car" or "get car" */
  /*  say you want to park */
  
  /*  rotate floor stepper until it reaches any floor you want assume it's floor 1*/
  //Stepper_gotofloor( floor_1 );

  /*  then goto parking slot where you want to park car assume it's floor 1 slot 5 rotate the stepper*/
  //Stepper_gotoslot( slot_5 );

  /* then send data by nrf24 radio.write() to Ard3 to move the holder that carries the car*/
  /* holder moves untill the infrared sensor stops it */
  /* put the car then back the holder and send data to Ard2 here that the operation is done then Ard2 will go back the floor_street*/

#include <Wire.h> 
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include "bitmath.h"
#include "Stepper_header.h"
#include "Limit_switches.h"


#define parked      1
#define not_parked  0

RF24 radio( 9 , 10 );                        /* CE, CSN */


uint8_t rxBuffer_uart = 0;                   /*  UART recieved data  */

uint8_t nrf_txbuffer  = 0;                   /*  wireless data       */
uint8_t nrf_rxbuffer  = 0;  
                 
const uint64_t pipe_tx = 0xE6;                  /*  address of nrf24    */
const uint64_t pipe_rx = 0xA7;                  /*  address of nrf24    */
                                                 
uint8_t arr1[14] = { 0 };       /***initialize all elements to zero***/

uint8_t card_id = 0 ;                /*value comes from ARD1*/

void park_car(floor_required_t , parkingSlots_t);

void get_car(floor_required_t , parkingSlots_t);

uint8_t nrf_send(uint8_t);

uint8_t nrf_recieve();

void gohome_getcar (void);

void gohome_delivercar (void);

typedef struct{

  uint8_t user_id;
  uint8_t array_pos;
  bool    exist_status;
  floor_required_t car_floor_pos;
  parkingSlots_t car_slot_pos;
}user_t;

  /** Initialize array of structs **/
   user_t user[4];
   
void setup() 
{
    /**  UART Init   **/
    Serial.begin(9600);
    
    Stepper_init();

    limit_switches_init();

    /*Init of nrf24*/
    radio.begin();
    radio.setPALevel(RF24_PA_LOW); 
    radio.setPayloadSize(sizeof(nrf_rxbuffer));
    radio.openWritingPipe( 0xE6 ); /*E6*/
    radio.openReadingPipe(1,0xE6); /*A7*/
    radio.stopListening();

   /*  interrupt happens whenever limit_up or limit_down is pressed  */
    //attachInterrupt(digitalPinToInterrupt(2), ISR_Limit_Switch, RISING); 
 
  /** Initialization of users values **/
   user[0].array_pos      = 0;
   user[0].exist_status   = 0;
   user[0].car_floor_pos  =   NULL;
   user[0].car_slot_pos   =   NULL;
   user[0].user_id = 89 ;       //values of cards

   user[1].array_pos      = 0;
   user[1].exist_status   = 0;
   user[1].car_floor_pos  =   NULL;
   user[1].car_slot_pos   =   NULL;
   user[1].user_id = 3 ; 

   user[2].array_pos      = 0;
   user[2].exist_status   = 0;
   user[2].car_floor_pos  =   NULL;
   user[2].car_slot_pos   =   NULL;
   user[2].user_id = 131 ; 

   user[3].array_pos      = 0;
   user[3].exist_status   = 0;
   user[3].car_floor_pos  =   NULL;
   user[3].car_slot_pos   =   NULL;
   user[3].user_id = 57 ; 
  
}

void loop() {
  
  gohome_getcar();    /*initialized waiting users*/
//    Serial.print(digitalRead(A2));
//    Serial.print(digitalRead(A1));
//    Serial.print(digitalRead(A0));
//    Serial.println();
 if(Serial.available() > 0)
 {
  card_id = Serial.read();
 
//  if(card_id == 'u')
//  Stepper_rotate(rotation_stepper, rotate_cw);
//  
//  else if(card_id == 'd')
//  Stepper_rotate(rotation_stepper, rotate_ccw);
//
//  else if(card_id == 's')
//  Stepper_stop(rotation_stepper);
//
//
//  else if(card_id == '1')
//  Stepper_gotoslot(slot_1);
//  
//  
//  else if(card_id == '2')
//  Stepper_gotoslot(slot_2);
//  
//  else if(card_id == '3')
//  Stepper_gotoslot(slot_3);
//  
//  else if(card_id == '4')
//  Stepper_gotoslot(slot_4);
//  
//  else if(card_id == '5')
//  {
//   Stepper_gotoslot(slot_5);
//  }
//  
//  
//  else if(card_id == '6')
//  Stepper_gotoslot(slot_6);
//
//  else if(card_id == '7')
//  Stepper_gotoslot(slot_7);
//  
//  else if (card_id == 'y')
//  {
//    Stepper_gotofloor(floor_1);
//    Serial.println("o");
//  }
//  
//  else if (card_id == 'h')
//  {
//   Stepper_gotofloor(floor_2);
//   Serial.println("l");
//  }
// 
//  else if (card_id == 'n')
//  Stepper_gotofloor(floor_street);

  
/***********************************  USER 0 ****************************/
  if(card_id == user[0].user_id)
  {
    /*check exsistance*/
    if(user[0].exist_status == 0) /*not exsisted*/
    {
      user[0].car_floor_pos = floor_1;
      user[0].car_slot_pos  = slot_1;
      user[0].exist_status = 1;

      park_car(  user[0].car_floor_pos ,  user[0].car_slot_pos );
      gohome_getcar();
    }
    else if(user[0].exist_status == 1)  /*exsisted*/
    {
      user[0].exist_status = 0;
      get_car( user[0].car_floor_pos , user[0].car_slot_pos );
      
      gohome_delivercar();
      delay(300);
      gohome_getcar();
      
      user[0].car_floor_pos = NULL;
      user[0].car_slot_pos  = NULL;
    }
  }
/************************************************************************/

/***********************************  USER 1 ****************************/
  else if(card_id == user[1].user_id)
  {
      /*check exsistance*/
    if(user[1].exist_status == 0) /*not exsisted*/
    {
      user[1].car_floor_pos = floor_1;
      user[1].car_slot_pos  = slot_2;
      user[1].exist_status = 1;
      
      park_car( user[1].car_floor_pos , user[1].car_slot_pos );
      gohome_getcar();
    }
    else if(user[1].exist_status == 1) /*exsisted*/
    {
      user[1].exist_status = 0;
      get_car( user[1].car_floor_pos , user[1].car_slot_pos );
      
      gohome_delivercar();
      delay(300);
      gohome_getcar();
      
      user[1].car_floor_pos = NULL;
      user[1].car_slot_pos  = NULL;
    }
  }
/************************************************************************/

/***********************************  USER 2 ****************************/
  else if(card_id == user[2].user_id)
  {
     /*check exsistance*/
    if(user[2].exist_status == 0) /*not exsisted*/
    {
      user[2].car_floor_pos = floor_2;
      user[2].car_slot_pos  = slot_3;
      user[2].exist_status = 1;
      
      park_car( user[2].car_floor_pos , user[2].car_slot_pos );
      gohome_getcar();
    }
    else if(user[2].exist_status == 1) /*exsisted*/
    {
      user[2].exist_status = 0;
      get_car( user[2].car_floor_pos , user[2].car_slot_pos );

      gohome_delivercar();
      delay(300);
      gohome_getcar();
      
      user[2].car_floor_pos = NULL;
      user[2].car_slot_pos  = NULL;
    }
  }
/************************************************************************/

/***********************************  USER 3 ****************************/
  else if(card_id == user[3].user_id)
  {
    /*check exsistance*/
    if(user[3].exist_status == 0) /*not exsisted*/
    {
      user[3].car_floor_pos = floor_2;
      user[3].car_slot_pos  = slot_1;
      user[3].exist_status = 1; /*assign it as existed*/
      park_car( user[3].car_floor_pos , user[3].car_slot_pos );
      gohome_getcar();
    }
    else if(user[3].exist_status == 1) /*exsisted*/
    {
      user[3].exist_status = 0; /*assign it as not existed*/
      get_car( user[3].car_floor_pos , user[3].car_slot_pos );

      gohome_delivercar();
      delay(300);
      gohome_getcar();
      
      user[3].car_floor_pos = NULL;
      user[3].car_slot_pos  = NULL;
    } 
  }
/************************************************************************/

 }

 /* This saves the entire process when idle */
  
}

uint8_t nrf_send(uint8_t data)
{
  bool report = false;
  radio.stopListening();  //write
             
               /* 0x01 code to make ARD3 to the specific algorithm*/
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

void park_car(floor_required_t floor_t, parkingSlots_t slot_t)
{
  
  Stepper_gotofloor( floor_t );
  
  delay(150);
  
  Stepper_gotoslot ( slot_t );
  
  delay(150);
 /**we reached the destination**/
 
 /** move the carriage forward then life down then move backward **/
  nrf_send( 0x01 );     //send to ARD3 to park
   
  uint8_t recieved = 0;
  while(recieved != 0x04)    //wait untill ARD3 sends back that he is on carriage
  {
    recieved = nrf_recieve();
  }
  
  /*function done*/
  return;
}

void get_car(floor_required_t floor_t , parkingSlots_t slot_t)
{
  
  Stepper_gotofloor( floor_t );

  delay(150);
  
  Stepper_gotoslot ( slot_t );

  delay(150);
  /***we reached the destination of slot***/
  
  /**move the carriage forward then lift up then move backward**/
  nrf_send( 0x02 );     //send to ARD3 to get
    
  uint8_t recieved = 0;
  while(recieved != 0x06)    //wait untill ARD3 sends back that he is on carriage 
  {
    recieved = nrf_recieve();
  }
 
  return;  
}



void ISR_Limit_Switch() {
  /*limit up or limit down is pressed then stop the stepper first*/
  Stepper_stop(floor_stepper);

  /* read the output to know which one of them is pressed*/
  /*case it is limit up pressed*/
  if((  PINC & 0x38 ) == 0x10)
  {
    Stepper_rotate(floor_stepper , rotate_down);
    search_limit_pressed  (sw_floor2);
  }
  /*case it is limit down pressed*/
  else
  {
    Stepper_rotate(floor_stepper , rotate_up);
    search_limit_pressed (sw_floor2);
  }
  
}

void gohome_getcar (void)
{
  Stepper_gotofloor(floor_street);
  delay(150);
  Stepper_gotoslot(slot_1);
}

void gohome_delivercar (void)
{
  Stepper_gotoslot(slot_5);
  delay(150);
  Stepper_gotofloor(floor_street);
}

/*
  bool search_result = linear_search( arr1 , arraylength(arr1) , id );

   if(search_result == false) // id not exists before then it is not parked so you should park it
   {
    uint8_t pos = search_zero_pos(arr1 , arraylength(arr1)); //getting the first zero in array
    
    if(pos != arr_full)
    arr1[pos] = id;     // store id in that position
    else      //  no place for a new car
    return;   //  exit

    park_car( floor_1 , slot_2 );
    
   }
   else if( search_result == true)
   {
    
   }
   */

  /*
  if(id == 5) // first card 
  {
    //check its row is it parked or not parked
    if( arr[ 1 , 0 ] == parked )
    {
     //car_get(floor1 , slot1);  
    }
    else
    {
      //car_park(floor1 , slot1);
    }
  }
   */

/*
  if(Serial.available()>0)
  {
     rxBuffer_uart = Serial.read();
     if(rxBuffer_uart == '0')
     Stepper_gotofloor( floor_1 );

     else if(rxBuffer_uart == '1')
     Stepper_gotofloor( floor_2 );

     else if(rxBuffer_uart == '2')
     Stepper_gotofloor( floor_street );
  }
 */
  /*
  if(Serial.available()>0)
  {
     rxBuffer_uart = Serial.read();
     if(rxBuffer_uart == '1')
     Stepper_gotoslot( slot_1 );

     else if(rxBuffer_uart == '2')
     Stepper_gotoslot( slot_2 );

     else if(rxBuffer_uart == '3')
     Stepper_gotoslot( slot_3 );
     
     else if(rxBuffer_uart == '4')
     Stepper_gotoslot( slot_4 );

     
     else if(rxBuffer_uart == '5')
     Stepper_gotoslot( slot_5 );

     
     else if(rxBuffer_uart == '6')
     Stepper_gotoslot( slot_6 );

     
     else if(rxBuffer_uart == '7')
     Stepper_gotoslot( slot_7 );
  }
 */

 /*
  if(Serial.available() > 0)
  {
    rxBuffer_uart = Serial.read();
    
    if(rxBuffer_uart == '1')
    {   
      rxBuffer_spi = 1;
     bool report = radio.write(&rxBuffer_uart, sizeof(rxBuffer_spi));
    if(report)
    Serial.println("sent");
    else
    Serial.println("not sent");
    }
    
    if(rxBuffer_uart == '0')
    { 
      rxBuffer_spi = 0;
     bool report = radio.write(&rxBuffer_uart, sizeof(rxBuffer_spi));
    if(report)
    Serial.println("sent");
    else
    Serial.println("not sent");
    }

     if(rxBuffer_uart == '2')
    {
      rxBuffer_spi = 2;
     bool report = radio.write(&rxBuffer_uart, sizeof(rxBuffer_spi));
      if(report)
    Serial.println("sent");
    else
    Serial.println("not sent");
    }
    
  }
  */
