# Example ESPHome Component

This is a template repository used to develop [ESPHome](http://esphome.io) components, which can later be included in anyone's ESPHome configurations using the [`external_component`](https://esphome.io/components/external_components.html#external-components) syntax:

```yaml
external_components:
  - source: github://you/your-component-repo@ref
    components: [ your_component ]
```

## Approach

The dev environment resulted by instantiating this template repository is meant to be used inside a [dev container](https://containers.dev/). To do so, you must have Docker and a supported editor (such as [VSCode](https://code.visualstudio.com/)).

Within that devcontainer, you'll find a working instance of ESPHome, an instantance of [PlatformIO](https://platformio.org/) used for code completion and validation.

## Usage

1. Click the "Use this template" button in this repository's page on GitHub to create your own repo based on it.
1. Clone your new repository locally.
1. Open a new VSCode window and invoke the command palette, then select `Dev Containers: Open Folder in Container...`.
1. Point to the folder where you have cloned the repository locally and then wait for the devcontainer to be created and opened.
1. When asked, install the PlatformIO extension in the devcontainer.
1. When asked, reload the IDE window or restart VSCode.
1. Wait for PlatformIO to be configured (this will take a LONG time) and make sure there are no errors in `Output > PlatformIO: Project Configuration`.
1. Rename `example_component` files, paths and namespaces in the `esphome/components` folder.
1. Rename class names and [start developing](https://developers.esphome.io/architecture/components/) your own component.
