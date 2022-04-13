
//------------Controller_Init------------
// Initialize the periodic interrupt (Timer1A)
// 
// Input: none
// Output: none
void Controller_Init(void);

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