

/**************DC Motor**************/
#define LOW(value) ((value) & 0xFF)
#define SET_BITS(reg,mask) ((reg) |= (mask))
#define CLR_BITS(reg,mask) ((reg) &= LOW(~mask))
#define FLIP_BITS(reg,mask) ((reg) ^= (mask))
#define FORCE_BITS(reg,mask,value) ((reg) = ((reg) & LOW(~mask)) | ((value) & (mask)))
#define ENABLE_PWM_CHNL(chnl) SETBITS(pwme,0x01<<(chnl))
#define DISABLE_PWM_CHNL(chnl) CLR_BITS(pwme,0x01<<(chnl))
#define FOREVER for(;;)
#define BIT_IS_CLR(val,mask) (!((val)&(mask)))
#define BIT_IS_SET(val,mask) ((val)&(mask))

#define EIGHT_BIT_MODE                    0x00
#define PWM_RESET                         0x00
#define PWM_ECLK                          0x00

#define SA_DIVIDE_2                       0x01
#define PERIOD_22KHZ_LEFT_ALINGED         182U
#define PWM_DTY_10                        18U
#define PWM_DTY_60                        108U
#define PWM_DTY_75                        135U

//Both
#define DIR1                              0x0A
#define DIR2                              0x05
#define BRAKE                             0x0F
//Motor 1
#define M1_DIR1                           0x02
#define M1_DIR2                           0x01
#define M1_BRAKE                          0x03
//Motor 2
#define M2_DIR1                           0x08
#define M2_DIR2                           0x04
#define M2_BRAKE                          0x0C

#define SET_PB                            0x0F
#define SET_PP                            0x03

static int M1 = 0;    //Motor State 1
static int M2 = 0;    //Motor State 2
static int pos = 0;

/****************RS232***************/
#define RDRF_MASK 0x20
#define RingMask  0x7F

#define BaudRate  9600
#define ClockRate 8000000
#define Divider (ClockRate/16/BaudRate)

static char recievebuff[3];


/***************SERVO***************/
#define SET  1U                       //define for ones
#define ZERO 0U                       //define for zeros

#define TCTL2_LOW  0x20               //defined for the TCTL2 register to be set low
#define TCTL2_HIGH 0x30               //defined for the TCTL2 register to be set high
#define ENABLE_TIMER_RTI  0x04        //defined for the enabling interrupts for timer channel 2
#define PRESCALER_VAL  0x03           //defined for the TSCR2 to set the prescaler to be 8 
#define TSCR_INIT   0xB0              //defined for the TSCR1 to enable the TCNT and to clear the timer flag

#define RC_gain     9
#define offset_time 650
#define RC_PERIOD   20000U            //period of the RC servo 

/**************STEPPER***************/
#define MOTOR_A                  0x80
#define MOTOR_AC                 0xA0
#define MOTOR_C                  0x20
#define MOTOR_BC                 0x60
#define MOTOR_B                  0x40
#define MOTOR_BD                 0x50
#define MOTOR_D                  0x10
#define MOTOR_AD                 0x90

#define REVERSE                  (-1)
#define STEPPER_MASK             0x07
#define PORT_T_WRITE_MASK        0xF0
#define RTI_PERIOD               0x49                                                   // Sets RTI period to 5.12mS or 200Hz (Look up tabel in slides)
#define LIMIT_SWITCH_MASK        0xC0

unsigned char stepper_speed;
unsigned char rti_count;
unsigned char stepper_array[] = {MOTOR_A,MOTOR_AC,MOTOR_C,MOTOR_BC,MOTOR_B,MOTOR_BD,MOTOR_D,MOTOR_AD};        // array with the different outputs required for the stepper motor.
unsigned char state;                                                                                          // array index
char step_type;                                                                                               // full step or halfstep, forwards or backwards
char limit_switch;