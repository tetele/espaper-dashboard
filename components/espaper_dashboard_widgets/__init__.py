from typing import Any

import esphome.codegen as cg
from esphome.components import sensor, text_sensor
from esphome.components.espaper_dashboard import (
    WIDGET_SCHEMA_BASE,
    ESPaperDashboardWidget,
    espaper_dashboard_ns,
    supported_widgets,
)
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_MESSAGE

DEPENDENCIES = ["espaper_dashboard"]

CONFIG_SCHEMA = cv.Schema({})

WeatherWidget = espaper_dashboard_ns.class_("WeatherWidget", ESPaperDashboardWidget)
MessageWidget = espaper_dashboard_ns.class_("MessageWidget", ESPaperDashboardWidget)


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


async def weather_widget_to_code(widget: cg.MockObj, config: dict[str, Any]):
    cg.add(widget.set_temperature_uom(config[CONF_TEMPERATURE_UOM]))

    cg.add(
        widget.set_current_temperature_sensor(
            await cg.get_variable(config[CONF_CURRENT_TEMPERATURE_SENSOR_ID])
        )
    )
    cg.add(
        widget.set_current_condition_sensor(
            await cg.get_variable(config[CONF_CURRENT_CONDITION_SENSOR_ID])
        )
    )
    cg.add(
        widget.set_forecast_sensor(
            await cg.get_variable(config[CONF_FORECAST_SENSOR_ID])
        )
    )


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
