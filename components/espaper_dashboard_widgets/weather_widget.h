#pragma once

#include "esphome/components/espaper_dashboard/espaper_dashboard_widget.h"

namespace esphome {
namespace espaper_dashboard_widgets {

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

WeatherCondition str_to_condition_(std::string condition);
std::string condition_to_icon_(WeatherCondition condition);
std::string condition_to_icon_(std::string condition);

struct WeatherStatus {
    std::string title;
    float temperature;
    WeatherCondition condition;

    WeatherStatus(std::string p_title, float p_temperature, WeatherCondition p_condition)
        : title(std::move(p_title)), temperature(p_temperature), condition(p_condition)
    {}

    WeatherStatus(std::string p_title, float p_temperature, std::string p_condition)
        : title(std::move(p_title)), temperature(p_temperature), condition(str_to_condition_(p_condition))
    {}

    WeatherStatus(const WeatherStatus& other)
        : title(std::move(other.title)), temperature(other.temperature), condition(other.condition)
    {}

    WeatherStatus(WeatherStatus&& other)
        : title(std::move(other.title)), temperature(other.temperature), condition(other.condition)
    {}

    WeatherStatus& operator=(const WeatherStatus& other) = default;
};

class WeatherWidget : public espaper_dashboard::ESPaperDashboardWidget {
public:
    void draw(int start_x, int start_y) override;
    void init_size() override;
    void dump_config() override;

    void set_temperature_uom(std::string uom) { this->temperature_uom_ = uom; };
    template<typename T> void set_current_temperature(T current_temperature) { this->current_temperature_ = current_temperature; };
    template<typename T> void set_current_condition(T current_condition) { this->current_condition_ = current_condition; };
    template<typename T> void set_forecast(T forecast) { this->forecast_ = forecast; };

protected:
    std::string temperature_uom_{""};
    TemplatableValue<float> current_temperature_{};
    TemplatableValue<std::string> current_condition_{};
    TemplatableValue<std::vector<WeatherStatus>> forecast_{};
};

} // namespace espaper_dashboard_widgets
} // namespace esphome