
// Board ====================

#define UNO  1
#define MEGA 2

#define ANA 1
#define DIG 2

#define MAX_PIN_IR_UNO  2
#define MAX_PIN_IR_MEGA 6

#define MAX_PIN_ANALOG_UNO   6
#define MAX_PIN_DIGITAL_UNO 14

#define MAX_PIN_ANALOG_MEGA  16
#define MAX_PIN_DIGITAL_MEGA 54

#define MAX_TOTAL_PINS 100

#define MAX_SOURCE_LINES 1000

#define D00  0
#define D01  1
#define D02  2
#define D03  3
#define D04  4
#define D05  5
#define D06  6
#define D07  7
#define D08  8
#define D09  9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

#define A00 14
#define A01 15
#define A02 16
#define A03 17
#define A04 18
#define A05 19

#define IR0  2
#define IR1  3
#define IR2 21
#define IR3 20
#define IR4 19
#define IR5 18


// Logical ===================
#define S_ON  1
#define S_OFF 0

#define S_YES 1
#define S_NO  0

#define HEAL 101
#define FAIL 102

#define S_OK  0
#define S_NOK 1

// Values ====================

#define INTERRUPT 3

#define BYTE 1
#define BIN  2
#define OCT  3
#define DEC  4
#define HEX  5

#define S_ADD    10
#define S_DELETE 20

// Interrupt =================

#define I_CHANGE  21
#define I_RISING  22
#define I_FALLING 23
#define I_LOW     24

#define FREE 0
#define RX   4
#define TX   5

#define S_RUN   1
#define S_ADMIN 2
#define S_STOP  1

#define S_FORWARD  1
#define S_BACKWARD 2

// Dimension
#define SCEN_MAX  2000
#define MAX_LOOPS 2000
#define LOG_MAX   200
#define LOG_TEXT_SIZE 220
#define MAX_READ 900
#define MAX_STEP 2000
#define MAX_LOOP 2000
#define MAX_SERIAL_BUFFER 900
#define SIZE_ROW 280

#define T_READ  1
#define T_WRITE 2

#define S_WRONG 99

int g_boardType = UNO;
void errorLog(const char msg[], int x);
int checkRange(int mode, const char check[], int value);

int max_digVal, min_digVal, max_anaVal, min_anaVal;
int max_digPin, min_digPin, max_anaPin, min_anaPin;
int max_irPin, min_irPin;
int max_pwm, min_pwm;
int max_totPin;

int max_steps;
int max_loops;

int inrpt[MAX_PIN_IR_MEGA];
int attached[MAX_PIN_IR_MEGA];
