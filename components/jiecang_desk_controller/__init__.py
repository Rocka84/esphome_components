import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart, sensor, button
from esphome.const import CONF_ID, CONF_HEIGHT, UNIT_CENTIMETER, UNIT_PERCENT

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor', 'button']

jiecang_desk_controller_ns = cg.esphome_ns.namespace('jiecang_desk_controller')

JiecangDeskController = jiecang_desk_controller_ns.class_('JiecangDeskController', cg.Component, uart.UARTDevice)
JiecangDeskButton = jiecang_desk_controller_ns.class_('JiecangDeskButton', button.Button, cg.Component)


CONF_SENSORS = "sensors"
CONF_BUTTONS = "buttons"
CONF_UNIT = "unit"
CONF_RAISE = "raise"
CONF_LOWER = "lower"
CONF_STOP = "stop"
CONF_HEIGHT_MIN = "height_min"
CONF_HEIGHT_MAX = "height_max"
CONF_HEIGHT_PCT = "height_pct"
CONF_POSITION1 = "position1"
CONF_POSITION2 = "position2"
CONF_POSITION3 = "position3"
CONF_POSITION4 = "position4"


button_constants = {}
button_constants[CONF_RAISE] = 0
button_constants[CONF_LOWER] = 1
button_constants[CONF_STOP] = 2
button_constants[CONF_POSITION1] = 3
button_constants[CONF_POSITION2] = 4
button_constants[CONF_POSITION3] = 5
button_constants[CONF_POSITION4] = 6

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(JiecangDeskController),
    cv.Optional(CONF_SENSORS): cv.Schema({
        cv.Optional(CONF_HEIGHT): sensor.sensor_schema(
            accuracy_decimals = 1,
            unit_of_measurement = UNIT_CENTIMETER
        ),
        cv.Optional(CONF_UNIT): sensor.sensor_schema(
            accuracy_decimals = 0
        ),
        cv.Optional(CONF_HEIGHT_PCT): sensor.sensor_schema(
            accuracy_decimals = 1,
            unit_of_measurement = UNIT_PERCENT
        ),
        cv.Optional(CONF_HEIGHT_MIN): sensor.sensor_schema(
            accuracy_decimals = 1,
            unit_of_measurement = UNIT_CENTIMETER
        ),
        cv.Optional(CONF_HEIGHT_MAX): sensor.sensor_schema(
            accuracy_decimals = 1,
            unit_of_measurement = UNIT_CENTIMETER
        ),
        cv.Optional(CONF_POSITION1): sensor.sensor_schema(
            accuracy_decimals = 1,
            unit_of_measurement = UNIT_CENTIMETER
        ),
        cv.Optional(CONF_POSITION2): sensor.sensor_schema(
            accuracy_decimals = 1,
            unit_of_measurement = UNIT_CENTIMETER
        ),
        cv.Optional(CONF_POSITION3): sensor.sensor_schema(
            accuracy_decimals = 1,
            unit_of_measurement = UNIT_CENTIMETER
        ),
        cv.Optional(CONF_POSITION4): sensor.sensor_schema(
            accuracy_decimals = 1,
            unit_of_measurement = UNIT_CENTIMETER
        ),
    }),
    cv.Optional(CONF_BUTTONS): cv.Schema({
        cv.Optional(CONF_RAISE): button.BUTTON_SCHEMA.extend({cv.GenerateID(): cv.declare_id(JiecangDeskButton)}),
        cv.Optional(CONF_LOWER): button.BUTTON_SCHEMA.extend({cv.GenerateID(): cv.declare_id(JiecangDeskButton)}),
        cv.Optional(CONF_STOP): button.BUTTON_SCHEMA.extend({cv.GenerateID(): cv.declare_id(JiecangDeskButton)}),
        cv.Optional(CONF_POSITION1): button.BUTTON_SCHEMA.extend({cv.GenerateID(): cv.declare_id(JiecangDeskButton)}),
        cv.Optional(CONF_POSITION2): button.BUTTON_SCHEMA.extend({cv.GenerateID(): cv.declare_id(JiecangDeskButton)}),
        cv.Optional(CONF_POSITION3): button.BUTTON_SCHEMA.extend({cv.GenerateID(): cv.declare_id(JiecangDeskButton)}),
        cv.Optional(CONF_POSITION4): button.BUTTON_SCHEMA.extend({cv.GenerateID(): cv.declare_id(JiecangDeskButton)}),
    }),
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_SENSORS in config:
        sensors = config[CONF_SENSORS]

        if CONF_HEIGHT in sensors:
            sens = await sensor.new_sensor(sensors[CONF_HEIGHT])
            cg.add(var.set_sensor_height(sens))
        if CONF_UNIT in sensors:
            sens = await sensor.new_sensor(sensors[CONF_UNIT])
            cg.add(var.set_sensor_unit(sens))
        if CONF_HEIGHT_MIN in sensors:
            sens = await sensor.new_sensor(sensors[CONF_HEIGHT_MIN])
            cg.add(var.set_sensor_height_min(sens))
        if CONF_HEIGHT_MAX in sensors:
            sens = await sensor.new_sensor(sensors[CONF_HEIGHT_MAX])
            cg.add(var.set_sensor_height_max(sens))
        if CONF_HEIGHT_PCT in sensors:
            sens = await sensor.new_sensor(sensors[CONF_HEIGHT_PCT])
            cg.add(var.set_sensor_height_pct(sens))
        if CONF_POSITION1 in sensors:
            sens = await sensor.new_sensor(sensors[CONF_POSITION1])
            cg.add(var.set_sensor_position1(sens))
        if CONF_POSITION2 in sensors:
            sens = await sensor.new_sensor(sensors[CONF_POSITION2])
            cg.add(var.set_sensor_position2(sens))
        if CONF_POSITION3 in sensors:
            sens = await sensor.new_sensor(sensors[CONF_POSITION3])
            cg.add(var.set_sensor_position3(sens))
        if CONF_POSITION4 in sensors:
            sens = await sensor.new_sensor(sensors[CONF_POSITION4])
            cg.add(var.set_sensor_position4(sens))

    if CONF_BUTTONS in config:
        buttons = config[CONF_BUTTONS]
        for button_type in buttons.keys():
            btn = await button.new_button(buttons[button_type])
            cg.add(var.add_button(btn, button_constants[button_type]))

