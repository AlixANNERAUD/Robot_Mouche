# 🪰 Robot mouche 🤖

## Introduction

For the "electronic for embedded systems" course at the [INSA Rouen](https://www.insa-rouen.fr/), we had to make a robot that can either move freely thanks to a remote control over WiFi or follow a line and avoid obstacles using a LiDAR sensor and a camera.

## 🚀 Getting started

### 🔩 Bill of materials

- Raspberry Pi 4.
- 1620A : 16x2 LCD Display.
- PCF8574T : I2C to 8-bit GPIO expander (for 1620A).
- [Benewake SJ-GU-TF-Luna](https://www.gotronic.fr/pj2-sj-pm-tf-luna-a03-product-manual-2195.pdf) : Short-range LiDAR distance sensor.
- [ST Microelectronics L293D](https://www.st.com/resource/en/datasheet/l293d.pdf) : Push-pull four channel driver with diodes. Used here as a motor driver.
- [MAX 98357A](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-max98357-i2s-class-d-mono-amp.pdf) : I2S 3W Class D Amplifier. Used here as a speaker driver.
- [Joy-it RB Camera](https://joy-it.net/en/products/rb-camera_JT) : Raspberry Pi camera module based on the OV5647 sensor.

### 📦 Installation

Clone the repository on your Raspberry Pi, open the `code` folder in Platform IO and compile the project. Then launch the executable in sudo mode:
```bash
sudo code/.pio/build/Raspberry_Pi_4_Release/robot_mouche
# or
sudo code/.pio/build/Raspberry_Pi_4_Debug/robot_mouche
```

# ℹ️ About

## 📃 Credits

This project uses the following resources :
- [cpp-httplib](https://github.com/yhirose/cpp-httplib) : C++ HTTP client and server library.
- [WiringPi](http://wiringpi.com/) : GPIO access library for the Raspberry Pi.

## 👤 Authors

- Alix ANNERAUD
- Dimitri TIMOZ
- Simon GIRARD
- Mathis SAUNIER

## ⚖️ Licence

This project is under the [MIT License](License).