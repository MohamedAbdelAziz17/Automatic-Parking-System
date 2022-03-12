
#define floor_dirPin    8       /*changeble*/
#define floor_stepPin   3 
#define floor_enablepin 7

#define rotation_dirPin    6       /*changeble*/
#define rotation_stepPin   5 
#define rotation_enablepin 4

typedef enum{
  rotate_down,  /* case of floor stepper  */
  rotate_up,
  
  rotate_cw,  /* case of rotation stepper */
  rotate_ccw
}StepperMotor_dir;

typedef enum{
  floor_stepper,
  rotation_stepper
}stepperType_t;

/*********floor distances in Cm definition in case using ultrasonic*********/
typedef enum{
  floor_1,
  floor_2,
  floor_street,
}floor_required_t;

/*******************************************/

/*********Parking Slots Numbers definition*********/
typedef enum  {
  /*  floor parking slots   */
slot_1,
slot_2,
slot_3,
slot_4,
slot_5,
slot_6,
slot_7,  

}parkingSlots_t;
/*************************************************/

void Stepper_init();

void Stepper_rotate(stepperType_t , StepperMotor_dir);

void Stepper_stop( stepperType_t );

void Stepper_gotofloor( floor_required_t );

void Stepper_gotoslot( parkingSlots_t );

StepperMotor_dir decide_cw_ccw (unsigned char, unsigned char);
