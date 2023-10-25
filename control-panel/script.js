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

function gameLoop() {
    const gamepads = navigator.getGamepads();
    if (!gamepads) {
        return;
    }

    const gp = gamepads[0];
    console.log(gp.axes);
    if (buttonPressed(gp.buttons[0])) {
        console.log(b--);
    } else if (buttonPressed(gp.buttons[2])) {
        console.log(b++);
    }
    if (buttonPressed(gp.buttons[1])) {
        console.log(a++);
    } else if (buttonPressed(gp.buttons[3])) {
        console.log(a--);
    }

    //ball.style.left = `${a * 2}px`;
    //ball.style.top = `${b * 2}px`;

    requestAnimationFrame(gameLoop);
}
