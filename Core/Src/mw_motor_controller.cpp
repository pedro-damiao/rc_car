#include "mw_motor_controller.hpp"

static inline bool buffers_equal(const uint8_t *a, const uint8_t *b, int len) {
    for (int i = 0; i < len; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

MotorController::MotorController(Spi& spi, PWM& pwm, Gpio& enable, Gpio& sleep)
    : m_spi(spi), m_pwm(pwm), m_enable(enable), m_sleep(sleep) {}

uint8_t MotorController::read_register(uint32_t *data_out, uint8_t register_address)
{
    uint8_t write_buf[5] = {0};
    uint8_t read_buf[5] = {0};
    write_buf[0] = register_address & (~SPI_READ_WRITE_BIT);
    write_buf[1] = DUMMY;
    write_buf[2] = DUMMY;
    write_buf[3] = DUMMY;
    write_buf[4] = calculateCRC(write_buf, 4);

    m_spi.select();
    m_spi.write(write_buf, 5);
    m_spi.deselect();
    
    LL_mDelay(1);

    m_spi.select();
    m_spi.writeRead(DUMMY, read_buf, 5);
    m_spi.deselect();

    *data_out = ( ( uint32_t ) (read_buf[ 1 ] << 16) ) | ( ( uint16_t ) (read_buf[ 2 ] << 8) ) | read_buf[ 3 ];

    if (( write_buf[ 0 ] != read_buf[ 0 ] ) || ( read_buf[ 4 ] != calculateCRC ( read_buf, 4 ) ) ) {
        return 0;
    }
    else {
        return 1;
    }
}

uint8_t MotorController::write_register(uint32_t data_in, uint8_t register_address)
{
    uint8_t write_buf[5] = {0};
    uint8_t read_buf[5] = {0};
    write_buf[0] = register_address | SPI_READ_WRITE_BIT;
    write_buf[1] = ( uint8_t ) ( ( data_in >> 16 ) & 0xFF );
    write_buf[2] = ( uint8_t ) ( ( data_in >> 8 ) & 0xFF );
    write_buf[3] = ( uint8_t ) ( data_in & 0xFF );
    write_buf[4] = calculateCRC(write_buf, 4);

    m_spi.select();
    m_spi.write(write_buf, 5);
    m_spi.deselect();

    LL_mDelay(1);

    m_spi.select();
    m_spi.writeRead(DUMMY, read_buf, 5);
    m_spi.deselect();

    write_buf[ 0 ] &= ( ~SPI_READ_WRITE_BIT );
    if (( read_buf[ 0 ] & DCMOTOR12_ERROR_FLAG_BIT ) || ( !buffers_equal( write_buf, read_buf, 4 ) ) || ( read_buf[ 4 ] != calculateCRC ( read_buf, 4 ) ) ) {
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

uint8_t MotorController::startMotor(){
    m_pwm.enableChannel(LL_TIM_CHANNEL_CH1);
    m_pwm.enableChannel(LL_TIM_CHANNEL_CH2);
    m_pwm.enableCounter();
    m_spi.enable();

    write_register(0x00000007, register_CONFIG1);
    write_register(0xFFFFFFFF, register_STATUS1);
    
    m_enable.set();
    m_sleep.set();
    return 1;
}

void MotorController::updateSpeed(uint16_t ch1, uint16_t ch2) {
    m_pwm.setDutyCycle(LL_TIM_CHANNEL_CH1, ch1);
    m_pwm.setDutyCycle(LL_TIM_CHANNEL_CH2, ch2);
}