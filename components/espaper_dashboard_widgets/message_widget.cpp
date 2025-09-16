#include "message_widget.h"
#include "esphome/core/log.h"

#include <map>

namespace esphome {
namespace espaper_dashboard {

static const char *TAG = "espaper_dashboard_widgets.message_widget";

void MessageWidget::draw(int start_x, int start_y) {
    display::Display *it = this->get_display_();


    if(this->icon_.has_value()) {
        // icon
        it->printf(start_x+this->width_.value()/10, start_y+this->height_.value()/2, this->target_->get_large_glyph_font(), this->target_->get_light_color(), display::TextAlign::CENTER, "%s", this->icon_.value().c_str());
        // message
        it->printf(start_x+this->width_.value()*3/10, start_y+this->height_.value()/2, this->target_->get_large_font(), this->target_->get_foreground_color(), display::TextAlign::CENTER_LEFT, "%s", this->message_.value().c_str());
    } else {
        // message
        it->printf(start_x+this->width_.value()/10, start_y+this->height_.value()/2, this->target_->get_large_font(), this->target_->get_foreground_color(), display::TextAlign::CENTER_LEFT, "%s", this->message_.value().c_str());
    }

}

void MessageWidget::init_size() {
    if(!this->width_.has_value() || !this->width_.value()) this->width_ = this->get_display_()->get_width();
    if(!this->height_.has_value() || !this->height_.value()) this->height_ = this->width_.value()/4;
}

void MessageWidget::dump_config() {
    ESP_LOGCONFIG(TAG, "  Message widget");
}

}
}