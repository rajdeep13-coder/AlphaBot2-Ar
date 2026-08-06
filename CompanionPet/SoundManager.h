/**
 * @file SoundManager.h
 * @brief Plays sound cues — buzzer melodies (always) + DFPlayer tracks (when enabled).
 *
 * Each SoundCue maps to a short non-blocking buzzer melody AND a DFPlayer
 * track number.  When ENABLE_DFPLAYER is not defined the DFPlayer code is
 * compiled out entirely; the buzzer provides all audio feedback.
 *
 * SD-card track numbering (for when you add the DFPlayer):
 *   001_chirp.mp3, 002_curious.mp3, 003_yelp.mp3, 004_purr.mp3,
 *   005_bark.mp3,  006_yawn.mp3,    007_snore.mp3
 */

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "PetConfig.h"

#ifdef ENABLE_DFPLAYER
  #include <SoftwareSerial.h>
  #include <DFRobotDFPlayerMini.h>
#endif

// ---- Buzzer melody data (PROGMEM) ----
// Pairs of {frequency_Hz, duration_ms} terminated by freq==0.

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
    _playing   = false;
    _noteStep  = 0;
    _melPtr    = nullptr;

    #ifdef ENABLE_DFPLAYER
      _dfSerial = new SoftwareSerial(DFP_RX_PIN, DFP_TX_PIN);
      _dfSerial->begin(9600);
      _dfPresent = _dfPlayer.begin(*_dfSerial, /*isACK=*/false);
      if (_dfPresent) {
        _dfPlayer.volume(20);          // 0-30
        _dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
      }
    #endif
  }

  /** Trigger a sound cue (non-blocking). */
  void playCue(SoundCue cue) {
    if (cue == SND_NONE) return;

    // --- DFPlayer path (if present) ---
    #ifdef ENABLE_DFPLAYER
      if (_dfPresent) {
        _dfPlayer.play((int)cue);      // track number == cue enum value
      }
    #endif

    // --- Buzzer fallback (always) ---
    _melPtr = getMelody(cue);
    if (_melPtr) {
      _noteStep  = 0;
      _noteStart = millis();
      _playing   = true;
      playBuzzerNote();
    }
  }

  /** Advance the buzzer melody — call every loop(). */
  void tick() {
    if (!_playing || !_melPtr) return;

    uint16_t dur = pgm_read_word(&_melPtr[_noteStep * 2 + 1]);
    if (millis() - _noteStart >= dur) {
      _noteStep++;
      uint16_t freq = pgm_read_word(&_melPtr[_noteStep * 2]);
      if (freq == 0) {                 // end-of-melody sentinel
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
  uint8_t         _noteStep;
  unsigned long   _noteStart;
  bool            _playing;

  #ifdef ENABLE_DFPLAYER
    SoftwareSerial*       _dfSerial;
    DFRobotDFPlayerMini   _dfPlayer;
    bool                  _dfPresent;
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

#endif // SOUND_MANAGER_H
