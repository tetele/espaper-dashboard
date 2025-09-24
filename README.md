# ESPaper Dashboard

An [ESPHome](http://esphome.io) component meant to be used with e-paper displays in order to create informative dashboards.

You can define a number of widgets and specify via [lambda functions](https://esphome.io/automations/templates) if they should be displayed, in which order. It is optimized for e-paper displays in that it only refreshes if widget data is old or "stale" of the a previously shown widget should not be shown or viceversa.

There are a few built-in widgets (including a `custom` widget that you can program yourself), but the plan is to make widgets extensible and implemented by the community.

During development, it is recommended to test using the [SDL2 component](https://esphome.io/components/display/sdl/#sdl) for the [`host` platform](https://esphome.io/components/host/) in ESPHome, so that compile times are greatly reduced. Once the design of the dashboard is complete, you can obviously move to an e-paper display.

To test with real Home Assistant data, the `host` platform can be connected to a HA instance on port `6053`, which is forwarded in the devcontainer. However, keep in mind that firewalls may play a part and that you are responsible to facilitate HA to connect to the port on your development machine.

The component is included as follows.

```yaml
external_components:
  - source: github://tetele/espaper-dashboard@main
    components: [ espaper_dashboard, espaper_dashboard_widgets ]
```

## Configuration variables

- **id** (*Optional*, [ID](https://www.esphome.io/guides/configuration-types#config-id)): Manually specify the ID used for code generation. Required if there are multiple dashboards.
- **display_id** (*Optional*, [ID](https://www.esphome.io/guides/configuration-types#config-id)): The ID of the [display](https://www.esphome.io/components/display) component the dashboard will be output to. Optional if there is only one display.
- **background_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to fill the background. Defaults to `COLOR_ON`.
- **foreground_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to render text. Defaults to `COLOR_OFF`.
- **label_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to render text labels. Defaults to `COLOR_OFF`.
- **light_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to render elements of the least importance. Defaults to `COLOR_OFF`.
- **dark_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to render elements of the second least importance. Defaults to `COLOR_OFF`.
- **default_font** (**Required**, [Font](https://www.esphome.io/components/font/)): The default font if no other font is specified.
- **large_font** (*Optional*, [Font](https://www.esphome.io/components/font/)): The font used for main information.
- **glyph_font** (**Required**, [Font](https://www.esphome.io/components/font/)): The font containing glyphs, icons and symbols.
- **large_glyph_font** (*Optional*, [Font](https://www.esphome.io/components/font/)): The glyph font used for main information.
- **widgets** (**Required**, list): The list of widgets that the dashboard will have to register. Adding a widget here does not guarantee it will be rendered.

```yaml
espaper_dashboard: # component name
  id: my_dashboard # optional
  background_color: color_white # customize the rendering
  foreground_color: color_black
  light_color: color_light_gray
  dark_color: color_dark_gray
  default_font: default_font
  large_font: large_font
  glyph_font: glyph_font
  widgets: # list of widgets
    - id: weather_today
      should_draw: !lambda 'return true;' # common to all widgets, optional, defaults to true
      width: 480 # common to all widgets, optional, templatable
      height: !lambda 'return 200;' # common to all widgets, optional, templatable
      type: weather # required
      current_condition_sensor_id: weather_current_condition
      current_temperature_sensor_id: weather_current_temperature
      forecast_sensor_id: weather_hourly_forecast
```

### Widget configuration variables

- **id** (*Optional*, [ID](https://www.esphome.io/guides/configuration-types#config-id)): The ID of the widget
- **should_draw** (*Optional*, [templatable](https://esphome.io/automations/templates#config-templatable)): A `bool` value or a lambda returning a `bool` which dictates whether the widget should be rendered or not on the next dashboard update. Defaults to `true` (widget will render).
- **priority** (*Optional*, [templatable](https://esphome.io/automations/templates#config-templatable)): A numeric priority template for sorting widgets in the dashboard. The larger the number, the higher the priority, the sooner the widget will be rendered. Defaults to `0`.
- **type** (**Required**): The type of widget. See below
- **width** (*Optional*, [templatable](https://esphome.io/automations/templates#config-templatable)): The desired width of the widget. All widgets have internal, default sizes based on type.
- **height** (*Optional*, [templatable](https://esphome.io/automations/templates#config-templatable)): The desired height of the widget. All widgets have internal, default sizes based on type.

### Types of widgets

#### Weather widget

Specific configuration:

- **temperature_uom** (*Optional*, `string`): The unit of measurement for the temperature. Defaults to `°C`.
- **current_temperature** (**Required**, [templatable](https://esphome.io/automations/templates#config-templatable)): The current temperature to display
- **current_condition** (**Required**, [templatable](https://esphome.io/automations/templates#config-templatable)): The current weather condition, which must be one of [these values](https://developers.home-assistant.io/docs/core/entity/weather#recommended-values-for-state-and-condition).
- **forecast** (**Required**, [templatable](https://esphome.io/automations/templates#config-templatable)): A list of objects with the forecast. The mandatory format of each object is:
  - **forecast**: a list of objects containing forcasts as `WeatherStatus` objects. Each forecast item contains
    - **title**: a string which designates the timeframe that the forecast describes (e.g. `23:00` or `Tuesday`)
    - **condition**: a string [from this list](https://developers.home-assistant.io/docs/core/entity/weather#recommended-values-for-state-and-condition) containing the weather condition
    - **temperature**: a numeric value representing the temperature for the forecast

Here's an example `forecast` list:

```yaml
espaper_dashboard:
  widgets:
    - type: weather
      forecast:
        - condition: "sunny"
          label: "12:00"
          temperature: 26.7
        - condition: "sunny"
          label: "14:00"
          temperature: 28.5
        - condition: "sunny"
          label: "16:00"
          temperature: 29.2
        - condition: "partlycloudy"
          label: "18:00"
          temperature: 28.3
    - type: weather
      forecast: !lambda |-
        return std::vector<espaper_dashboard_widgets::WeatherStatus> {
          espaper_dashboard_widgets::WeatherStatus("12:00", 26.7, "sunny"),
          espaper_dashboard_widgets::WeatherStatus("14:00", 28.5, "sunny"),
          espaper_dashboard_widgets::WeatherStatus("16:00", 29.2, "sunny"),
          espaper_dashboard_widgets::WeatherStatus("18:00", 28.3, "partlycloudy")
        };
```

You can create such a sensor from HA using a template such as

```yaml
template:
  - triggers:
      - trigger: time_pattern
        hours: /1
      - trigger: event
        event_type: manual_event_template_reloaded
    actions:
      - action: weather.get_forecasts
        target:
          entity_id: weather.home
        data:
          type: hourly
        response_variable: forecast
    sensor:
      - name: Hourly weather forecast
        state: "{{ now().isoformat() }}"
        unique_id: hourly_weather_forecast
        attributes:
          current_condition: "{{ states('weather.home') }}"
          current_temperature: "{{ state_attr('weather.home', 'temperature') }}"
          temperature_uom: "{{ state_attr('weather.home', 'temperature_unit') }}"
          forecast: >-
            {% set forecast_attr = [
              {
                "title": as_timestamp(forecast['weather.home'].forecast[1].datetime) | timestamp_custom("%H:00"),
                "condition": forecast['weather.home'].forecast[1].condition,
                "temperature": forecast['weather.home'].forecast[1].temperature,
              },
              {
                "title": as_timestamp(forecast['weather.home'].forecast[3].datetime) | timestamp_custom("%H:00"),
                "condition": forecast['weather.home'].forecast[3].condition,
                "temperature": forecast['weather.home'].forecast[3].temperature,
              },
              {
                "title": as_timestamp(forecast['weather.home'].forecast[5].datetime) | timestamp_custom("%H:00"),
                "condition": forecast['weather.home'].forecast[5].condition,
                "temperature": forecast['weather.home'].forecast[5].temperature,
              },
              {
                "title": as_timestamp(forecast['weather.home'].forecast[7].datetime) | timestamp_custom("%H:00"),
                "condition": forecast['weather.home'].forecast[7].condition,
                "temperature": forecast['weather.home'].forecast[7].temperature,
              },
            ] %}
            {{ {'forecast': forecast_attr} | tojson }}
```

### Message Widget

Displays a message with an optional icon.

Specific configuration:

- **icon** (*Optional*, [templatable](https://esphome.io/automations/templates#config-templatable)): The character used for the icon
- **message** (**Required**, [templatable](https://esphome.io/automations/templates#config-templatable)): The message to display

### Custom Widget

Displays a custom widget however you want it.

Specific configuration:

- **lambda** (**Required**, [lambda](https://www.esphome.io/automations/templates/#all-lambda-calls)): A function you define that will be called when the widget needs to be drawn. The function receives 3 input arguments:
  - **it**: A [`Display`](https://www.esphome.io/components/display) component that you can use to do the actual drawing like `it.draw_pixel_at(2,3);`
  - **start_x**: The X coordinate where the widget should be drawn
  - **start_y**: The Y coordinate where the widget should be drawn

## Actions

### `espaper_dashboard_widget.mark_stale`

If data is being imported from another system, we need a way to signal to the dashboard that the currently displayed data is no longer valid. For this purpose, you can mark a widget as "stale" when the data it uses is refreshed.

Parameters:
- **widget_id** (**Required**, ID): The ID of the widget you want to mark as stale

There is a shorthand version of this action. The following are equivalent:

```yaml
actions:
  - espaper_dashboard_widget.mark_stale:
      widget_id: my_widget
  - espaper_dashboard_widget.mark_stale: my_widget
```

Example implementation:

```yaml
sensor:
  - id: external_sensor
    platform: homeassistant
    entity_id: sensor.my_sensor
    on_value:
      then:
        - espaper_dashboard_widget.mark_stale: widget_using_sensor

espaper_dashboard:
  ...
  widgets:
    - id: widget_using_sensor
      ...
```

## Usage

1. Define a dashboard with some widgets
1. Add `should_draw` and `priority` lambdas to those widgets to filter and sort which of them should get drawn
1. Call `component.update` on the `espaper_dashboard` component to redraw it, if needed

The dashboard will NOT refresh automatically, you need to call `component.update` to refresh it. Even then, it will only actually refresh if any of the following conditions are true:
- a previously displayed widget's `should_draw` lambda returns `false`
- a previously hidden widget's `should_draw` lambda returns `true`
- a visible widget's `priority` has changed
- a visible widget was marked as stale using `espaper_dashboard_widget.mark_stale`
