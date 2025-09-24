#include "weather_widget.h"
#include "esphome/core/log.h"
#include "esphome/components/json/json_util.h"

#include <map>

namespace esphome {
namespace espaper_dashboard_widgets {

static const char *TAG = "espaper_dashboard_widgets.weather_widget";

void WeatherWidget::init_size() {
    if(!this->width_.has_value() || !this->width_.value()) this->width_ = this->get_display_()->get_width();
    if(!this->height_.has_value() || !this->height_.value()) this->height_ = this->width_.value()*7/16;
}

void WeatherWidget::dump_config() {
    ESP_LOGCONFIG(TAG, "  Weather widget %s", this->id_.c_str());
}

WeatherCondition str_to_condition_(std::string condition) {
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

std::string condition_to_icon_(WeatherCondition condition) {
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

std::string condition_to_icon_(std::string condition) {
    return condition_to_icon_(str_to_condition_(condition));


}

void WeatherWidget::internal_draw(int start_x, int start_y) {
    display::Display *it = this->get_display_();

    // current condition icon
    it->printf(start_x+this->width_.value()/3, start_y+this->height_.value()/4, this->target_->get_large_glyph_font(), this->target_->get_light_color(), display::TextAlign::CENTER_RIGHT, "%s", condition_to_icon_(this->current_condition_.value()).c_str());
    // current temperature UOM
    it->printf(start_x+this->width_.value()/2, start_y+this->height_.value()/4, this->target_->get_large_font(), this->target_->get_dark_color(), display::TextAlign::CENTER_LEFT, "%.1f%s", this->current_temperature_.value(), this->temperature_uom_.c_str());
    // current temperature
    it->printf(start_x+this->width_.value()/2, start_y+this->height_.value()/4, this->target_->get_large_font(), this->target_->get_foreground_color(), display::TextAlign::CENTER_LEFT, "%.1f", this->current_temperature_.value());

    std::vector<WeatherStatus> forecast = this->forecast_.value();

    int i=1, n=(forecast.size()+2)*2;
    for(const WeatherStatus& interval : forecast) {
        // forcast timespan
        it->printf((i*2+1)*this->width_.value()/n, start_y+this->height_.value()/2, this->target_->get_default_font(), this->target_->get_dark_color(), display::TextAlign::TOP_CENTER, "%s", interval.title.c_str());
        // forecast condition icon
        it->printf((i*2+1)*this->width_.value()/n, start_y+2*this->height_.value()/3, this->target_->get_glyph_font(), this->target_->get_light_color(), display::TextAlign::TOP_CENTER, "%s", condition_to_icon_(interval.condition).c_str());
        // forecast temperature UOM
        int x, y, w, h;
        char t[20];
        sprintf(t, "%.1f%s", interval.temperature, this->temperature_uom_.c_str());
        it->get_text_bounds(start_x+(i*2+1)*this->width_.value()/n, start_y+5*this->height_.value()/6, t, this->target_->get_default_font(), display::TextAlign::TOP_CENTER, &x, &y, &w, &h);
        it->printf(x, y, this->target_->get_default_font(), this->target_->get_dark_color(), display::TextAlign::TOP_LEFT, "%.1f%s", interval.temperature, this->temperature_uom_.c_str());
        // forecast temperature
        it->printf(x, y, this->target_->get_default_font(), this->target_->get_foreground_color(), display::TextAlign::TOP_LEFT, "%.1f", interval.temperature);
        i++;
    }
}

}
}