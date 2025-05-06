#ifndef __GPIOPIN_HPP
#define __GPIOPIN_HPP

#include "stm32f4xx_ll_gpio.h"

class GpioPin {
public:
    GpioPin(GPIO_TypeDef* port, uint16_t pin);

    void set();
    void reset();
    void toggle();
    uint32_t read();

private:
    GPIO_TypeDef* m_port;
    uint16_t m_pin;
};

#endif /* __GPIOPIN_HPP */
