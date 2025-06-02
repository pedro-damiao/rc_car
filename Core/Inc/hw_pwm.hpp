#ifndef __HW_PWM_HPP
#define __HW_PWM_HPP

#include "stm32f4xx_ll_tim.h" // For LL_TIM_* functions

class PWM {
public:
    PWM(TIM_TypeDef* TIMx);
    void setDutyCycle(uint32_t channel, uint32_t value);
    void enableChannel(uint32_t channel);
    void enableCounter();
private:
    TIM_TypeDef* m_timer;
};

#endif /* _HW_PWM_HPP */
