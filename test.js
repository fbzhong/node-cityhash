var cityhash = require('./build/default/cityhash.node');

function assertEqual(expected, actual, message) {
    if(expected != actual) {
        if(message) {
            message += '. ';
        } else {
            message = '';
        }

        console.log('[Error]' + message + 'Expected ' + expected + ' , but actual is ' + actual);
    }
}

assertEqual('6596376470467341850', cityhash.hash64('Hello'), 'Hash64 for "Hello"');
assertEqual('2578220239953316063', cityhash.hash64('hello'), 'Hash64 for "hello"');
assertEqual('15738392108067291633', cityhash.hash64('Hello', 87392039), 'Hash64 for "hello" with seed 87392039');
assertEqual('16267654833214665223', cityhash.hash64('Hello', 87392039, 1230234), 'Hash64 for "hello" with seed 87392039 and 1230234');

assertEqual('9138004313465017137,12242971252332641544', cityhash.hash128('Hello'), 'Hash128 for "Hello"');
assertEqual('13523890104784088047,17404193039403234796', cityhash.hash128('hello'), 'Hash128 for "hello"');
assertEqual('15779891233746910938,15118107765960464233', cityhash.hash128('Hello', '12343,30293'), 'Hash128 for "Hello" with seed 12343,30293');
