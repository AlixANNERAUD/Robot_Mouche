console.log("Hello World!");

window.addEventListener("gamepadconnected", (e) => {
    console.log(
        "Gamepad connected at index %d: %s. %d buttons, %d axes.",
        e.gamepad.index,
        e.gamepad.id,
        e.gamepad.buttons.length,
        e.gamepad.axes.length,
    );
    gameLoop();    
});

window.addEventListener("gamepaddisconnected", (e) => {
    console.log(
        "Gamepad disconnected from index %d: %s",
        e.gamepad.index,
        e.gamepad.id,
    );
});

function buttonPressed(b) {
    if (typeof b === "object") {
        return b.pressed;
    }
    return b === 1.0;
}

let controller_display = document.querySelector("#controller-display");
let cd_inner = controller_display.firstElementChild;
let cd_bounding_rect = controller_display.getBoundingClientRect();
let rem = cd_bounding_rect.width / 7;

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
