#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display.h"

namespace esphome {
namespace espaper_dashboard {

class ESPaperDashboard : public Component
{
public:
    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_display(display::Display *display) { this->display_ = display; };
    void set_background_color(Color color) { this->bg_color_ = color; };
    void set_foreground_color(Color color) { this->fg_color_ = color; };

protected:
    display::Display *display_{nullptr};
    Color bg_color_{display::COLOR_ON};
    Color fg_color_{display::COLOR_OFF};
};

} // namespace espaper_dashboard
} // namespace esphome