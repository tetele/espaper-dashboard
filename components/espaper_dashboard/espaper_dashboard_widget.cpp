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

}
}