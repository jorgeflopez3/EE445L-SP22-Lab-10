#include <stdint.h>

// ***************** Timer2A_Init ****************
// Activate Timer2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
//          priority 0 (highest) to 7 (lowest)
// Outputs: none
void Timer2A_Init(void(*task)(void), uint32_t period, uint32_t priority); 

//------------Calculate_Error------------
// Calculate the error between the desired
// speed and the actual speed 
// Input: int actualSpeed, int desiredSpeed
// Output: int
int Calculate_Error(int actualSpeed, int desiredSpeed);

//------------Estimate_MotorSpeed------------
// Make an estimate of motor speed in RPM vs.
// PWM duty cycle 
// Input: int dutyCycle
// Output: int
int Estimate_MotorSpeed(int dutyCycle); 