#include "esphome/core/log.h"
#include "example_component.h"

namespace esphome {
namespace example_component {

static const char *TAG = "example_component.component";

void ExampleComponent::setup()
{
    // Code here should perform all component initialization,
    //  whether hardware, memory, or otherwise
}

void ExampleComponent::loop()
{
    // Tasks here will be performed at every call of the main application loop.
    // Note: code here MUST NOT BLOCK (see below)
}

void ExampleComponent::dump_config()
{
    ESP_LOGCONFIG(TAG, "Example component");
    ESP_LOGCONFIG(TAG, "  foo = %s", TRUEFALSE(this->foo_));
    ESP_LOGCONFIG(TAG, "  bar = %s", this->bar_.c_str());
    ESP_LOGCONFIG(TAG, "  baz = %i", this->baz_);
}

} // namespace example_component
} // namespace esphome