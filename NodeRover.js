const five = require('johnny-five');
const board = new five.Board({ port: "COM6" });


board.on('ready', function () {
    let lightSensorAry = [];

    let tempVal;
    const leftMotor = new five.Motor({
        pins: {
            pwm: 11,
            dir: 13
        }
    });
    const rightMotor = new five.Motor({
        pins: {
            pwm: 3,
            dir: 12
        }
    });

    const thermometer = new Thermometer({
        controller: "TMP36",
        pin: "10"
    });

    const photoRes1 = new board.Sensor({
        pin: "A2",
        freq: 250
    });
    const photoRes2 = new board.Sensor({
        pin: "A4",
        freq: 250
    });

    lightSensorAry = [photoResValue(photoRes1), photoResValue(photoRes2)];
    tempVal = getTemp(thermometer);
    moveRover([leftMotor, rightMotor], lightSensorAry, tempVal, five);
})

function photoResValue(photo) {
    await photo.on('data', function () {
        return this.value;
    })

}

function getTemp(therm) {
    therm.on('change', () => {
        return this.celsius;
    })
}

function moveRover(motors, lightSenAr, tempVal, board) {
    let frontVal = ((lightSenAr[0] + lightSenAr[1]) / 2) + 40;
    let tolerance = 100;
    let sittingValue = 500;
    let maxValue = Math.max(...lightSenAr);
    maxValue = Math.max(maxValue, frontVal);
    if (tempVal <= 28) {
        if (maxValue >= sittingValue) {
            motors[0].stop();
            motors[1].stop();
            board.wait(1000, () => { });
        }
        else if (frontVal > tolerance && frontVal == maxValue) {
            motors[0].forward(400);
            motors[1].forward(400);

            board.wait(1000, () => { });
            motors[0].stop();
            motors[1].stop();
            
        else if (lightSenAr[1] > tolerance && lightSenAr[1] == maxValue) {
                motors[0].forward(400);
                motors[1].reverse(400):
                board.wait(1000, () => { });
                motors[0].stop();
                motors[1].stop();
            }
            //right
            else if (lightSenAr[0] > tolerance && lightSenAr[0] == maxValue) {
                motors[1].forward(400);
                motors[0].reverse(400):
                board.wait(1000, () => { });
                motors[0].stop();
                motors[1].stop();
            } else {
                board.wait(1000, () => { });
                motors[0].stop();
                motors[1].stop();
            }
        }
        else {
            motors[0].reverse(400);
            motors[1].reverse(400):
            board.wait(1000, () => { });
            motors[0].stop();
            motors[1].stop();
        }


        board.wait(1000, () => { });
    }
}