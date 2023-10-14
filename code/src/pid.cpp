#include "pid.hpp"
#include "log.hpp"


PidControlClass::PidControlClass(long double Kp, long double Ki, long double Kd, long double setpoint)
    : Kp(Kp), Ki(Ki), Kd(Kd), setpoint(setpoint) 
{
    tau = Kd / (Kp * PID_N);

}

double PidControlClass::getSteeringAngle(double measuredValue, clock_t recordClock) {
    records[2] = records[1];
    records[1] = records[0];
    records[0] = recordClock;

    errors[2] = errors[1];
    errors[1] = errors[0];
    errors[0] = setpoint - measuredValue;

    // Propotional integral terms
    output += A0 * errors[0] + A1 * errors[1];
    return 0.0;
}