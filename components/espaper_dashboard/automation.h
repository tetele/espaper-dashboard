#pragma once

#include "espaper_dashboard_widget.h"

namespace esphome {
namespace espaper_dashboard {

template<typename... Ts> class ESPaperDashboardNeedsRedrawCondition : public Condition<Ts...> {
 public:
  ESPaperDashboardNeedsRedrawCondition(ESPaperDashboard *parent) : parent_(parent) {}
  bool check(Ts... x) override { return this->parent_->needs_redraw(); }

 protected:
  ESPaperDashboard *parent_;
};

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