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
    this->display_->set_writer([this](display::Display &it) -> void {
        this->draw();
    });

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
    for(auto widget : this->widgets_) {
        widget->dump_config();
    }
}

void ESPaperDashboard::draw() {
    this->display_->fill(this->get_background_color());

    int total_height = 0;
    std::vector<ESPaperDashboardWidget*> sorted_widgets = this->widgets_;
    auto compare_priorities = [](ESPaperDashboardWidget *a, ESPaperDashboardWidget *b) {
        return (a->get_priority() > b->get_priority());
    };
    std::sort(sorted_widgets.begin(), sorted_widgets.end(), compare_priorities);
    for(auto widget : sorted_widgets) {
        if(total_height + widget->get_height() > this->display_->get_height()) break;
        if(!widget->should_draw()) continue;

        this->display_->start_clipping(0, total_height, widget->get_width(), total_height+widget->get_height());
        widget->draw(0, total_height);
        this->display_->end_clipping();
        total_height += widget->get_height();
    }
}

void ESPaperDashboard::add_widget(ESPaperDashboardWidget *widget) {
    this->widgets_.push_back(widget);
}

} // namespace espaper_dashboard
} // namespace esphome