#ifndef F_CPU
#define F_CPU 16000000UL
#endif // F_CPU

#include <avr/io.h>
#include <util/delay.h>

#define DIG_LED_PORT    PORTB
#define DIG_LED_DIR     DDRB
#define PWM_LED_PORT    PORTD
#define PWM_LED_DIR     DDRD

#define DIG_LED1        PB0
#define DIG_LED2        PB1
#define PWM_LED1        PD6

#define BAUD            9600

void setupPwm(void)
{
    SREG &= ~0x80;

    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS00);
    TCNT0 = 0;
    OCR0A = 0;

    SREG |= 0x80;
}

void setPwm(uint8_t pwm)
{
    OCR0A = pwm;
}

void setupUart(void)
{
    /*
     * Set baud rate
     */
    uint32_t baudRate = F_CPU / BAUD / 16 - 1;
    UBRR0H = (uint8_t)(baudRate >> 8);
    UBRR0L = (uint8_t)baudRate;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uartTransmit(uint8_t data)
{
    while(!(UCSR0A & (1 << UDRE0)));

    UDR0 = data;
}

void uartRecive(uint8_t *data)
{
    while(!(UCSR0A & (1 << RXC0)));

    *data = UDR0;
}

int main(void)
{
    /*
     * Setup GPIO
     */
    DIG_LED_DIR = (1 << DIG_LED1) | (1 << DIG_LED2);
    PWM_LED_DIR = (1 << PWM_LED1);

    setupPwm();
    setupUart();

    /*
     * Data
     */
    uint8_t data[3];
    uint8_t dataIndex = 0;

    /*
     * Main loop
     */
    while(1)
    {
        /*
         * Communication
         */

        uartRecive(data + dataIndex++);
        
        if(dataIndex >= 3)
        {
            dataIndex = 0;

            if(data[0] == 'g' || data[0] == 'G')
            {
                switch(data[1])
                {
                    case 1:
                        uartTransmit((DIG_LED_PORT >> DIG_LED1) & 0x01);
                        break;
                    case 2:
                        uartTransmit((DIG_LED_PORT >> DIG_LED2) & 0x01);
                        break;
                    case 3:
                        uartTransmit(OCR0A);
                        break;
                    default:
                        uartTransmit('e');
                        break;
                }
            } else if(data[0] == 's' || data[0] == 'S')
            {
                switch(data[1])
                {
                    case 1:
                        if(data[2] > 0)
                            DIG_LED_PORT |= (1 << DIG_LED1);
                        else
                            DIG_LED_PORT &= ~(1 << DIG_LED1);
                        break;
                    case 2:
                        if(data[2] > 0)
                            DIG_LED_PORT |= (1 << DIG_LED2);
                        else
                            DIG_LED_PORT &= ~(1 << DIG_LED2);
                        break;
                    case 3:
                        setPwm(data[2]);
                        break;
                    default:
                        uartTransmit('e');
                        break;
                }
            }
        }
    }

    return 0;
}

