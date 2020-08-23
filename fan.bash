#!/usr/bin/bash

    ### Raspberry Pi >> Bash Script >> Fan Control

    fan_pin=26

    fan_value=0

    trap service_destruct exit

    function service_destruct
    {
        echo "0" > /sys/class/gpio/gpio${fan_pin}/value

        echo "${fan_pin}" > /sys/class/gpio/unexport
    }

    echo "${fan_pin}" > /sys/class/gpio/export

    echo "out" > /sys/class/gpio/gpio${fan_pin}/direction

    while [ true ]; do

        temperature_value=$(cat /sys/class/thermal/thermal_zone0/temp)

        if [ ${temperature_value} -le 40000 ]; then

            fan_value=0

            echo "0" > /sys/class/gpio/gpio${fan_pin}/value

        fi

        if [ ${temperature_value} -gt 50000 ]; then

            fan_value=1

            echo "1" > /sys/class/gpio/gpio${fan_pin}/value

        fi

        echo "${temperature_value} -- ${fan_value}"

        sleep 10

    done
