console.log("Hello World!");

let robot_addr_input = document.getElementById("robot-address");
let controller_title = document.getElementById("controller-title");
let controller_display = document.getElementById("controller-display");
let logs = document.getElementById("logs");
let cd_inner = controller_display.firstElementChild;
let cd_bounding_rect = controller_display.getBoundingClientRect();
let rem = cd_bounding_rect.width / 7;
var info_last_update = 0;
var camera_data = [];
var line_position = 0;
let mode_selector = document.getElementById("mode-selector");

let soundboard_rick = document.querySelector("#soundboard>div>div:nth-child(1)");
let soundboard_bagpipes = document.querySelector("#soundboard>div>div:nth-child(2)");
let soundboard_dwarf = document.querySelector("#soundboard>div>div:nth-child(3)");
let soundboard_fly = document.querySelector("#soundboard>div>div:nth-child(4)");
let soundboard_silence = document.querySelector("#soundboard>div>div:nth-child(5)");
let soundboard_pig1 = document.querySelector("#soundboard>div>div:nth-child(6)");
let soundboard_pig2 = document.querySelector("#soundboard>div>div:nth-child(7)");
let soundboard_pig3 = document.querySelector("#soundboard>div>div:nth-child(8)");
let soundboard_achievement = document.querySelector("#soundboard>div>div:nth-child(9)");
soundboard_rick.addEventListener("click", (e) => { play_sound("rick.mp3") });
soundboard_bagpipes.addEventListener("click", (e) => { play_sound("bagpipes.mp3") });
soundboard_dwarf.addEventListener("click", (e) => { play_sound("dwarf.mp3") });
soundboard_fly.addEventListener("click", (e) => { play_sound("fly.mp3") });
soundboard_silence.addEventListener("click", (e) => { play_sound("silence.mp3") });
soundboard_pig1.addEventListener("click", (e) => { play_sound("pig1.mp3") });
soundboard_pig2.addEventListener("click", (e) => { play_sound("pig2.mp3") });
soundboard_pig3.addEventListener("click", (e) => { play_sound("pig3.mp3") });
soundboard_achievement.addEventListener("click", (e) => { play_sound("achievement.mp3") });

robot_addr_input.value = localStorage.getItem("robot_addr") || "http://alimon.local:80";
robot_addr_input.addEventListener("change", (e) => {
    localStorage.setItem("robot_addr", robot_addr_input.value);
});

function play_sound(sound) {
    // API addr
    let addr = robot_addr_input.value;

    // Send request
    fetch(`${addr}/play-sound`, {
        method: "POST",
        body: sound,
    })
        .then((res) => {
            if (res.ok) {
                append_to_logs("Sound sent successfully");
            } else {
                append_to_logs("Error sending sound");
            }
        })
        .catch((err) => {
            append_to_logs(`Error sending sound: ${err}`);
        });
}

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
    let addr = robot_addr_input.value;

    // Get mode
    let mode = mode_selector.selectedIndex;

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
                append_to_logs("Settings sent successfully");
            } else {
                append_to_logs("Error sending settings");
            }
        })
        .catch((err) => {
            append_to_logs(`Error sending settings: ${err}`);
        });
}
document.getElementById("submit-settings").onclick = postSettings;

var sending_gamepad_direction = false;
var last_sent = [0, 0];
function postGamepadDirection(gpx, gpy) {
    // API addr
    let addr = document.getElementById("robot-address").value;

    let gpx_array = new Float32Array([gpx]).buffer;
    let gpy_array = new Float32Array([gpy]).buffer;
    let gpx_ints = new Uint8Array(gpx_array);
    let gpy_ints = new Uint8Array(gpy_array);

    // Send request
    if (sending_gamepad_direction) {
        return;
    }
    if (gpx == last_sent[0] && gpy == last_sent[1]) {
        return;
    }
    sending_gamepad_direction = true;
    fetch(`${addr}/gamepad-direction`, {
        method: "POST",
        body: new Uint8Array([...gpx_ints, ...gpy_ints]),
    })
        .then((res) => {
            last_sent = [gpx, gpy];
            sending_gamepad_direction = false;
            if (res.ok) {
                append_to_logs("Gamepad sent successfully");
            } else {
                append_to_logs("Error sending gamepad");
            }
        })
        .catch((err) => {
            append_to_logs(`Error sending gamepad: ${err}`);
        });
}

function buttonPressed(b) {
    if (typeof b === "object") {
        return b.pressed;
    }
    return b === 1.0;
}

function update_gamepad_display(x, y) {
    // calculate x and y so that sqrt(x^2 + y^2) = 1
    

    cd_inner.style.left = `${x * rem * 3.5 + 2.5 * rem}px`;
    cd_inner.style.top = `${y * rem * 3.5 + 2.5 * rem}px`;

}

function append_to_logs(text) {
    let lines = logs.innerText.split("\n");
    lines = lines.slice(0, 40);
    logs.innerText = lines.join("\n");
    let time = new Date().toLocaleTimeString();
    logs.innerText = `[${time}] ${text}\n${logs.innerText}`;
}

function update_info() {
    let now = new Date().getTime();
    if (now - info_last_update < 200) {
        return;
    }

    let addr = robot_addr_input.value;

    fetch(`${addr}/info`, {
        method: "GET",
    })
        .then((res) => {
            info_last_update = new Date().getTime();
            if (res.ok) {
                return res.arrayBuffer();
            } else {
                append_to_logs("Error getting info");
            }
        })
        .then((data) => {
            camera_data = new Uint8Array(data.slice(0, 640));
            line_position = new Float64Array(data.slice(640, 648))[0];
            update_info_display();
        })
        .catch((err) => {
            info_last_update = new Date().getTime();
            append_to_logs(`Error getting info: ${err}`);
        });
}

var chart;
async function update_info_display() {
    if (chart) {
        chart.destroy();
    }

    // Use line_position which is between -1 and 1 to calculate the position between 0 and 640
    let line_position_pixels = (line_position + 1) * 320;
    // Make all points blue except those near the line (20 pixels)
    let pointBackgroundColor = Array.from({ length: 640 }, (_, i) => {
        if (Math.abs(i - line_position_pixels) < 20) {
            return "#ff0000";
        }
        return "#0000ff";
    });

    chart = new Chart(
        document.getElementById('acquisitions'),
        {
            type: 'line',
            data: {
                labels: Array.from({ length: 640 }, (_, i) => i),
                datasets: [
                    {
                        data: camera_data,
                        label: 'Camera Data',
                        borderColor: '#3e95cd',
                        fill: false,
                        pointBackgroundColor: pointBackgroundColor,
                    }
                ]
            },
            options: {
                title: {
                    display: true,
                    text: 'Camera Acquisitions'
                },
                animation: false,
                scales: {
                    y: {
                        min: 0,
                        max: 255
                    }
                }
            }
        }
    );
}

function loop() {
    update_info();

    const gamepads = navigator.getGamepads();
    if (gamepads && gamepads[0]) {
        const gp = gamepads[0];
        let gpx = gp.axes[0];
        let gpy = gp.axes[1];

        let x = gpx;
        let y = gpy;
        let r = Math.sqrt(x * x + y * y);
        if (r > 1) {
            x /= r;
            y /= r;
        }

        postGamepadDirection(x, y);
        update_gamepad_display(x, y);
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
    } else if (e.key == "w") {
        postGamepadDirection(0, 0);
        update_gamepad_display(0, 0);
    }
});
