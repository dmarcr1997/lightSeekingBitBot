const five = require('johnny-five');
const board = new five.Board({port: "COM6"});

board.on('ready', function() {
    const leftMotor = new five.Motor({
        pins: {   
            pwm:11,
            dir:13
        }
    });
    const rightMotor = new five.Motor({
        pins: {   
                pwm:3,
                dir:12
        }
    });
    // Start the motor at maximum speed
    leftMotor.forward(255);
    rightMotor.forward(255);
})