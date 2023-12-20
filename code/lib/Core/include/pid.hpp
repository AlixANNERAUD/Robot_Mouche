#ifndef PID_CONTROL
#define PID_CONTROL

#include "time.h"

#define PID_N 5 

class PidControlClass
{
private:
    double Kp, Ki, Kd;
    double setpoint;
    double integral = 0.0;
    double previousError = 0.0;
    clock_t lastRecordClock = 0;
    

public:
    PidControlClass(double Kp, double Ki, double Kd, double setpoint);
    
    void updateConstants(double Kp, double Ki, double Kd);
    double getSteering(double measuredValue, clock_t recordClock);
    
};


/* Pseudocode
A0 := Kp + Ki*dt
A1 := -Kp
error[2] := 0 // e(t-2)
error[1] := 0 // e(t-1)
error[0] := 0 // e(t)
output := u0  // Usually the current value of the actuator
A0d = Kd/dt
A1d = - 2.0*Kd/dt
A2d = Kd/dt
N := 5
tau := Kd / (Kp*N) // IIR filter time constant
alpha = dt / (2*tau)
d0 := 0
d1 := 0
fd0 := 0
fd1 := 0
loop:
    error[2] := error[1]
    error[1] := error[0]
    error[0] := setpoint − measured_value
    // PI
    output := output + A0 * error[0] + A1 * error[1]
    // Filtered D
    d1 = d0
    d0 = A0d * error[0] + A1d * error[1] + A2d * error[2]
    fd1 = fd0
    fd0 = ((alpha) / (alpha + 1)) * (d0 + d1) - ((alpha - 1) / (alpha + 1)) * fd1
    output := output + fd0      
    wait(dt)
    goto loop
*/

#endif // !PID_CONTROL
