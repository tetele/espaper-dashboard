#pragma once

#include "espaper_dashboard_widget.h"

namespace esphome {
namespace espaper_dashboard {

class MessageWidget : public ESPaperDashboardWidget {
public:
    void draw(int start_x, int start_y) override;
    void init_size() override;
    void dump_config() override;

    template<typename T> void set_icon(T icon) { this->icon_ = icon; };
    template<typename T> void set_message(T message) { this->message_ = message; };

protected:
    TemplatableValue<std::string> icon_{};
    TemplatableValue<std::string> message_{};
};

} // namespace espaper_dashboard
} // namespace esphome