#include "hw_spi.hpp"

Spi::Spi(SPI_TypeDef* spi, GPIO_TypeDef* csPort, uint16_t csPin, uint32_t readCmd = 0x00, uint32_t writeCmd = 0x01)
    : m_spi(spi), m_csPort(csPort), m_csPin(csPin), m_READ_CMD(readCmd), m_WRITE_CMD(writeCmd) {}

void Spi::setReadCmd(uint32_t readCmd) {
    m_READ_CMD = readCmd;
}

void Spi::setWriteCmd(uint32_t writeCmd) {
    m_WRITE_CMD = writeCmd;
}

void Spi::deselect(){
    LL_GPIO_SetOutputPin(m_csPort, m_csPin);
    LL_mDelay(1);
}

void Spi::select(){
    LL_GPIO_ResetOutputPin(m_csPort, m_csPin);
    LL_mDelay(1);
}

void Spi::write(uint8_t *buffer, uint8_t length)
{
    for(uint8_t i=0; i < length; i++)
    { 
        TransmitAndReceive(buffer[i]);
    }
}

void Spi::read(uint8_t data, uint8_t *buffer, uint8_t length)
{
  // read multiple bytes
  for(uint8_t i=0; i < length; i++)
  {
    buffer[i] = TransmitAndReceive(data);
  }
}

uint8_t Spi::TransmitAndReceive(uint8_t data) {
    // transmit
    LL_SPI_TransmitData8(m_spi, data);
    while(!LL_SPI_IsActiveFlag_TXE(m_spi));

    // receive
    while(!LL_SPI_IsActiveFlag_RXNE(m_spi));
    return LL_SPI_ReceiveData8(m_spi);
}