#include "custom_widget.h"
#include "esphome/core/log.h"

namespace esphome {
namespace espaper_dashboard_widgets {

static const char *TAG = "espaper_dashboard_widgets.custom_widget";

void CustomWidget::init_size() {
    if(!this->width_.has_value() || !this->width_.value()) this->width_ = this->get_display_()->get_width();
    if(!this->height_.has_value() || !this->height_.value()) this->height_ = this->width_.value()/4;
}

void CustomWidget::dump_config() {
    ESP_LOGCONFIG(TAG, "  Custom widget %s", this->id_.c_str());
}

void CustomWidget::internal_draw(int start_x, int start_y) {
    display::Display *it = this->get_display_();

    if(this->lambda_.has_value()) {
        (*this->lambda_)(*it, start_x, start_y);
    }
}

}
}