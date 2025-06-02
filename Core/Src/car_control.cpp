#include "car_control.hpp"

CarControl::CarControl(MotorController& motor, PWM& servo)
    : m_motor(motor), m_servo_pwm(servo) {
    m_servo_pwm.setDutyCycle(LL_TIM_CHANNEL_CH1, 1500);
    m_servo_pwm.enableChannel(LL_TIM_CHANNEL_CH1);
    m_servo_pwm.enableCounter();
    
    m_motor.startMotor();
    setMotorSpeed(0, 0); // Initialize motor speed to 0    
}

MotorController& CarControl::motor() {
    return m_motor;
}

PWM& CarControl::servoPwm() {
    return m_servo_pwm;
}

void CarControl::setServoPosition(uint32_t value) {
    m_servo_pwm.setDutyCycle(LL_TIM_CHANNEL_CH1, value);
}

void CarControl::setMotorSpeed(uint16_t ch1, uint16_t ch2) {
    m_motor.updateSpeed(ch1, ch2);
}