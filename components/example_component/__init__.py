import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CONF_FOO = "foo"
CONF_BAR = "bar"
CONF_BAZ = "baz"

example_component_ns = cg.esphome_ns.namespace("example_component")
ExampleComponent = example_component_ns.class_("ExampleComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ExampleComponent),
        cv.Required(CONF_FOO): cv.boolean,
        cv.Optional(CONF_BAR): cv.string,
        cv.Optional(CONF_BAZ): cv.int_range(0, 255),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)

    cg.add(var.set_foo(config[CONF_FOO]))
    if bar := config.get(CONF_BAR):
        cg.add(var.set_bar(bar))
    if baz := config.get(CONF_BAZ):
        cg.add(var.set_baz(baz))
