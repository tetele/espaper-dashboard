from collections.abc import Callable
from typing import Any

from esphome import automation
import esphome.codegen as cg
from esphome.components import display
import esphome.config_validation as cv
from esphome.const import (
    CONF_BACKGROUND_COLOR,
    CONF_DISPLAY_ID,
    CONF_FOREGROUND_COLOR,
    CONF_HEIGHT,
    CONF_ID,
    CONF_PRIORITY,
    CONF_TYPE,
    CONF_WIDTH,
)

AUTO_LOAD = ["json", "espaper_dashboard_widgets"]

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

ESPaperDashboardWidgetMarkStaleAction = espaper_dashboard_ns.class_(
    "ESPaperDashboardWidgetMarkStaleAction", automation.Action
)

ESPaperDashboardNeedsRedrawCondition = espaper_dashboard_ns.class_(
    "ESPaperDashboardNeedsRedrawCondition", automation.Condition
)


class WidgetData:
    def __init__(
        self,
        type: cg.MockObjClass,
        schema: cv.Schema,
        to_code_func: Callable[[cg.MockObj, dict[str, Any]], None],
    ):
        self.type: cg.MockObjClass = type
        self.schema: cv.Schema = schema
        self.to_code: Callable[[cg.MockObj, dict[str, Any]], None] = to_code_func


class Widgets:
    def __init__(self):
        self.widget_types: dict[str, WidgetData] = {}

    def register(
        self,
        name: str,
        type: cg.MockObjClass,
        schema: cv.Schema,
        to_code_func: Callable[[cg.MockObj, dict], None],
    ):
        """Register a new widget type."""
        assert name not in self.widget_types, f"Widget {name} already registered"
        assert type.inherits_from(ESPaperDashboardWidget), (
            f"Widget {name} type ({type}) must inherit from {ESPaperDashboardWidget}"
        )

        self.widget_types[name] = WidgetData(type, schema, to_code_func)

    def get(self, name: str) -> WidgetData:
        if name in self.widget_types:
            return self.widget_types[name]
        return None

    def schema(self) -> cv.Schema:
        """Return the schema for all registered widgets."""
        return cv.typed_schema(
            ({name: data.schema for (name, data) in self.widget_types.items()}),
            lower=True,
        )

    def validate(self, value):
        # Custom validation is used so that we make sure that all widgets are registered at the time of schema validation
        return self.schema()(value)

    async def to_code(self, dashboard: cg.MockObj, config: dict[str, Any]) -> None:
        assert config[CONF_TYPE] in self.widget_types
        structure: WidgetData = self.widget_types[config[CONF_TYPE]]

        widget = cg.Pvariable(
            config[CONF_ID],
            structure.type.new(str(config[CONF_ID])),
            structure.type,
        )

        cg.add(dashboard.add_widget(widget))
        cg.add(widget.set_target(dashboard))

        if CONF_SHOULD_DRAW in config:
            should_draw = await cg.templatable(config[CONF_SHOULD_DRAW], [], cg.bool_)

            cg.add(widget.set_should_draw(should_draw))

        if CONF_PRIORITY in config:
            priority = await cg.templatable(config[CONF_PRIORITY], [], cg.int_)

            cg.add(widget.set_priority(priority))

        if CONF_WIDTH in config and CONF_HEIGHT in config:
            width = await cg.templatable(config[CONF_WIDTH], [], cg.int_)
            height = await cg.templatable(config[CONF_HEIGHT], [], cg.int_)
            cg.add(widget.set_width(width))
            cg.add(widget.set_height(height))

        await structure.to_code(widget, config)


supported_widgets = Widgets()

Color = cg.esphome_ns.class_("Color")
Font = cg.esphome_ns.namespace("font").class_("Font")

WIDGET_SCHEMA_BASE = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(ESPaperDashboardWidget),
        cv.Optional(CONF_SHOULD_DRAW): cv.templatable(cv.boolean),
        cv.Optional(CONF_PRIORITY): cv.templatable(cv.int_),
        cv.Optional(CONF_WIDTH): cv.templatable(cv.Range(min=1)),
        cv.Optional(CONF_HEIGHT): cv.templatable(cv.Range(min=1)),
    },
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
        cv.Required(CONF_WIDGETS): cv.ensure_list(supported_widgets.validate),
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
        await supported_widgets.to_code(var, widget_conf)


ESPAPER_DASHBOARD_NEEDS_REDRAW_CONDITION_SCHEMA = automation.maybe_simple_id(
    {
        cv.GenerateID(): cv.use_id(ESPaperDashboard),
    }
)


@automation.register_condition(
    "espaper_dashboard.needs_redraw",
    ESPaperDashboardNeedsRedrawCondition,
    ESPAPER_DASHBOARD_NEEDS_REDRAW_CONDITION_SCHEMA,
)
async def espaper_dashboard_needs_redraw_to_code(
    config, condition_id, template_arg, args
):
    parent = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(condition_id, template_arg, parent)


CONF_WIDGET_ID = "widget_id"
ESPAPER_DASHBOARD_WIDGET_MARK_STALE_ACTION_SCHEMA = cv.maybe_simple_value(
    {
        cv.GenerateID(CONF_WIDGET_ID): cv.use_id(ESPaperDashboardWidget),
    },
    key=CONF_WIDGET_ID,
)


@automation.register_action(
    "espaper_dashboard_widget.mark_stale",
    ESPaperDashboardWidgetMarkStaleAction,
    ESPAPER_DASHBOARD_WIDGET_MARK_STALE_ACTION_SCHEMA,
)
async def espaper_dashboard_widget_mark_stale_to_code(
    config, action_id, template_arg, args
):
    parent = await cg.get_variable(config[CONF_WIDGET_ID])
    var = cg.new_Pvariable(action_id, template_arg, parent)
    return var
