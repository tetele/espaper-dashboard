#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display.h"

namespace esphome {
namespace espaper_dashboard {

class ESPaperDashboardWidget;

class ESPaperDashboard : public Component
{
public:
    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_display(display::Display *display) { this->display_ = display; };
    display::Display *get_display() { return this->display_; };
    void set_background_color(Color color) { this->bg_color_ = color; };
    Color get_background_color() { return this->bg_color_; };
    void set_foreground_color(Color color) { this->fg_color_ = color; };
    Color get_foreground_color() { return this->fg_color_; };
    void set_label_color(Color color) { this->label_color_ = color; };
    Color get_label_color() { return this->label_color_; };
    void set_light_color(Color color) { this->light_color_ = color; };
    Color get_light_color() { return this->light_color_; };
    void set_dark_color(Color color) { this->dark_color_ = color; };
    Color get_dark_color() { return this->dark_color_; };

    void add_widget(ESPaperDashboardWidget *widget);

protected:
    display::Display *display_{nullptr};
    Color bg_color_{display::COLOR_ON};
    Color fg_color_{display::COLOR_OFF};
    Color label_color_{display::COLOR_OFF};
    Color light_color_{display::COLOR_OFF};
    Color dark_color_{display::COLOR_OFF};

    std::vector<ESPaperDashboardWidget *> widgets_;
};

} // namespace espaper_dashboard
} // namespace esphome