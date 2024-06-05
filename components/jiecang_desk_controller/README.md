# ESPHome Jiecang Desk Controller

[ESPHome](https://esphome.io/) component for controlling Jiecang desk controllers via their serial protocol.

![](https://github.com/Rocka84/esphome_components/assets/2353088/b04d1fd7-d0ed-443d-87b7-87454757c0f0)

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
 5  | RX
 6  | NC (pulled up)

#### RJ45

**Untested** and only for reference!

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

![](https://github.com/Rocka84/esphome_components/assets/2353088/39e08774-a215-4e7e-8345-a2acafce28a2)

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
  numbers:
    height:
      name: "Height"
  buttons:
    step_up:
      name: "Step up"
    position1:
      name: "Position 1"

button:
  - platform: template
    name: "Step down"
    on_press:
      lambda: "id(my_desk).goto_height(84.0);"
```

### Examples

* basic, sufficient for most people: [example_basic.yaml](example_basic.yaml)
* full with all features in use: [example_full.yaml](example_full.yaml)

### Features

#### sensors

sensor     | description
-----------|----------------------------
height     | current height of the desk
height_pct | height in percent
height_min | minimal height
height_max | maximal height
position1  | 1st stored height
position2  | 2nd stored height
position3  | 3rd stored height
position4  | 4th stored height

####  number entities

Hold current values read from the desk and set values to desk when changed.

sensor     | description
-----------|----------------------------
height     | current height of the desk


####  buttons

button        | description
--------------|---------------------------
step_up       | step_up desk by one step (~14mm)
step_down     | step_down desk by one step (~14mm)
move_up       | move up
move_down     | move down
stop          | stop movement of desk
position1     | move to 1st stored height
position2     | move to 2nd stored height
position3     | move to 3rd stored height
position4     | move to 4th stored height
save_position | press position button afterwards to store current height to that position

#### lambda methods

lambda method                     | description
----------------------------------|---------------------------
`id(my_desk).step_up()`           | step_up desk by one step (~14mm)
`id(my_desk).step_down()`         | step_down desk by one step (~14mm)
`id(my_desk).move_up(height)`     | move up
`id(my_desk).move_down(height)`   | move down
`id(my_desk).stop()`              | stop movement of desk
`id(my_desk).goto_position(pos)`  | move to stored height `pos`
`id(my_desk).save_position(pos)`  | save current height to position `pos`
`id(my_desk).goto_height(height)` | move to specified height

## Case

You can find a 3D-printable case in the [stl folder](stl/) or on [onshape](https://cad.onshape.com/documents/9619b6e4e11b26a3e9d82630/w/ca8259951d8b6bb3513992f7/e/2aa1144a31e5b4c252a13681?renderMode=0&uiState=665987de1c1f4a6c5d2d97a4).

## Sources

Thanks to [phord/Jarvis](https://github.com/phord/Jarvis) for reverse engineering the UART interface and most control messages!  
Thanks to [OkhammahkO](https://github.com/OkhammahkO) for collecting the scattered information at [pimp-my-desk/desk-control](https://gitlab.com/pimp-my-desk/desk-control) and for his work in the home assistant community!

