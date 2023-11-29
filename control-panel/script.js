console.log("Hello World!");

let controller_title = document.getElementById("controller-title");
let controller_display = document.getElementById("controller-display");
let logs = document.getElementById("logs");
let cd_inner = controller_display.firstElementChild;
let cd_bounding_rect = controller_display.getBoundingClientRect();
let rem = cd_bounding_rect.width / 7;

window.addEventListener("gamepadconnected", (e) => {
    console.log(
        "Gamepad connected at index %d: %s. %d buttons, %d axes.",
        e.gamepad.index,
        e.gamepad.id,
        e.gamepad.buttons.length,
        e.gamepad.axes.length,
    );
    controller_title.innerText = "Controller";
    loop();
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

function loop() {
    const gamepads = navigator.getGamepads();
    if (!gamepads) {
        return;
    }

    const gp = gamepads[0];

    let gpx = gp.axes[0];
    let gpy = gp.axes[1];
    postGamepadDirection(gpx, gpy);
    update_gamepad_display(gpx, gpy);
    requestAnimationFrame(loop);
}

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
