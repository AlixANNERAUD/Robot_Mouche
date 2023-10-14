#include <unity.h>
#include "pid.hpp"
#include <ctime>
#include "log.hpp"

void test_init() {
    // Positive error -> negative steering angle
    PidControlClass pid(1.0, 1.0, 1.0, 0.0);
    TEST_ASSERT(pid.getSteeringAngle(1.0, clock()) < 0.0); 
    // Negative error -> positive steering angle
    pid = PidControlClass(1.0, 1.0, 1.0, 0.0);
    TEST_ASSERT(pid.getSteeringAngle(-1.0, clock()) > 0.0);
    // No error -> no steering angle
    pid = PidControlClass(1.0, 1.0, 1.0, 0.0);
    TEST_ASSERT(pid.getSteeringAngle(0.0, clock()) == 0.0);
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_init);
    UNITY_END();
}
