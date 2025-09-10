import esphome.codegen as cg
from esphome.components import display, sensor, text_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_BACKGROUND_COLOR,
    CONF_DISPLAY_ID,
    CONF_FOREGROUND_COLOR,
    CONF_ICON,
    CONF_ID,
    CONF_MESSAGE,
    CONF_TYPE,
)

AUTO_LOAD = ["json"]

CONF_LABEL_COLOR = "label_color"
CONF_LIGHT_COLOR = "light_color"
CONF_DARK_COLOR = "dark_color"
CONF_DEFAULT_FONT = "default_font"
CONF_LARGE_FONT = "large_font"
CONF_GLYPH_FONT = "glyph_font"
CONF_LARGE_GLYPH_FONT = "large_glyph_font"
CONF_SHOULD_DRAW = "should_draw"
CONF_WIDGETS = "widgets"

espaper_dashboard_ns = cg.esphome_ns.namespace("espaper_dashboard")
ESPaperDashboard = espaper_dashboard_ns.class_("ESPaperDashboard", cg.Component)
ESPaperDashboardWidget = espaper_dashboard_ns.class_("ESPaperDashboardWidget")

WeatherWidget = espaper_dashboard_ns.class_("WeatherWidget", ESPaperDashboardWidget)
MessageWidget = espaper_dashboard_ns.class_("MessageWidget", ESPaperDashboardWidget)

Color = cg.esphome_ns.class_("Color")
Font = cg.esphome_ns.namespace("font").class_("Font")

WIDGET_SCHEMA_BASE = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(ESPaperDashboardWidget),
        cv.Optional(CONF_SHOULD_DRAW): cv.templatable(cv.boolean),
    },
)


CONF_TEMPERATURE_UOM = "temperature_uom"
CONF_CURRENT_TEMPERATURE_SENSOR_ID = "current_temperature_sensor_id"
CONF_CURRENT_CONDITION_SENSOR_ID = "current_condition_sensor_id"
CONF_FORECAST_SENSOR_ID = "forecast_sensor_id"

WEATHER_WIDGET_SCHEMA = WIDGET_SCHEMA_BASE.extend(
    {
        cv.Optional(CONF_TEMPERATURE_UOM, default="°C"): cv.string,
        cv.Required(CONF_CURRENT_TEMPERATURE_SENSOR_ID): cv.use_id(sensor.Sensor),
        cv.Required(CONF_CURRENT_CONDITION_SENSOR_ID): cv.use_id(
            text_sensor.TextSensor
        ),
        cv.Required(CONF_FORECAST_SENSOR_ID): cv.use_id(text_sensor.TextSensor),
    }
)

MESSAGE_WIDGET_SCHEMA = WIDGET_SCHEMA_BASE.extend(
    {
        cv.Optional(CONF_ICON): cv.templatable(cv.string),
        cv.Required(CONF_MESSAGE): cv.templatable(cv.string),
    }
)

WIDGET_TYPES = {
    "weather": (WeatherWidget, WEATHER_WIDGET_SCHEMA),
    "message": (MessageWidget, MESSAGE_WIDGET_SCHEMA, [CONF_ICON, CONF_MESSAGE]),
}

WIDGET_SCHEMA = cv.typed_schema(
    ({type: data[1] for (type, data) in WIDGET_TYPES.items()}),
    lower=True,
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESPaperDashboard),
        cv.GenerateID(CONF_DISPLAY_ID): cv.use_id(display.Display),
        cv.Optional(CONF_BACKGROUND_COLOR): cv.use_id(Color),
        cv.Optional(CONF_FOREGROUND_COLOR): cv.use_id(Color),
        cv.Optional(CONF_LABEL_COLOR): cv.use_id(Color),
        cv.Optional(CONF_LIGHT_COLOR): cv.use_id(Color),
        cv.Optional(CONF_DARK_COLOR): cv.use_id(Color),
        cv.Required(CONF_DEFAULT_FONT): cv.use_id(Font),
        cv.Optional(CONF_LARGE_FONT): cv.use_id(Font),
        cv.Required(CONF_GLYPH_FONT): cv.use_id(Font),
        cv.Optional(CONF_LARGE_GLYPH_FONT): cv.use_id(Font),
        cv.Required(CONF_WIDGETS): cv.ensure_list(WIDGET_SCHEMA),
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
    if CONF_LIGHT_COLOR in config:
        light_color = await cg.get_variable(config[CONF_LIGHT_COLOR])
        cg.add(var.set_light_color(light_color))
    if CONF_DARK_COLOR in config:
        dark_color = await cg.get_variable(config[CONF_DARK_COLOR])
        cg.add(var.set_dark_color(dark_color))

    default_font = await cg.get_variable(config[CONF_DEFAULT_FONT])
    cg.add(var.set_default_font(default_font))

    if CONF_LARGE_FONT in config:
        large_font = await cg.get_variable(config[CONF_LARGE_FONT])
        cg.add(var.set_large_font(large_font))
    else:
        cg.add(var.set_large_font(default_font))

    glyph_font = await cg.get_variable(config[CONF_GLYPH_FONT])
    cg.add(var.set_glyph_font(glyph_font))

    if CONF_LARGE_GLYPH_FONT in config:
        large_glyph_font = await cg.get_variable(config[CONF_LARGE_GLYPH_FONT])
        cg.add(var.set_large_glyph_font(large_glyph_font))
    else:
        cg.add(var.set_large_glyph_font(glyph_font))

    for widget_conf in config[CONF_WIDGETS]:
        widget_type = WIDGET_TYPES[widget_conf[CONF_TYPE]]
        widget = cg.Pvariable(
            widget_conf[CONF_ID], widget_type[0].new(), widget_type[0]
        )

        cg.add(var.add_widget(widget))
        cg.add(widget.set_target(var))

        if CONF_SHOULD_DRAW in widget_conf:
            should_draw = await cg.templatable(
                widget_conf[CONF_SHOULD_DRAW], [], cg.bool_
            )

            cg.add(widget.set_should_draw(should_draw))

        for k, v in widget_conf.items():
            if k not in (CONF_ID, CONF_SHOULD_DRAW, CONF_TYPE):
                if k.endswith("_id"):
                    v = await cg.get_variable(v)
                    k = k[:-3]
                if (len(widget_type) > 2) and (
                    k in widget_type[2]
                ):  # if it's templateable
                    v = await cg.templatable(v, [], cg.std_string)
                method = getattr(widget, "set_" + k)
                cg.add(method(v))
