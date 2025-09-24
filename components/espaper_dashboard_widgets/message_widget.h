#pragma once

#include "esphome/components/espaper_dashboard/espaper_dashboard_widget.h"

namespace esphome {
namespace espaper_dashboard_widgets {

class MessageWidget : public espaper_dashboard::ESPaperDashboardWidget {
public:
    void init_size() override;
    void dump_config() override;

    template<typename T> void set_icon(T icon) { this->icon_ = icon; };
    template<typename T> void set_message(T message) { this->message_ = message; };

protected:
    void internal_draw(int start_x, int start_y) override;

    TemplatableValue<std::string> icon_{};
    TemplatableValue<std::string> message_{};
};

} // namespace espaper_dashboard_widgets
} // namespace esphome