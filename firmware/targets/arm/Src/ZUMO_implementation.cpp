#include "ZUMO_impementation.hpp"
#include "STM_hal.h"
#include "stm32f3xx_hal.h"
#include "Bosch_PCB_7183_di03_BMI160-7183_di03.2.1.11696_170103.h"


volatile uint16_t sensors[11];
volatile uint16_t *TEMP = &sensors[8];
volatile uint16_t *V_CURRENT_SENS = &sensors[9];
volatile uint16_t *V_BAT = &sensors[10];

bool static get_enable(const char* data) {
    auto [l] = parser::get<int>(data);
    return (1 == l);
}

// ZUMO I2C
extern I2C_HandleTypeDef hi2c1;
STM32_I2C ZUMO::IMU_I2C(hi2c1);

// ZUMO LEDS
STM32_GPIO ZUMO::LED1 = {LED1_GPIO_Port, LED1_Pin};
STM32_GPIO ZUMO::LED2 = {LED2_GPIO_Port, LED2_Pin};

// ZUMO WS2812
STM_WS2812B<2> ZUMO::ws2812b = {WS2812B_GPIO_Port, WS2812B_Pin, ZUMO::enableInterrupts, ZUMO::disableInterrupts};

void ZUMO::set_value_value_callback(const char* data) {
    auto [index, r, g, b] = parser::get<int, int, int, int>(data);
    ws2812b.set_color(index, r, g, b);
    ws2812b.send();
    ws2812b.send(); // TODO fix double sending
}

// ZUMO BME280
BME280 ZUMO::bme280 = {IMU_I2C, 0b1111110};
void ZUMO::set_bme280_enable_callback (const char* data) {
    LED2.toggle();
    bme280.set_enable(get_enable(data));
}

void ZUMO::get_bme280_value_callback(const char* data) {
    (void) data;
    command_manager.printer.print("b ");
    command_manager.printer.print(bme280.get_last_temperature());
    command_manager.printer.print(' ');
    command_manager.printer.print(bme280.get_last_humidity());
    command_manager.printer.print(' ');
    command_manager.printer.print(bme280.get_last_pressure());
    command_manager.printer.print('\n');
}

// ZUMO MPU6050
MPU6050 ZUMO::mpu6050 = {IMU_I2C, 0x68};
void ZUMO::get_mpu_accelerometer_value_callback(const char* data) {
    (void) data;
    command_manager.printer.print("ma ");
    auto raw_data = mpu6050.accelerometer.get_last_normalised_data();
    command_manager.printer.print(raw_data.x);
    command_manager.printer.print(' ');
    command_manager.printer.print(raw_data.y);
    command_manager.printer.print(' ');
    command_manager.printer.print(raw_data.z);
    command_manager.printer.print('\n');
}

void ZUMO::get_mpu_gyroscope_value_callback(const char* data) {
    (void) data;
    command_manager.printer.print("mg ");
    auto raw_data = mpu6050.gyroscope.get_last_normalised_data();
    command_manager.printer.print(raw_data.x);
    command_manager.printer.print(' ');
    command_manager.printer.print(raw_data.y);
    command_manager.printer.print(' ');
    command_manager.printer.print(raw_data.z);
    command_manager.printer.print('\n');
}

void ZUMO::set_mpu_accelerometer_enable_callback(const char* data) {
    mpu6050.accelerometer.set_enable(get_enable(data));
}

void ZUMO::set_mpu_gyroscope_enable_callback(const char* data)  {
    mpu6050.gyroscope.set_enable(get_enable(data));
}

// ZUMO MCP9700
MCP9700<uint16_t > ZUMO::mcp9700((uint16_t&)(*TEMP), 4095, 3.3f);

void ZUMO::set_mcp9700_enable_callback(const char* data) {
    mcp9700.set_enable(get_enable(data));
}

void ZUMO::get_mcp9700_value_callback(const char* data) {
    (void) data;
    command_manager.printer.print("t ");
    command_manager.printer.print(mcp9700.get_last_temperature());
}

// ZUMO BHI160
BHYSensor ZUMO::bhi160 = {IMU_I2C, BHY_I2C_ADDR};

// ZUMO MOTORS
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim15;

STM32_PWM<1000> ZUMO::PWM_1 = {&htim1, TIM_CHANNEL_2, HAL_TIMEx_PWMN_Start};
STM32_PWM<1000> ZUMO::PWM_2 = {&htim1, TIM_CHANNEL_3, HAL_TIMEx_PWMN_Start};
STM32_PWM<1000> ZUMO::PWM_3 = {&htim1, TIM_CHANNEL_1, HAL_TIMEx_PWMN_Start};
STM32_PWM<1000> ZUMO::PWM_4 = {&htim15, TIM_CHANNEL_1, HAL_TIM_PWM_Start};

STM32_GPIO_FAKE ZUMO::nSleep;
STM32_GPIO ZUMO::fault(FAULT_GPIO_Port, FAULT_Pin);

DRV8833 ZUMO::motor_driver = {ZUMO::PWM_1, ZUMO::PWM_2, ZUMO::PWM_3, ZUMO::PWM_4,ZUMO::nSleep, ZUMO::fault };

void ZUMO::set_motors_callback(const char* data) {
    auto [m1, m2] = parser::get<float, float>(data);
    motor_driver.Motor_A.set_duty_cycle(m1);
    motor_driver.Motor_B.set_duty_cycle(m2);
}

// ZUMO ENCODERS
STM32_GPIO ZUMO::MOT_L_A = {MOT_L_A_GPIO_Port, MOT_L_A_Pin};
STM32_GPIO ZUMO::MOT_L_B = {MOT_L_A_GPIO_Port, MOT_L_B_Pin};
STM32_GPIO ZUMO::MOT_R_A = {MOT_R_A_GPIO_Port, MOT_R_A_Pin};
STM32_GPIO ZUMO::MOT_R_B = {MOT_R_B_GPIO_Port, MOT_R_B_Pin};

Encoder ZUMO::encoderL = {MOT_L_A, MOT_L_B, 1};
Encoder ZUMO::encoderR = {MOT_R_A, MOT_R_B, 1};

// ZUMO UARTS
Uart ZUMO::uart1 = {USART1, 230400};

ZUMO::CommandManagerTempalte ZUMO::command_manager(enableInterrupts, disableInterrupts, {
        Command("ma?", ZUMO::get_mpu_accelerometer_value_callback),
        Command("mg?", ZUMO::get_mpu_gyroscope_value_callback),
        Command("b?", ZUMO::get_bme280_value_callback),
        Command("t?", get_mcp9700_value_callback),
        Command("m", set_motors_callback),
        Command("ma", set_mpu_accelerometer_enable_callback),
        Command("mg", set_mpu_gyroscope_enable_callback),
        Command("t", set_mcp9700_enable_callback),
        Command("b", ZUMO::set_bme280_enable_callback),
        Command("ws", ZUMO::set_value_value_callback),
        Command("test", ZUMO::test_callback)
    });

void ZUMO::test_callback(const char* data) {
    (void) data;
    command_manager.printer.print("1234");
}

void printUart1(uint8_t c) {
    ZUMO::uart1.write(c);
}

extern ADC_HandleTypeDef hadc1;

void ZUMO::init() {
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)sensors, 11);

    motor_driver.init();
    uart1.init();
    command_manager.init();
    PrintManager::setPrintFunction(printUart1);
    uart1.setRedirectHandler(ReadManager::putChar);
    ws2812b.init();
    bme280.init();
    mpu6050.init(MPU6050::GyroscopeData::Scale::DPS_2000, MPU6050::AccelerometerData::Range::G2);
    mcp9700.init();

    // Hello
    command_manager.printer.print("Hello world\r\n");

    bhi160.init();
    //bhi160.run();
    BHIInit(bhi160);

}

void ZUMO::loop() {
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)sensors, 11);
    command_manager.run();
    bme280.run_measurements();
    mpu6050.run_measurements();
    mcp9700.get_temperature();

    static uint32_t k = 0;
    k++;
    if (k > 100000) {
        k =  0;
        LED1.toggle();
    }
}

void print(const char *str){
    ZUMO::command_manager.printer.print(str);
}

volatile bool intrToggled = false;
bool newOrientationData = false;
float heading, roll, pitch;
uint8_t status;

void bhyInterruptHandler(void)
{
    intrToggled = true;
}
void waitForBhyInterrupt(void)
{
    while (!intrToggled)
        ;
    intrToggled = false;
}

void accelerometerHandler(bhyVector data, bhyVirtualSensor type)
{
    heading = data.x;
    roll = data.z;
    pitch = data.y;
    status = data.status;
    newOrientationData = true;
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin == INT_BHI_Pin) {
        print("\r\ninterrupt working\r\n");
        intrToggled = true;
    }
}

void BHIInit(BHYSensor &bhi160){

    print("BHI testing\r\n");

    // Check to see if something went wrong.
    if (!checkSensorStatus(bhi160))
        return;


    bhi160.bhiPrint("Uploading Firmware.");
    bhi160.wait(1000);
    bhi160.loadFirmware(bhy1_fw);

    if (!checkSensorStatus(bhi160))
        return;

    intrToggled = false; /* Clear interrupt status received during firmware upload */
    waitForBhyInterrupt();  /* Wait for meta events from boot up */
    bhi160.bhiPrint("Firmware booted");

    /* Install a metaevent callback handler and a timestamp callback handler here if required before the first run */
    bhi160.run(); /* The first run processes all boot events */


    if (bhi160.installSensorCallback(BHY_VS_ACCELEROMETER, true, accelerometerHandler))
    {
        checkSensorStatus(bhi160);
        return;
    }
    else {
        bhi160.bhiPrint("ACCELEROMETER callback installed");
        bhi160.bhiPrint("\r\n");
    }
    /* Link callbacks and configure desired virtual sensors here */

    if (bhi160.configVirtualSensor(BHY_VS_ORIENTATION, true, BHY_FLUSH_ALL, 200, 0, 0, 0))
    {
        bhi160.bhiPrint("Failed to enable virtual sensor ");
    }
    else {
        bhi160.bhiPrint(" virtual sensor enabled");
    }

    if (checkSensorStatus(bhi160))
        bhi160.bhiPrint("All ok");

    while(true){
        if(intrToggled) {
            intrToggled = false;
            bhi160.run();
            if (!checkSensorStatus(bhi160)) {
                bhi160.bhiPrint("\r\nsomething is wrong");
                bhi160.bhiPrint("\r\n");
            }
//            if (newOrientationData)
//            {
            bhi160.bhiPrint("Readed data: ");
            /* Can also be viewed using the plotter */
//                sprintf(buf, "%f ", heading);
//                bhi160.bhiPrint(buf);
            bhi160.bhiPrint((uint32_t) pitch);
            bhi160.bhiPrint((uint32_t) roll);
            bhi160.bhiPrint("\r\n");
            newOrientationData = false;
//            }else{
//                bhi160.bhiPrint("no more data orientation\r\n");
//            }
        }

    }
}

bool checkSensorStatus(BHYSensor &bhi160)
{
    if (bhi160.status == BHY_OK)
        return true;

    if (bhi160.status < BHY_OK) /* All error codes are negative */
    {
        char buf[10];
        bhi160.bhiPrint("Error code: (");
        sprintf(buf, "%d", bhi160.status);
        bhi160.bhiPrint(buf);
        bhi160.bhiPrint("). ");

        return false; /* Something has gone wrong */
    }
    else /* All warning codes are positive */
    {
        bhi160.bhiPrint("Warning code: (");
        bhi160.bhiPrint(bhi160.status);
        bhi160.bhiPrint(").");

        return true;
    }

    return true;
}