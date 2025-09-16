#pragma once

#include "espaper_dashboard_widget.h"

namespace esphome {
namespace espaper_dashboard {

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
    void dump_config() override;

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

} // namespace espaper_dashboard
} // namespace esphome