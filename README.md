# ESP8266 Automated 6-Port Injection Valve

A custom automation system developed to convert a **manual 6-port laboratory injection valve** into an electronically controlled device using an **ESP8266 ESP-01, servo motor, 3D-printed gears, and a Wi-Fi web interface**.

The system allows valve positioning through a smartphone or computer browser, as well as automatic switching between two calibrated positions using an external electrical trigger.

The complete **3D mechanical design, electronic circuit, assembly, and ESP8266 firmware** were developed and implemented by me.

**Development:** August 2024  
**Microcontroller:** ESP8266 ESP-01  
**Programming:** Arduino C/C++, HTML, CSS  
**Application:** Laboratory instrumentation and sample injection automation

**Complete automated injection valve:**

<img width="1848" height="634" alt="Automated 6-port injection valve" src="https://github.com/user-attachments/assets/a9490f3d-3184-49ca-82a4-dedbef9491e0" />

<br>
<br>

## 3D Mechanical Design and Printing

The mechanical system consists of a custom **3D-printed valve holder, servo support, transmission gears, and electronics enclosure**.

Since the manual valve rotates only approximately **60°**, exceeding its mechanical limits could damage the valve. To prevent this during calibration, I designed a **movable servo support that allows the gears to be engaged or disengaged**, enabling independent adjustment of the servo and valve positions before connecting them.

The servo support is secured to the base using an **M4 × 20 mm screw and nut**, as shown in the images below.

### Gear Assembly

The gears were designed for direct integration with the servo and injection valve:

- **Servo gear:** attached to the circular servo horn supplied with the servo motor. The horn is screwed directly to the 3D-printed gear, and the assembled gear/horn is secured to the servo shaft using the original servo screw.
- **Valve gear:** includes a hole for an **M3 screw**, used to secure the gear to the injection valve shaft.
- **Movable support:** allows the gears to be disengaged for calibration and re-engaged after confirming the correct positions.

This arrangement provides a simple, adjustable, and compact mechanical transmission.

**3D CAD assembly:**

<img width="855" height="763" alt="3D CAD assembly" src="https://github.com/user-attachments/assets/b63859d4-aec8-4122-aa2b-738bdb6233a9" />

<br>
<br>

**Adjustable gear engagement mechanism:**

<table>
  <tr>
    <td width="33.33%">
      <img width="100%" alt="Adjustable servo support" src="https://github.com/user-attachments/assets/1c768b50-75ff-43df-9af5-d9857c79bb18" />
    </td>
    <td width="33.33%">
      <img width="100%" alt="Gear engagement mechanism" src="https://github.com/user-attachments/assets/82ac437e-9e28-4f83-854c-1341e68f5968" />
    </td>
    <td width="33.33%">
      <img width="643" height="457" alt="3" src="https://github.com/user-attachments/assets/aebda799-3e77-4060-8503-4174763a3fe0" />
    </td>
  </tr>
</table>

<table>
  <tr>
    <td width="50%">
      <img width="100%" alt="Gear mounting and fixation" src="https://github.com/user-attachments/assets/4635f24a-be49-456c-aabf-0c74543f351c" />
    </td>
    <td width="50%">
      <img width="100%" alt="Servo and valve gear assembly" src="https://github.com/user-attachments/assets/2c684780-1f4b-4710-b389-f42c193406fb" />
    </td>
  </tr>
</table>

<br>
<br>

**3D-printed components:**

<img width="1236" height="786" alt="3D-printed mechanical components" src="https://github.com/user-attachments/assets/529e008f-6343-44ac-9b74-b86b6aa88b84" />

### Electronics Enclosure

A custom **3D-printed enclosure** was also designed to accommodate the electronic circuit. It includes a **sliding lid** for easy access to the components and an opening for a **5.5 × 2.1 mm DC barrel jack connector**, used to connect the external 12 V power supply.

### STL Files

The printable files for the valve holder, gears, movable servo support, and electronics enclosure are available here:

**[STL Files](STL-files/)**

<br>

## Electronic Circuit

The circuit was designed around the compact **ESP8266 ESP-01**, with a single **12 V / 2 A power supply** and dedicated voltage regulation for the servo and microcontroller.

### Power Supply

- **LM7805:** regulates 12 V to 5 V for the servo motor.
- **AMS1117-3.3 regulator module:** converts 5 V to 3.3 V for the ESP-01. The module includes the supporting capacitors required for voltage regulation.
- **3300 µF / 50 V capacitor:** connected across the 5 V supply to help reduce voltage fluctuations caused by servo operation. This value was used because the capacitor was available; a smaller capacitance may also be suitable.

All circuit stages share a common ground reference.

### External Trigger Input

The valve can be automatically controlled by an external laboratory instrument providing an approximately **24 V signal**.

To interface this signal with the ESP8266's 3.3 V GPIO, the input circuit includes:

- **1N4007 diode:** connected before the voltage divider to protect against accidental reverse-polarity connections.
- **10 kΩ trimmer potentiometer:** configured as a voltage divider to reduce the external signal to approximately 3 V.
- **3 V Zener diode:** connected at the GPIO input to provide additional voltage limiting.

The trimmer allows the input voltage to be adjusted for different external trigger levels.

**Important:** The GPIO input must not receive 24 V directly. The conditioned voltage must be verified before connecting the ESP-01. The trimmer, series diode, and Zener provide basic conditioning but do not guarantee protection against every overvoltage or transient condition.

### Circuit Schematic

<img width="982" height="677" alt="image" src="https://github.com/user-attachments/assets/c461ebcb-52a3-470c-b0d7-e7de8cf101ba" />

<br>
<br>

**Internal electronics and assembly:**

<table>
  <tr>
    <td width="50%">
      <img width="100%" alt="Electronic circuit assembly" src="https://github.com/user-attachments/assets/8405582c-62b8-40e3-a019-05fbc8dbbae2" />
    </td>
    <td width="50%">
      <img width="100%" alt="Internal electronic components" src="https://github.com/user-attachments/assets/a9a3c43c-f7fe-4966-8b97-21cf4c012f7a" />
    </td>
  </tr>
</table>

<br>
<br>

## ESP8266 Programming and Web Interface

The ESP-01 was programmed in **Arduino C/C++** to operate as a Wi-Fi access point with an integrated HTTP server and DNS-based captive portal.

The web interface allows the user to:

- Adjust the **OPEN** and **CLOSE** servo angles (0–180°).
- Save calibrated positions in EEPROM.
- Move the servo to either position for testing.
- Switch between **Test Mode** and **Automatic Mode**.
- Configure the valve using a smartphone or computer without an external Wi-Fi network.

### Wi-Fi Connection

| Parameter | Configuration |
|---|---|
| Wi-Fi SSID | `AUTO_VALVE` |
| IP address | `192.168.4.1` |
| Password | None (open network) |

Connect to `AUTO_VALVE` and open **http://192.168.4.1** in a browser.

**Web interface:**

<img width="350" alt="ESP8266 web interface" src="https://github.com/user-attachments/assets/dc8b249f-9b3e-4a48-af20-700abbc27a26" />

<br>
<br>

### Operating Modes

**Test Mode:** the operator manually selects the OPEN or CLOSE position through the web interface. The external trigger is temporarily ignored, allowing safe servo calibration and gear alignment.

**Automatic Mode:** the ESP-01 continuously monitors the external trigger input and moves the valve according to its logic level.

| Trigger input | Valve position |
|---|---|
| LOW | OPEN |
| HIGH | CLOSE |

### Firmware and Connections

| ESP-01 Pin | Function |
|---|---|
| GPIO2 | Servo control signal |
| GPIO3 (RX) | External trigger input |

The firmware uses the `ESP8266WiFi`, `ESP8266WebServer`, `DNSServer`, `EEPROM`, and `Servo` libraries.

**Source code:** [ESP8266 Valve Controller](code.ino)

<br>

## Calibration and Operation

To calibrate the valve:

1. Disengage the gears using the movable servo support.
2. Connect to the ESP-01 web interface.
3. Adjust and save the OPEN and CLOSE servo angles.
4. Test the servo positions and independently align the valve.
5. Engage the gears and secure the movable support using the M4 × 20 mm screw and nut.
6. Verify both positions without exceeding the valve's approximately 60° mechanical range.
7. Return to Automatic Mode for external trigger operation.

**Important:** The firmware does not detect mechanical end stops. Correct gear alignment and servo calibration are essential to prevent valve damage.

<br>

## Laboratory Application and Demonstration

The automated valve was developed for integration with laboratory analytical instrumentation, allowing the sample injection process to be controlled by an external signal rather than manual operation.

**Automated injection valve integrated with an atomic absorption spectrometer (AAS):**

<img width="1369" height="1004" alt="IMG_5196" src="https://github.com/user-attachments/assets/82f125ca-1a5e-4e22-b324-d2578c92cf8f" />

<br>
<br>

**Automated valve demonstration:**

https://github.com/user-attachments/assets/dcbb1c09-ef61-4bb4-bdd8-b5062a94fd51

<br>
<br>

## Skills Demonstrated

- **3D Design & Printing:** custom gears, mechanical supports, and electronics enclosure.
- **Electronics:** voltage regulation, signal conditioning, and circuit assembly.
- **Embedded Programming:** ESP8266 development using Arduino C/C++.
- **Web & Wireless Control:** HTML/CSS interface, HTTP server, and Wi-Fi communication.
- **Laboratory Automation:** servo control, EEPROM calibration, and external trigger integration.

<br>

## Author

**Gilberto Coelho**  
Electronics & Automation Developer

I personally developed the complete system, including **3D mechanical modeling, 3D printing, gear design, electronic circuit development, assembly, ESP8266 programming, and automation of the injection valve**.
