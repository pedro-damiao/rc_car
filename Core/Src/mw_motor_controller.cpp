#include "mw_motor_controller.hpp"

MotorController::MotorController(Spi spi, uint32_t readCmd, uint32_t writeCmd)
    : m_spi(spi), m_READ_CMD(readCmd), m_WRITE_CMD(writeCmd) {
        m_spi.setReadCmd(readCmd);
        m_spi.setWriteCmd(writeCmd);
    }

uint8_t MotorController::read_Status1(uint32_t *data_out)
{
    uint8_t write_buf[5] = {0};
    uint8_t read_buf[5] = {0};
    write_buf[0] = register_STATUS1 & (~SPI_READ_WRITE_BIT);
    write_buf[1] = DUMMY;
    write_buf[2] = DUMMY;
    write_buf[3] = DUMMY;
    write_buf[4] = calculateCRC(write_buf, 4);

    m_spi.select();
    m_spi.write(write_buf, 5);
    m_spi.deselect();
    
    LL_mDelay(1);

    m_spi.select();
    m_spi.read(DUMMY, read_buf, 5);
    m_spi.deselect();

    *data_out = ( ( uint32_t ) (read_buf[ 1 ] << 16) ) | ( ( uint16_t ) (read_buf[ 2 ] << 8) ) | read_buf[ 3 ];

    if (( write_buf[ 0 ] != read_buf[ 0 ] ) || ( read_buf[ 4 ] != calculateCRC ( read_buf, 4 ) ) ) {
        return 0;
    }
    else {
        return 1;
    }
}

uint8_t MotorController::calculateCRC ( uint8_t *data_buf, uint8_t len )
{
    uint8_t crc = 0x00;
    for ( uint8_t byte_cnt = 0; byte_cnt < len; byte_cnt++ ) 
    {
        crc ^= data_buf[ byte_cnt ];
        for ( uint8_t bit_cnt = 0; bit_cnt < 8; bit_cnt++ ) 
        {
            if ( crc & 0x80 )
            {    
                crc = ( uint8_t ) ( ( crc << 1 ) ^ 0x1D );
            }
            else
            {
                crc <<= 1;
            }    
        }
    }
    return crc;
}