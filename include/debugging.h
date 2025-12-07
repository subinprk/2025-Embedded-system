#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/debugging.h"
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"
#include "../include/mlxProcess.h"
#include "../include/pwm.h"

void initial_debugging(void);
void sensor_loop_debugging(int loop_count);
void pwm_loop_debugging(int loop_count);
#endif // DEBUGGING_H