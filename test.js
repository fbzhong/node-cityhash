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
        console.log('[Error]' + message +
                    'Expected ' + JSON.stringify(expected, null, 4) +
                    ' , but actual is ' + JSON.stringify(actual, null, 4));
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
      "low": 2999840452,
      "high": 0,
      "value": "2999840452",
      "uint64": false
    }, cityhash.hash32('Hello'), "Hash32 for 'Hello'");

assertEqual({
      "low": 845881479,
      "high": 3586685982,
      "value": "15404698994557526151",
      "uint64": true
    }, cityhash.hash64('Hello'), 'Hash64 for "Hello"');

assertEqual({
      "low": 825756904,
      "high": 3029067177,
      "value": "13009744463427800296",
      "uint64": true
    }, cityhash.hash64('hello'), 'Hash64 for "hello"');

assertEqual({
      "low": 3137522328,
      "high": 3490861664,
      "value": "14993136684877662872",
      "uint64": true
    }, cityhash.hash64('Hello', 87392039), 'Hash64 for "hello" with seed 87392039');

var b = new Buffer(4)
b.writeUInt32LE(7771789, 0)
assertEqual({
      "low": 1072348245,
      "high": 3201827915,
      "value": "13751746183417216085",
      "uint64": true
    }, cityhash.hash64(b, "2310915232984335893"), 'Hash64 for Buffer');

  assertEqual({
      "low": 3137522328,
      "high": 3490861664,
      "value": "14993136684877662872",
      "uint64": true
    }, cityhash.hash64('Hello', cityhash.objectify(87392039)), 'Hash64 for "hello" with objectify seed 87392039');

assertEqual({
      "low": 4137286146,
      "high": 1829140166,
      "value": "7856097196907297282",
      "uint64": true
    }, cityhash.hash64('Hello', 87392039, 1230234), 'Hash64 for "hello" with seed 87392039 and 1230234');

assertEqual({
        "low": {
            "low": 1746744350,
            "high": 270899971,
            "value": "1163506517679092766",
            "uint64": true
        },
        "high": {
            "low": 1430058029,
            "high": 2521510841,
            "value": "10829806600034513965",
            "uint64": true
        },
        "value": "1163506517679092766,10829806600034513965",
        "uint128": true
    }, cityhash.hash128('Hello'), 'Hash128 for "Hello"');

assertEqual({
        "low": {
            "low": 3029444426,
            "high": 1869800619,
            "value": "8030732511675000650",
            "uint64": true
        },
        "high": {
            "low": 189133639,
            "high": 1695846232,
            "value": "7283604105673962311",
            "uint64": true
        },
        "value": "8030732511675000650,7283604105673962311",
        "uint128": true
    }, cityhash.hash128('hello'), 'Hash128 for "hello"');

assertEqual({
        "low": {
            "low": 2988849008,
            "high": 3870103733,
            "value": "16621968968351364976",
            "uint64": true
        },
        "high": {
            "low": 71986147,
            "high": 2418007629,
            "value": "10385263688105487331",
            "uint64": true
        },
        "value": "16621968968351364976,10385263688105487331",
        "uint128": true
    }, cityhash.hash128('Hello', '12343,30293'), 'Hash128 for "Hello" with seed 12343,30293');

assertEqual({
        "low": {
            "low": 2988849008,
            "high": 3870103733,
            "value": "16621968968351364976",
            "uint64": true
        },
        "high": {
            "low": 71986147,
            "high": 2418007629,
            "value": "10385263688105487331",
            "uint64": true
        },
        "value": "16621968968351364976,10385263688105487331",
        "uint128": true
    }, cityhash.hash128('Hello', cityhash.objectify('12343,30293')), 'Hash128 for "Hello" with objectify seed 12343,30293');

if (cityhash.isCrcSupported()) {
  assertEqual({
          "low": {
              "low": 1746744350,
              "high": 270899971,
              "value": "1163506517679092766",
              "uint64": true
          },
          "high": {
              "low": 1430058029,
              "high": 2521510841,
              "value": "10829806600034513965",
              "uint64": true
          },
          "value": "1163506517679092766,10829806600034513965",
          "uint128": true
      }, cityhash.crc128('Hello'), 'Crc128 for "Hello"');

  assertEqual({
          "low": {
              "low": 2988849008,
              "high": 3870103733,
              "value": "16621968968351364976",
              "uint64": true
          },
          "high": {
              "low": 71986147,
              "high": 2418007629,
              "value": "10385263688105487331",
              "uint64": true
          },
          "value": "16621968968351364976,10385263688105487331",
          "uint128": true
      }, cityhash.crc128('Hello', '12343,30293'), 'Crc128 for "Hello" with seed 12343,30293');

  assertEqual({
          "low": {
              "low": 2988849008,
              "high": 3870103733,
              "value": "16621968968351364976",
              "uint64": true
          },
          "high": {
              "low": 71986147,
              "high": 2418007629,
              "value": "10385263688105487331",
              "uint64": true
          },
          "value": "16621968968351364976,10385263688105487331",
          "uint128": true
      }, cityhash.crc128('Hello', cityhash.objectify('12343,30293')), 'Crc128 for "Hello" with objectify seed 12343,30293');

  assertEqual([
      {
          "low":1520785073,
          "high":3649709413,
          "value":"15675382570259142321",
          "uint64":true
      },
      {
          "low":3695378336,
          "high":2185935084,
          "value":"9388519700654391200",
          "uint64":true
      },
      {
          "low":2432607983,
          "high":589289348,
          "value":"2530978479973770991",
          "uint64":true
      },
      {
          "low":1486001901,
          "high":2811866387,
          "value":"12076874174372681453",
          "uint64":true
      }], cityhash.crc256('Hello'), 'Crc256 for "Hello"');
}
end();
