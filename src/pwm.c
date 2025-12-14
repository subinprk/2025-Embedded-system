// pwm.c
// Motor control with speed regulation for Double L9100S using digital pins
#include <stdio.h>
#include "../include/pwm.h"
#include "../include/uart.h"


// Motor speed storage (range: 0 to 100, forward only)
// Made non-static so ISR can access them
volatile uint8_t motorA_speed = 40;  // Start at 40% for visible PWM effect
volatile uint8_t motorB_speed = 60; // Start at 60% for visible PWM effect 

// Helper function: get duty cycle from speed (0-10, where 10 = 100%)
static uint8_t get_duty_cycle(uint8_t speed)
{
    // Convert 0-100 to 0-10 duty cycles
    // Add rounding: (speed * 10 + 50) / 100
    if (speed > 100) speed = 100;
    else if (speed < 5 && speed > 0) return 5;
    else if (speed == 0) return 0;
    return ((uint16_t)speed * 10 + 50) / 100;
}

// Initialize motor pins as outputs and set them low (coast)
void motor_init(void)
{
    // Configure Motor A pins (PC0, PC1)
    PORTC.DIRSET = MOTOR_A_IN1_bm | MOTOR_A_IN2_bm;
    PORTC.OUTCLR = MOTOR_A_IN1_bm | MOTOR_A_IN2_bm;

    // Configure Motor B pins: IN1 on PC5, IN2 on PD0
    MOTOR_B_IN1_PORT.DIRSET = MOTOR_B_IN1_bm;
    // MOTOR_B_IN1_PORT.OUTCLR = MOTOR_B_IN1_bm;
    MOTOR_B_IN2_PORT.DIRSET = MOTOR_B_IN2_bm;
    // MOTOR_B_IN2_PORT.OUTCLR = MOTOR_B_IN2_bm;
}

// Motor A control (digital only)
void motorA_forward(void)
{
    PORTC.OUTSET = MOTOR_A_IN1_bm;
    PORTC.OUTCLR = MOTOR_A_IN2_bm;
}

void motorA_backward(void)
{
    // Backward disabled: just stop
    motorA_stop();
}

void motorA_stop(void)
{
    // coast: both low
    PORTC.OUTCLR = MOTOR_A_IN1_bm | MOTOR_A_IN2_bm;
}

void motorA_set(int8_t speed)
{
    if (speed > 0) {
        motorA_forward();
    } else if (speed < 0) {
        motorA_stop();
    } else {
        motorA_stop();
    }
}

// Store motor A speed for PWM control (range 0 to 100, forward only)
void motorA_set_speed(uint8_t speed)
{
    // Clamp to valid range
    if (speed > 100) speed = 100;
    motorA_speed = speed;
}

// Motor B control (digital only)
void motorB_stop(void)
{
    MOTOR_B_IN1_PORT.OUTSET = MOTOR_B_IN1_bm;
    MOTOR_B_IN2_PORT.OUTCLR = MOTOR_B_IN2_bm;
}

void motorB_backward(void)
{
    // Backward disabled: just stop
    motorB_stop();
}

void motorB_forward(void)
{
    // coast: both low
    MOTOR_B_IN1_PORT.OUTCLR = MOTOR_B_IN1_bm;
    MOTOR_B_IN2_PORT.OUTCLR = MOTOR_B_IN2_bm;
}

void motorB_set(int8_t speed)
{
    if (speed > 0) {
        motorB_forward();
    } else if (speed < 0) {
        motorB_stop();
    } else {
        motorB_stop();
    }
}

// Store motor B speed for PWM control (range 0 to 100, forward only)
void motorB_set_speed(uint8_t speed)
{
    // Clamp to valid range
    if (speed > 100) speed = 100;
    motorB_speed = speed;
}

// PWM update task: now only for debug logging (actual PWM is in ISR)
void task_pwm_update(void)
{
    sched_flags.pwm_due = false;

    // Debug: report motor speeds
    char buf[60];
    snprintf(buf, sizeof(buf), "[PWM] A:%u B:%u\\r\\n", motorA_speed, motorB_speed);
    USART2_sendString(buf);
}