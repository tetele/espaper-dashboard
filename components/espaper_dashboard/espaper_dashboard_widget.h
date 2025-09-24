#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/application.h"
#include "espaper_dashboard.h"

namespace esphome {
namespace espaper_dashboard {

class ESPaperDashboardWidget {
public:
    void draw(int start_x, int start_y);
    virtual void init_size() = 0;
    virtual void dump_config() = 0;

    void set_target(ESPaperDashboard *target) { this->target_ = target; };
    template<typename T> void set_width(T width) { this->width_ = width; };
    int get_width() { return this->width_.value(); };
    template<typename T> void set_height(T height) { this->height_ = height; };
    int get_height() { return this->height_.value(); };
    template<typename T> void set_should_draw(T should_draw) { this->should_draw_ = should_draw; };
    bool should_draw();
    template<typename T> void set_priority(T priority) { this->priority_ = priority; };
    int get_priority() { return this->priority_.value(); };

    bool is_stale() { return this->is_stale_; };
    void mark_stale() { this->is_stale_ = true; };
    void mark_not_drawn() { this->was_drawn_ = false; };
    bool needs_redraw();

protected:
    virtual void internal_draw(int start_x, int start_y) = 0;

    ESPaperDashboard *target_{nullptr};
    TemplatableValue<int> width_{0};
    TemplatableValue<int> height_{0};
    TemplatableValue<bool> should_draw_{};
    TemplatableValue<int> priority_{};

    display::Display *get_display_() { return this->target_->get_display(); };
    bool is_stale_{true};
    bool was_drawn_{false};
};

} // namespace espaper_dashboard
} // namespace esphome