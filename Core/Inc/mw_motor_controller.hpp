#ifndef __MW_MOTOR_CONTROLLER_HPP
#define __MW_MOTOR_CONTROLLER_HPP

#include "hw_spi.hpp"
#include "stm32f4xx_ll_spi.h"
#include <stdint.h>

#define register_STATUS1 0x01
#define register_STATUS2 0x02
#define register_STATUS3 0x03
#define register_CONFIG1 0x04


#define DUMMY                       0x00
#define SPI_READ_WRITE_BIT          0x80
#define DCMOTOR12_ERROR_FLAG_BIT    0x80


class MotorController {
public:
    MotorController(Spi spi, uint32_t readCmd, uint32_t writeCmd);
    uint8_t calculateCRC (uint8_t *data_buf, uint8_t len);
    uint8_t read_register(uint32_t *data_out, uint8_t register_address);
    uint8_t write_register(uint32_t data_in, uint8_t register_address);

private:
    Spi m_spi;
    uint32_t m_READ_CMD;
    uint32_t m_WRITE_CMD;
};

#endif /* __MW_MOTOR_CONTROLLER_HPP */
