#include <cmath>

#include <MPU6050.h>

bool MPU6050::init(mpu6050_dps_t scale, mpu6050_range_t range) {
    // Reset calibrate values
    dg.XAxis = 0;
    dg.YAxis = 0;
    dg.ZAxis = 0;
    useCalibrate = false;

    // Reset threshold values
    tg.XAxis = 0;
    tg.YAxis = 0;
    tg.ZAxis = 0;
    actualThreshold = 0;

    // Check MPU6050 Who Am I Register
    if (fastRegister8(MPU6050_REG_WHO_AM_I) != 0x68) {
        return false;
    }

    // Set Clock Source
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);

    // Set Scale & Range
    setScale(scale);
    setRange(range);

    // Disable Sleep Mode
    setSleepEnabled(false);

    return true;
}

void MPU6050::setScale(mpu6050_dps_t scale) {
    switch (scale) {
        case MPU6050_SCALE_250DPS:
            dpsPerDigit = .007633f;
            break;
        case MPU6050_SCALE_500DPS:
            dpsPerDigit = .015267f;
            break;
        case MPU6050_SCALE_1000DPS:
            dpsPerDigit = .030487f;
            break;
        case MPU6050_SCALE_2000DPS:
            dpsPerDigit = .060975f;
            break;
        default:
            break;
    }

    uint8_t value = readRegister8(MPU6050_REG_GYRO_CONFIG);
    value &= 0b11100111u;
    value |= uint8_t(scale << 3u);
    writeRegister8(MPU6050_REG_GYRO_CONFIG, value);
}

mpu6050_dps_t MPU6050::getScale() {
    uint8_t value;
    value = readRegister8(MPU6050_REG_GYRO_CONFIG);
    value &= 0b00011000u;
    value >>= 3u;
    return (mpu6050_dps_t)value;
}

void MPU6050::setRange(mpu6050_range_t range) {
    switch (range) {
        case MPU6050_RANGE_2G:
            rangePerDigit = .000061f;
            break;
        case MPU6050_RANGE_4G:
            rangePerDigit = .000122f;
            break;
        case MPU6050_RANGE_8G:
            rangePerDigit = .000244f;
            break;
        case MPU6050_RANGE_16G:
            rangePerDigit = .0004882f;
            break;
        default:
            break;
    }

    uint8_t value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
    value &= 0b11100111u;
    value |= uint8_t(range << 3u);
    writeRegister8(MPU6050_REG_ACCEL_CONFIG, value);
}

mpu6050_range_t MPU6050::getRange() {
    uint8_t value;
    value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
    value &= 0b00011000u;
    value >>= 3u;
    return (mpu6050_range_t)value;
}

void MPU6050::setDHPFMode(mpu6050_dhpf_t dhpf) {
    uint8_t value;
    value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
    value &= 0b11111000u;
    value |= dhpf;
    writeRegister8(MPU6050_REG_ACCEL_CONFIG, value);
}

void MPU6050::setDLPFMode(mpu6050_dlpf_t dlpf) {
    uint8_t value;
    value = readRegister8(MPU6050_REG_CONFIG);
    value &= 0b11111000u;
    value |= dlpf;
    writeRegister8(MPU6050_REG_CONFIG, value);
}

void MPU6050::setClockSource(mpu6050_clockSource_t source) {
    uint8_t value;
    value = readRegister8(MPU6050_REG_PWR_MGMT_1);
    value &= 0b11111000u;
    value |= source;
    writeRegister8(MPU6050_REG_PWR_MGMT_1, value);
}

mpu6050_clockSource_t MPU6050::getClockSource() {
    uint8_t value;
    value = readRegister8(MPU6050_REG_PWR_MGMT_1);
    value &= 0b00000111u;
    return (mpu6050_clockSource_t)value;
}

bool MPU6050::getSleepEnabled() {
    return readRegisterBit(MPU6050_REG_PWR_MGMT_1, 6);
}

void MPU6050::setSleepEnabled(bool state) {
    writeRegisterBit(MPU6050_REG_PWR_MGMT_1, 6, state);
}

bool MPU6050::getIntZeroMotionEnabled() {
    return readRegisterBit(MPU6050_REG_INT_ENABLE, 5);
}

void MPU6050::setIntZeroMotionEnabled(bool state) {
    writeRegisterBit(MPU6050_REG_INT_ENABLE, 5, state);
}

bool MPU6050::getIntMotionEnabled() {
    return readRegisterBit(MPU6050_REG_INT_ENABLE, 6);
}

void MPU6050::setIntMotionEnabled(bool state) {
    writeRegisterBit(MPU6050_REG_INT_ENABLE, 6, state);
}

bool MPU6050::getIntFreeFallEnabled() {
    return readRegisterBit(MPU6050_REG_INT_ENABLE, 7);
}

void MPU6050::setIntFreeFallEnabled(bool state) {
    writeRegisterBit(MPU6050_REG_INT_ENABLE, 7, state);
}

uint8_t MPU6050::getMotionDetectionThreshold() {
    return readRegister8(MPU6050_REG_MOT_THRESHOLD);
}

void MPU6050::setMotionDetectionThreshold(uint8_t threshold)
{
    writeRegister8(MPU6050_REG_MOT_THRESHOLD, threshold);
}

uint8_t MPU6050::getMotionDetectionDuration() {
    return readRegister8(MPU6050_REG_MOT_DURATION);
}

void MPU6050::setMotionDetectionDuration(uint8_t duration) {
    writeRegister8(MPU6050_REG_MOT_DURATION, duration);
}

uint8_t MPU6050::getZeroMotionDetectionThreshold() {
    return readRegister8(MPU6050_REG_ZMOT_THRESHOLD);
}

void MPU6050::setZeroMotionDetectionThreshold(uint8_t threshold) {
    writeRegister8(MPU6050_REG_ZMOT_THRESHOLD, threshold);
}

uint8_t MPU6050::getZeroMotionDetectionDuration() {
    return readRegister8(MPU6050_REG_ZMOT_DURATION);
}

void MPU6050::setZeroMotionDetectionDuration(uint8_t duration) {
    writeRegister8(MPU6050_REG_ZMOT_DURATION, duration);
}

uint8_t MPU6050::getFreeFallDetectionThreshold() {
    return readRegister8(MPU6050_REG_FF_THRESHOLD);
}

void MPU6050::setFreeFallDetectionThreshold(uint8_t threshold) {
    writeRegister8(MPU6050_REG_FF_THRESHOLD, threshold);
}

uint8_t MPU6050::getFreeFallDetectionDuration() {
    return readRegister8(MPU6050_REG_FF_DURATION);
}

void MPU6050::setFreeFallDetectionDuration(uint8_t duration) {
    writeRegister8(MPU6050_REG_FF_DURATION, duration);
}

bool MPU6050::getI2CMasterModeEnabled() {
    return readRegisterBit(MPU6050_REG_USER_CTRL, 5);
}

void MPU6050::setI2CMasterModeEnabled(bool state) {
    writeRegisterBit(MPU6050_REG_USER_CTRL, 5, state);
}

void MPU6050::setI2CBypassEnabled(bool state) {
    return writeRegisterBit(MPU6050_REG_INT_PIN_CFG, 1, state);
}

bool MPU6050::getI2CBypassEnabled() {
    return readRegisterBit(MPU6050_REG_INT_PIN_CFG, 1);
}

void MPU6050::setAccelPowerOnDelay(mpu6050_onDelay_t delay)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_MOT_DETECT_CTRL);
    value &= 0b11001111u;
    value |= uint8_t(delay << 4u);
    writeRegister8(MPU6050_REG_MOT_DETECT_CTRL, value);
}

mpu6050_onDelay_t MPU6050::getAccelPowerOnDelay(void)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_MOT_DETECT_CTRL);
    value &= 0b00110000u;
    return (mpu6050_onDelay_t)(value >> 4u);
}

uint8_t MPU6050::getIntStatus(void)
{
    return readRegister8(MPU6050_REG_INT_STATUS);
}

Activites MPU6050::readActivites(void)
{
    uint8_t data = readRegister8(MPU6050_REG_INT_STATUS);

    a.isOverflow = (uint8_t(data >> 4u) & 1u);
    a.isFreeFall = (uint8_t(data >> 7u) & 1u);
    a.isInactivity = (uint8_t(data >> 5u) & 1u);
    a.isActivity = (uint8_t(data >> 6u) & 1u);
    a.isDataReady = (uint8_t(data >> 0u) & 1u);

    data = readRegister8(MPU6050_REG_MOT_DETECT_STATUS);

    a.isNegActivityOnX = (uint8_t(data >> 7u) & 1u);
    a.isPosActivityOnX = (uint8_t(data >> 6u) & 1u);

    a.isNegActivityOnY = (uint8_t(data >> 5u) & 1u);
    a.isPosActivityOnY = (uint8_t(data >> 4u) & 1u);

    a.isNegActivityOnZ = (uint8_t(data >> 3u) & 1u);
    a.isPosActivityOnZ = (uint8_t(data >> 2u) & 1u);

    return a;
}

Vector MPU6050::readRawAccel() {
    uint8_t * data = i2c.read(address, MPU6050_REG_ACCEL_XOUT_H, 6);

    ra.XAxis = (int16_t)(uint16_t(data[0] << 8u) | data[1]);
    ra.YAxis = (int16_t)(uint16_t(data[2] << 8u) | data[3]);
    ra.ZAxis = (int16_t)(uint16_t(data[4] << 8u) | data[5]);

    return ra;
}

Vector MPU6050::readNormalizeAccel() {
    readRawAccel();

    na.XAxis = ra.XAxis * rangePerDigit * 9.80665f;
    na.YAxis = ra.YAxis * rangePerDigit * 9.80665f;
    na.ZAxis = ra.ZAxis * rangePerDigit * 9.80665f;

    return na;
}

Vector MPU6050::readScaledAccel() {
    readRawAccel();

    na.XAxis = ra.XAxis * rangePerDigit;
    na.YAxis = ra.YAxis * rangePerDigit;
    na.ZAxis = ra.ZAxis * rangePerDigit;

    return na;
}


Vector MPU6050::readRawGyro() {
    uint8_t * data = i2c.read(address, MPU6050_REG_GYRO_XOUT_H, 6);

    rg.XAxis = (int16_t)(uint16_t(data[0] << 8u) | data[1]);
    rg.YAxis = (int16_t)(uint16_t(data[2] << 8u) | data[3]);
    rg.ZAxis = (int16_t)(uint16_t(data[4] << 8u) | data[5]);

    return rg;
}

Vector MPU6050::readNormalizeGyro() {
    readRawGyro();

    if (useCalibrate) {
        ng.XAxis = (rg.XAxis - dg.XAxis) * dpsPerDigit;
        ng.YAxis = (rg.YAxis - dg.YAxis) * dpsPerDigit;
        ng.ZAxis = (rg.ZAxis - dg.ZAxis) * dpsPerDigit;
    } else {
        ng.XAxis = rg.XAxis * dpsPerDigit;
        ng.YAxis = rg.YAxis * dpsPerDigit;
        ng.ZAxis = rg.ZAxis * dpsPerDigit;
    }

    if (actualThreshold == 0.0) {
        if (std::abs(ng.XAxis) < tg.XAxis) ng.XAxis = 0;
        if (std::abs(ng.YAxis) < tg.YAxis) ng.YAxis = 0;
        if (std::abs(ng.ZAxis) < tg.ZAxis) ng.ZAxis = 0;
    }

    return ng;
}

float MPU6050::readTemperature() {

    int16_t T = readRegister16(MPU6050_REG_TEMP_OUT_H);

    return float(T) / 340.0f + 36.53f;
}

int16_t MPU6050::getGyroOffsetX() {
    return readRegister16(MPU6050_REG_GYRO_XOFFS_H);
}

int16_t MPU6050::getGyroOffsetY() {
    return readRegister16(MPU6050_REG_GYRO_YOFFS_H);
}

int16_t MPU6050::getGyroOffsetZ() {
    return readRegister16(MPU6050_REG_GYRO_ZOFFS_H);
}

void MPU6050::setGyroOffsetX(int16_t offset) {
    writeRegister16(MPU6050_REG_GYRO_XOFFS_H, offset);
}

void MPU6050::setGyroOffsetY(int16_t offset) {
    writeRegister16(MPU6050_REG_GYRO_YOFFS_H, offset);
}

void MPU6050::setGyroOffsetZ(int16_t offset) {
    writeRegister16(MPU6050_REG_GYRO_ZOFFS_H, offset);
}

int16_t MPU6050::getAccelOffsetX() {
    return readRegister16(MPU6050_REG_ACCEL_XOFFS_H);
}

int16_t MPU6050::getAccelOffsetY() {
    return readRegister16(MPU6050_REG_ACCEL_YOFFS_H);
}

int16_t MPU6050::getAccelOffsetZ() {
    return readRegister16(MPU6050_REG_ACCEL_ZOFFS_H);
}

void MPU6050::setAccelOffsetX(int16_t offset) {
    writeRegister16(MPU6050_REG_ACCEL_XOFFS_H, offset);
}

void MPU6050::setAccelOffsetY(int16_t offset) {
    writeRegister16(MPU6050_REG_ACCEL_YOFFS_H, offset);
}

void MPU6050::setAccelOffsetZ(int16_t offset) {
    writeRegister16(MPU6050_REG_ACCEL_ZOFFS_H, offset);
}

// Calibrate algorithm
void MPU6050::calibrateGyro(uint8_t samples) {
    // Set calibrate
    useCalibrate = true;

    // Reset values
    float sumX = 0;
    float sumY = 0;
    float sumZ = 0;
    float sigmaX = 0;
    float sigmaY = 0;
    float sigmaZ = 0;

    // Read n-samples
    for (uint8_t i = 0; i < samples; ++i) {
        readRawGyro();
        sumX += rg.XAxis;
        sumY += rg.YAxis;
        sumZ += rg.ZAxis;

        sigmaX += rg.XAxis * rg.XAxis;
        sigmaY += rg.YAxis * rg.YAxis;
        sigmaZ += rg.ZAxis * rg.ZAxis;

        for (volatile uint32_t j = 0; j < 10000; j++) { }
    }

    // Calculate delta vectors
    dg.XAxis = sumX / static_cast<float>(samples);
    dg.YAxis = sumY / static_cast<float>(samples);
    dg.ZAxis = sumZ / static_cast<float>(samples);

    // Calculate threshold vectors
    th.XAxis = sqrt((sigmaX / 50) - (dg.XAxis * dg.XAxis));
    th.YAxis = sqrt((sigmaY / 50) - (dg.YAxis * dg.YAxis));
    th.ZAxis = sqrt((sigmaZ / 50) - (dg.ZAxis * dg.ZAxis));

    // If already set threshold, recalculate threshold vectors
    if (actualThreshold > 0) {
        setThreshold(actualThreshold);
    }
}

// Get current threshold value
uint8_t MPU6050::getThreshold() {
    return actualThreshold;
}

// Set treshold value
void MPU6050::setThreshold(uint8_t multiple) {
    if (multiple > 0) {
        // If not calibrated, need calibrate
        if (!useCalibrate) {
            calibrateGyro();
        }

        // Calculate threshold vectors
        tg.XAxis = th.XAxis * static_cast<float>(multiple);
        tg.YAxis = th.YAxis * static_cast<float>(multiple);
        tg.ZAxis = th.ZAxis * static_cast<float>(multiple);
    } else {
        // No threshold
        tg.XAxis = 0;
        tg.YAxis = 0;
        tg.ZAxis = 0;
    }

    // Remember old threshold value
    actualThreshold = multiple;
}

uint8_t MPU6050::fastRegister8(uint8_t reg) {
    return i2c.read(address, reg );
}

uint8_t MPU6050::readRegister8(uint8_t reg) {
    return i2c.read(address, reg );
}

void MPU6050::writeRegister8(uint8_t reg, uint8_t value) {
    i2c.write(address, reg, value);
}

int16_t MPU6050::readRegister16(uint8_t reg) {
    uint8_t * data = i2c.read(address, reg, 2);
    return (uint16_t(data[0] << 8u) | data[1]);
}

void MPU6050::writeRegister16(uint8_t reg, int16_t value) {
    i2c.write(address, reg, uint16_t(value));
}

bool MPU6050::readRegisterBit(uint8_t reg, uint8_t pos) {
    uint8_t value;
    value = readRegister8(reg);
    return ((value >> pos) & 1u);
}

void MPU6050::writeRegisterBit(uint8_t reg, uint8_t pos, bool state)
{
    uint8_t value;
    value = readRegister8(reg);

    if (state) {
        value |= (1u << pos);
    } else {
        value &= ~(1u << pos);
    }

    writeRegister8(reg, value);
}