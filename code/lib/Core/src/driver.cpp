#include "driver.hpp"
#include "log.hpp"
#include <algorithm>
#include <chrono>
#include <array>
#include <thread>

using namespace std;

DriverClass::DriverClass(LiDARClass& lidar, MotorClass &left, MotorClass &right)
    : running(false), left(left), right(right), settings(settings), pid(settings.KP, settings.KI, settings.KD, 0.0), mode(RobotMode::Manual), lidar(lidar)
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
    // TODO get line position from file saved by python script

    int qtr1Sum = 0; // FIXME
    int qtr2Sum = 0; // FIXME
    double position = 0.0; // FIXME
    
    // Detect no line present 
    LOG_DEBUG("Driver", "SUM QTR : %f", (float)(qtr1Sum + qtr1Sum));
    if (qtr1Sum + qtr2Sum < 100)
    {
        position = this->lastPositionKnown;
    }
    else
    {
        this->lastPositionKnown = position;
    }

    return position;
}

void DriverClass::update()
{
    this->linePosition = this->computeLinePosition();
    LOG_DEBUG("Driver", "Line position : %f", this->linePosition);

    if (this->mode == RobotMode::LineFollower)
    {
        float steering = (float)this->pid.getSteering(this->linePosition, clock());
        LOG_DEBUG("Driver", "Steering before processing : %f", steering);
        this->steering = std::clamp(steering / 1024.0f, -1.0f, 1.0f); // NORMALIZE it from -1.0 to 1.0
        LOG_DEBUG("Driver", "Steering after processing : %f", this->steering);
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
