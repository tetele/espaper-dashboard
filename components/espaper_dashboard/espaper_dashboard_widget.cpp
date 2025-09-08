#include "espaper_dashboard_widget.h"
#include "esphome/core/log.h"



namespace esphome {
namespace espaper_dashboard {

static const char *TAG = "espaper_dashboard.widget";

void ESPaperDashboardWidget::set_size(int width, int height) {
    if(width > this->get_display_()->get_width() || height > this->get_display_()->get_height()) {
        ESP_LOGW(TAG, "Cannot set widget dimensions larger than the display it is shown on: %dx%d > %dx%d", width, height, this->get_display_()->get_width(), this->get_display_()->get_height());
        return;
    }

    this->width_ = width;
    this->height_ = height;
}

bool ESPaperDashboardWidget::should_draw() {
    if (this->should_draw_.has_value())
        return (*this->should_draw_)();
    return true;
}

void WeatherWidget::draw(int start_x, int start_y) {
    display::Display *it = this->get_display_();

    // TODO - this is just a placeholder for now

    // current condition icon
    it->filled_rectangle(start_x+this->width_/6, start_y+this->height_/6, this->width_/6, this->height_/3, this->target_->get_light_color());
    // current temperature
    it->filled_rectangle(start_x+this->width_/2, start_y+this->height_/6, this->width_/6, this->height_/3, this->target_->get_foreground_color());
    // current temperature UOM
    it->filled_rectangle(start_x+2*this->width_/3, start_y+this->height_/6, this->width_/6, this->height_/3, this->target_->get_dark_color());

    for(int i=0; i<4; i++) {
        // forcast timespan
        it->filled_rectangle(start_x+(i+1)*this->width_/6, start_y+this->height_/2, this->width_/6, this->height_/6, this->target_->get_dark_color());
        // forecast condition icon
        it->filled_rectangle(start_x+(i+1)*this->width_/6, start_y+2*this->height_/3, this->width_/6, this->height_/6, this->target_->get_light_color());
        // forecast temperature
        it->filled_rectangle(start_x+(i+1)*this->width_/6, start_y+5*this->height_/6, this->width_/6, this->height_/6, this->target_->get_foreground_color());
        // forecast temperature UOM
    }
}

void WeatherWidget::init_size() {
    this->width_ = this->get_display_()->get_width();
    this->height_ = this->width_*7/16;
}

WeatherCondition WeatherWidget::str_to_condition_(std::string condition) {
    if(condition == "cloudy") return CONDITION_CLOUDY;
    else if(condition == "fog") return CONDITION_FOG;
    else if(condition == "hail") return CONDITION_HAIL;
    else if(condition == "lightning") return CONDITION_LIGHTNING;
    else if(condition == "lightning-rainy") return CONDITION_LIGHTNING_RAINY;
    else if(condition == "clear-night") return CONDITION_CLEAR_NIGHT;
    else if(condition == "partlycloudy") return CONDITION_PARTLYCLOUDY;
    else if(condition == "pouring") return CONDITION_POURING;
    else if(condition == "rainy") return CONDITION_RAINY;
    else if(condition == "snowy") return CONDITION_SNOWY;
    else if(condition == "snowy-rainy") return CONDITION_SNOWY_RAINY;
    else if(condition == "sunny") return CONDITION_SUNNY;
    else if(condition == "windy") return CONDITION_WINDY;
    else if(condition == "windy-variant") return CONDITION_WINDY_VARIANT;
    // else if(condition == "exceptional") return CONDITION_;
    return CONDITION_PARTLYCLOUDY;
}

std::string WeatherWidget::condition_to_icon_(WeatherCondition condition) {
    // TODO allow custom glyphs
    std::map<WeatherCondition, std::string> weather_icon_map {
        {CONDITION_CLOUDY, "\ue2bd"},
        {CONDITION_FOG, "\ue818"},
        {CONDITION_HAIL, "\uf67f"},
        {CONDITION_LIGHTNING, "\uebdb"},
        {CONDITION_LIGHTNING_RAINY, "\uebdb"},
        {CONDITION_CLEAR_NIGHT, "\uef44"},
        {CONDITION_PARTLYCLOUDY, "\uf172"},
        {CONDITION_POURING, "\uf61f"},
        {CONDITION_RAINY, "\uf176"},
        {CONDITION_SNOWY, "\ue2cd"},
        {CONDITION_SNOWY_RAINY, "\uf61d"},
        {CONDITION_SUNNY, "\ue81a"},
        {CONDITION_WINDY, "\uefd8"},
        {CONDITION_WINDY_VARIANT, "\uefd8"},
    };

    if(weather_icon_map.count(condition))
        return weather_icon_map[condition];

    return "";
}

std::string WeatherWidget::condition_to_icon_(std::string condition) {
    return this->condition_to_icon_(this->str_to_condition_(condition));
}

}
}