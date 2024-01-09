# ESPHome Jiecang Desk Controller

[ESPHome](https://esphome.io/) component for controlling Jiecang desk controllers via their serial protocol.

Attention: I only have one [RJ12 model](https://www.jiecang.com/product/jcb35m11c.html) to test this
but it's likely that other Jiecang controllers are supported as their serial protocol should be compatible.

## Usage

### What you need

* ESPHome compatible microcontroller
* depending on your model of desk controller
    * cable with RJ12 connector (phone cable, RJ11 may also work)
    * cable with RJ45 connector (network cable)

### Wiring

#### RJ12

Please double check this for your specific model!

pin | function
----|---------
 1  | NC (pulled up)
 2  | GND
 3  | TX
 4  | VCC
 5  | GND
 6  | NC (pulled up)

#### RJ45

Untested and only for reference!

pin | function
----|---------
 1  | HS3 [^1]
 2  | TX
 3  | GND
 4  | RX
 5  | VCC
 6  | HS2 [^1]
 7  | HS1 [^1]
 8  | HS0 [^1]

[^1]: not used here

#### microcontroller

ESP | desk
----|-----
GND | GND
5V  | VCC
RX  | TX
TX  | RX

## Usage

```yaml
esphome:
  ...
  on_boot:
    # This script is required to initialize the following sensors:
    #    height_pct, height_min, height_max, position1 - position4
    # You can skip this if you don't use those.
    priority: 0 # when mostly everything else is done
    then:
      - lambda: "id(my_desk).request_physical_limits();"
      - delay: 0.1s
      - lambda: "id(my_desk).request_limits();"
      - delay: 0.1s
      - lambda: "id(my_desk).request_settings();"

external_components:
  - source:
      type: git
      url: https://github.com/Rocka84/esphome_components/
    components: [ jiecang_desk_controller ]

uart:
  id: uart_bus
  tx_pin: TX
  rx_pin: RX
  baud_rate: 9600

logger:
  baud_rate: 0 # disable logging over uart, required when using the RX/TX pins for the controller

jiecang_desk_controller:
  id: my_desk
  sensors:
    height:
      name: "Height"
  buttons:
    raise:
      name: "Raise"
    position1:
      name: "Position 1"

button:
  - platform: template
    name: "Lower"
    on_press:
      lambda: "id(my_desk).lower();"
```

See also [example_jiecang_desk_controller.yaml](../../example_jiecang_desk_controller.yaml).

### Available sensors

sensor     | description
-----------|----------------------------
height     | current height of the desk
height_pct | height in percent
height_min | minimal height
height_max | maximal height
position1  | height of 1st stored height
position2  | height of 2nd stored height
position3  | height of 3rd stored height
position4  | height of 4th stored height

### Available buttons and methods

button     | lambda method                  | description
-----------|--------------------------------|---------------------------
raise      | `id(my_desk).raise()`          | raise desk by one step (~14mm)
lower      | `id(my_desk).raise()`          | lower desk by one step (~14mm)
position1  | `id(my_desk).goto_position(1)` | move to 1st stored height
position2  | `id(my_desk).goto_position(2)` | move to 2nd stored height
position3  | `id(my_desk).goto_position(3)` | move to 3rd stored height
position4  | `id(my_desk).goto_position(4)` | move to 4th stored height

## Sources

Thanks to [phord/Jarvis](https://github.com/phord/Jarvis) for reverse engineering the UART interface and most control messages

