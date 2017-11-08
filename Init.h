

//DC Motor Initiation
void DC_motor_init(void) {
  
  SET_BITS(DDRB, SET_PB); //DDR to output
  SET_BITS(PORTB,DIR1);   //PORTB to the initial direction
  SET_BITS(DDRP, SET_PP); //DDRP for Output
  SET_BITS(PTP, SET_PP);  //enable
  
  
  
  //Initilize PWM ports 4 & 5 \
  PWMCTL = EIGHT_BIT_MODE;                                                              // 8-bit mode 
  PWMPRCLK = PWM_ECLK;                                                                  // E-Clock
  PWMSCLA = SA_DIVIDE_2;                                                                // Divide by 2
  PWMCLK |= (PWMCLK_PCLK4_MASK | PWMCLK_PCLK5_MASK);                                    // Select clock SA as the clock source for PWM port 4 & 5 
  PWMPOL |= (PWMPOL_PPOL4_MASK | PWMPOL_PPOL5_MASK);                                    // Positive Polarity
  PWMCAE &= LOW(PWMCAE_CAE0_MASK);                                                      // Left alligned for both
  PWMCAE &= LOW(PWMCAE_CAE1_MASK);                                                      
  PWMPER4 = PERIOD_22KHZ_LEFT_ALINGED;                                                  // Frequency is 22KHz
  PWMPER5 = PERIOD_22KHZ_LEFT_ALINGED;
  PWMDTY4 = PWM_DTY_60;                                                                 // Duty cycle channel 4
  PWMDTY5 = PWM_DTY_60;                                                                 // Duty cycle channel 5
  PWMCNT4 = PWM_RESET;                                                                  // Reset PWM4 & PWM5 counter
  PWMCNT5 = PWM_RESET;

  PWME |= (PWME_PWME4_MASK|PWME_PWME5_MASK);                                            // Enable the PWM0 and PWM1 ports
  
  
}


//RS232 Initiation
void rsInit(void){
  
 SCISR1 = 0;
 SCIBD = Divider;                                                                       //configures Baud rate at set value
 SCICR1 = 0;                                                                            //First sci control register     set to 0x80 for loop select enable
 SCICR2 = 0;
 SCICR2 |= (SCICR2_RE_MASK | SCICR2_TE_MASK);
 SCICR2 |= RDRF_MASK;                                                                   //enables reciever full interrupts
  
}

//Servo Initiation
void timer_INIT(int c){
                                                                                        //the prescaler was set to 8 so 1us = 1 OC Value
  TSCR2 	= PRESCALER_VAL; 	                                                            // set prescaler to 8, no interrupt
  TSCR1 	= TSCR_INIT;    	                                                            // enable TCNT, fast timer flag clear, freeze
  
  //TCTL1 = ZERO;                                                                       // initilize the upper bits of TCTL register to zero so it does nothing 
  TCTL2 = TCTL2_LOW;                                                                    // initilize timer channel 2 to low
  
 
  TC2 	= TCNT + c;	                                                            // preset TC2 for first OC event
  TIE = ENABLE_TIMER_RTI;                                                               // enable interrupts for timer channel 2
 	
	TIOS   |= TIOS_IOS2_MASK;	                                                            // ready to go - enable TC2 as OC
                             
}

//Servo Code 
int degree2time(unsigned int degrees){
  int dutycycle;
  dutycycle = (RC_gain*degrees) + offset_time;
  return(dutycycle);
}

//Stepper Initiation
void stepper_init(void) {
  
  rti_count = 1;
  stepper_speed = 2;
  state = 0;
  step_type = -1;
  limit_switch = 0; 
  
  CLR_BITS(DDRAD,(DDRAD_DDRAD6_MASK|DDRAD_DDRAD7_MASK));                                                   // configures PAD6 & PAD7 to be inputs
  SET_BITS(ATDDIEN,(ATDDIEN_IEN6_MASK|ATDDIEN_IEN7_MASK));                                                 // Must be set for PAD registers to read digital. Read Datasheet for more
  
  SET_BITS(DDRP,DDRP_DDRP3_MASK);                                                                          // enables the stepper motor by enabeling PP3 to an output
  SET_BITS(PTP,PTP_PTP3_MASK);                                                                             // sets PP3 to a high
  
  SET_BITS(DDRT, (DDRT_DDRT7_MASK|DDRT_DDRT6_MASK|DDRT_DDRT5_MASK|DDRT_DDRT4_MASK));                       // configures Port T 4,5,6,7 to be outputs
  
  FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);                                                                             // loads the first value of stepper_array into the Port T
  
  //Stuff
  RTICTL = RTI_PERIOD;                                                      // Set RTI period 
  COPCTL |= COPCTL_RSBCK_MASK;                                              // Freeze RTI during BDM active
  CRGFLG = CRGFLG_RTIF_MASK;                                                // Clear any possibly pending RTI interrupts
  CRGINT |= CRGINT_RTIE_MASK;                                               // Enable RTI interrupt
}