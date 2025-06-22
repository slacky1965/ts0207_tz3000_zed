import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    zigbeeModel: ['ZG-222ZA-z-SlD'],
    model: 'ZG-222ZA-z-SlD',
    vendor: 'Slacky-DIY',
    description: 'Water leak sensor with custom firmware',
    extend: [
      m.battery({"voltage": true, "voltageReporting": true,
                 "percentageReportingConfig": {"min": 3600, "max": 0, "change": 0}, 
                 "voltageReportingConfig": {"min": 3600, "max": 0, "change": 0}}),
      m.iasZoneAlarm({"zoneType":"water_leak","zoneAttributes":["alarm_1","battery_low"]}),
      m.commandsOnOff(),
      m.enumLookup({
        name: "switch_actions",
        lookup: {off: 0, on: 1, toggle: 2},
        cluster: "genOnOffSwitchCfg",
        attribute: "switchActions",
        description: "Actions switch",
      }),
      ],
    meta: {},
    ota: true,
};
