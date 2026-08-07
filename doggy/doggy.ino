#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#ifdef ENABLE_DFPLAYER
  #include <SoftwareSerial.h>
  #include <DFRobotDFPlayerMini.h>
#endif

#ifdef ENABLE_SERVO
  #include <Servo.h>
#endif

// ============================================================================
// FEATURE FLAGS - Uncomment when the hardware is physically connected
// ============================================================================
// #define ENABLE_DFPLAYER
// #define ENABLE_TOUCH_SENSOR
// #define ENABLE_SOUND_SENSOR
// #define ENABLE_SERVO

// ============================================================================
// PIN DEFINITIONS - AlphaBot2-Ar stock wiring
// ============================================================================

#define MOTOR_A_PWM    5
#define MOTOR_A_DIR    4
#define MOTOR_B_PWM    6
#define MOTOR_B_DIR    7

#define US_TRIG_PIN    8
#define US_ECHO_PIN    9

#define BUZZER_PIN     10
#define IR_RECV_PIN    11

#define RGB_LED_PIN    12
#define NUM_LEDS       4

#define OLED_ADDR      0x3C
#define SCREEN_W       128
#define SCREEN_H       64

#ifndef ENABLE_SOUND_SENSOR
  #define TOUCH_FALLBACK_PIN  2
#endif

#ifdef ENABLE_TOUCH_SENSOR
  #define TOUCH_PIN    A0
#endif

#ifdef ENABLE_DFPLAYER
  #define DFP_TX_PIN   A1
  #define DFP_RX_PIN   A2
#endif

#ifdef ENABLE_SOUND_SENSOR
  #define SOUND_PIN    2
#endif

#ifdef ENABLE_SERVO
  #define SERVO_PIN    3
#endif

// ============================================================================
// BEHAVIOUR TIMING (ms)
// ============================================================================

#define SENSOR_POLL_MS        50UL
#define SLEEPY_TIMEOUT_MS     180000UL
#define ASLEEP_TIMEOUT_MS     30000UL
#define CURIOUS_DURATION_MS   5000UL
#define STARTLED_DURATION_MS  2000UL
#define HAPPY_DURATION_MS     4000UL
#define ALERT_DURATION_MS     3000UL
#define IDLE_CHIRP_MIN_MS     20000UL
#define IDLE_CHIRP_MAX_MS     60000UL
#define SNORE_MIN_MS          15000UL
#define SNORE_MAX_MS          30000UL

// ============================================================================
// PROXIMITY THRESHOLDS (cm)
// ============================================================================

#define PROX_NEAR_CM   30
#define PROX_CLOSE_CM  10

// ============================================================================
// MOTOR GESTURE SPEEDS
// ============================================================================

#define GESTURE_SPEED_LOW   60
#define GESTURE_SPEED_MED   80
#define GESTURE_SPEED_HIGH  100

// ============================================================================
// ENUMS
// ============================================================================

enum PetState : uint8_t {
  STATE_IDLE = 0,
  STATE_CURIOUS,
  STATE_STARTLED,
  STATE_HAPPY,
  STATE_ALERT,
  STATE_SLEEPY,
  STATE_ASLEEP
};

enum Expression : uint8_t {
  EXPR_NEUTRAL = 0,
  EXPR_WIDE,
  EXPR_STARTLED,
  EXPR_HAPPY,
  EXPR_ALERT,
  EXPR_SLEEPY,
  EXPR_ASLEEP
};

enum LightEffect : uint8_t {
  LIGHT_BREATHE_BLUE = 0,
  LIGHT_PULSE_CYAN,
  LIGHT_FLASH_WHITE_RED,
  LIGHT_PULSE_WARM,
  LIGHT_FLASH_WHITE_2X,
  LIGHT_DIM_DOWN,
  LIGHT_OFF_PULSE,
  LIGHT_OFF
};

enum GestureType : uint8_t {
  GESTURE_NONE = 0,
  GESTURE_IDLE_SWAY,
  GESTURE_LOOK_TOWARD,
  GESTURE_BACK_UP,
  GESTURE_WIGGLE,
  GESTURE_TURN_TOWARD
};

enum SoundCue : uint8_t {
  SND_NONE = 0,
  SND_CHIRP,
  SND_CURIOUS,
  SND_YELP,
  SND_PURR,
  SND_BARK,
  SND_YAWN,
  SND_SNORE
};

// ============================================================================
// FACE RENDERER
// ============================================================================

#define EYE_L_CX   32
#define EYE_R_CX   96
#define EYE_CY     28

class FaceRenderer {
public:
  FaceRenderer() : _disp(SCREEN_W, SCREEN_H, &Wire, -1) {}

  bool begin() {
    if (!_disp.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) return false;
    _disp.clearDisplay();
    _disp.display();
    _currentExpr   = EXPR_NEUTRAL;
    _blinkFrame    = 0;
    _nextBlinkTime = millis() + random(4000, 7001);
    _dirty         = true;
    return true;
  }

  void bootAnimation() {
    _disp.clearDisplay();
    drawAsleepEyes();
    _disp.display();
    delay(500);

    for (uint8_t i = 4; i >= 1; i--) {
      _disp.clearDisplay();
      drawEyes(36, 24, 5, 0, 0, i * 20);
      _disp.display();
      delay(150);
    }

    _disp.clearDisplay();
    drawEyes(36, 24, 5, 0, 0, 0);
    _disp.display();
    delay(400);

    _disp.clearDisplay();
    drawHappyEyes();
    _disp.display();
    delay(500);

    _currentExpr = EXPR_NEUTRAL;
    _dirty = true;
  }

  void setExpression(Expression expr) {
    if (expr != _currentExpr) {
      _currentExpr = expr;
      _dirty = true;
    }
  }

  void tick() {
    unsigned long now = millis();

    if (_blinkFrame == 0) {
      if (now >= _nextBlinkTime &&
          _currentExpr != EXPR_HAPPY &&
          _currentExpr != EXPR_ASLEEP) {
        _blinkFrame = 1;
        _blinkTimer = now;
        _dirty = true;
      }
    } else {
      if (now - _blinkTimer >= 40) {
        _blinkTimer = now;
        _blinkFrame++;
        if (_blinkFrame > 5) {
          _blinkFrame = 0;
          _nextBlinkTime = now + random(4000, 7001);
        }
        _dirty = true;
      }
    }

    if (_dirty) {
      render();
      _dirty = false;
    }
  }

private:
  Adafruit_SSD1306 _disp;
  Expression _currentExpr;
  uint8_t _blinkFrame;
  unsigned long _blinkTimer;
  unsigned long _nextBlinkTime;
  bool _dirty;

  void render() {
    _disp.clearDisplay();

    uint8_t blinkLid = 0;
    if (_blinkFrame > 0) {
      static const uint8_t bLid[] = {0, 25, 50, 100, 50, 25};
      blinkLid = bLid[_blinkFrame];
    }

    switch (_currentExpr) {
      case EXPR_NEUTRAL:
        drawEyes(36, 24, 5, 0, 0, blinkLid);
        break;
      case EXPR_WIDE:
        drawEyes(42, 30, 7, 0, -2, blinkLid);
        break;
      case EXPR_STARTLED:
        drawEyes(44, 34, 3, 0, 0, blinkLid);
        _disp.drawCircle(64, 52, 4, SSD1306_WHITE);
        break;
      case EXPR_HAPPY:
        drawHappyEyes();
        _disp.drawLine(54, 50, 64, 54, SSD1306_WHITE);
        _disp.drawLine(64, 54, 74, 50, SSD1306_WHITE);
        break;
      case EXPR_ALERT: {
        int8_t ox = ((millis() / 300) & 1) ? 6 : -6;
        drawEyes(36, 24, 5, ox, 0, blinkLid);
        break;
      }
      case EXPR_SLEEPY:
        drawEyes(36, 24, 5, 0, 2, max(blinkLid, (uint8_t)55));
        break;
      case EXPR_ASLEEP:
        drawAsleepEyes();
        break;
    }

    _disp.display();
  }

  void drawEyes(uint8_t eyeW, uint8_t eyeH, uint8_t pupR,
                int8_t pupOX, int8_t pupOY, uint8_t lidPct) {
    uint8_t r = min(eyeW, eyeH) / 3;
    int16_t lx = EYE_L_CX - (int16_t)eyeW / 2;
    int16_t rx = EYE_R_CX - (int16_t)eyeW / 2;
    int16_t ey = EYE_CY - (int16_t)eyeH / 2;

    _disp.fillRoundRect(lx, ey, eyeW, eyeH, r, SSD1306_WHITE);
    _disp.fillRoundRect(rx, ey, eyeW, eyeH, r, SSD1306_WHITE);
    _disp.fillCircle(EYE_L_CX + pupOX, EYE_CY + pupOY, pupR, SSD1306_BLACK);
    _disp.fillCircle(EYE_R_CX + pupOX, EYE_CY + pupOY, pupR, SSD1306_BLACK);

    if (pupR >= 4) {
      _disp.drawPixel(EYE_L_CX + pupOX + 2, EYE_CY + pupOY - 2, SSD1306_WHITE);
      _disp.drawPixel(EYE_R_CX + pupOX + 2, EYE_CY + pupOY - 2, SSD1306_WHITE);
    }

    if (lidPct > 0) {
      uint8_t lidH = (uint16_t)eyeH * lidPct / 100;
      _disp.fillRect(lx - 1, ey - 1, eyeW + 2, lidH + 2, SSD1306_BLACK);
      _disp.fillRect(rx - 1, ey - 1, eyeW + 2, lidH + 2, SSD1306_BLACK);
    }
  }

  void drawHappyEyes() {
    for (int8_t t = -1; t <= 1; t++) {
      _disp.drawLine(EYE_L_CX - 14, EYE_CY + 6,
                     EYE_L_CX,      EYE_CY - 8 + t, SSD1306_WHITE);
      _disp.drawLine(EYE_L_CX,      EYE_CY - 8 + t,
                     EYE_L_CX + 14, EYE_CY + 6,     SSD1306_WHITE);
      _disp.drawLine(EYE_R_CX - 14, EYE_CY + 6,
                     EYE_R_CX,      EYE_CY - 8 + t, SSD1306_WHITE);
      _disp.drawLine(EYE_R_CX,      EYE_CY - 8 + t,
                     EYE_R_CX + 14, EYE_CY + 6,     SSD1306_WHITE);
    }
  }

  void drawAsleepEyes() {
    _disp.drawFastHLine(EYE_L_CX - 12, EYE_CY,     24, SSD1306_WHITE);
    _disp.drawFastHLine(EYE_L_CX - 12, EYE_CY + 1, 24, SSD1306_WHITE);
    _disp.drawFastHLine(EYE_R_CX - 12, EYE_CY,     24, SSD1306_WHITE);
    _disp.drawFastHLine(EYE_R_CX - 12, EYE_CY + 1, 24, SSD1306_WHITE);

    _disp.setTextSize(1);
    _disp.setTextColor(SSD1306_WHITE);
    _disp.setCursor(100, 4);
    _disp.print(F("zzZ"));
  }
};

// ============================================================================
// LIGHT MANAGER
// ============================================================================

class LightManager {
public:
  LightManager()
    : _strip(NUM_LEDS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800) {}

  void begin() {
    _strip.begin();
    _strip.setBrightness(40);
    _strip.show();
    _effect = LIGHT_OFF;
    _effectStart = millis();
    _lastUpdate = 0;
  }

  void setEffect(LightEffect eff) {
    _effect = eff;
    _effectStart = millis();
  }

  void tick() {
    unsigned long now = millis();
    if (now - _lastUpdate < 30) return;
    _lastUpdate = now;

    unsigned long elapsed = now - _effectStart;

    switch (_effect) {
      case LIGHT_BREATHE_BLUE: {
        uint8_t phase = (uint8_t)((elapsed % 3000UL) * 256UL / 3000UL);
        uint8_t brt = breathe(phase);
        uint8_t b = (uint8_t)map(brt, 0, 255, 8, 55);
        setAll(0, 0, b);
        break;
      }
      case LIGHT_PULSE_CYAN: {
        uint8_t phase = (uint8_t)((elapsed % 1000UL) * 256UL / 1000UL);
        uint8_t brt = breathe(phase);
        setAll(0, brt / 5, brt / 3);
        break;
      }
      case LIGHT_FLASH_WHITE_RED: {
        if (elapsed < 400) {
          uint8_t idx = (uint8_t)(elapsed / 100);
          if (idx & 1) setAll(255, 0, 0);
          else setAll(255, 255, 255);
        } else {
          setAll(25, 0, 0);
        }
        break;
      }
      case LIGHT_PULSE_WARM: {
        uint8_t phase = (uint8_t)((elapsed % 1500UL) * 256UL / 1500UL);
        uint8_t brt = breathe(phase);
        uint8_t r = (uint8_t)((uint16_t)255 * brt / 255);
        uint8_t g = (uint8_t)((uint16_t)140 * brt / 255);
        uint8_t b = (uint8_t)((uint16_t)50 * brt / 255);
        setAll(r, g, b);
        break;
      }
      case LIGHT_FLASH_WHITE_2X: {
        if      (elapsed < 120) setAll(255, 255, 255);
        else if (elapsed < 220) setAll(0, 0, 0);
        else if (elapsed < 340) setAll(255, 255, 255);
        else setAll(0, 0, 0);
        break;
      }
      case LIGHT_DIM_DOWN: {
        if (elapsed < 5000UL) {
          uint8_t brt = (uint8_t)map(elapsed, 0, 5000, 40, 2);
          setAll(0, 0, brt);
        } else {
          setAll(0, 0, 2);
        }
        break;
      }
      case LIGHT_OFF_PULSE: {
        uint8_t phase = (uint8_t)((elapsed % 8000UL) * 256UL / 8000UL);
        uint8_t brt = breathe(phase);
        setAll(0, 0, brt / 28);
        break;
      }
      case LIGHT_OFF:
      default:
        setAll(0, 0, 0);
        break;
    }
  }

private:
  Adafruit_NeoPixel _strip;
  LightEffect _effect;
  unsigned long _effectStart;
  unsigned long _lastUpdate;

  static uint8_t breathe(uint8_t phase) {
    uint16_t v = (phase < 128) ? (uint16_t)phase * 2
                                : (uint16_t)(255 - phase) * 2;
    return (uint8_t)(v * v >> 8);
  }

  void setAll(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      _strip.setPixelColor(i, _strip.Color(r, g, b));
    }
    _strip.show();
  }
};

// ============================================================================
// SOUND MANAGER
// ============================================================================

static const uint16_t PROGMEM mel_chirp[]   = { 2000, 50,  2500, 50,   0,0 };
static const uint16_t PROGMEM mel_curious[] = { 1500, 80,  2000, 80,   0,0 };
static const uint16_t PROGMEM mel_yelp[]    = { 2800, 60,  1200, 90,   0,0 };
static const uint16_t PROGMEM mel_purr[]    = {  250,200,   300,200, 250,200, 0,0 };
static const uint16_t PROGMEM mel_bark[]    = {  900, 70,  1400, 70,   0,0 };
static const uint16_t PROGMEM mel_yawn[]    = { 1000,150,   700,200, 400,250, 0,0 };
static const uint16_t PROGMEM mel_snore[]   = {  200,300,   0,0 };

class SoundManager {
public:
  void begin() {
    pinMode(BUZZER_PIN, OUTPUT);
    noTone(BUZZER_PIN);
    _playing = false;
    _noteStep = 0;
    _melPtr = nullptr;

    #ifdef ENABLE_DFPLAYER
      _dfSerial = new SoftwareSerial(DFP_RX_PIN, DFP_TX_PIN);
      _dfSerial->begin(9600);
      _dfPresent = _dfPlayer.begin(*_dfSerial, false);
      if (_dfPresent) {
        _dfPlayer.volume(20);
        _dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
      }
    #endif
  }

  void playCue(SoundCue cue) {
    if (cue == SND_NONE) return;

    #ifdef ENABLE_DFPLAYER
      if (_dfPresent) {
        _dfPlayer.play((int)cue);
      }
    #endif

    _melPtr = getMelody(cue);
    if (_melPtr) {
      _noteStep = 0;
      _noteStart = millis();
      _playing = true;
      playBuzzerNote();
    }
  }

  void tick() {
    if (!_playing || !_melPtr) return;

    uint16_t dur = pgm_read_word(&_melPtr[_noteStep * 2 + 1]);
    if (millis() - _noteStart >= dur) {
      _noteStep++;
      uint16_t freq = pgm_read_word(&_melPtr[_noteStep * 2]);
      if (freq == 0) {
        noTone(BUZZER_PIN);
        _playing = false;
      } else {
        _noteStart = millis();
        playBuzzerNote();
      }
    }
  }

  bool isPlaying() const { return _playing; }

private:
  const uint16_t* _melPtr;
  uint8_t _noteStep;
  unsigned long _noteStart;
  bool _playing;

  #ifdef ENABLE_DFPLAYER
    SoftwareSerial* _dfSerial;
    DFRobotDFPlayerMini _dfPlayer;
    bool _dfPresent;
  #endif

  void playBuzzerNote() {
    uint16_t freq = pgm_read_word(&_melPtr[_noteStep * 2]);
    if (freq > 0) {
      tone(BUZZER_PIN, freq);
    }
  }

  static const uint16_t* getMelody(SoundCue cue) {
    switch (cue) {
      case SND_CHIRP:   return mel_chirp;
      case SND_CURIOUS: return mel_curious;
      case SND_YELP:    return mel_yelp;
      case SND_PURR:    return mel_purr;
      case SND_BARK:    return mel_bark;
      case SND_YAWN:    return mel_yawn;
      case SND_SNORE:   return mel_snore;
      default:          return nullptr;
    }
  }
};

// ============================================================================
// MOTION GESTURES
// ============================================================================

class MotionGestures {
public:
  void begin() {
    pinMode(MOTOR_A_PWM, OUTPUT);
    pinMode(MOTOR_A_DIR, OUTPUT);
    pinMode(MOTOR_B_PWM, OUTPUT);
    pinMode(MOTOR_B_DIR, OUTPUT);
    motorStop();

    _gesture = GESTURE_NONE;
    _step = 0;
    _active = false;
    _swayDelay = 5000;

    #ifdef ENABLE_SERVO
      _servo.attach(SERVO_PIN);
      _servo.write(90);
    #endif
  }

  void startGesture(GestureType g) {
    _gesture = g;
    _step = 0;
    _stepStart = millis();
    _active = true;
    if (g == GESTURE_IDLE_SWAY) {
      _swayDelay = random(3000, 8001);
      motorStop();
    }
    applyStepEntry();
  }

  void stopGesture() {
    _active = false;
    _gesture = GESTURE_NONE;
    motorStop();
  }

  bool isActive() const { return _active; }

  void tick() {
    if (!_active) return;
    unsigned long elapsed = millis() - _stepStart;

    switch (_gesture) {
      case GESTURE_IDLE_SWAY:   tickIdleSway(elapsed);   break;
      case GESTURE_LOOK_TOWARD: tickLookToward(elapsed); break;
      case GESTURE_BACK_UP:     tickBackUp(elapsed);     break;
      case GESTURE_WIGGLE:      tickWiggle(elapsed);     break;
      case GESTURE_TURN_TOWARD: tickTurnToward(elapsed);  break;
      default: _active = false; break;
    }
  }

private:
  GestureType _gesture;
  uint8_t _step;
  unsigned long _stepStart;
  bool _active;
  unsigned long _swayDelay;

  #ifdef ENABLE_SERVO
    Servo _servo;
  #endif

  void advanceStep() {
    _step++;
    _stepStart = millis();
    applyStepEntry();
  }

  void finishGesture() {
    motorStop();
    _active = false;
    _gesture = GESTURE_NONE;
  }

  void applyStepEntry() {
  }

  void tickIdleSway(unsigned long el) {
    switch (_step) {
      case 0:
        if (el >= _swayDelay) {
          _step = 1;
          _stepStart = millis();
          motorTurnLeft(GESTURE_SPEED_LOW);
        }
        break;
      case 1:
        if (el >= 60) {
          motorStop();
          _step = 2;
          _stepStart = millis();
        }
        break;
      case 2:
        if (el >= 80) {
          _step = 3;
          _stepStart = millis();
          motorTurnRight(GESTURE_SPEED_LOW);
        }
        break;
      case 3:
        if (el >= 60) {
          motorStop();
          _step = 0;
          _stepStart = millis();
          _swayDelay = random(3000, 8001);
        }
        break;
    }
  }

  void tickLookToward(unsigned long el) {
    switch (_step) {
      case 0:
        if (_step == 0 && el < 5) motorTurnRight(GESTURE_SPEED_MED);
        if (el >= 150) { motorStop(); advanceStep(); }
        break;
      case 1:
        if (el >= 200) { motorForward(GESTURE_SPEED_LOW); advanceStep(); }
        break;
      case 2:
        if (el >= 200) finishGesture();
        break;
    }
  }

  void tickBackUp(unsigned long el) {
    switch (_step) {
      case 0:
        if (el < 5) motorBackward(GESTURE_SPEED_HIGH);
        if (el >= 300) { motorStop(); advanceStep(); }
        break;
      case 1:
        if (el >= 100) { motorTurnRight(GESTURE_SPEED_MED); advanceStep(); }
        break;
      case 2:
        if (el >= 200) finishGesture();
        break;
    }
  }

  void tickWiggle(unsigned long el) {
    if (_step >= 4) { finishGesture(); return; }
    if (el < 5) {
      if (_step & 1) motorTurnRight(GESTURE_SPEED_MED);
      else motorTurnLeft(GESTURE_SPEED_MED);
    }
    if (el >= 80) { motorStop(); advanceStep(); }
  }

  void tickTurnToward(unsigned long el) {
    if (el < 5) motorTurnLeft(GESTURE_SPEED_MED);
    if (el >= 180) finishGesture();
  }

  static void motorForward(uint8_t speed) {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_PWM, speed);
    analogWrite(MOTOR_B_PWM, speed);
  }

  static void motorBackward(uint8_t speed) {
    digitalWrite(MOTOR_A_DIR, LOW);
    digitalWrite(MOTOR_B_DIR, LOW);
    analogWrite(MOTOR_A_PWM, speed);
    analogWrite(MOTOR_B_PWM, speed);
  }

  static void motorTurnLeft(uint8_t speed) {
    digitalWrite(MOTOR_A_DIR, LOW);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_PWM, speed);
    analogWrite(MOTOR_B_PWM, speed);
  }

  static void motorTurnRight(uint8_t speed) {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, LOW);
    analogWrite(MOTOR_A_PWM, speed);
    analogWrite(MOTOR_B_PWM, speed);
  }

  static void motorStop() {
    analogWrite(MOTOR_A_PWM, 0);
    analogWrite(MOTOR_B_PWM, 0);
  }
};

// ============================================================================
// SENSOR MANAGER
// ============================================================================

class SensorManager {
public:
  bool objectNear;
  bool objectClose;
  bool touched;
  bool loudSound;
  bool anyEvent;

  void begin() {
    _lastPoll = 0;
    _touchDebounce = 0;
    _soundDebounce = 0;
    _distanceCm = 999;
    _prevTouchRaw = false;
    _prevSoundRaw = false;

    pinMode(US_TRIG_PIN, OUTPUT);
    pinMode(US_ECHO_PIN, INPUT);

    #ifdef TOUCH_FALLBACK_PIN
      pinMode(TOUCH_FALLBACK_PIN, INPUT_PULLUP);
    #endif
    #ifdef ENABLE_TOUCH_SENSOR
      pinMode(TOUCH_PIN, INPUT);
    #endif
    #ifdef ENABLE_SOUND_SENSOR
      pinMode(SOUND_PIN, INPUT);
    #endif

    clearEvents();
  }

  void update() {
    unsigned long now = millis();
    if (now - _lastPoll < SENSOR_POLL_MS) return;
    _lastPoll = now;

    clearEvents();

    _distanceCm = readUltrasonic();
    if (_distanceCm <= PROX_CLOSE_CM) {
      objectClose = true;
    } else if (_distanceCm <= PROX_NEAR_CM) {
      objectNear = true;
    }

    bool tRaw = readTouch();
    if (tRaw && !_prevTouchRaw && (now - _touchDebounce > 100)) {
      touched = true;
      _touchDebounce = now;
    }
    _prevTouchRaw = tRaw;

    bool sRaw = readSound();
    if (sRaw && !_prevSoundRaw && (now - _soundDebounce > 200)) {
      loudSound = true;
      _soundDebounce = now;
    }
    _prevSoundRaw = sRaw;

    anyEvent = objectNear || objectClose || touched || loudSound;
  }

  uint16_t getDistance() const { return _distanceCm; }

private:
  unsigned long _lastPoll;
  unsigned long _touchDebounce;
  unsigned long _soundDebounce;
  uint16_t _distanceCm;
  bool _prevTouchRaw;
  bool _prevSoundRaw;

  void clearEvents() {
    objectNear = false;
    objectClose = false;
    touched = false;
    loudSound = false;
    anyEvent = false;
  }

  uint16_t readUltrasonic() {
    digitalWrite(US_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(US_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_TRIG_PIN, LOW);
    unsigned long dur = pulseIn(US_ECHO_PIN, HIGH, 20000UL);
    if (dur == 0) return 999;
    return (uint16_t)(dur * 0.034f * 0.5f);
  }

  bool readTouch() {
    #ifdef ENABLE_TOUCH_SENSOR
      return digitalRead(TOUCH_PIN) == HIGH;
    #elif defined(TOUCH_FALLBACK_PIN)
      return digitalRead(TOUCH_FALLBACK_PIN) == LOW;
    #else
      return false;
    #endif
  }

  bool readSound() {
    #ifdef ENABLE_SOUND_SENSOR
      return digitalRead(SOUND_PIN) == HIGH;
    #else
      return false;
    #endif
  }
};

// ============================================================================
// BEHAVIOR FSM
// ============================================================================

class BehaviorFSM {
public:
  void begin(SensorManager& s, FaceRenderer& f, LightManager& l,
             SoundManager& snd, MotionGestures& m) {
    _sen = &s;  _face = &f;  _lgt = &l;  _snd = &snd;  _mot = &m;

    _state = STATE_IDLE;
    _stateEntry = millis();
    _lastEventTime = millis();
    _nextChirpTime = millis() + random(IDLE_CHIRP_MIN_MS, IDLE_CHIRP_MAX_MS + 1);
    _nextSnoreTime = millis() + random(SNORE_MIN_MS, SNORE_MAX_MS + 1);
  }

  void forceState(PetState s) { enterState(s); }

  PetState getState() const { return _state; }

  void update() {
    unsigned long now = millis();

    if (_sen->anyEvent) {
      _lastEventTime = now;
    }

    if (_sen->objectClose && _state != STATE_STARTLED) {
      enterState(STATE_STARTLED);
      return;
    }

    if (_sen->loudSound &&
        _state != STATE_STARTLED && _state != STATE_ALERT) {
      enterState(STATE_ALERT);
      return;
    }

    if (_sen->touched &&
        _state != STATE_STARTLED && _state != STATE_ALERT &&
        _state != STATE_HAPPY) {
      enterState(STATE_HAPPY);
      return;
    }

    if ((_state == STATE_SLEEPY || _state == STATE_ASLEEP) &&
        _sen->anyEvent) {
      enterState(STATE_IDLE);
      return;
    }

    unsigned long inState = now - _stateEntry;

    switch (_state) {
      case STATE_IDLE:
        if (_sen->objectNear) {
          enterState(STATE_CURIOUS);
          return;
        }
        if (now - _lastEventTime >= SLEEPY_TIMEOUT_MS) {
          enterState(STATE_SLEEPY);
          return;
        }
        if (now >= _nextChirpTime) {
          _snd->playCue(SND_CHIRP);
          _nextChirpTime = now + random(IDLE_CHIRP_MIN_MS, IDLE_CHIRP_MAX_MS + 1);
        }
        break;

      case STATE_CURIOUS:
        if (inState >= CURIOUS_DURATION_MS) enterState(STATE_IDLE);
        break;

      case STATE_STARTLED:
        if (inState >= STARTLED_DURATION_MS) enterState(STATE_IDLE);
        break;

      case STATE_HAPPY:
        if (inState >= HAPPY_DURATION_MS) enterState(STATE_IDLE);
        break;

      case STATE_ALERT:
        if (inState >= ALERT_DURATION_MS) enterState(STATE_IDLE);
        break;

      case STATE_SLEEPY:
        if (inState >= ASLEEP_TIMEOUT_MS) enterState(STATE_ASLEEP);
        break;

      case STATE_ASLEEP:
        if (now >= _nextSnoreTime) {
          _snd->playCue(SND_SNORE);
          _nextSnoreTime = now + random(SNORE_MIN_MS, SNORE_MAX_MS + 1);
        }
        break;
    }
  }

private:
  SensorManager* _sen;
  FaceRenderer* _face;
  LightManager* _lgt;
  SoundManager* _snd;
  MotionGestures* _mot;

  PetState _state;
  unsigned long _stateEntry;
  unsigned long _lastEventTime;
  unsigned long _nextChirpTime;
  unsigned long _nextSnoreTime;

  void enterState(PetState ns) {
    _state = ns;
    _stateEntry = millis();

    if (ns != STATE_SLEEPY && ns != STATE_ASLEEP) {
      _lastEventTime = millis();
    }

    switch (ns) {
      case STATE_IDLE:
        _face->setExpression(EXPR_NEUTRAL);
        _lgt->setEffect(LIGHT_BREATHE_BLUE);
        _snd->playCue(SND_CHIRP);
        _mot->startGesture(GESTURE_IDLE_SWAY);
        _nextChirpTime = millis() + random(IDLE_CHIRP_MIN_MS, IDLE_CHIRP_MAX_MS + 1);
        break;
      case STATE_CURIOUS:
        _face->setExpression(EXPR_WIDE);
        _lgt->setEffect(LIGHT_PULSE_CYAN);
        _snd->playCue(SND_CURIOUS);
        _mot->startGesture(GESTURE_LOOK_TOWARD);
        break;
      case STATE_STARTLED:
        _face->setExpression(EXPR_STARTLED);
        _lgt->setEffect(LIGHT_FLASH_WHITE_RED);
        _snd->playCue(SND_YELP);
        _mot->startGesture(GESTURE_BACK_UP);
        break;
      case STATE_HAPPY:
        _face->setExpression(EXPR_HAPPY);
        _lgt->setEffect(LIGHT_PULSE_WARM);
        _snd->playCue(SND_PURR);
        _mot->startGesture(GESTURE_WIGGLE);
        break;
      case STATE_ALERT:
        _face->setExpression(EXPR_ALERT);
        _lgt->setEffect(LIGHT_FLASH_WHITE_2X);
        _snd->playCue(SND_BARK);
        _mot->startGesture(GESTURE_TURN_TOWARD);
        break;
      case STATE_SLEEPY:
        _face->setExpression(EXPR_SLEEPY);
        _lgt->setEffect(LIGHT_DIM_DOWN);
        _snd->playCue(SND_YAWN);
        _mot->stopGesture();
        break;
      case STATE_ASLEEP:
        _face->setExpression(EXPR_ASLEEP);
        _lgt->setEffect(LIGHT_OFF_PULSE);
        _mot->stopGesture();
        _nextSnoreTime = millis() + random(SNORE_MIN_MS, SNORE_MAX_MS + 1);
        break;
    }

    Serial.print(F("[PET] -> "));
    printStateName(ns);
    Serial.println();
  }

  static void printStateName(PetState s) {
    switch (s) {
      case STATE_IDLE:     Serial.print(F("IDLE")); break;
      case STATE_CURIOUS:  Serial.print(F("CURIOUS")); break;
      case STATE_STARTLED: Serial.print(F("STARTLED")); break;
      case STATE_HAPPY:    Serial.print(F("HAPPY")); break;
      case STATE_ALERT:    Serial.print(F("ALERT")); break;
      case STATE_SLEEPY:   Serial.print(F("SLEEPY")); break;
      case STATE_ASLEEP:   Serial.print(F("ASLEEP")); break;
    }
  }
};

// ============================================================================
// GLOBAL MODULES
// ============================================================================

SensorManager  sensors;
FaceRenderer   face;
LightManager   lights;
SoundManager   sound;
MotionGestures motion;
BehaviorFSM    behavior;

// ============================================================================
// SKETCH ENTRY POINTS
// ============================================================================

void setup() {
  Serial.begin(115200);
  Serial.println(F(""));
  Serial.println(F("================================="));
  Serial.println(F(" AlphaBot2  Companion Pet  v1.0"));
  Serial.println(F("================================="));

  randomSeed(analogRead(A3));
  Wire.begin();

  sensors.begin();
  Serial.println(F("[INIT] Sensors   OK"));

  if (!face.begin()) {
    Serial.println(F("[INIT] OLED FAIL - halting"));
    while (1) ;
  }
  Serial.println(F("[INIT] OLED      OK"));

  lights.begin();
  Serial.println(F("[INIT] NeoPixel  OK"));

  sound.begin();
  Serial.println(F("[INIT] Sound     OK"));

  motion.begin();
  Serial.println(F("[INIT] Motors    OK"));

  behavior.begin(sensors, face, lights, sound, motion);
  Serial.println(F("[INIT] FSM       OK"));

  Serial.println(F("[BOOT] Waking up..."));
  face.bootAnimation();
  sound.playCue(SND_CHIRP);

  behavior.forceState(STATE_IDLE);

  Serial.println(F("[BOOT] Ready! Press joystick button to pet."));
  Serial.println(F("       Wave hand near ultrasonic to interact."));

  #ifndef ENABLE_TOUCH_SENSOR
    Serial.println(F("       (TTP223 not enabled - using joy btn on D2)"));
  #endif
  #ifndef ENABLE_SOUND_SENSOR
    Serial.println(F("       (KY-038 not enabled - ALERT disabled)"));
  #endif
  #ifndef ENABLE_DFPLAYER
    Serial.println(F("       (DFPlayer not enabled - buzzer fallback)"));
  #endif
}

void loop() {
  sensors.update();
  behavior.update();
  face.tick();
  lights.tick();
  sound.tick();
  motion.tick();
}