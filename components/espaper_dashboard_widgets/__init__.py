from typing import Any

import esphome.codegen as cg
from esphome.components import sensor, text_sensor
from esphome.components.espaper_dashboard import (
    WIDGET_SCHEMA_BASE,
    ESPaperDashboardWidget,
    supported_widgets,
)
import esphome.config_validation as cv
from esphome.const import CONF_CONDITION, CONF_ICON, CONF_MESSAGE, CONF_TEMPERATURE

DEPENDENCIES = ["espaper_dashboard"]

CONFIG_SCHEMA = cv.Schema({})

espaper_dashboard_widgets_ns = cg.esphome_ns.namespace("espaper_dashboard_widgets")

WeatherWidget = espaper_dashboard_widgets_ns.class_(
    "WeatherWidget", ESPaperDashboardWidget
)
MessageWidget = espaper_dashboard_widgets_ns.class_(
    "MessageWidget", ESPaperDashboardWidget
)


CONF_TEMPERATURE_UOM = "temperature_uom"
CONF_CURRENT_TEMPERATURE = "current_temperature"
CONF_CURRENT_CONDITION = "current_condition"
CONF_FORECAST = "forecast"
CONF_TITLE = "title"
WeatherCondition = espaper_dashboard_widgets_ns.enum("WeatherCondition")
str_to_condition = espaper_dashboard_widgets_ns.str_to_condition_
WeatherStatus = espaper_dashboard_widgets_ns.struct("WeatherStatus")

WEATHER_WIDGET_SCHEMA = WIDGET_SCHEMA_BASE.extend(
    {
        cv.Optional(CONF_TEMPERATURE_UOM, default="°C"): cv.string,
        cv.Required(CONF_CURRENT_TEMPERATURE): cv.templatable(cv.float_),
        cv.Required(CONF_CURRENT_CONDITION): cv.templatable(cv.string),
        cv.Required(CONF_FORECAST): cv.templatable(
            cv.ensure_list(
                cv.ensure_schema(
                    {
                        cv.Required(CONF_TITLE): cv.string,
                        cv.Required(CONF_TEMPERATURE): cv.float_,
                        cv.Required(CONF_CONDITION): cv.string,
                    }
                )
            )
        ),
    }
)


async def weather_widget_to_code(widget: cg.MockObj, config: dict[str, Any]):
    cg.add(widget.set_temperature_uom(config[CONF_TEMPERATURE_UOM]))

    current_condition = await cg.templatable(
        config[CONF_CURRENT_CONDITION], [], cg.std_string
    )
    cg.add(widget.set_current_condition(current_condition))

    current_temperature = await cg.templatable(
        config[CONF_CURRENT_TEMPERATURE], [], cg.float_
    )
    cg.add(widget.set_current_temperature(current_temperature))

    if CONF_FORECAST in config:

        def forecast_to_vector(forecast_values):
            return cg.std_vector.template(WeatherStatus)(
                [
                    WeatherStatus(
                        f[CONF_TITLE],
                        f[CONF_TEMPERATURE],
                        str_to_condition(f[CONF_CONDITION]),
                    )
                    for f in forecast_values
                ]
            )

        forecast = await cg.templatable(
            config[CONF_FORECAST],
            [],
            cg.std_vector.template(WeatherStatus),
            forecast_to_vector,
        )
        cg.add(widget.set_forecast(forecast))


MESSAGE_WIDGET_SCHEMA = WIDGET_SCHEMA_BASE.extend(
    {
        cv.Optional(CONF_ICON): cv.templatable(cv.string),
        cv.Required(CONF_MESSAGE): cv.templatable(cv.string),
    }
)


async def message_widget_to_code(widget: cg.MockObj, config: dict[str, Any]):
    if CONF_ICON in config:
        icon = await cg.templatable(config[CONF_ICON], [], cg.std_string)
        cg.add(widget.set_icon(icon))

    message = await cg.templatable(config[CONF_MESSAGE], [], cg.std_string)
    cg.add(widget.set_message(message))


supported_widgets.register(
    "weather", WeatherWidget, WEATHER_WIDGET_SCHEMA, weather_widget_to_code
)
supported_widgets.register(
    "message", MessageWidget, MESSAGE_WIDGET_SCHEMA, message_widget_to_code
)
