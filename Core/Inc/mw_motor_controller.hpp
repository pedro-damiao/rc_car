#ifndef __MW_MOTOR_CONTROLLER_HPP
#define __MW_MOTOR_CONTROLLER_HPP

#include "hw_spi.hpp"
#include "stm32f4xx_ll_spi.h"

class MotorController {
public:
    MotorController(Spi spi);
    uint8_t calculateCRC (uint8_t *data_buf, uint8_t len);

private:
    Spi m_spi;
};

#endif /* __MW_MOTOR_CONTROLLER_HPP */
