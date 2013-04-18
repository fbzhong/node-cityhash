var cityhash = require('./');

var passed = true;

function equal(expected, actual) {
    var type_expected = typeof expected;
    var type_actual = typeof actual;

    if(type_expected != type_actual) {
        return false;
    } else if(type_expected === 'object') {
        for(var key in expected) {
            var expected_value = expected[key];
            var actual_value = actual[key];
            if(!equal(expected_value, actual_value)) {
                return false;
            }
        }
    } else if(expected != actual) {
        return false;
    }
    return true;
}

function assertEqual(expected, actual, message) {
    var failed = !equal(expected, actual);

    if(failed) {
        if(message) {
            message += '. ';
        } else {
            message = '';
        }
        passed = false;
        console.log('[Error]' + message + 'Expected ' + JSON.stringify(expected) + ' , but actual is ' + JSON.stringify(actual));
    } else {
        console.log('[PASS] ' + message);
    }
}

function end() {
    if(passed) {
        console.log('All tests are passed.');
    }
}

assertEqual('6596376470467341850', cityhash.stringify({low:1727229466, high:1535838579}), 'Stringify for uint64 object');
assertEqual({low:1727229466, high:1535838579, uint64:true}, cityhash.objectify('6596376470467341850'), 'Objectify for uint64 hash string');

assertEqual('9138004313465017137,12242971252332641544', cityhash.stringify({
            "low": {
                "low": 68277041,
                "high": 2127607426
            },
            "high": {
                "low": 3936042248,
                "high": 2850538876
            }}), 'Stringify for uint128 object');

assertEqual({
            "low": {
                "low": 68277041,
                "high": 2127607426
            },
            "high": {
                "low": 3936042248,
                "high": 2850538876
            }}, cityhash.objectify('9138004313465017137,12242971252332641544'), 'Stringify for uint128 object');

assertEqual({
    low: 1727229466,
    high: 1535838579,
    uint64: true,
    value:'6596376470467341850'
    }, cityhash.hash64('Hello'), 'Hash64 for "Hello"');

assertEqual({
        low: 4079208671,
        high: 600288677,
        uint64: true,
        value: '2578220239953316063'
    }, cityhash.hash64('hello'), 'Hash64 for "hello"');

assertEqual({
        low: 2569634289,
        high: 3664379964,
        uint64: true,
        value: '15738392108067291633'
    }, cityhash.hash64('Hello', 87392039), 'Hash64 for "hello" with seed 87392039');

var b = new Buffer(4)
b.writeUInt32LE(7771789, 0)
assertEqual({
    low: 3883967176,
    high: 3780832496,
    uint64: true,
    value:'16238551925858017992'
    }, cityhash.hash64(b, "2310915232984335893"), 'Hash64 for Buffer');

assertEqual({
        low: 2569634289,
        high: 3664379964,
        uint64: true,
        value: '15738392108067291633'
    }, cityhash.hash64('Hello', cityhash.objectify(87392039)), 'Hash64 for "hello" with objectify seed 87392039');

assertEqual({
        low: 2389520903,
        high: 3787608545,
        value: '16267654833214665223',
        uint64: true,
    }, cityhash.hash64('Hello', 87392039, 1230234), 'Hash64 for "hello" with seed 87392039 and 1230234');

assertEqual({
        "low": {
            "low": 68277041,
            "high": 2127607426,
            "value": "9138004313465017137",
            "uint64": true
        },
        "high": {
            "low": 3936042248,
            "high": 2850538876,
            "value": "12242971252332641544",
            "uint64": true
        },
        "value": "9138004313465017137,12242971252332641544",
        "uint128": true
    }, cityhash.hash128('Hello'), 'Hash128 for "Hello"');

assertEqual({
        "low": {
            "low": 3440602095,
            "high": 3148776037,
            "value": "13523890104784088047",
            "uint64": true
        },
        "high": {
            "low": 2750723564,
            "high": 4052229467,
            "value": "17404193039403234796",
            "uint64": true
        },
        "value": "13523890104784088047,17404193039403234796",
        "uint128": true
    }, cityhash.hash128('hello'), 'Hash128 for "hello"');

assertEqual({
        "low": {
            "low": 3184066266,
            "high": 3674042232,
            "value": "15779891233746910938",
            "uint64": true
        },
        "high": {
            "low": 4196783977,
            "high": 3519958761,
            "value": "15118107765960464233",
            "uint64": true
        },
        "value": "15779891233746910938,15118107765960464233",
        "uint128": true
    }, cityhash.hash128('Hello', '12343,30293'), 'Hash128 for "Hello" with seed 12343,30293');

assertEqual({
        "low": {
            "low": 3184066266,
            "high": 3674042232,
            "value": "15779891233746910938",
            "uint64": true
        },
        "high": {
            "low": 4196783977,
            "high": 3519958761,
            "value": "15118107765960464233",
            "uint64": true
        },
        "value": "15779891233746910938,15118107765960464233",
        "uint128": true
    }, cityhash.hash128('Hello', cityhash.objectify('12343,30293')), 'Hash128 for "Hello" with objectify seed 12343,30293');

if (cityhash.isCrcSupported()) {
  assertEqual({
          "low": {
              "low": 68277041,
              "high": 2127607426,
              "value": "9138004313465017137",
              "uint64": true
          },
          "high": {
              "low": 3936042248,
              "high": 2850538876,
              "value": "12242971252332641544",
              "uint64": true
          },
          "value": "9138004313465017137,12242971252332641544",
          "uint128": true
      }, cityhash.crc128('Hello'), 'Crc128 for "Hello"');

  assertEqual({
          "low": {
              "low": 3184066266,
              "high": 3674042232,
              "value": "15779891233746910938",
              "uint64": true
          },
          "high": {
              "low": 4196783977,
              "high": 3519958761,
              "value": "15118107765960464233",
              "uint64": true
          },
          "value": "15779891233746910938,15118107765960464233",
          "uint128": true
      }, cityhash.crc128('Hello', '12343,30293'), 'Crc128 for "Hello" with seed 12343,30293');

  assertEqual({
          "low": {
              "low": 3184066266,
              "high": 3674042232,
              "value": "15779891233746910938",
              "uint64": true
          },
          "high": {
              "low": 4196783977,
              "high": 3519958761,
              "value": "15118107765960464233",
              "uint64": true
          },
          "value": "15779891233746910938,15118107765960464233",
          "uint128": true
      }, cityhash.crc128('Hello', cityhash.objectify('12343,30293')), 'Crc128 for "Hello" with objectify seed 12343,30293');

// since my computer does not support SSE4.2, can not calculate the CRC by CityHashCrc256 algorithm.
// assertEqual({
//     }, cityhash.crc256('Hello'), 'Crc256 for "Hello"');
}
end();
