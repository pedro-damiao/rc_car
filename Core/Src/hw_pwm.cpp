#include "hw_pwm.hpp"
#include "stm32f4xx_ll_tim.h"

PWM::PWM(TIM_TypeDef* TIMx) : m_timer(TIMx) {}

void PWM::setDutyCycle(uint16_t ch1, uint16_t ch2) {
    LL_TIM_OC_SetCompareCH1(m_timer, ch1);
    LL_TIM_OC_SetCompareCH2(m_timer, ch2);
}

void PWM::enable() {
    LL_TIM_CC_EnableChannel(m_timer, LL_TIM_CHANNEL_CH1);
    LL_TIM_CC_EnableChannel(m_timer, LL_TIM_CHANNEL_CH2);
    LL_TIM_EnableCounter(m_timer);
}