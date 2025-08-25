import esphome.codegen as cg
from esphome.components import display
import esphome.config_validation as cv
from esphome.const import (
    CONF_BACKGROUND_COLOR,
    CONF_DISPLAY_ID,
    CONF_FOREGROUND_COLOR,
    CONF_ID,
)

CONF_LABEL_COLOR = "label_color"

espaper_dashboard_ns = cg.esphome_ns.namespace("espaper_dashboard")
ESPaperDashboard = espaper_dashboard_ns.class_("ESPaperDashboard", cg.Component)

Color = cg.esphome_ns.class_("Color")

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESPaperDashboard),
        cv.GenerateID(CONF_DISPLAY_ID): cv.use_id(display.Display),
        cv.Optional(CONF_BACKGROUND_COLOR): cv.use_id(Color),
        cv.Optional(CONF_FOREGROUND_COLOR): cv.use_id(Color),
        cv.Optional(CONF_LABEL_COLOR): cv.use_id(Color),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)

    target_display = await cg.get_variable(config[CONF_DISPLAY_ID])
    cg.add(var.set_display(target_display))

    if CONF_BACKGROUND_COLOR in config:
        bg_color = await cg.get_variable(config[CONF_BACKGROUND_COLOR])
        cg.add(var.set_background_color(bg_color))
    if CONF_FOREGROUND_COLOR in config:
        label_color = await cg.get_variable(config[CONF_FOREGROUND_COLOR])
        cg.add(var.set_foreground_color(label_color))
    if CONF_LABEL_COLOR in config:
        label_color = await cg.get_variable(config[CONF_LABEL_COLOR])
        cg.add(var.set_label_color(label_color))
