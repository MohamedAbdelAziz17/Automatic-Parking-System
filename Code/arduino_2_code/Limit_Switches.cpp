#include "Limit_Switches.h"
#include "bitmath.h"
#include "Stepper_header.h"
#import <Arduino.h>

void limit_switches_init( void ){

  DDRC |= ( ( DDRC & 0x3F ) | 0x00 );  /* input */
}

/*  parking slots switches  */
void search_limit_pressed( parking_sw_t sw ){

  switch ( sw ){

    case sw_slot1: while( (PINC & 0x07 )!= 0x00 );
                   break;

    case sw_slot2: while( (PINC & 0x07 )!= 0x06 );
                   break;
                   
    case sw_slot3: while( (PINC & 0x07 )!= 0x05 );
                   break;
                   
    case sw_slot4: while( (PINC & 0x07 )!= 0x04 );
                   break; 

    case sw_slot5: while( (PINC & 0x07 )!= 0x03 );
                   break;

    case sw_slot6: while( (PINC & 0x07 )!= 0x01 );
                   break;

    case sw_slot7: while( (PINC & 0x07 )!= 0x02 );
                   break;    

    default:       break;                                                             
  }

 
  
  

   
}
/*  floors switches  */
void search_limit_pressed( floor_sw_t sw )
{

  switch ( sw )
  {

    case sw_floor1: while( (PINC & 0x38 )!= 0x18 );
                    break;

    case sw_floor2: while( (PINC & 0x38 )!= 0x28 );
                    break;
                   
    case sw_floor_street: while( (PINC & 0x38 )!= 0x30 );
                    break;
                   
    case sw_limit_up: while( (PINC & 0x38 )!= 0x10 );
                     break; 

    case sw_limit_down: while( (PINC & 0x38 )!= 0x08 );
                     break;            

    default:         break;                                                             
  }
  
}
