const {identify, reporting, ota} = require('zigbee-herdsman-converters/lib/modernExtend');

const definition = {
    fingerprint: [{manufacturerName: '_TZ3000_k4ej3ww2'},
                  {manufacturerName: '_TZ3000_upgcbody'}
                 ],
    zigbeeModel: ['TS0207', 'SNZB-05'],
    model: 'Original Tuya Water Leak sensor ready for update',
    vendor: 'Slacky-DIY',
    description: 'Original Tuya waterleak sensor ready for custom Firmware update',
    extend: [
      identify(),
    ],
    meta: {},
    ota: true,
};

module.exports = definition;