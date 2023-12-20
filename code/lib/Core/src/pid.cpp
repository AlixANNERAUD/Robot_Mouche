#include "pid.hpp"
#include <ctime>
#include "log.hpp"
#include <cmath>

using namespace std;

PidControlClass::PidControlClass(double Kp, double Ki, double Kd, double setpoint)
    : Kp(Kp), Ki(Ki), Kd(Kd), setpoint(setpoint) 
{
    this->lastRecordClock = clock();
}

void PidControlClass::updateConstants(double Kp, double Ki, double Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->integral = 0.0;
    this->previousError = 0.0;
}

double PidControlClass::getSteering(double measuredValue, clock_t recordClock) {
    double dt = (double)(recordClock - this->lastRecordClock) / CLOCKS_PER_SEC;
    this->lastRecordClock = recordClock;

    double error = this->setpoint - measuredValue;
    double proportional = error;
    double integral = this->integral + error * dt;
    double derivative = (error - this->previousError) / dt;
    double output = this->Kp * proportional + this->Ki * integral + this->Kd * derivative;
    this->previousError = error;

    return output;
}

/*
previous_error := 0
integral := 0
loop:
    error := setpoint − measured_value
    proportional := error;
    integral := integral + error × dt
    derivative := (error − previous_error) / dt
    output := Kp × proportional + Ki × integral + Kd × derivative
    previous_error := error
    wait(dt)
    goto loop
*/