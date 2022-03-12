
#define p_sw_out1 A0
#define p_sw_out2 A1
#define p_sw_out3 A2

#define f_sw_out1 A3
#define f_sw_out2 A4
#define f_sw_out3 A5

 
typedef enum {
sw_slot1,
sw_slot2,
sw_slot3,
sw_slot4,
sw_slot5,
sw_slot6,
sw_slot7,

}parking_sw_t;

typedef enum{
sw_floor1,
sw_floor2,
sw_floor_street,
sw_limit_up,    /*interrupt happen --> stop floor_stepper and goto floor2*/
sw_limit_down,  /*interrupt happen --> stop floor_stepper and goto floor2*/
}floor_sw_t;

void limit_switches_init( void );

/* wait on infinite while untill the required switch pressed */
void search_limit_pressed( parking_sw_t );

void search_limit_pressed( floor_sw_t );
