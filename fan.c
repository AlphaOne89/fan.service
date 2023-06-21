// gcc -Wall -Werror -Wpedantic fan.c -o fan.binary -lwiringPi -lcrypt -lrt -lm

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

int main( void )
{
    if ( wiringPiSetup() == -1 )
    {
        return 1;
    }

    FILE * handle;
    char * buffer = NULL;
    size_t buffer_length = 0;
    ssize_t bytes_read = 0;
    unsigned int fan_pin = 25;
    unsigned int fan_value = 0;
    softPwmCreate( fan_pin, 0, 100 );

    for ( ;; )
    {
        buffer = NULL;
        buffer_length = 0;
        bytes_read = 0;
        handle = fopen( "/sys/class/thermal/thermal_zone0/temp", "r" );
        bytes_read = getdelim( &buffer, &buffer_length, '\0', handle );
        fclose( handle );

        if ( bytes_read != -1 )
        {
            int temperature_value = atoi( buffer );
            fan_value = temperature_value < 65000 ? 0 : fan_value;
            fan_value = temperature_value > 70000 ? 25 : fan_value;
            fan_value = temperature_value > 73333 ? 50 : fan_value;
            fan_value = temperature_value > 76666 ? 75 : fan_value;
            fan_value = temperature_value > 79999 ? 100 : fan_value;
            softPwmWrite( fan_pin, fan_value );
            // printf( "%d -- %d\n", temperature_value, fan_value );
        }

        delay( 10000 );
    }

    return 0;
}
