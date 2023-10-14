#include "pid.hpp"
#include "log.hpp"


PidControlClass::PidControlClass(long double Kp, long double Ki, long double Kd, long double setpoint)
    : Kp(Kp), Ki(Ki), Kd(Kd), setpoint(setpoint) 
{
    tau = Kd / (Kp * PID_N);
    A1 = -Kp;
}

long double PidControlClass::getSteeringAngle(double measuredValue, clock_t recordClock) {
    dt[2] = dt[1];
    dt[1] = dt[0];
    dt[0] = (long double) ((recordClock - lastRecordClock) / CLOCKS_PER_SEC);
    lastRecordClock = recordClock;

    errors[2] = errors[1];
    errors[1] = errors[0];
    errors[0] = setpoint - (long double)measuredValue;

    // Adapt to the shift of the time steps
    A0 = Kp + Ki * dt[0];
    A0d = Kd / dt[0];
    A1d = -2.0 * Kd / dt[1];
    A2d = Kd / dt[2];
    alpha = dt[0] / (2 * tau);
    // Propotional integral terms
    output += A0 * errors[0] + A1 * errors[1];

    // Filtered derivative term
    d1 = d0;
    d0 = A0d * errors[0] + A1d * errors[1] + A2d * errors[2];
    fd1 = fd0;
    fd0 = ((alpha) / (alpha + 1)) * (d0 + d1) - ((alpha - 1) / (alpha + 1)) * fd1;
    output += fd0;

    return output;
}