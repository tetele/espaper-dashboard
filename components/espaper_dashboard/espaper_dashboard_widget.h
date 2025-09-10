#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
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
    CONDITION_CLEAR_NIGHT,
    CONDITION_CLOUDY,
    CONDITION_EXCEPTIONAL,
    CONDITION_FOG,
    CONDITION_HAIL,
    CONDITION_LIGHTNING,
    CONDITION_LIGHTNING_RAINY,
    CONDITION_PARTLYCLOUDY,
    CONDITION_POURING,
    CONDITION_RAINY,
    CONDITION_SNOWY,
    CONDITION_SNOWY_RAINY,
    CONDITION_SUNNY,
    CONDITION_WINDY,
    CONDITION_WINDY_VARIANT,
} WeatherCondition;

class WeatherWidget : public ESPaperDashboardWidget {
public:
    void draw(int start_x, int start_y) override;
    void init_size() override;

    void set_current_temperature_sensor(sensor::Sensor *sensor) { this->current_temperature_sensor_ = sensor; };
    void set_current_condition_sensor(text_sensor::TextSensor *sensor) { this->current_condition_sensor_ = sensor; };
    void set_forecast_sensor(text_sensor::TextSensor *sensor) { this->forecast_sensor_ = sensor; };
    void set_temperature_uom(std::string uom) { this->temperature_uom_ = uom; };

protected:
    sensor::Sensor *current_temperature_sensor_{nullptr};
    text_sensor::TextSensor *current_condition_sensor_{nullptr};
    text_sensor::TextSensor *forecast_sensor_{nullptr};
    std::string temperature_uom_{""};

    WeatherCondition str_to_condition_(std::string condition);
    std::string condition_to_icon_(WeatherCondition condition);
    std::string condition_to_icon_(std::string condition);
};

class MessageWidget : public ESPaperDashboardWidget {
public:
    void draw(int start_x, int start_y) override;
    void init_size() override;

    template<typename T> void set_icon(T icon) { this->icon_ = icon; };
    template<typename T> void set_message(T message) { this->message_ = message; };

protected:
    TemplatableValue<std::string> icon_{};
    TemplatableValue<std::string> message_{};
};

} // namespace espaper_dashboard
} // namespace esphome