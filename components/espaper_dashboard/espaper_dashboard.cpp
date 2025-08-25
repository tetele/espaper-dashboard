#include "esphome/core/log.h"
#include "espaper_dashboard.h"

namespace esphome {
namespace espaper_dashboard {

static const char *TAG = "espaper_dashboard.component";

void ESPaperDashboard::setup()
{
    // Code here should perform all component initialization,
    //  whether hardware, memory, or otherwise
}

void ESPaperDashboard::loop()
{
    // Tasks here will be performed at every call of the main application loop.
    // Note: code here MUST NOT BLOCK (see below)
}

void ESPaperDashboard::dump_config()
{
    ESP_LOGCONFIG(TAG, "ESPaper Dashboard");
    ESP_LOGCONFIG(TAG, "  foo = %s", TRUEFALSE(this->foo_));
    ESP_LOGCONFIG(TAG, "  bar = %s", this->bar_.c_str());
    ESP_LOGCONFIG(TAG, "  baz = %i", this->baz_);
}

} // namespace espaper_dashboard
} // namespace esphome