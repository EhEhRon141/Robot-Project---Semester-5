#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "declarations.h"
#include "Init.h"


/**************DC Motor**************/
void DC_motor_init(void);


/****************RS232***************/
//void putcSCI( char cx );
//void putsSCI( char *str );
//char getcSCI( void );
void rsInit(void);

/**************Stepper***************/
void stepper_init(void);

/***************Servo***************/
int degree2time(unsigned int degrees);
void timer_INIT(int c);

static unsigned char pwm = 90;
static unsigned int dutycycle;
unsigned int degrees = 0;

void main(void) {
  /* put your own code here */
  

  DC_motor_init();  //Initialize DC Motor Operations
  rsInit();         //Initialize the RS232 Operations
  timer_INIT(dutycycle);
  stepper_init();   //Initializa Stepper Motor Operations

	EnableInterrupts;


  for(;;) {
  dutycycle = degree2time(degrees);
 if(M1 == 1) {         //Chooses direction of DC motor 1
   CLR_BITS(PORTB,M1_DIR2);
   SET_BITS(PORTB,M1_DIR1);
 }else if(M1 == 0){
   CLR_BITS(PORTB,M1_DIR1);
   SET_BITS(PORTB,M1_DIR2);
 }else{
   CLR_BITS(PORTB,M1_BRAKE);
 } 
 
 
 if(M2 == 1) {         //Chooses direction of DC motor 2
   CLR_BITS(PORTB,M2_DIR2);
   SET_BITS(PORTB,M2_DIR1);
 }else if(M2 == 0){
   CLR_BITS(PORTB,M2_DIR1);
   SET_BITS(PORTB,M2_DIR2);
 }else{
   CLR_BITS(PORTB,M2_BRAKE);
 } 
 
 if(pos == 0) {         //Chooses direction of DC motor 2
   degrees = 0;
 }else if(pos == 1){
   degrees = 36;
 }else if(pos == 2){
   degrees = 72;
 }else if(pos == 3){
   degrees = 108;
 } else if(pos == 4){
   degrees = 144;
 }else {
   degrees = 180;
 }
  PWMDTY4 = pwm;                                                                 
  PWMDTY5 = pwm;
                                               
  } 
  
}





/****************RS232***************/
//Interrupt to recieve all of the input from the pi
interrupt 20 void SCIhandler( void ){
  static int buffoff = 0;
  DisableInterrupts;
  
  if ((SCISR1 &= RDRF_MASK)>0)   //if the RDRF flag is set, Take RX info and save to ring buffer                   
  {
    recievebuff[buffoff]= SCIDRL;
    buffoff++;
    if (buffoff > 2){
      buffoff = 0;
    }
    M1 = recievebuff[1] - '0';
    M2 = recievebuff[0] - '0';
    pos = recievebuff[2] - '0';
  }
  EnableInterrupts;
}


/***************SERVO***************/
interrupt 10 void ISR_usDelay(void){
    DisableInterrupts;
    if(TCTL2 == TCTL2_LOW){               // if the TCTL2 is low the it has completed the high toggle
    TC2 = TCNT + (RC_PERIOD - dutycycle); // rearm the OC register for low time
    TCTL2 = TCTL2_HIGH;                   // TCTL2 to high so it starts the period as high in the next interrupt
    } 
    
    else if(TCTL2 == TCTL2_HIGH){   // if the TCTL2 is high the it has completed the 20ms period or the low toggle 
    TC2 = TCNT + dutycycle;		      // rearm the OC register for high time
    TCTL2 = TCTL2_LOW;              // TCTL2 to low so it completes the 20ms period in the next interrupt
    }
    else{}
    EnableInterrupts;
}

/**************STEPPER***************/
interrupt VectorNumber_Vrti void RTIhandler (void) {
	
	
    CRGFLG = CRGFLG_RTIF_MASK;                                                // Clear any possibly pending RTI interrupts

   limit_switch = (PTAD & LIMIT_SWITCH_MASK);                                 // equates the value of PAD6 and PAD7 to limit_switch
   
    
	 if (rti_count < stepper_speed) {                                           // counter that devides the speed of the motor.
	  rti_count++;       
	 }
	 else{
	  
	  state += step_type;                                                       //mod 7
	  state &= STEPPER_MASK;
	  
	  FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);
	  
	  rti_count = 1; 
	 }
	}


