console.log("Hello World!");

let controller_title = document.getElementById("controller-title");
let controller_display = document.getElementById("controller-display");
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
    gameLoop();
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

    // KI, KP, KD
    let ki = document.getElementById("ki-input").value;
    let kp = document.getElementById("kp-input").value;
    let kd = document.getElementById("kd-input").value;
    let ki_bytes = new Float64Array([ki]).buffer;
    let kp_bytes = new Float64Array([kp]).buffer;
    let kd_bytes = new Float64Array([kd]).buffer;
    let ki_ints = new Uint8Array(ki_bytes);
    let kp_ints = new Uint8Array(kp_bytes);
    let kd_ints = new Uint8Array(kd_bytes);

    fetch(`${addr}/settings`, {
        method: "POST",
        body: new Uint8Array([mode, ...ki_ints, ...kp_ints, ...kd_ints]),
    });
}

function buttonPressed(b) {
    if (typeof b === "object") {
        return b.pressed;
    }
    return b === 1.0;
}

function gameLoop() {
    const gamepads = navigator.getGamepads();
    if (!gamepads) {
        return;
    }

    const gp = gamepads[0];

    let gpx = gp.axes[0];
    let gpy = gp.axes[1];

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

    requestAnimationFrame(gameLoop);
}
