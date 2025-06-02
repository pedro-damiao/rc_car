#include "hw_pwm.hpp"
#include "stm32f4xx_ll_tim.h"

PWM::PWM(TIM_TypeDef* TIMx) : m_timer(TIMx) {}

void PWM::setDutyCycle(uint32_t channel, uint32_t value) {
    switch(channel) {
        case LL_TIM_CHANNEL_CH1:
            LL_TIM_OC_SetCompareCH1(m_timer, value);
            break;
        case LL_TIM_CHANNEL_CH2:
            LL_TIM_OC_SetCompareCH2(m_timer, value);
            break;
        case LL_TIM_CHANNEL_CH3:
            LL_TIM_OC_SetCompareCH3(m_timer, value);
            break;
        case LL_TIM_CHANNEL_CH4:
            LL_TIM_OC_SetCompareCH4(m_timer, value);
            break;
    }
}

void PWM::enableChannel(uint32_t channel) {
    LL_TIM_CC_EnableChannel(m_timer, channel);
}

void PWM::enableCounter() {
    LL_TIM_EnableCounter(m_timer);
}