#include "espaper_dashboard_widget.h"
#include "esphome/core/log.h"
#include "esphome/components/json/json_util.h"

#include <map>



namespace esphome {
namespace espaper_dashboard {

static const char *TAG = "espaper_dashboard.widget";

bool ESPaperDashboardWidget::should_draw() {
    if (this->should_draw_.has_value())
        return this->should_draw_.value();
    return true;
}

}
}