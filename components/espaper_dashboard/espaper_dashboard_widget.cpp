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

void ESPaperDashboardWidget::init() {
    this->init_size();

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
    int priority = this->get_priority();
    if(this->old_priority_ != priority) {
        ESP_LOGV(TAG, "[%s] Needs redraw: priority changed from %d to %d", this->id_.c_str(), this->old_priority_, priority);
        return true;
    }

    if(this->was_drawn_ != this->should_draw()) {
        ESP_LOGV(TAG, "[%s] Needs redraw: widget needs to %s", this->id_.c_str(), (this->was_drawn_ ? "disappear" : "appear"));
        return true;
    }

    if(this->was_drawn_ && this->is_stale_) {
        ESP_LOGV(TAG, "[%s] Needs redraw: widget data is stale", this->id_.c_str());
        return true;
    }

    return false;
}

}
}