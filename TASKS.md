# AlphaBot V2-Ar - Complete Tasks Roadmap

## Project Overview

This document outlines all the learning activities and challenges for the **AlphaBot V2-Ar Arduino Robot Kit**. The tasks are structured progressively from basic sensor learning to advanced multi-sensor integration challenges.

---

## Phase 1: Sensor Learning & Individual Component Testing

### 1. Motor Wheels & Motion Control

#### Learn Section
- [ ] Study motor wheel documentation
- [ ] Understand PWM signal control
- [ ] Learn H-bridge motor driver (TB6612FNG) operation
- [ ] Study gear motor specifications (N20, 1:30 reduction rate, 6V/600RPM)

#### Activity 1.1: Motion Control (Rectangular Patterns)
- [ ] Implement basic forward motion
- [ ] Implement backward motion
- [ ] Implement left turn (differential motor control)
- [ ] Implement right turn (differential motor control)
- [ ] Create motion functions for reusability
- [ ] Code file: `01_motor_control.ino`

#### Activity 1.2: Square Motion
- [ ] Program robot to move in a perfect square pattern
- [ ] Implement timing-based movement
- [ ] Add distance calculation (optional: use encoder feedback)
- [ ] Code file: `02_square_motion.ino`

#### Activity 1.3: Circular Motion
- [ ] Program robot to move in circular patterns
- [ ] Implement arc control using motor speed differential
- [ ] Create function for variable radius circles
- [ ] Code file: `03_circular_motion.ino`

---

### 2. RGB LED Control (WS2812B)

#### Learn Section
- [ ] Study WS2812B addressable LED specifications
- [ ] Understand NeoPixel library communication protocol
- [ ] Learn RGB color mixing basics
- [ ] Study onboard LED count and pin connections

#### Activity 2.1: Sequential LED Activation
- [ ] Turn on LEDs one by one in sequence
- [ ] Control brightness levels
- [ ] Create rainbow color sequences
- [ ] Code file: `04_rgb_sequential.ino`

#### Activity 2.2: Paired LED Activation
- [ ] Control LED pairs simultaneously
- [ ] Implement complementary color schemes
- [ ] Create mirror/symmetrical patterns
- [ ] Code file: `05_rgb_paired.ino`

#### Activity 2.3: Full Forward & Reverse Sequence
- [ ] Create animation sequences for forward motion
- [ ] Create animation sequences for reverse motion
- [ ] Sync LED patterns with motor movement
- [ ] Code file: `06_rgb_motion_sync.ino`

---

### 3. Buzzer Control

#### Learn Section
- [ ] Study buzzer specifications and frequency ranges
- [ ] Understand PWM-based tone generation
- [ ] Learn tone() function in Arduino IDE
- [ ] Study frequency ranges for musical notes

#### Activity 3.1: Beep Sound
- [ ] Generate single beep sound
- [ ] Create pattern-based beeps (SOS, etc.)
- [ ] Implement variable beep duration
- [ ] Code file: `07_buzzer_beep.ino`

#### Activity 3.2: Musical Tone
- [ ] Play musical notes
- [ ] Create simple melodies
- [ ] Implement tempo control
- [ ] Code file: `08_buzzer_melody.ino`

---

### 4. Joystick Input

#### Learn Section
- [ ] Study joystick analog input specifications
- [ ] Understand analog-to-digital conversion (ADC)
- [ ] Learn dead zone implementation
- [ ] Study button press detection

#### Activity 4.1: Joystick Direction Detection
- [ ] Read X-axis analog value
- [ ] Read Y-axis analog value
- [ ] Detect 8-direction input (up, down, left, right, diagonals)
- [ ] Detect button press
- [ ] Print detected direction via Serial
- [ ] Code file: `09_joystick_control.ino`

---

### 5. OLED Display (SSD1306)

#### Learn Section
- [ ] Study OLED specifications (128x64 resolution, 0.96")
- [ ] Understand I2C communication protocol
- [ ] Learn Adafruit display library
- [ ] Study coordinate system for graphics

#### Activity 5.1: Printing Name
- [ ] Display text on OLED
- [ ] Implement different font sizes
- [ ] Test scrolling text
- [ ] Code file: `10_oled_text.ino`

#### Activity 5.2: Printing Circle
- [ ] Draw circles of different sizes
- [ ] Implement filled vs. outlined circles
- [ ] Position circles at various coordinates
- [ ] Code file: `11_oled_shapes.ino`

#### Activity 5.3: Printing Rectangle
- [ ] Draw rectangles of different dimensions
- [ ] Create filled and outlined rectangles
- [ ] Combine multiple rectangles
- [ ] Code file: `11_oled_shapes.ino` (extended)

#### Activity 5.4: Printing Triangle
- [ ] Draw triangles using line commands
- [ ] Create filled triangles
- [ ] Implement rotatable triangle patterns
- [ ] Code file: `11_oled_shapes.ino` (extended)

#### Activity 5.5: Printing Stickman
- [ ] Create stickman figure using lines and circles
- [ ] Implement animation (walking, jumping)
- [ ] Create multiple stickman poses
- [ ] Code file: `12_oled_stickman.ino`

---

### 6. IR Receiver (Infrared Remote Control)

#### Learn Section
- [ ] Study IR receiver specifications
- [ ] Understand IR protocol (NEC, etc.)
- [ ] Learn IR library usage
- [ ] Study remote control button mapping

#### Activity 6.1: Decode IR Signal
- [ ] Detect IR signals from remote
- [ ] Decode button presses
- [ ] Map buttons to their names/codes
- [ ] Display detected codes via Serial
- [ ] Code file: `13_ir_receiver.ino`

---

### 7. Ultrasonic Distance Sensor (HC-SR04)

#### Learn Section
- [ ] Study HC-SR04 sensor specifications
- [ ] Understand ultrasonic pulse measurement
- [ ] Learn timing and distance calculation formula
- [ ] Study detection range (2cm - 4m)

#### Activity 7.1: Print Distance
- [ ] Trigger ultrasonic sensor
- [ ] Measure echo pulse duration
- [ ] Calculate distance in cm
- [ ] Display distance via Serial
- [ ] Add error handling for out-of-range
- [ ] Code file: `14_ultrasonic_distance.ino`

---

### 8. White Line Sensor (Reflective Infrared)

#### Learn Section
- [ ] Study line sensor specifications (TCRT5000 / ITR20001)
- [ ] Understand reflectance detection
- [ ] Learn analog threshold calibration
- [ ] Study sensor array configuration (3-5 sensors)

#### Activity 8.1: Print Sensor Value
- [ ] Read individual sensor values
- [ ] Display analog readings via Serial
- [ ] Implement sensor calibration routine
- [ ] Test on white and black surfaces
- [ ] Code file: `15_line_sensor.ino`

---

## Phase 2: Multi-Sensor Integration Tasks

### Task 1: Joystick Motor Control
**Difficulty: Basic**
- **Requirements**: Joystick (Activity 4.1) + Motor Control (Activity 1.1)
- [ ] Read joystick input
- [ ] Map joystick directions to motor commands
- [ ] Implement smooth speed control
- [ ] Add dead zone filtering
- **Code file**: `T01_joystick_motor.ino`

---

### Task 2: Joystick OLED Control
**Difficulty: Intermediate**
- **Requirements**: Joystick (Activity 4.1) + OLED (Activity 5.1)
- [ ] Display joystick position on OLED
- [ ] Show real-time X, Y coordinates
- [ ] Display direction arrow based on input
- [ ] Add digital button state display
- **Code file**: `T02_joystick_oled.ino`

---

### Task 3: IR Control RGB LED
**Difficulty: Basic**
- **Requirements**: IR Receiver (Activity 6.1) + RGB LED (Activity 2.1)
- [ ] Map remote buttons to LED colors
- [ ] Implement color selection via remote
- [ ] Add brightness control
- [ ] Create custom color sequences
- **Code file**: `T03_ir_rgb_control.ino`

---

### Task 4: IR & RGB & Motor Control
**Difficulty: Intermediate**
- **Requirements**: IR (6.1) + RGB (2.1) + Motors (1.1)
- [ ] Control motor movement via IR remote
- [ ] Change LED color based on motor direction
- [ ] Implement speed control buttons
- [ ] Add status indication on RGB LEDs
- **Code file**: `T04_ir_rgb_motor.ino`

---

### Task 5: Ultrasonic, RGB & Motor Control
**Difficulty: Intermediate**
- **Requirements**: Ultrasonic (7.1) + RGB (2.1) + Motors (1.1)
- [ ] Detect obstacles using ultrasonic sensor
- [ ] Display distance on RGB LED (green=far, yellow=medium, red=close)
- [ ] Stop motor when obstacle detected
- [ ] Add buzzer alert at critical distance
- **Code file**: `T05_ultrasonic_rgb_motor.ino`

---

### Task 6: Edge Detection & Line Following
**Difficulty: Basic**
- **Requirements**: Line Sensor (8.1) + Motors (1.1)
- [ ] Implement binary edge detection (on line vs. off line)
- [ ] Create simple turn algorithm
- [ ] Test on straight line track
- [ ] Add LED feedback for line status
- **Code file**: `T06_edge_detection.ino`

---

### Task 7: White Line Tracking
**Difficulty: Intermediate**
- **Requirements**: Line Sensor (8.1) + Motors (1.1) + OLED (5.1)
- [ ] Read all line sensors simultaneously
- [ ] Implement PID control algorithm
- [ ] Calculate steering correction
- [ ] Adjust motor speeds for smooth curve following
- [ ] Display sensor values and steering correction on OLED
- **Code file**: `T07_line_tracking.ino`

---

### Task 8: Line Sensor & RGB Brightness
**Difficulty: Basic**
- **Requirements**: Line Sensor (8.1) + RGB (2.1)
- [ ] Map sensor reflectance to RGB brightness
- [ ] Brighter on white, dimmer on black
- [ ] Create gradient effect across line sensors
- **Code file**: `T08_line_rgb_brightness.ino`

---

### Task 9: Right-Side Line Following
**Difficulty: Intermediate**
- **Requirements**: Line Sensor (8.1) + Motors (1.1)
- [ ] Implement right-wall following algorithm
- [ ] Detect line on right side only
- [ ] Turn right when line exits
- [ ] Handle intersections
- **Code file**: `T09_right_line_following.ino`

---

### Task 10: Shortest Path/Maze Solver
**Difficulty: Advanced**
- **Requirements**: All sensors (comprehensive)
- [ ] Detect maze walls using IR sensors
- [ ] Implement maze solving algorithm (wall follower or A*)
- [ ] Store optimal path in memory
- [ ] Navigate maze on second attempt with optimal route
- [ ] Display path on OLED (optional)
- [ ] Log distance traveled and time taken
- **Code file**: `T10_maze_solver.ino`

---

## Phase 3: Advanced Projects (Optional)

### Project A: Obstacle Avoidance Robot
- [ ] Continuous obstacle scanning with ultrasonic
- [ ] Real-time path planning
- [ ] Avoid obstacles while moving to destination
- [ ] Log avoided obstacles

### Project B: Bluetooth Remote Control
- [ ] Implement HC-05 Bluetooth module
- [ ] Create Android/iOS app for control
- [ ] Stream sensor data via Bluetooth
- [ ] Implement wireless configuration

### Project C: Autonomous Patrol
- [ ] Combine line tracking and obstacle avoidance
- [ ] Define patrol area
- [ ] Return to starting point
- [ ] Log patrol statistics

### Project D: Data Logger
- [ ] Log all sensor readings to EEPROM
- [ ] Implement data storage structure
- [ ] Export logged data via Serial
- [ ] Analyze robot movement patterns

---

## Testing & Validation Checklist

### Hardware Verification
- [ ] All motors moving smoothly
- [ ] RGB LEDs displaying correct colors
- [ ] OLED display showing clear text/graphics
- [ ] Joystick input responsive
- [ ] IR receiver detecting remote signals
- [ ] Ultrasonic sensor measuring distances accurately
- [ ] Line sensors calibrated for white/black distinction
- [ ] Buzzer producing clear tones

### Code Quality Standards
- [ ] All code properly commented
- [ ] Function documentation complete
- [ ] Variable names descriptive
- [ ] No hardcoded magic numbers (use #define)
- [ ] Proper error handling implemented
- [ ] Serial debugging enabled
- [ ] Code compiled without warnings

### Documentation Requirements
- [ ] Circuit diagram for each component
- [ ] Pin mapping documentation
- [ ] Library dependencies listed
- [ ] Usage instructions for each .ino file
- [ ] Troubleshooting guide
- [ ] Video demonstrations (optional)

---

## Notes

- **Testing Order**: Always complete individual sensor tests before combining sensors
- **Calibration**: Each sensor may need calibration for optimal performance
- **Power Management**: Monitor battery voltage during extended testing
- **Safety**: Ensure clear testing area to prevent robot damage
- **Documentation**: Update README as you complete each task

---

## Legend
- ⭐ = Beginner (1-2 sensors, simple logic)
- ⭐⭐ = Intermediate (2-3 sensors, moderate complexity)
- ⭐⭐⭐ = Advanced (3+ sensors, PID/complex algorithms)
- ⭐⭐⭐⭐ = Expert (full system integration, optimization)
