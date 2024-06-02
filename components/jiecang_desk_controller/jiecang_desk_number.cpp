#include "jiecang_desk_number.h"
#include "jiecang_desk_controller.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace jiecang_desk_controller  {

        static const char *const TAG = "jiecang_desk.number";

        void JiecangDeskNumber::setup() {}

        void JiecangDeskNumber::dump_config() {
            LOG_NUMBER("", "JiecangDeskNumber", this);
            ESP_LOGCONFIG(TAG, " type %i", type);
        }

        void JiecangDeskNumber::control(float value) {
            this->publish_state(value);
            parent->number_control(this->type, value);
        }

        void JiecangDeskNumber::set_min_value(float value) {
            traits.set_min_value(value);
        }

        void JiecangDeskNumber::set_max_value(float value) {
            traits.set_max_value(value);
        }


    }  // namespace jiecang_desk_controller
}  // namespace esphome
