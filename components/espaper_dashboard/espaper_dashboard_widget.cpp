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

    ESP_LOGD(TAG, "Setting weater widget dimensions: %dx%d", this->width_, this->height_);
}

}
}