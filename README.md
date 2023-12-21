# Robot_Mouche

## Hardware

### Components

- Raspberry Pi 4
- 1620A : 16x2 LCD Display
- PCF8574T : I2C to 8-bit GPIO expander (for 1620A).
- [Benewake SJ-GU-TF-Luna](https://www.gotronic.fr/pj2-sj-pm-tf-luna-a03-product-manual-2195.pdf) : Short-range LiDAR distance sensor.
- [ST Microelectronics L293D](https://www.st.com/resource/en/datasheet/l293d.pdf) : Push-pull four channel driver with diodes. Used here as a motor driver.
- [MAX 98357A](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-max98357-i2s-class-d-mono-amp.pdf) : I2S 3W Class D Amplifier. Used here as a speaker driver.

### Wiring

- L293D :
    - 1 (Enable 1) -> GPIO 19
    - 2 (Input 1) -> GPIO 17
    - 3 (Output 1) -> Left motor phase 1
    - 4, 5, 12 and 13 (Ground and heat sink) -> Ground
    - 6 (Output 2) -> Left motor phase 2
    - 7 (Input 2) -> GPIO 27
    - 8 (Logic supply) -> 5V
    - 9 (Enable 2) -> GPIO 13
    - 10 (Input 3) -> GPIO 23
    - 11 (Output 3) -> Left motor phase 1
    - 14 (Output 4) -> Left motor phase 2
    - 15 (Input 4) -> GPIO 24
    - 16 (Motor supply) -> External power supply (0-36V)

- PCF8574T : 
    - VCC -> 5V
    - Ground -> Ground
    - SDA -> GPIO 2
    - SCL -> GPIO 3

- 1602A : Connected directly to PCF8574T.

- SJ-GU-TF-Luna :
    - 1 (VCC) -> 5V
    - 2 (SDA) -> GPIO 2
    - 3 (SCL) -> GPIO 3 
    - 4 (GND) -> Ground
    - 5 (Configuration) -> Ground (for I2C)
    - 6 (Multiplexing) -> Not connected

- MAX 98357A :
    - Vin -> 5V
    - GND -> Ground
    - SD -> Not connected
    - GAIN -> Not connected
    - DIN -> GPIO 21 (I2S DOUT)
    - BCLK -> GPIO 18 (I2S CLK)
    - LRCLK -> GPIO 19 (I2S FS)
