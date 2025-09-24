#pragma once

#include "esphome/components/espaper_dashboard/espaper_dashboard_widget.h"

namespace esphome {
namespace espaper_dashboard_widgets {

typedef std::function<void(display::Display &, int, int)> custom_widget_writer_t;

class CustomWidget : public espaper_dashboard::ESPaperDashboardWidget {
public:
    void init_size() override;
    void dump_config() override;

    void set_lambda(custom_widget_writer_t &&lambda_) { this->lambda_ = lambda_; }

protected:
    void internal_draw(int start_x, int start_y) override;

    optional<custom_widget_writer_t> lambda_{};
};

} // namespace espaper_dashboard_widgets
} // namespace esphome