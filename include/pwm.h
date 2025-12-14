// pwm.h
// Motor control interface for Double L9100S (two channels)
// Pins used:
//  - Motor A: PC0 (IN1), PC1 (IN2)
//  - Motor B: PC5 (IN1), PD0 (IN2)

#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <avr/io.h>
#include "../include/flags.h"

// Motor A pin masks (PORTC)
#define MOTOR_A_IN1_bm PIN0_bm
#define MOTOR_A_IN2_bm PIN1_bm

// Motor B pin definitions (IN1 on PORTC, IN2 on PORTD)
#define MOTOR_B_IN1_PORT PORTC
#define MOTOR_B_IN1_bm PIN5_bm
#define MOTOR_B_IN2_PORT PORTD
#define MOTOR_B_IN2_bm PIN0_bm

// Motor speed variables (accessed by ISR for PWM)
extern volatile uint8_t motorA_speed;
extern volatile uint8_t motorB_speed;

// Initialize motor driver pins (set as outputs, outputs low)
void motor_init(void);

// Set motor A speed/direction. Range 0..100 (forward only, no backward).
// Speed 100 = always on, 50 = 50% duty cycle, 0 = stop.
void motorA_set(int8_t speed);
void motorA_set_speed(uint8_t speed);
void motorA_forward(void);
void motorA_backward(void);  // Disabled: redirects to stop
void motorA_stop(void);

// Set motor B speed/direction. Range 0..100 (forward only, no backward).
void motorB_set(int8_t speed);
void motorB_set_speed(uint8_t speed);
void motorB_forward(void);
void motorB_backward(void);  // Disabled: redirects to stop
void motorB_stop(void);

void task_pwm_update(void);

#endif // PWM_H
