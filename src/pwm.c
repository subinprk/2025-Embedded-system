// pwm.c
// Basic motor control for Double L9100S using digital pins

#include "../include/pwm.h"

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

void task_pwm_update(void)
{
    sched_flags.pwm_due = false;

    static uint8_t motor_phase = 0;
    switch (motor_phase) {
        case 0:
            motorA_forward();
            motorB_forward();
            break;
        case 1:
            motorA_stop();
            motorB_stop();
            break;
        case 2:
            motorA_backward();
            motorB_backward();
            break;
        case 3:
            motorA_forward();
            motorB_backward();
            break;
    }
    motor_phase = (motor_phase + 1) & 0x03;
}