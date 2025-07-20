#ifndef __CAR_CONTROL_HPP
#define __CAR_CONTROL_HPP

#include "mw_motor_controller.hpp"
#include "hw_pwm.hpp"

class CarControl {
public:
    CarControl(MotorController& motor, PWM& servo);

    MotorController& motor();
    PWM& servoPwm();

    void setServoPosition(uint32_t value);
    void setMotorSpeed(uint16_t ch1, uint16_t ch2);

    MotorController& m_motor;

private:
    PWM& m_servo_pwm;
};

#endif // __CAR_CONTROL_HPP