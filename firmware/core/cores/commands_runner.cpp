#include "ZUMO_devices/ZUMO.hpp"
#include "command_terminal/Command.h"
#include "command_terminal/command_manager.h"


// TODO ZYMO
//bool static get_enable(const char* data) {
//    auto [l] = parser::get<int>(data);
//    return (1 == l);
//}

volatile bool get_sensors_flag = false;
void get_line_value_callback(const char* data){
    (void)data;
    get_sensors_flag = true;
}

volatile bool test_flag = false;
void test_callback(const char* data){
    (void)data;
    test_flag = true;
}

void set_motors_callback(const char* data) {
    auto [m1, m2] = parser::get<float, float>(data);
//    zumo().motor_driver.Motor_A.set_duty_cycle(m1);
//    zumo().motor_driver.Motor_B.set_duty_cycle(m2);
}

void set_line_enable_callback(const char* data) {
//    zumo().line_sensors.set_enable(get_enable(data));
}

volatile bool exit_flag = false;
void exit_callback(const char* data) {
    (void)data;
    exit_flag = true;
}

void set_hcsr04_enable_callback(const char* data) {
//    zumo().hcsr04.set_enable(get_enable(data));
}

volatile bool hcsr04_flag = false;

void get_hcsro4_value_callback(const char* data) {
    (void) data;
    hcsr04_flag = true;
}

//void set_mcp9700_enable_callback(const char* data) {
////    zumo().mcp9700.set_enable(get_enable(data));
//}
//
//volatile bool mcp9700_flag = false;
//void get_mcp9700_value_callback(const char* data) {
//    (void) data;
//    mcp9700_flag = true;
//}

//void set_bme280_enable_callback (const char* data) {
////    zumo().bme280.set_enable(get_enable(data));
//}
//
//volatile bool bme280_flag = false;
//void get_bme280_value_callback(const char* data) {
//    (void) data;
//    bme280_flag = true;
//}


//volatile bool mpu_accelerometer_flag = false;
//void get_mpu_accelerometer_value_callback(const char* data) {
//    (void) data;
//    mpu_accelerometer_flag = true;
//}
//
//volatile bool mpu_gyroscope_flag = false;
//void get_mpu_gyroscope_value_callback(const char* data) {
//    (void) data;
//    mpu_gyroscope_flag = true;
//}

//void set_mpu_accelerometer_enable_callback(const char* data) {
////    zumo().mpu6050.accelerometer.set_enable(get_enable(data));
//}
//
//void set_mpu_gyroscope_enable_callback(const char* data) {
////    zumo().mpu6050.gyroscope.set_enable(get_enable(data));
//}

//volatile bool ws2812b_flag = false;
//void set_ws2812B_value_callback(const char* data) {
//    (void) data;
//    auto [index, r, g, b] = parser::get<int, int, int, int>(data);
////    zumo().ws2812b.set_color(index, r, g, b);
//    ws2812b_flag = true;
//}

void callbacks_runner(PrintManager& command_manager) {
    if (get_sensors_flag) {
        command_manager.print('s');
//        volatile unsigned short * data = zumo().line_sensors.get_data_pointer();
//        for (int i =0; i < zumo().line_sensors.size(); i ++) {
//            uint16_t val = *data++;
//            command_manager.print((uint16_t)val);
////            labels[i]->set(val);
//        }
        get_sensors_flag = false;
    }

    if (test_flag) {
        test_flag = false;
        command_manager.print("1234");
    }

    if (exit_flag) {
        exit_flag = false;
//        command_manager.deinit();
    }

    if (hcsr04_flag) {
        hcsr04_flag = false;
        command_manager.print("h ");
//        command_manager.print(zumo().hcsr04.get_last_value());
    }

//    if (mcp9700_flag) {
//        mcp9700_flag = false;
//        command_manager.print("t ");
////        command_manager.print(zumo().mcp9700.get_last_temperature());
//    }

//    if (bme280_flag) {
//        bme280_flag = false;
//        command_manager.print("b ");
////        command_manager.print(zumo().bme280.get_last_temperature());
//        command_manager.print(' ');
////        command_manager.print(zumo().bme280.get_last_humidity());
//        command_manager.print(' ');
////        command_manager.print(zumo().bme280.get_last_pressure());
//        command_manager.print('\r');
//    }

//    if (mpu_accelerometer_flag) {
//        mpu_accelerometer_flag = false;
//        command_manager.print("ma ");
////        auto raw_data = zumo().mpu6050.accelerometer.get_last_normalised_data();
////        command_manager.print(raw_data.x);
//        command_manager.print(' ');
////        command_manager.print(raw_data.y);
//        command_manager.print(' ');
////        command_manager.print(raw_data.z);
//        command_manager.print('\r');
//    }

//    if (mpu_gyroscope_flag) {
//        mpu_gyroscope_flag = false;
//        command_manager.print("mg ");
////        auto raw_data = zumo().mpu6050.gyroscope.get_last_normalised_data();
////        command_manager.print(raw_data.x);
//        command_manager.print(' ');
////        command_manager.print(raw_data.y);
//        command_manager.print(' ');
////        command_manager.print(raw_data.z);
//        command_manager.print('\r');
//    }

//    if (ws2812b_flag) {
//        ws2812b_flag = false;
////        zumo().ws2812b.send();
//    }
}