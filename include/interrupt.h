// interrupt.h
// Lightweight cooperative scheduler driven by TCA0 1 kHz tick

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"
#include "../include/mlxProcess.h"
#include "../include/pwm.h"
#include "../include/flags.h"

// Initialize timer and enable global interrupts
void scheduler_init(void);

// Run pending tasks flagged by the ISR; call from main loop
void scheduler_service_tasks(void);

void timer_init_1khz(void);



#endif // INTERRUPT_H
