#pragma once

#include <future>
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/button/button.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

#define BUTTON_RAISE 0
#define BUTTON_LOWER 1
#define BUTTON_POSITION1 2
#define BUTTON_POSITION2 3
#define BUTTON_POSITION3 4
#define BUTTON_POSITION4 5

namespace esphome {
    namespace jiecang_desk_controller {

        class JiecangDeskController : public PollingComponent, public sensor::Sensor, public uart::UARTDevice {
            private:
                float current_height = 0;
                float limit_min = 0;
                float limit_max = 0;
                float physical_min = 0;
                float physical_max = 0;

                float byte2float(int high, int low);
                bool bufferMessage(int data, unsigned int *buffer, int len);
                void handleMessage(unsigned int *message);

            public:
                void update() override;

                void set_sensor_height(sensor::Sensor *sensor) { this->height = sensor; }
                void set_sensor_unit(sensor::Sensor *sensor) { this->unit = sensor; }
                void set_sensor_height_min(sensor::Sensor *sensor) { this->height_min = sensor; }
                void set_sensor_height_max(sensor::Sensor *sensor) { this->height_max = sensor; }
                void set_sensor_height_pct(sensor::Sensor *sensor) { this->height_pct = sensor; }

                void set_sensor_position1(sensor::Sensor *sensor) { this->position1 = sensor; }
                void set_sensor_position2(sensor::Sensor *sensor) { this->position2 = sensor; }
                void set_sensor_position3(sensor::Sensor *sensor) { this->position3 = sensor; }
                void set_sensor_position4(sensor::Sensor *sensor) { this->position4 = sensor; }

                void send_simple_command(unsigned char cmd);
                void add_button(button::Button *btn, int action);

                void raise();
                void lower();
                void goto_position(int pos);

                void request_physical_limits();
                void request_limits();
                void request_settings();

            protected:
                Sensor *height{nullptr};
                Sensor *unit{nullptr};
                Sensor *height_min{nullptr};
                Sensor *height_max{nullptr};
                Sensor *height_pct{nullptr};

                Sensor *position1{nullptr};
                Sensor *position2{nullptr};
                Sensor *position3{nullptr};
                Sensor *position4{nullptr};

                void button_press_action(int type);
        };

        class JiecangDeskButton : public Component, public button::Button {
            protected:
                void press_action() override;
        };

    } //namespace jiecang_desk_controller
} //namespace esphome
