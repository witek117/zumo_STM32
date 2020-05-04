#pragma once

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include "cores/commands_interface.h"
#include "ZUMO_hal/HALina_I2C.hpp"

struct Vector {
    float XAxis;
    float YAxis;
    float ZAxis;
};

class MPU6050 : CommandsInterface {
public:
    enum class RegisterNames : uint8_t {
        // accelerometer offsets
        ACCEL_XOFFS_H     = (0x06), ACCEL_XOFFS_L     = (0x07), ACCEL_YOFFS_H     = (0x08), ACCEL_YOFFS_L     = (0x09), ACCEL_ZOFFS_H     = (0x0A), ACCEL_ZOFFS_L     = (0x0B),
        // gyroscope offsets
        GYRO_XOFFS_H      = (0x13), GYRO_XOFFS_L      = (0x14), GYRO_YOFFS_H      = (0x15), GYRO_YOFFS_L      = (0x16), GYRO_ZOFFS_H      = (0x17), GYRO_ZOFFS_L      = (0x18),
        // configs
        CONFIG            = (0x1A), GYRO_CONFIG       = (0x1B), ACCEL_CONFIG      = (0x1C),
        //
        FF_THRESHOLD      = (0x1D), FF_DURATION       = (0x1E), MOT_THRESHOLD     = (0x1F), MOT_DURATION      = (0x20), ZMOT_THRESHOLD    = (0x21), ZMOT_DURATION     = (0x22),
        //
        INT_PIN_CFG       = (0x37), INT_ENABLE        = (0x38), INT_STATUS        = (0x3A),
        // accelerometer data
        ACCEL_XOUT_H      = (0x3B), ACCEL_XOUT_L      = (0x3C), ACCEL_YOUT_H      = (0x3D), ACCEL_YOUT_L      = (0x3E), ACCEL_ZOUT_H      = (0x3F), ACCEL_ZOUT_L      = (0x40),
        // temperature data
        TEMP_OUT_H        = (0x41), TEMP_OUT_L        = (0x42),
        // gyroscope data
        GYRO_XOUT_H       = (0x43), GYRO_XOUT_L       = (0x44), GYRO_YOUT_H       = (0x45), GYRO_YOUT_L       = (0x46), GYRO_ZOUT_H       = (0x47), GYRO_ZOUT_L       = (0x48),
        //
        MOT_DETECT_STATUS = (0x61), MOT_DETECT_CTRL   = (0x69),
        USER_CTRL         = (0x6A), // User Control
        //Power Management 1
        PWR_MGMT_1        = (0x6B), PWR_MGMT_2        = (0x6C),

        FIFO_COUNTH       = (0x72), FIFO_COUNTL       = (0x73), FIFO_R_W          = (0x74),
        WHO_AM_I          = (0x75)  // Who Am I
    };

    HALina_I2C& i2c;
    uint8_t address;
    bool enable = false;

    class DeviceData : public CommandsInterface {
    public:
        template <typename T>
        struct Axis {
            T x = 0;
            T y = 0;
            T z = 0;

            Axis<T> operator+(Axis<T> const &obj){
                Axis<T> res;
                res.x = x + obj.x;
                res.y = y + obj.y;
                res.z = z + obj.z;
                return res;
            }

            Axis<float> operator*(float multiply) {
                Axis<float> res;
                res.x = static_cast<float>(x) * multiply;
                res.y = static_cast<float>(y) * multiply;
                res.z = static_cast<float>(z) * multiply;
                return res;
            }
        };
    protected:
        MPU6050& mpu;
        RegisterNames data_register;
        RegisterNames offset_register;
        Axis<int16_t> raw_data;
        Axis<float> normalised_data;
        bool enable = false;
    public:
        virtual Axis<float> get_normalised_data() = 0;

        DeviceData(MPU6050& mpu, RegisterNames data_register, RegisterNames offset_register) : mpu(mpu), data_register(data_register), offset_register(offset_register){ }

        Axis<int16_t> get_raw_data() {
            if (enable) {
                uint8_t* data = mpu.i2c.read(mpu.address, uint8_t(data_register), 6);
                raw_data.x = (int16_t)(uint16_t(data[0] << 8u) | data[1]);
                raw_data.y = (int16_t)(uint16_t(data[2] << 8u) | data[3]);
                raw_data.z = (int16_t)(uint16_t(data[4] << 8u) | data[5]);
            }
            return raw_data;
        }

        void set_offset() {

        }

        Axis<int16_t> get_last_raw_data() {
            return raw_data;
        }

        void set_enable(bool enable_) override {
            enable = enable_;
        }

        Axis<float> get_last_normalised_data() {
            return normalised_data;
        }
    };

    class AccelerometerData : public DeviceData {
        float rangePerDigit = 1.0f;
    public:
        enum class Range : uint8_t {
            G16 = 0b11,
            G8  = 0b10,
            G4  = 0b01,
            G2  = 0b00,
        };

        AccelerometerData(MPU6050& mpu) : DeviceData(mpu, RegisterNames::ACCEL_XOUT_H, RegisterNames::ACCEL_XOFFS_H) { }

        Axis<float> get_normalised_data() override {
            if (!enable) {
                return normalised_data;
            }
            get_raw_data();
            normalised_data = raw_data * rangePerDigit;
            return normalised_data;
        }

        void set_range(Range range) {
            switch (range) {
                case Range::G2:
                    rangePerDigit = .00061f;
                    break;
                case Range::G4:
                    rangePerDigit = .00122f;
                    break;
                case Range::G8:
                    rangePerDigit = .00244f;
                    break;
                case Range::G16:
                    rangePerDigit = .004882f;
                    break;
                default:
                    break;
            }

            uint8_t value = mpu.read_register_8bit(RegisterNames::ACCEL_CONFIG);
            value &= 0b11100111u;
            value |= uint8_t(uint8_t(range) << 3u);
            mpu.write_register_8bit(RegisterNames::ACCEL_CONFIG, value);
        }

        Range get_range() {
            uint8_t value;
            value = mpu.read_register_8bit(RegisterNames::ACCEL_CONFIG);
            value &= 0b00011000u;
            value >>= 3u;
            return Range(value);
        }
    };

    class GyroscopeData : public DeviceData {
        bool calibrated = false;
        Vector dg;
        Vector tg, th;     // Threshold
        float actualThreshold = 0.0;
        float dpsPerDigit = 1.0;
    public:
        enum class Scale : uint8_t {
            DPS_2000 = 0b11,
            DPS_1000 = 0b10,
            DPS_500  = 0b01,
            DPS_250  = 0b00
        };

        GyroscopeData(MPU6050& mpu) : DeviceData(mpu, RegisterNames::GYRO_XOUT_H, RegisterNames::GYRO_XOFFS_H){ }

        Axis<float> get_normalised_data() override {
            if (!enable) {
                return normalised_data;
            }

            get_raw_data();

            if (calibrated) {
                normalised_data.x = (raw_data.x - dg.XAxis) * dpsPerDigit;
                normalised_data.y = (raw_data.y - dg.YAxis) * dpsPerDigit;
                normalised_data.z = (raw_data.z - dg.ZAxis) * dpsPerDigit;
            } else {
                normalised_data.x = raw_data.x  * dpsPerDigit;
                normalised_data.y = raw_data.y  * dpsPerDigit;
                normalised_data.z = raw_data.z  * dpsPerDigit;
            }

            if (actualThreshold == 0.0) {
                if (std::abs(normalised_data.x) < tg.XAxis) normalised_data.x = 0;
                if (std::abs(normalised_data.y) < tg.YAxis) normalised_data.y = 0;
                if (std::abs(normalised_data.z) < tg.ZAxis) normalised_data.z = 0;
            }
            return normalised_data;
        }

        void calibrate(uint8_t samples) {
            // Set calibrate
            calibrated = true;

            // Reset values
            float sumX = 0;
            float sumY = 0;
            float sumZ = 0;
            float sigmaX = 0;
            float sigmaY = 0;
            float sigmaZ = 0;

            // Read n-samples
            for (uint8_t i = 0; i < samples; ++i) {
                auto raw_data = get_raw_data();
                sumX += static_cast<float>(raw_data.x);
                sumY += static_cast<float>(raw_data.y);
                sumZ += static_cast<float>(raw_data.z);

                sigmaX += static_cast<float>(raw_data.x) * static_cast<float>(raw_data.x);
                sigmaY += static_cast<float>(raw_data.y) * static_cast<float>(raw_data.y);
                sigmaZ += static_cast<float>(raw_data.z) * static_cast<float>(raw_data.z);
                for (volatile uint32_t j = 0; j < 10000; j++) { }
            }

            // Calculate delta vectors
            dg.XAxis = sumX / static_cast<float>(samples);
            dg.YAxis = sumY / static_cast<float>(samples);
            dg.ZAxis = sumZ / static_cast<float>(samples);

            // Calculate threshold vectors
            th.XAxis = std::sqrt((sigmaX / 50) - (dg.XAxis * dg.XAxis));
            th.YAxis = std::sqrt((sigmaY / 50) - (dg.YAxis * dg.YAxis));
            th.ZAxis = std::sqrt((sigmaZ / 50) - (dg.ZAxis * dg.ZAxis));

            // If already set threshold, recalculate threshold vectors
            if (actualThreshold > 0) {
                set_threshold(actualThreshold);
            }
        }

        void set_threshold(uint8_t multiple) {
            if (multiple > 0) {
                // If not calibrated, need calibrate
                if (!calibrated) {
                    calibrate(1);
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

        void set_scale(Scale scale) {
            switch (scale) {
                case Scale::DPS_250:
                    dpsPerDigit = .007633f;
                    break;
                case Scale::DPS_500:
                    dpsPerDigit = .015267f;
                    break;
                case Scale::DPS_1000:
                    dpsPerDigit = .030487f;
                    break;
                case Scale::DPS_2000:
                    dpsPerDigit = .060975f;
                    break;
                default:
                    break;
            }

            uint8_t value = mpu.read_register_8bit(RegisterNames::GYRO_CONFIG);
            value &= 0b11100111u;
            value |= uint8_t(uint8_t(scale) << 3u);
            mpu.write_register_8bit(RegisterNames::GYRO_CONFIG, value);
        }

        Scale get_scale() {
            uint8_t value;
            value = mpu.read_register_8bit(RegisterNames::GYRO_CONFIG);
            value &= 0b00011000u;
            value >>= 3u;
            return Scale(value);
        }
    };

    GyroscopeData gyroscope;
    AccelerometerData accelerometer;

public:
    enum class ClockSource : uint8_t {
        KEEP_RESET      = 0b111,
        EXTERNAL_19MHZ  = 0b101,
        EXTERNAL_32KHZ  = 0b100,
        PLL_ZGYRO       = 0b011,
        PLL_YGYRO       = 0b010,
        PLL_XGYRO       = 0b001,
        INTERNAL_8MHZ   = 0b000
    };

    MPU6050(HALina_I2C& i2c, uint8_t address) :
        i2c(i2c), address(address << 1u),
        gyroscope(*this),
        accelerometer(*this) { }

    bool init(  GyroscopeData::Scale scale = GyroscopeData::Scale::DPS_2000,
                AccelerometerData::Range range = AccelerometerData::Range::G2);

    void set_enable(bool enable_) override {
        enable = enable_;
    }

    // for 8 bits
    uint8_t read_register_8bit(RegisterNames register_name);
    void write_register_8bit(RegisterNames reg, uint8_t value);

    // for 16 bits
    int16_t read_register_16bit(RegisterNames register_name);
    void write_register_16bit(RegisterNames register_name, int16_t value);

    // for 1 bit
    bool read_register_1bit(RegisterNames register_name, uint8_t pos);
    void write_register_1bit(RegisterNames register_name, uint8_t pos, bool state);

    void set_clock_source(ClockSource source) {
        uint8_t value = read_register_8bit(RegisterNames::PWR_MGMT_1);
        value &= ~ 0b00000111u;
        value |= uint8_t(source);
        write_register_8bit(RegisterNames::PWR_MGMT_1, value);
    }

    float get_temperature() {
        int16_t T = read_register_16bit(RegisterNames::TEMP_OUT_H);
        return float(T) / 340.0f + 21.0f;
    }

    uint16_t get_temperature_multiplied() {
        return static_cast<uint16_t >(get_temperature() * 100.0f);
    }

    void set_deep_sleep(bool state);

    void run_measurements() {
        accelerometer.get_normalised_data();
        gyroscope.get_normalised_data();
    }
};
