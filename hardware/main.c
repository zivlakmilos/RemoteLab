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

int main(void)
{
    DIG_LED_DIR = (1 << DIG_LED1) | (1 << DIG_LED2);
    PWM_LED_DIR = (1 << PWM_LED1);

    while(1)
    {
        DIG_LED_PORT = (1 << DIG_LED1);
        _delay_ms(1000);
        DIG_LED_PORT = (1 << DIG_LED2);
        _delay_ms(1000);
        PWM_LED_PORT ^= (1 << PWM_LED1);
    }

    return 0;
}

