#include "hw_spi.hpp"

Spi::Spi(SPI_TypeDef* spi, GPIO_TypeDef* csPort, uint16_t csPin, uint32_t readCmd, uint32_t writeCmd)
    : m_spi(spi), m_csPort(csPort), m_csPin(csPin), m_READ_CMD(readCmd), m_WRITE_CMD(writeCmd) {}

void Spi::deselect(){
    LL_GPIO_SetOutputPin(m_csPort, m_csPin);
    LL_mDelay(1);
}

void Spi::select(){
    LL_GPIO_SetOutputPin(m_csPort, m_csPin);
    LL_mDelay(1);
}

uint8_t Spi::readByte(uint8_t address)
{
    uint8_t ret;

    select();
    TransmitAndReceive(m_READ_CMD); // 1st byte
    TransmitAndReceive(address);  // 2nd byte
    // transmit garbage byte to receive
    ret = TransmitAndReceive(0x0); // Receive
    deselect();

    return ret;
}

void Spi::read(uint8_t address, uint8_t *buffer, uint8_t length)
{
  // read multiple bytes
  for(uint8_t i=0; i < length; i++)
  {
    buffer[i] = readByte(address+i);
  }
}

void Spi::writeByte(uint8_t address, uint8_t data)
{
  select();
  TransmitAndReceive(m_WRITE_CMD);
  TransmitAndReceive(address);
  TransmitAndReceive(data);
  deselect();

  // wait write cycle time 5ms
  LL_mDelay(5);
}

uint8_t Spi::TransmitAndReceive(uint8_t data) {
    // transmit
    LL_SPI_TransmitData8(m_spi, data);
    while(!LL_SPI_IsActiveFlag_TXE(m_spi));

    // receive
    while(!LL_SPI_IsActiveFlag_RXNE(m_spi));
    return LL_SPI_ReceiveData8(m_spi);
}