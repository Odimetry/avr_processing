/*
This code is written by Alfred kim.

Made for atmega128, and CodevisionAVR
////////////////////////////////////////////
Before uploading, open the mega128.h file and
Add this line.
#define TCNT3 (*(unsigned int *) 0x88)
///////////////////////////////////////////

///////////////////////////////////////////
Every 'char' datatype is signed.
Project - configure - C complier - char is unsigned uncheck.
///////////////////////////////////////////
*/

#include <mega128a.h>
#include <delay.h>

#define Echo PINC.2

void inituart(void); // UART setup
char getnum(void); // Data Receive function
void initservo(void); // Servo setup
void com1(char*, char*); // Collect mouse position
char getEcho(void); // Data Receive from ultrasonic sensor
void putnum(char); // Data Transmit

void main(void)
{
    char x, y, range, mode;
    unsigned int delaying, old;

    x = y = mode = 0; // reset position and mode
    delaying = old = 0;

    DDRC = 0x01; // registor connected to ultrasonic sensor. 0 pin output
    PORTC = 0x04; // 2 pin Pull-up activate

    DDRB = 0x60; // registor connected to Servo. 5, 6 pin output
    PORTB = 0x00;

    DDRF = 0x00; // registor connected to switch. 1, 3 pin input
    PORTF = 0x0A; // 1, 3 pin Pull-up activate

    inituart(); // initiate UART
    initservo(); // initiate TIM1 for servo

    TCCR3A = 0x00; // TIM3 for ultrasonic sensor
    TCCR3B = 0x00; // reset registor

    #asm("sei") // activate global interrupts

        while (1)
        {

            if (delaying % 100 == 0) // Every 100ms
                range = getEcho(); // Read the ultrasonic data

            if (!PINF.1) // when the switch is pushed
            {
                mode = 1; // change mode
                old = delaying; // save the time when mode changed
                OCR1B = 375; // set Servo2 angle
            }

            else if (!PINF.3) // when the switch is pushed
            {
                mode = 0; // change mode
            }

            if (mode == 0)
            {
                com1(&x, &y); // receive mouse cordinate
                putnum(range); // transmit ultrasonic data
                OCR1A = x + 375; // set Servo1 angle
                OCR1B = y + 375; // set Servo2 angle
            }

            if (mode == 1)
            {
                com1(&x, &y);
                putnum(range);

                switch ((delaying - old) / 200) { 

                case 0:
                    OCR1A = 250;
                    break;
                case 1:
                    OCR1A = 375;
                    break;
                case 2:
                    OCR1A = 500;
                    break;
                case 3:
                    OCR1A = 375; 
                    break;
                case 4:
                    OCR1A = 250; 
                    old = delaying;
                    break;

                }
            }

            delaying++;
            delay_ms(1);
        }
}

void inituart(void) // UART setup
{
    UCSR0A = 0x00;
    UCSR0B = 0x18; // Rx, Tx activate
    UCSR0C = 0x06; // Data bit size is 8 bit
    UBRR0H = 0x00;
    UBRR0L = 0x67; // Baudrate 9600
}

char getnum(void) // data Receive function
{
    while (!(UCSR0A & 0x80)); // when the flag is on
    return UDR0;
}

void com1(char* A, char* B) // Communicate with Processing
{
    char a = getnum();
    if (!a) // if initial signal is 0
    {
        *A = getnum();
        *B = getnum(); // Store x, y cordinate
    }
}

char getEcho(void)
{
    char range = 0;

    PORTC = 0x05;
    delay_us(10);
    PORTC = 0x04;

    while (!Echo);
    TCNT3H = 0x00;
    TCNT3L = 0x00;
    TCCR3B = 0x02;

    while (Echo);
    TCCR3B = 0x00;
    range = TCNT3 * 0.0085;
    TCNT3 = 0x0000;
    return (range);
}

void initservo(void) // TIM1 for Servos
{
    TCCR1A = 0xA2; // A, B of TIM1 are comparision mode
    TCCR1B = 0x1B;
    ICR1 = 4999; // TOP value setup
    OCR1A = 375; // initial comparision value
    OCR1B = 375;
}

void putnum(char data) // Data transmit function
{
    while (!(UCSR0A & 0x20)); // when transmitting is ready
    UDR0 = data;
}
