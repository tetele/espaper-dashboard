# ESPaper Dashboard

An [ESPHome](http://esphome.io) component meant to be used with e-paper displays in order to create informative dashboards.

```yaml
external_components:
  - source: github://tetele/espaper-dashboard@main
    components: [ espaper_dashboard ]
```

## Configuration variables

- **id** (*Optional*, [ID](https://www.esphome.io/guides/configuration-types#config-id)): Manually specify the ID used for code generation. Required if there are multiple dashboards.
- **display_id** (*Optional*, [ID](https://www.esphome.io/guides/configuration-types#config-id)): The ID of the [display](https://www.esphome.io/components/display) component the dashboard will be output to. Optional if there is only one display.
- **background_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to fill the background. Defaults to `COLOR_ON`.
- **foreground_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to render text. Defaults to `COLOR_OFF`.
- **label_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to render text labels. Defaults to `COLOR_OFF`.
- **light_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to render elements of the least importance. Defaults to `COLOR_OFF`.
- **dark_color** (*Optional*, [Color](https://www.esphome.io/components/display/#color)): The color used to render elements of the second least importance. Defaults to `COLOR_OFF`.
- **widgets** (**Required**, list): The list of widgets that the dashboard will have to register. Adding a widget here does not guarantee it will be rendered.

```yaml
espaper_dashboard: # component name
  id: my_dashboard # optional
  background_color: color_white # customize the rendering
  foreground_color: color_black
  light_color: color_light_gray
  dark_color: color_dark_gray
  widgets: # list of widgets
    - id: weather_today
      should_draw: !lambda 'return true;' # common to all widgets, optional, defaults to true
      type: weather # required
      forecast_type: hourly # weather-specific field
```

### Widget configuration variables

- **id** (*Optional*, [ID](https://www.esphome.io/guides/configuration-types#config-id)): The ID of the widget
- **should_draw** (*Optional*, [lambda](https://www.esphome.io/automations/templates#config-lambda)): A lambda function used to specify whether the widget should render or not on the next dashboard update. If no lambda is set, the widget will render.
- **type** (**Required**): The type of widget. See below

### Types of widgets

#### Weather widget

Specific configuration:

- **forecast_type** (*Optional*): The type of forecast to display. Default is `hourly`.
  - **hourly**: Hourly forecast
  - **twice_daily**: Twice daily forecast
  - **daily**: Daily forecast

## Usage

TODO
