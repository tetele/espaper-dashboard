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
    void set_should_draw(std::function<bool()> &&should_draw) { this->should_draw_ = should_draw; };
    bool should_draw();

protected:
    ESPaperDashboard *target_{nullptr};
    int width_{0};
    int height_{0};
    optional<std::function<bool()>> should_draw_{};

    display::Display *get_display_() { return this->target_->get_display(); };
};

typedef enum {
    FORECAST_DAILY,
    FORECAST_TWICE_DAILY,
    FORECAST_HOURLY,
} WeatherForecastType;

class WeatherWidget : public ESPaperDashboardWidget {
public:
    void draw(int start_x, int start_y) override;
    void init_size() override;

    void set_forecast_type(WeatherForecastType forecast_type) { this->forecast_type_ = forecast_type; };

protected:
    WeatherForecastType forecast_type_{FORECAST_HOURLY};
};

} // namespace espaper_dashboard
} // namespace esphome