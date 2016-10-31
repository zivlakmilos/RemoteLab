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

    DIG_LED_PORT = (1 << DIG_LED1);

    while(1)
    {
        uint8_t i;

        for(i = 0; i < 255; i++)
        {
            setPwm(i);
            _delay_ms(10);
        }

        for(; i > 0; i--)
        {
            setPwm(i);
            _delay_ms(10);
        }
    }

    return 0;
}

