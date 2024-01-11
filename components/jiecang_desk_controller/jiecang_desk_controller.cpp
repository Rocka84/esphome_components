#include "jiecang_desk_controller.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace jiecang_desk_controller {

        static const char *const TAG = "jiecang_desk_controller";

        float JiecangDeskController::byte2float(int high, int low) {
            return static_cast<float>((high<<8) + low)/10;
        }

        bool JiecangDeskController::bufferMessage(int data, unsigned int *buffer, int len) {
            // This is a very simple method of receiving messages from the desk.
            // It will fail on messages that contain the value 0x7E in their payload.
            // But it is super simple and works for the messages we care about.

            // format: 0xF2 0xF2 [command] [param_count] [[param] ...] [checksum] 0x7E
            // checksum: sum of [command], [param_count] and all [param]s

            static int cmd_incoming = 0; // 0: wait for F2, 1: wait for 2nd F2, 2: buffer data
            static int pos = 0;

            if (cmd_incoming < 2 && data == 0xF2) { // start of message, must appear twice
                cmd_incoming++;
                pos = 0;

            } else if (cmd_incoming == 1) { // no second F2 received
                cmd_incoming = 0;

            } else if (cmd_incoming == 2) {
                if (data == 0x7E) { // end of message
                    cmd_incoming = 0;
                    for (;pos<len-1;pos++) { // fill rest of buffer with zeros
                        buffer[pos]=0;
                    }
                    return true;

                } else if (pos >= len) { // message too long, drop it
                    cmd_incoming = 0;

                } else {
                    buffer[pos++] = data; // buffer data
                }

            } // else: received garbage

            return false;
        }

        void JiecangDeskController::handleMessage(unsigned int *message) {
            // ESP_LOGV("jiecang_desk_controller", "message %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", message[0], message[1], message[2], message[3], message[4], message[5], message[6], message[7], message[8], message[9]);

            switch (message[0]) {
                case 0x01:
                    ESP_LOGV("jiecang_desk_controller", "height 0x%0X%0X", message[2], message[3]);
                    float new_height;
                    new_height = byte2float(message[2], message[3]);
                    if (new_height == current_height) return;
                    current_height = new_height;
                    if (height != nullptr) height->publish_state(current_height);

                    if (height_pct != nullptr && limit_max != 0)
                        height_pct->publish_state((current_height - limit_min) / (limit_max - limit_min) * 100);
                    break;

                case 0x20:
                    ESP_LOGV("jiecang_desk_controller", "limits 0x%0X  max %i min %i", message[2], (message[2] & 1), (message[2]>>4));

                    if ((message[2] & 1) == 0) { // low nibble 0 -> no max limit, use physical_max
                        limit_max = physical_max;
                        if (height_max != nullptr) height_max->publish_state(limit_max);
                    }
                    if ((message[2]>>4) == 0) { // high nibble 0 -> no min limit, use physical_min
                        limit_min = physical_min;
                        if (height_min != nullptr) height_min->publish_state(limit_min);
                    }
                    break;

                case 0x07:
                    ESP_LOGV("jiecang_desk_controller", "physical limits 0x%02X%02X 0x%02X%02X", message[2], message[3], message[4], message[5]);
                    physical_max = byte2float(message[2], message[3]);
                    physical_min = byte2float(message[4], message[5]);
                    break;

                case 0x21:
                    ESP_LOGV("jiecang_desk_controller", "height_max 0x%02X%02X", message[2], message[3]);
                    limit_max = byte2float(message[2], message[3]);
                    if (height_max != nullptr) height_max->publish_state(limit_max);
                    break;

                case 0x22:
                    ESP_LOGV("jiecang_desk_controller", "height_min 0x%02X%02X", message[2], message[3]);
                    limit_min = byte2float(message[2], message[3]);
                    if (height_min != nullptr) height_min->publish_state(limit_min);
                    break;

                case 0x25:
                    ESP_LOGV("jiecang_desk_controller", "position1 0x%02X%02X", message[2], message[3]);
                    if (position1 != nullptr) position1->publish_state(byte2float(message[2], message[3]));
                    break;

                case 0x26:
                    ESP_LOGV("jiecang_desk_controller", "position2 0x%02X%02X", message[2], message[3]);
                    if (position2 != nullptr) position2->publish_state(byte2float(message[2], message[3]));
                    break;

                case 0x27:
                    ESP_LOGV("jiecang_desk_controller", "position3 0x%02X%02X", message[2], message[3]);
                    if (position3 != nullptr) position3->publish_state(byte2float(message[2], message[3]));
                    break;

                case 0x28:
                    ESP_LOGV("jiecang_desk_controller", "position4 0x%02X%02X", message[2], message[3]);
                    if (position4 != nullptr) position4->publish_state(byte2float(message[2], message[3]));
                    break;

                default:
                    ESP_LOGV("jiecang_desk_controller", "unknown message %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", message[0], message[1], message[2], message[3], message[4], message[5], message[6], message[7], message[8], message[9]);
            }
        }

        void JiecangDeskController::update() {
            const int max_length = 10;
            static unsigned int buffer[max_length];
            while (available()) {
                if(bufferMessage(read(), buffer, max_length)) {
                    handleMessage(buffer);
                }
            }
        }

        void JiecangDeskController::send_simple_command(unsigned char cmd) {
            write_array({ 0xF1, 0xF1, cmd, 0x00, cmd, 0x7E });
        }

        void JiecangDeskController::add_button(button::Button *btn, int action) {
            btn->add_on_press_callback([this, action]() { this->button_press_action(action); });
        }

        void JiecangDeskController::raise() {
            send_simple_command(0x01);
        }

        void JiecangDeskController::lower() {
            send_simple_command(0x02);
        }

        void JiecangDeskController::goto_position(int pos) {
            switch (pos) {
                case 1:
                    send_simple_command(0x05);
                    break;
                case 2:
                    send_simple_command(0x06);
                    break;
                case 3:
                    send_simple_command(0x27);
                    break;
                case 4:
                    send_simple_command(0x28);
                    break;
            }
        }

        void JiecangDeskController::request_physical_limits() {
            send_simple_command(0x0C);
        }

        void JiecangDeskController::request_limits() {
            send_simple_command(0x20);
        }

        void JiecangDeskController::request_settings() {
            send_simple_command(0x07);
        }

        void JiecangDeskController::button_press_action(int action) {
            ESP_LOGV("JiecangDeskController", "button_press_action %i", action);
            switch (action) {
                case BUTTON_RAISE:
                    raise();
                    break;
                case BUTTON_LOWER:
                    lower();
                    break;
                case BUTTON_POSITION1:
                    goto_position(1);
                    break;
                case BUTTON_POSITION2:
                    goto_position(2);
                    break;
                case BUTTON_POSITION3:
                    goto_position(3);
                    break;
                case BUTTON_POSITION4:
                    goto_position(4);
                    break;
            }
        }

        void JiecangDeskButton::press_action() {}

    } //namespace jiecang_desk_controller
} //namespace esphome

