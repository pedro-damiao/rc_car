#ifndef __MW_MOTOR_CONTROLLER_HPP
#define __MW_MOTOR_CONTROLLER_HPP

#include "hw_spi.hpp"
#include "stm32f4xx_ll_spi.h"

#define register_STATUS1 0x01
#define register_STATUS2 0x02
#define register_STATUS3 0x02

#define DUMMY                 0x00
#define SPI_READ_WRITE_BIT    0x80

class MotorController {
public:
    MotorController(Spi spi, uint32_t readCmd, uint32_t writeCmd);
    uint8_t calculateCRC (uint8_t *data_buf, uint8_t len);
    uint8_t read_Status1(uint32_t *data_out);
private:
    Spi m_spi;
    uint32_t m_READ_CMD;
    uint32_t m_WRITE_CMD;
};

#endif /* __MW_MOTOR_CONTROLLER_HPP */
