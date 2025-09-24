#pragma once

#include "espaper_dashboard_widget.h"

namespace esphome {
namespace espaper_dashboard {

template<typename... Ts> class ESPaperDashboardWidgetMarkStaleAction : public Action<Ts...> {
 public:
  explicit ESPaperDashboardWidgetMarkStaleAction(ESPaperDashboardWidget *widget) : widget_(widget) {}

  void play(Ts... x) override {
    this->widget_->mark_stale();
  }

 protected:
  ESPaperDashboardWidget *widget_;
};

} // namespace espaper_dashboard
} // namespace esphome