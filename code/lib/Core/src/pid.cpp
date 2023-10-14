#include "pid.hpp"
#include <ctime>
#include "log.hpp"


PidControlClass::PidControlClass(double Kp, double Ki, double Kd, double setpoint)
    : Kp(Kp), Ki(Ki), Kd(Kd), setpoint(setpoint) 
{
    this->tau = this->Kd / (this->Kp * PID_N);
    this->A1 = -this->Kp;
}

double PidControlClass::getSteeringAngle(double measuredValue, clock_t recordClock) {
    this->dt[2] = this->dt[1];
    this->dt[1] = this->dt[0];
    this->dt[0] = (double) ((recordClock - this->lastRecordClock) / (CLOCKS_PER_SEC/1000000));
    this->lastRecordClock = recordClock;

    this->errors[2] = this->errors[1];
    this->errors[1] = this->errors[0];
    this->errors[0] = setpoint - (double)measuredValue;

    // Adapt to the shift of the time steps
    this->A0 = this->Kp + this->Ki * this->dt[0];
    this->A0d = this->Kd / this->dt[0];
    this->A1d = -2.0 * this->Kd / this->dt[1];
    this->A2d = this->Kd / this->dt[2];
    this->alpha = this->dt[0] / (2 * this->tau);
    // Propotional integral terms
    this->output += this->A0 * this->errors[0] + this->A1 * this->errors[1];
    // Filtered derivative term
    this->d1 = this->d0;
    this->d0 = this->A0d * this->errors[0] + this->A1d * this->errors[1] + this->A2d * this->errors[2];
    this->fd1 = this->fd0;
    this->fd0 = ((this->alpha) / (this->alpha + 1)) * (this->d0 + this->d1) - ((this->alpha - 1) / (this->alpha + 1)) * this->fd1;
    this->output += this->fd0;

    return this->output;
}