#ifndef __HW_SPI_HPP
#define __HW_SPI_HPP

#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_utils.h"

class Spi {
public:
    Spi(SPI_TypeDef* spi, GPIO_TypeDef* csPort, uint16_t csPin, uint32_t readCmd, uint32_t writeCmd);

    void write(uint8_t *buffer, uint8_t length);
    void read(uint8_t data, uint8_t *buffer, uint8_t length);
    void setReadCmd(uint32_t readCmd);
    void setWriteCmd(uint32_t writeCmd);
    void select();
    void deselect();

private:
    uint8_t TransmitAndReceive(uint8_t data);

    SPI_TypeDef* m_spi;
    GPIO_TypeDef* m_csPort;
    uint16_t m_csPin;

    uint32_t m_READ_CMD;
    uint32_t m_WRITE_CMD;
};

#endif /* __HW_SPI_HPP */
