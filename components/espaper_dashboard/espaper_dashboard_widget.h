#pragma once

#include "espaper_dashboard.h"

namespace esphome {
namespace espaper_dashboard {

class ESPaperDashboardWidget {
public:
    virtual void draw(int start_x, int start_y) = 0;
    virtual void init_size() = 0;

    void set_target(ESPaperDashboard *target) { this->target_ = target; };
    void set_size(int width, int height);
    int get_width() { return this->width_; };
    int get_height() { return this->height_; };

protected:
    ESPaperDashboard *target_{nullptr};
    int width_{0};
    int height_{0};

    display::Display *get_display_() { return this->target_->get_display(); };
};

} // namespace espaper_dashboard
} // namespace esphome