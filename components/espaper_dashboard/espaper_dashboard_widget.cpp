#include "espaper_dashboard_widget.h"
#include "esphome/core/log.h"
#include "esphome/components/json/json_util.h"

#include <map>



namespace esphome {
namespace espaper_dashboard {

static const char *TAG = "espaper_dashboard.widget";

void ESPaperDashboardWidget::draw(int start_x, int start_y) {
    this->was_drawn_ = true;
    this->is_stale_ = false;
    this->old_priority_ = this->get_priority();
    this->internal_draw(start_x, start_y);
}

bool ESPaperDashboardWidget::should_draw() {
    if (this->should_draw_.has_value())
        return this->should_draw_.value();
    return true;
}

int ESPaperDashboardWidget::get_priority()  {
    if(this->priority_.has_value()) {
        return this->priority_.value();
    }
    return 0;
}

bool ESPaperDashboardWidget::needs_redraw() {
    return this->is_stale() || (this->was_drawn_ != this->should_draw());
}

}
}