// pwm.c
// Motor control with speed regulation for Double L9100S using digital pins

#include "../include/pwm.h"

// Motor speed storage (range: -100 to 100)
// positive = forward, negative = backward
static volatile int8_t motorA_speed = 0;
static volatile int8_t motorB_speed = 0;

// PWM counter for duty cycle (0-9, 10 steps per 20ms = 2ms per step)
static volatile uint8_t pwm_tick = 0;

// Helper function: get duty cycle from speed (0-10, where 10 = 100%)
static uint8_t get_duty_cycle(int8_t speed)
{
    uint8_t abs_speed = (speed < 0) ? -speed : speed;
    // Convert 0-100 to 0-10 duty cycles
    return (abs_speed * 10) / 100;
}

// Initialize motor pins as outputs and set them low (coast)
void motor_init(void)
{
    // Configure Motor A pins (PC0, PC1)
    PORTC.DIRSET = MOTOR_A_IN1_bm | MOTOR_A_IN2_bm;
    PORTC.OUTCLR = MOTOR_A_IN1_bm | MOTOR_A_IN2_bm;

    // Configure Motor B pins: IN1 on PC5, IN2 on PD0
    MOTOR_B_IN1_PORT.DIRSET = MOTOR_B_IN1_bm;
    MOTOR_B_IN1_PORT.OUTCLR = MOTOR_B_IN1_bm;
    MOTOR_B_IN2_PORT.DIRSET = MOTOR_B_IN2_bm;
    MOTOR_B_IN2_PORT.OUTCLR = MOTOR_B_IN2_bm;
}

// Motor A control (digital only)
void motorA_forward(void)
{
    PORTC.OUTSET = MOTOR_A_IN1_bm;
    PORTC.OUTCLR = MOTOR_A_IN2_bm;
}

void motorA_backward(void)
{
    PORTC.OUTCLR = MOTOR_A_IN1_bm;
    PORTC.OUTSET = MOTOR_A_IN2_bm;
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
        motorA_backward();
    } else {
        motorA_stop();
    }
}

// Store motor A speed for PWM control (range -100 to 100)
void motorA_set_speed(int8_t speed)
{
    // Clamp to valid range
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    motorA_speed = speed;
}

// Motor B control (digital only)
void motorB_forward(void)
{
    MOTOR_B_IN1_PORT.OUTSET = MOTOR_B_IN1_bm;
    MOTOR_B_IN2_PORT.OUTCLR = MOTOR_B_IN2_bm;
}

void motorB_backward(void)
{
    MOTOR_B_IN1_PORT.OUTCLR = MOTOR_B_IN1_bm;
    MOTOR_B_IN2_PORT.OUTSET = MOTOR_B_IN2_bm;
}

void motorB_stop(void)
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
        motorB_backward();
    } else {
        motorB_stop();
    }
}

// Store motor B speed for PWM control (range -100 to 100)
void motorB_set_speed(int8_t speed)
{
    // Clamp to valid range
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    motorB_speed = speed;
}

// PWM update task: called every 20ms from ISR
// Implements duty-cycle based speed control by toggling motor on/off
void task_pwm_update(void)
{
    sched_flags.pwm_due = false;

    pwm_tick++;
    if (pwm_tick >= 10) pwm_tick = 0;  // 10 steps per 20ms = 2ms per step

    // Motor A PWM control
    uint8_t duty_a = get_duty_cycle(motorA_speed);
    if (motorA_speed > 0) {
        // Forward: ON for duty_a steps, OFF for (10-duty_a) steps
        if (pwm_tick < duty_a) {
            motorA_forward();
        } else {
            motorA_stop();
        }
    } else if (motorA_speed < 0) {
        // Backward: ON for duty_a steps, OFF for (10-duty_a) steps
        if (pwm_tick < duty_a) {
            motorA_backward();
        } else {
            motorA_stop();
        }
    } else {
        motorA_stop();
    }

    // Motor B PWM control
    uint8_t duty_b = get_duty_cycle(motorB_speed);
    if (motorB_speed > 0) {
        // Forward: ON for duty_b steps, OFF for (10-duty_b) steps
        if (pwm_tick < duty_b) {
            motorB_forward();
        } else {
            motorB_stop();
        }
    } else if (motorB_speed < 0) {
        // Backward: ON for duty_b steps, OFF for (10-duty_b) steps
        if (pwm_tick < duty_b) {
            motorB_backward();
        } else {
            motorB_stop();
        }
    } else {
        motorB_stop();
    }
}