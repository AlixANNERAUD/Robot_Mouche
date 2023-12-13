#include "driver.hpp"
#include "log.hpp"
#include <algorithm>
#include <chrono>
#include <array>
#include <thread>

using namespace std;

DriverClass::DriverClass(LiDARClass& lidar, MotorClass &left, MotorClass &right, QTRClass &qtr1, QTRClass &qtr2)
    : running(false), left(left), right(right), qtr1(qtr1), qtr2(qtr2), settings(settings), pid(settings.KP, settings.KI, settings.KD, 0.0), mode(RobotMode::Manual), lidar(lidar)
{
    this->speed = 0.0f;
    this->steering = 0.0f;
}

void DriverClass::start()
{
    if (this->running)
        return;

    std::thread(&DriverClass::run, this).detach();
}

void DriverClass::run()
{
    this->running = true;
    while (this->running)
    {
        this->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    this->left.setSpeed(0);
    this->right.setSpeed(0);
}

double DriverClass::computeLinePosition()
{
    std::array<clock_t, 3> arrayQtr1 = this->qtr1.getTimesElapsed();
    std::array<clock_t, 3> arrayQtr2 = this->qtr2.getTimesElapsed();

    // We put the values in an array to have a symetric formula 0 will be the center of the line
    std::array<double, 3> qtr1 = {(double)arrayQtr1[2], (double)arrayQtr1[1], (double)arrayQtr1[0]};
    std::array<double, 3> qtr2 = {(double)arrayQtr2[0], (double)arrayQtr2[1], (double)arrayQtr2[2]};

    double qtr1Sum, qtr2Sum = 0.0;

    for (int i = 0; i < 3; i++)
    {
        qtr1Sum += qtr1[i];
        qtr2Sum += qtr2[i];
    }

    for (int i = 0; i < 3; i++)
    {
        qtr1[i] /= qtr1Sum + qtr2Sum;
    }

    double qtr1Position = (qtr1[0] + qtr1[1] * 2.0 + qtr1[2] * 3.0);
    double qtr2Position = (qtr2[0] + qtr2[1] * 2.0 + qtr2[2] * 3.0);

    double position = -qtr1Position + qtr2Position;

    return position;
}

void DriverClass::update()
{
    this->linePosition = this->computeLinePosition();

    if (this->mode == RobotMode::LineFollower)
    {
        this->steering = std::clamp((float)(this->pid.getSteering(this->linePosition, clock()) / 1024.0), -1.0f, 1.0f); // NORMALIZE it from -1.0 to 1.0


        float speedLeft = std::min(this->speed - this->steering, 1.0f) * 1024.0f;
        float speedRight = std::min(this->speed - this->steering, 1.0f) * 1024.0f;

        if (this->mode == RobotMode::LineFollower && this->lidar.getDistance() < 100)
        {
            speedLeft = 0;
            speedRight = 0;
        }
        
        this->left.setSpeed((unsigned int)speedLeft);
        this->right.setSpeed((unsigned int)speedRight);
    
    }

}

void DriverClass::stop()
{
    this->running = false;
}

void DriverClass::setSpeed(float speed)
{
    if (speed >= 0)
    {
        this->speed = clamp(speed, 0.0f, 1.0f);
        this->right.set(MotorDirection::Forward, (unsigned int)(speed * 1024.0f));
        this->left.set(MotorDirection::Forward, (unsigned int)(speed * 1024.0f));
    }
    else
    {
        this->speed = clamp(-speed, 0.0f, 1.0f);
        this->right.set(MotorDirection::Backward, (unsigned int)(-speed * 1024.0f));
        this->left.set(MotorDirection::Backward, (unsigned int)(-speed * 1024.0f));
    }
}

void DriverClass::setDirection(float direction)
{
    this->steering = clamp(direction, -1.0f, 1.0f);
}

void DriverClass::updateGamepad(float direction, float speed)
{
    if (this->settings.mode == RobotMode::LineFollower)
        return;



    float velocity = speed;
    float rotation = direction;

    float left_speed = velocity - rotation;
    float right_speed = velocity + rotation;

    printf("Left speed : %f\n", left_speed);
    printf("Right speed : %f\n", right_speed);

    if (left_speed >= 0.0f) {
        this->left.set(MotorDirection::Forward, (unsigned int)(left_speed * 1024.0f));
    }
    else {
        this->left.set(MotorDirection::Backward, (unsigned int)(-left_speed * 1024.0f));
    }

    if (right_speed >= 0.0f) {
        this->right.set(MotorDirection::Forward, (unsigned int)(right_speed * 1024.0f));
    }
    else {
        this->right.set(MotorDirection::Backward, (unsigned int)(-right_speed * 1024.0f));
    }

    //this->setSpeed(speed);
    //this->setDirection(direction);
}

void DriverClass::updatePidConstants(SettingsClass settings)
{
    this->settings = settings;
    this->pid.updateConstants(settings.KP, settings.KI, settings.KD);
}
