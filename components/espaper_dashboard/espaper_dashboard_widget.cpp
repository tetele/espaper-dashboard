#include "espaper_dashboard_widget.h"
#include "esphome/core/log.h"
#include "esphome/components/json/json_util.h"

#include <map>



namespace esphome {
namespace espaper_dashboard {

static const char *TAG = "espaper_dashboard.widget";

void ESPaperDashboardWidget::draw(int start_x, int start_y) {
    this->set_was_drawn_(true);
    this->set_is_stale_(false);
    this->set_old_priority_(this->get_priority());
    this->internal_draw(start_x, start_y);
}

void ESPaperDashboardWidget::init() {
    this->init_size();

    if(this->target_->get_persist_data()) {
#ifdef USE_HOST
        uint32_t hash = fnv1_hash(this->id_); // you can't `esphome run` without compiling in host mode
#else
        uint32_t hash = fnv1_hash(App.get_compilation_time() + this->id_);
#endif
        this->old_priority_pref_ = global_preferences->make_preference<int>(hash);
        int old_priority;
        if(this->old_priority_pref_.load(&old_priority)) this->old_priority_ = old_priority;

        this->is_stale_pref_ = global_preferences->make_preference<bool>(hash+1);
        bool is_stale;
        if(this->is_stale_pref_.load(&is_stale)) this->is_stale_ = is_stale;

        this->was_drawn_pref_ = global_preferences->make_preference<bool>(hash+2);
        bool was_drawn;
        if(this->was_drawn_pref_.load(&was_drawn)) this->was_drawn_ = was_drawn;

        ESP_LOGV(TAG, "[%s] Loaded old data: old_priority(%d), is_stale(%s), was_drawn(%s)", this->id_.c_str(), this->old_priority_, YESNO(this->is_stale_), YESNO(this->was_drawn_));
    }
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

void ESPaperDashboardWidget::set_is_stale_(bool is_stale) {
    this->is_stale_ = is_stale;
    if(this->target_->get_persist_data()) {
        this->is_stale_pref_.save(&this->is_stale_);
        ESP_LOGV(TAG, "[%s] Persisting is_stale(%s)", this->id_.c_str(), YESNO(this->is_stale_));
    }
}

void ESPaperDashboardWidget::set_was_drawn_(bool was_drawn) {
    this->was_drawn_ = was_drawn;
    if(this->target_->get_persist_data()) {
        this->was_drawn_pref_.save(&this->was_drawn_);
        ESP_LOGV(TAG, "[%s] Persisting was_drawn(%s)", this->id_.c_str(), YESNO(this->was_drawn_));
    }
}


void ESPaperDashboardWidget::set_old_priority_(int old_priority) {
    this->old_priority_ = old_priority;
    if(this->target_->get_persist_data()) {
        this->old_priority_pref_.save(&this->old_priority_);
        ESP_LOGV(TAG, "[%s] Persisting was_drawn(%d)", this->id_.c_str(), this->old_priority_);
    }
}

}
}