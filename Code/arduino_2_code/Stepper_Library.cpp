#include "Stepper_header.h"
#include "Ultrasonic_header.h"
#include "bitmath.h"
#include "Limit_Switches.h"
#import <Arduino.h>


void Stepper_init()
{
  pinMode(floor_stepPin,     OUTPUT);
  pinMode(floor_dirPin,      OUTPUT);
  pinMode(floor_enablepin,   OUTPUT);
  
  pinMode(rotation_stepPin,  OUTPUT);
  pinMode(rotation_dirPin,   OUTPUT);
  pinMode(rotation_enablepin, OUTPUT);

  digitalWrite(floor_enablepin,    HIGH);  /* initial off */
  digitalWrite(rotation_enablepin, HIGH);  /* initial off */

    /* step pin pwm (490Hz) for floor_stepper */
    analogWrite(floor_stepPin, 128);    
    
    /*     244Hz for parkingSlot_stepper */
    /* if you want 61Hz --> TCCR0B = 0x05*/
    TCCR0B = 0x05; /* choose prescalar */
    TCCR0A = 0x23; /* choose fast pwm and non inverting on OC0B */
    OCR0B  = 127; /* 50% duty cycle */
  
}


void Stepper_rotate(stepperType_t stepper_name , StepperMotor_dir rotation_dir)
{

    switch(stepper_name)
    {
      /*  which stepper?  */
      case floor_stepper: 
          /* enable it */
        digitalWrite(floor_enablepin, LOW);     /* turn that on  */

        /*  which direction?  */
         if(rotation_dir == rotate_up)
              /*up*/
           digitalWrite(floor_dirPin, LOW);
         else /*down*/
           digitalWrite(floor_dirPin, HIGH);
      break;
      
      case  rotation_stepper: 

        digitalWrite(rotation_enablepin,LOW);
      
          if(rotation_dir == rotate_cw)
            digitalWrite(rotation_dirPin, HIGH);
          else /*down*/
            digitalWrite(rotation_dirPin, LOW);
      break;

      default: break;
    
    }

}

void Stepper_stop(stepperType_t stepper_name)
{
      if(stepper_name == floor_stepper)
      digitalWrite(floor_enablepin, HIGH);    /*stop stepper*/
      else if(stepper_name == rotation_stepper)
      digitalWrite(rotation_enablepin, HIGH);   /*stop stepper*/    
}

/* will be changed to limit switches */
void Stepper_gotofloor(floor_required_t req_floor)
{
  bool IsReqReached = false ; 
 
  /********** Code to know which floor we stand at **********/
  static floor_sw_t reached_floor = sw_floor_street;

  
  if((PINC & 0x38 ) == 0x18)    /*floor 1*/
  {
    reached_floor = sw_floor1;
  }
  else if((PINC & 0x38 ) == 0x28) /*floor 2*/
  {
    reached_floor = sw_floor2;
  }
  else /*floor street*/
  {
    reached_floor = sw_floor_street;
  }
    /*  if we already stand at the required floor then don't move the stepper*/
    if(req_floor == reached_floor)
  {
    IsReqReached = true;
  }
   /******** ************************************** ********/

  /*  Code to move the stepper to the required floor  */
   if(req_floor < reached_floor && IsReqReached != true)
   {
    Stepper_rotate(floor_stepper , rotate_down);
    search_limit_pressed ( (floor_sw_t)req_floor );
   }
   
   else if(req_floor > reached_floor && IsReqReached != true)
   {
    Stepper_rotate(floor_stepper , rotate_up);
    search_limit_pressed ((floor_sw_t) req_floor );
   }
   /******** ************************************** ********/

  /*  required floor reached if we get in here  */
    Stepper_stop ( floor_stepper );
 }


StepperMotor_dir decide_cw_ccw (unsigned char reached, unsigned char req){
     uint8_t dist1 = 0;
     uint8_t dist2 = 0;

    if(reached > req)
    {
        dist1 = reached - req;
        dist2 = req + 1 + (6 - reached);

        if(dist2 < dist1)
        return rotate_cw;
        else
        return rotate_ccw;
    }
    else
    {
        dist1 = req - reached;
        dist2 = (6 - req) + reached + 1;
        
        if(dist2 < dist1)
        return rotate_ccw;
        else
        return rotate_cw;
    }
   

}

void Stepper_gotoslot(parkingSlots_t req_slot)
{

  static parking_sw_t reached_slot = sw_slot1;
  bool  IsReqReached = false;
  
  /***************Code to know which slot we stand at****************/
  unsigned char looping_slots = 0;
  unsigned char read_pinC = 0;
 
   
  if((PINC & 0x07 ) == 0x00)    /*floor 1*/
  {
    reached_slot = sw_slot1;
  }
  else if((PINC & 0x07 ) == 0x06) /*floor 2*/
  {
    reached_slot = sw_slot2;
  }
  else if((PINC & 0x07 ) == 0x05) /*floor 2*/
  {
    reached_slot = sw_slot3;
  }
  else if((PINC & 0x07 ) == 0x04) /*floor 2*/
  {
    reached_slot = sw_slot4;
  }
  else if((PINC & 0x07 ) == 0x03) /*floor 2*/
  {
    reached_slot = sw_slot5;
  }
  else if((PINC & 0x07 ) == 0x01) /*floor 2*/
  {
    reached_slot = sw_slot6;
  }
  else if((PINC & 0x07 ) == 0x02) /*floor 2*/
  {
    reached_slot = sw_slot7;
  }
  else
  {
    reached_slot = sw_slot1;
  }

  /*  if we already stand at the required floor then don't move the stepper*/
  if(req_slot == reached_slot)
  {
    IsReqReached = true;
  }
  /*******************************************************************/

  /*  Code to move rotation stepper to the required slot  */
  if(IsReqReached != true)
  {
     StepperMotor_dir returned_decision = decide_cw_ccw ( reached_slot, req_slot );
   
     Stepper_rotate( rotation_stepper , returned_decision );
     
     if(returned_decision == rotate_cw)
     Serial.println("rotate_cw");
     else
     Serial.println("rotate_ccw");
     
     Serial.println(req_slot);
     search_limit_pressed((parking_sw_t) req_slot );


     
     if(req_slot == slot_5)
     {
      Stepper_gotoslot(slot_5);
     }
     
  }
  
  /******************************************************************/
    
  /*  required slot reached if we get in here  */
    Stepper_stop ( rotation_stepper );
   
}







/*
  if(req_slot > reached_slot && IsReqReached != true)
  {
    Stepper_rotate( rotation_stepper , rotate_cw );
    search_limit_pressed((parking_sw_t) req_slot );
  }
  else if(req_slot < reached_slot && IsReqReached != true)
  {
    Stepper_rotate( rotation_stepper , rotate_ccw );
    search_limit_pressed((parking_sw_t) req_slot );
  }
  */
  /*
    int result = reached_slot - req_slot ;
  if(result <0 && result>= -4 && IsReqReached != true)
  {
    Stepper_rotate( rotation_stepper , rotate_cw );
    search_limit_pressed((parking_sw_t) req_slot );
  }
  else if(result < 0 && result < -4 && IsReqReached != true)
  {
    Stepper_rotate( rotation_stepper , rotate_ccw );
    search_limit_pressed((parking_sw_t) req_slot );
  }
  
  else if(result > 0 && result <= 4 && IsReqReached != true)
  {
    Stepper_rotate( rotation_stepper , rotate_ccw );
    search_limit_pressed((parking_sw_t) req_slot );
  }
  
  else if(result > 0 && result >4  && IsReqReached != true)
  {
    Stepper_rotate( rotation_stepper , rotate_cw );
    search_limit_pressed((parking_sw_t) req_slot );
  }
  */
