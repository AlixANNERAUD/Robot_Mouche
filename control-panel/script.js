console.log("Hello World!");

let controller_title = document.getElementById("controller-title");
let controller_display = document.getElementById("controller-display");
let logs = document.getElementById("logs");
let cd_inner = controller_display.firstElementChild;
let cd_bounding_rect = controller_display.getBoundingClientRect();
let rem = cd_bounding_rect.width / 7;
var info = {
    t1: 0,
    t2: 0,
    t3: 0,
    t4: 0,
    t5: 0,
    t6: 0,
    last_update: 0,
};
let qtr_value1 = document.querySelector("#irs-sensors-display>div:nth-child(1)>div:nth-child(2)");
let qtr_value2 = document.querySelector("#irs-sensors-display>div:nth-child(2)>div:nth-child(2)");
let qtr_value3 = document.querySelector("#irs-sensors-display>div:nth-child(3)>div:nth-child(2)");
let qtr_value4 = document.querySelector("#irs-sensors-display>div:nth-child(4)>div:nth-child(2)");
let qtr_value5 = document.querySelector("#irs-sensors-display>div:nth-child(5)>div:nth-child(2)");
let qtr_value6 = document.querySelector("#irs-sensors-display>div:nth-child(6)>div:nth-child(2)");
let qtr_box1 = document.getElementById("irs-sensor-box-1");
let qtr_box2 = document.getElementById("irs-sensor-box-2");
let qtr_box3 = document.getElementById("irs-sensor-box-3");
let qtr_box4 = document.getElementById("irs-sensor-box-4");
let qtr_box5 = document.getElementById("irs-sensor-box-5");
let qtr_box6 = document.getElementById("irs-sensor-box-6");

window.addEventListener("gamepadconnected", (e) => {
    console.log(
        "Gamepad connected at index %d: %s. %d buttons, %d axes.",
        e.gamepad.index,
        e.gamepad.id,
        e.gamepad.buttons.length,
        e.gamepad.axes.length,
    );
    controller_title.innerText = "Controller";
});

window.addEventListener("gamepaddisconnected", (e) => {
    console.log(
        "Gamepad disconnected from index %d: %s",
        e.gamepad.index,
        e.gamepad.id,
    );
    controller_title.innerText = "Controller (disconnected)";
});

function postSettings() {
    // API addr
    let addr = document.getElementById("robot-address").value;

    // Get mode
    let mode = 0;

    // KP, KI, KD
    let kp = document.getElementById("kp-input").value;
    let ki = document.getElementById("ki-input").value;
    let kd = document.getElementById("kd-input").value;
    let kp_array = new Float64Array([kp]).buffer;
    let ki_array = new Float64Array([ki]).buffer;
    let kd_array = new Float64Array([kd]).buffer;
    let kp_ints = new Uint8Array(kp_array);
    let ki_ints = new Uint8Array(ki_array);
    let kd_ints = new Uint8Array(kd_array);

    // IRS threshold (integer)
    let irs_threshold = document.getElementById("irs-threshold-input").value;
    let irs_threshold_array = new Uint16Array([irs_threshold]).buffer;
    let irs_threshold_ints = new Uint8Array(irs_threshold_array);

    // Send request
    fetch(`${addr}/settings`, {
        method: "POST",
        body: new Uint8Array([mode, ...kp_ints, ...ki_ints, ...kd_ints, ...irs_threshold_ints]),
    })
        .then((res) => {
            if (res.ok) {
                logs.innerText += "Settings sent successfully\n";
            } else {
                logs.innerText += "Error sending settings\n";
            }
        })
        .catch((err) => {
            logs.innerText += `Error sending settings: ${err}\n`;
        });
}
document.getElementById("submit-settings").onclick = postSettings;

function postGamepadDirection(gpx, gpy) {
    // API addr
    let addr = document.getElementById("robot-address").value;

    let gpx_array = new Float32Array([gpx]).buffer;
    let gpy_array = new Float32Array([gpy]).buffer;
    let gpx_ints = new Uint8Array(gpx_array);
    let gpy_ints = new Uint8Array(gpy_array);

    // Send request
    fetch(`${addr}/gamepad-direction`, {
        method: "POST",
        body: new Uint8Array([...gpx_ints, ...gpy_ints]),
    })
        .then((res) => {
            if (res.ok) {
                logs.innerText += "Gamepad sent successfully\n";
            } else {
                logs.innerText += "Error sending gamepad\n";
            }
        })
        .catch((err) => {
            logs.innerText += `Error sending gamepad: ${err}\n`;
        });
}

function buttonPressed(b) {
    if (typeof b === "object") {
        return b.pressed;
    }
    return b === 1.0;
}

function update_gamepad_display(gpx, gpy) {
    // calculate x and y so that sqrt(x^2 + y^2) = 1
    let x = gpx;
    let y = gpy;
    let r = Math.sqrt(x * x + y * y);
    if (r > 1) {
        x /= r;
        y /= r;
    }

    cd_inner.style.left = `${x * rem * 3.5 + 2.5 * rem}px`;
    cd_inner.style.top = `${y * rem * 3.5 + 2.5 * rem}px`;

}

function update_info() {
    let now = new Date().getTime();
    if (now - info["last_update"] < 200) {
        return;
    }

    let addr = document.getElementById("robot-address").value;

    fetch(`${addr}/info`, {
        method: "GET",
    })
        .then((res) => {
            if (res.ok) {
                return res.arrayBuffer();
            } else {
                logs.innerText += "Error getting info\n";
            }
        })
        .then((data) => {
            let size_of_clock_t = data.byteLength / 6;
            info["t1"] = new Uint32Array(data.slice(0, size_of_clock_t))[0];
            info["t2"] = new Uint32Array(data.slice(size_of_clock_t, size_of_clock_t*2))[0];
            info["t3"] = new Uint32Array(data.slice(size_of_clock_t*2, size_of_clock_t*3))[0];
            info["t4"] = new Uint32Array(data.slice(size_of_clock_t*3, size_of_clock_t*4))[0];
            info["t5"] = new Uint32Array(data.slice(size_of_clock_t*4, size_of_clock_t*5))[0];
            info["t6"] = new Uint32Array(data.slice(size_of_clock_t*5, size_of_clock_t*6))[0];
            info["last_update"] = now;
            update_qtr_display();
        })
        .catch((err) => {
            logs.innerText += `Error getting info: ${err}\n`;
        });
}

function update_qtr_display() {
    qtr_value1.innerText = info["t1"];
    qtr_value2.innerText = info["t2"];
    qtr_value3.innerText = info["t3"];
    qtr_value4.innerText = info["t4"];
    qtr_value5.innerText = info["t5"];
    qtr_value6.innerText = info["t6"];
    qtr_box1.style.backgroundColor = "rgba(0, 0, 0, " + (info["t1"] / 100) + ")";
    qtr_box2.style.backgroundColor = "rgba(0, 0, 0, " + (info["t2"] / 100) + ")";
    qtr_box3.style.backgroundColor = "rgba(0, 0, 0, " + (info["t3"] / 100) + ")";
    qtr_box4.style.backgroundColor = "rgba(0, 0, 0, " + (info["t4"] / 100) + ")";
    qtr_box5.style.backgroundColor = "rgba(0, 0, 0, " + (info["t5"] / 100) + ")";
    qtr_box6.style.backgroundColor = "rgba(0, 0, 0, " + (info["t6"] / 100) + ")";
}

function loop() {
    update_info();

    const gamepads = navigator.getGamepads();
    if (gamepads && gamepads[0]) {
        const gp = gamepads[0];
        let gpx = gp.axes[0];
        let gpy = gp.axes[1];
        postGamepadDirection(gpx, gpy);
        update_gamepad_display(gpx, gpy);
    }

    requestAnimationFrame(loop);
}
loop();

document.addEventListener("keydown", (e) => {
    if (e.key == "ArrowLeft") {
        gpx = -1;
        gpy = 0;
        update_gamepad_display(gpx, gpy);
        postGamepadDirection(-1, 0);
    } else if (e.key == "ArrowRight") {
        gpx = 1;
        gpy = 0;
        update_gamepad_display(gpx, gpy);
        postGamepadDirection(1, 0);
    } else if (e.key == "ArrowUp") {
        gpx = 0;
        gpy = -1;
        update_gamepad_display(gpx, gpy);
        postGamepadDirection(0, -1);
    } else if (e.key == "ArrowDown") {
        gpx = 0;
        gpy = 1;
        update_gamepad_display(gpx, gpy);
        postGamepadDirection(0, 1);
    }
});
