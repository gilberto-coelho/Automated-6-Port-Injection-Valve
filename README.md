# ESP8266 Automated 6-Port Injection Valve

A custom automation system developed to convert a **manual 6-port laboratory injection valve** into an electronically controlled device using an **ESP8266 ESP-01, servo motor, 3D-printed gears, and a Wi-Fi web interface**.

Manual injection valves are widely used in analytical laboratories to switch between sample loading and injection positions. Although reliable, manual operation can limit automation and integration with other laboratory instruments.

To overcome this limitation, I developed a compact system combining **3D CAD modeling, 3D printing, mechanical transmission, electronic circuit design, voltage regulation, and embedded programming**.

The valve can be operated through a web browser or automatically switched between two calibrated positions using an external electrical trigger.

The entire mechanical design, electronic integration, assembly, and ESP8266 firmware were developed and implemented by me.

**Development:** 2024
**Microcontroller:** ESP8266 ESP-01  
**Programming:** Arduino C/C++, HTML, CSS  
**Application:** Laboratory instrumentation and sample injection automation

**Complete automated injection valve:**

[photo of the assembled system here]

<br>
<br>

## Overview

The project was designed to automate a conventional manual 6-port injection valve while preserving its original operating mechanism.

The main features include:

- Servo-driven valve rotation through custom 3D-printed gears
- Adjustable mechanical coupling for safe gear alignment
- Compact ESP8266 ESP-01 control system
- Wi-Fi access point with browser-based configuration
- Adjustable OPEN and CLOSE servo positions
- Non-volatile storage of calibrated positions using EEPROM
- Manual testing and positioning through the web interface
- Automatic operation using an external ~24 V trigger signal
- Custom 12 V, 5 V, and 3.3 V power distribution circuit
- 3D-printed valve support, gears, and electronics enclosure

The system was designed to operate without requiring a computer or external Wi-Fi network during normal use.

<br>

## 3D Mechanical Design and Printing

A major part of the project involved designing a **custom mechanical assembly** to connect a servo motor to the original manual injection valve.

The assembly consists of:

- **Valve holder:** supports and positions the manual injection valve.
- **Servo mounting structure:** maintains the motor in alignment with the valve.
- **Custom gears:** transmit the servo rotation to the valve mechanism.
- **Adjustable gear engagement mechanism:** allows the gears to be connected or disconnected during calibration.
- **Electronics enclosure:** houses and protects the control circuit.

All components were designed using 3D CAD software and fabricated through 3D printing.

### Adjustable Gear Engagement

One of the most important mechanical design requirements was protecting the injection valve against excessive rotation.

The manual valve operates over a limited angular range of approximately **90°**. Rotating beyond its mechanical limits could damage the internal mechanism.

To address this, I developed a **movable support that allows the gears to be temporarily disengaged**.

This feature enables the servo and valve positions to be adjusted independently before engaging the gears, ensuring proper alignment without forcing the valve beyond its mechanical limits.

Once the correct positions are established, the gears can be engaged for normal operation.

This mechanism was essential for safe installation and calibration.

**3D CAD assembly:**

[CAD rendering of the complete valve automation system]

<br>
<br>

**Adjustable gear engagement mechanism:**

[Images showing engaged and disengaged gear positions]

<br>
<br>

**3D-printed components:**

[Photos of the printed holder, gears, and enclosure]

<br>
<br>

### STL Files

The 3D-printable components are available in the repository:

**[STL Files](STL-files/)**

<br>

## Electronic Circuit and Power Management

The electronic system was designed around the compact **ESP8266 ESP-01**, which controls the servo motor and receives the external trigger signal.

A single **12 V / 2 A power supply** powers the system through two voltage regulation stages.

### Voltage Regulation

The circuit uses:

- **12 V / 2 A external power supply:** main system input.
- **LM7805:** reduces 12 V to 5 V for the servo motor.
- **AMS1117-3.3 module:** reduces the regulated 5 V to 3.3 V for the ESP-01.
- **3300 µF / 50 V capacitor:** connected across the 5 V output to provide additional energy storage and help reduce voltage fluctuations during servo operation.

The 3300 µF capacitor was selected because it was available during development; a smaller capacitor may also be suitable depending on the servo current requirements and circuit configuration.

The servo is powered from the dedicated 5 V regulator rather than from the ESP-01, which cannot supply the motor's required current.

All control and power stages share a common ground reference.

### External Trigger Signal Conditioning

The automated valve can be controlled by an external laboratory instrument providing an approximately **24 V electrical signal**.

Because the ESP8266 GPIO pins operate at **3.3 V logic levels**, the external voltage cannot be connected directly to the microcontroller.

The input circuit uses:

- **10 kΩ trimmer potentiometer:** adjusted to reduce the external signal to approximately 3 V at the ESP-01 input. The potentiometer can be adjusted if higher or lower voltage is used for triggering.
- **3 V Zener diode:** connected to the signal input as an additional voltage-limiting.

This arrangement was implemented to interface the external trigger with the ESP-01 while reducing the risk of applying excessive voltage to its GPIO input.

The input circuit must be properly adjusted and verified before connection to the microcontroller. A trimmer and Zener diode alone do not guarantee protection against all input conditions or electrical transients.

### Circuit Schematic

[complete electronic circuit schematic]

The schematic include the power supply, LM7805, AMS1117-3.3, filtering capacitor, ESP-01, servo connections, and external trigger conditioning circuit.

**Internal electronics:**

[photo of the assembled circuit and electronics enclosure]

<br>

## ESP8266 Firmware and Web Interface

The system uses an **ESP8266 ESP-01 programmed in Arduino C/C++**, with an integrated HTTP server for configuration and control.

The ESP-01 creates its own Wi-Fi access point, allowing the valve to be configured using a smartphone, tablet, or computer without requiring an existing wireless network.

### Wi-Fi Configuration

| Parameter | Value |
|---|---|
| Wi-Fi network (SSID) | `AUTO_VALVE` |
| Access point IP | `192.168.4.1` |
| Wi-Fi password | None (open network) |

To access the interface:

1. Power on the device.
2. Connect to the `AUTO_VALVE` Wi-Fi network.
3. Open a browser and navigate to **http://192.168.4.1**.
4. Configure the servo positions or test the valve movement.

The firmware also implements a DNS-based captive portal mechanism to help direct connected devices toward the control interface.

### Web Interface Features

The interface was developed using HTML and CSS embedded directly in the ESP8266 firmware.

It allows the operator to:

- Set the servo angle corresponding to the **OPEN** position.
- Set the servo angle corresponding to the **CLOSE** position.
- Save both positions in EEPROM.
- Move the servo to either position for testing.
- Return the system to automatic operation.
- View the current operating mode.

The configured positions can be adjusted between **0° and 180°**, although the safe mechanical range must be established according to the valve and gear configuration.

**Web control interface:**

[screenshot of the ESP8266 web interface]

<br>
<br>

## Operating Modes

The firmware supports two operating modes.

### Test Mode — Manual Positioning

Test mode allows the operator to move the valve through the browser to verify its mechanical alignment.

The available commands are:

- **Move OPEN position:** moves the servo to the stored OPEN angle.
- **Move CLOSE position:** moves the servo to the stored CLOSE angle.

Selecting either command switches the firmware into test mode, preventing the external trigger from overriding the manually selected position.

This is particularly useful when calibrating the servo and aligning the 3D-printed gears with the valve.

### Automatic Mode — External Trigger

In automatic mode, the ESP-01 continuously monitors the external trigger input.

The valve position depends on the input logic level:

| External input | Servo position |
|---|---|
| LOW | OPEN |
| HIGH | CLOSE |

An external instrument can therefore control the valve by changing the state of its electrical output.

This allows the injection valve to be integrated into automated analytical procedures without requiring manual operation.

The external trigger voltage is conditioned by the input circuit before reaching the ESP-01.

### Operating Sequence

```text
             Power ON
                │
                ▼
         ESP-01 initializes
                │
                ▼
     Read saved servo positions
            from EEPROM
                │
                ▼
      Create Wi-Fi Access Point
          AUTO_VALVE
                │
                ▼
       Start Web Server
                │
                ▼
        Automatic Mode
                │
                ▼
      Read External Trigger
                │
         ┌──────┴──────┐
         │             │
        LOW           HIGH
         │             │
         ▼             ▼
     OPEN Position  CLOSE Position
         │             │
         └──────┬──────┘
                │
                ▼
       Continue Monitoring
```

<br>

## Servo Calibration and EEPROM

The valve requires two calibrated servo positions corresponding to its mechanical OPEN and CLOSE states.

The firmware stores these values in the ESP8266's emulated EEPROM:

| EEPROM Address | Stored Parameter |
|---|---|
| `0` | OPEN servo angle |
| `2` | CLOSE servo angle |

The settings are written using `EEPROM.write()` and saved with `EEPROM.commit()`.

This allows the calibrated positions to be restored automatically after a power cycle.

### Calibration Procedure

1. Disconnect the gears using the movable mechanical support.
2. Connect to the ESP-01 web interface.
3. Set the desired OPEN and CLOSE servo angles.
4. Test the servo movement using the browser.
5. Align the valve and servo positions independently.
6. Engage the gears after verifying the mechanical alignment.
7. Test both positions and confirm that the valve does not exceed its approximately 90° operating range.
8. Return to automatic mode.

**Important:** The firmware controls servo angles but does not independently detect the valve's mechanical end stops. Correct calibration and gear alignment are essential to prevent mechanical damage.

<br>

## Firmware Details

The firmware uses the following Arduino libraries:

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <Servo.h>
```

### GPIO Configuration

| ESP-01 GPIO | Function |
|---|---|
| GPIO2 | Servo control signal |
| GPIO3 (RX) | External trigger input |

The ESP8266 operates at **3.3 V logic levels**, and the servo is powered separately at 5 V.

The firmware continuously processes DNS requests, handles HTTP connections, and monitors the external trigger when automatic mode is enabled.

### HTTP Routes

| Route | Function |
|---|---|
| `/` | Main web interface |
| `/setdegrees` | Save OPEN and CLOSE angles |
| `/moveopen` | Move servo to OPEN position |
| `/moveclose` | Move servo to CLOSE position |
| `/automatic` | Return to automatic mode |

Unrecognized routes are redirected to the main page.

**Source code:** [ESP8266 Valve Controller](firmware/)

<br>

## Operation Demonstration

**Complete automated injection valve:**

[video of the assembled valve operating]

<br>
<br>

**Browser-based servo positioning:**

[video of the web interface controlling the valve]

<br>
<br>

## Skills Demonstrated

- **3D CAD Modeling:** mechanical design of the valve holder, gears, adjustable coupling, and electronics enclosure.
- **3D Printing:** fabrication, assembly, and mechanical fitting of custom components.
- **Mechanical Automation:** servo-driven gear transmission and valve positioning.
- **Electronic Circuit Design:** voltage regulation, filtering, signal conditioning, and component integration.
- **Embedded Programming:** ESP8266 firmware development in Arduino C/C++.
- **Web Development:** HTML/CSS interface and HTTP server implementation.
- **Wireless Communication:** ESP8266 Wi-Fi access point and browser-based control.
- **Non-Volatile Memory:** EEPROM-based storage of calibration parameters.
- **Laboratory Automation:** external trigger integration and automated sample injection control.

<br>

## Author

**Gilberto Coelho**  
Electronics & Automation Developer

The complete development of this project was carried out by me, including **3D mechanical modeling, gear and support design, 3D printing, electronic circuit development, assembly, ESP8266 programming, and implementation of the automated control system**.
