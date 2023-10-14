#ifndef PID_CONTROL
#define PID_CONTROL

#include "time.h"

#define PID_N 5 

class PidControlClass
{
private:
    long double errors[3] = {0.0, 0.0, 0.0};
    long double A0, A1;
    long double A0d, A1d, A2d;
    long double d0, d1 = 0.0;
    long double fd0, fd1 = 0.0;
    long double Kp, Ki, Kd;
    long double setpoint;
    long double output;
    long double alpha;
    long double tau;
    
    // time step
    clock_t dt[3] = {0, 0, 0};
    

public:
    PidControlClass() = delete;
    PidControlClass(long double Kp, long double Ki, long double Kd, long double setpoint);
    ~PidControlClass();

    double getSteeringAngle(double measuredValue, clock_t recordClock);
};





#endif // !PID_CONTROL