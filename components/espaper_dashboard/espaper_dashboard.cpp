#include "esphome/core/log.h"
#include "espaper_dashboard.h"
#include "espaper_dashboard_widget.h"

namespace esphome {
namespace espaper_dashboard {

static const char *TAG = "espaper_dashboard.component";

void ESPaperDashboard::setup()
{
    // Code here should perform all component initialization,
    //  whether hardware, memory, or otherwise

    for(auto widget : this->widgets_) {
        widget->init_size();
    }
}

void ESPaperDashboard::loop()
{
    // Tasks here will be performed at every call of the main application loop.
    // Note: code here MUST NOT BLOCK (see below)
}

void ESPaperDashboard::dump_config()
{
    ESP_LOGCONFIG(TAG, "ESPaper Dashboard");
}

void ESPaperDashboard::add_widget(ESPaperDashboardWidget *widget) {
    this->widgets_.push_back(widget);
}

} // namespace espaper_dashboard
} // namespace esphome