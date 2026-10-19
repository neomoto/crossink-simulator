#pragma once

#include <Arduino.h>

#include "HalGPIO.h"

namespace CrossPointOrientation {
enum Value : uint8_t {
  PORTRAIT = 0,
  LANDSCAPE_CW = 1,
  INVERTED = 2,
  LANDSCAPE_CCW = 3
};
}

namespace CrossPointTiltPageTurn {
enum Value : uint8_t { TILT_OFF = 0, TILT_NORMAL = 1, TILT_INVERTED = 2 };
}

class HalTiltSensor;
extern HalTiltSensor halTiltSensor;

class HalTiltSensor {
private:
  bool _available = false;
  bool _isAwake = false;
  bool _accelerationFresh = false;
  float _ax = 0.0f;
  float _ay = 0.0f;
  float _az = 0.0f;
  unsigned long _wakeMs = 0;
  unsigned long _lastPollMs = 0;

  static constexpr unsigned long POLL_INTERVAL_MS = 50;
  static constexpr unsigned long WAKE_STABILIZE_MS = 300;
  static constexpr unsigned long ACCELERATION_MAX_AGE_MS = 150;

public:
  void begin();
  bool wake();
  bool deepSleep();

  bool isAvailable() const { return _available; }
  void update(uint8_t enabled, uint8_t direction, uint8_t orientation,
              bool inReader, bool autoRotateEnabled = false);
  bool takeAcceleration(float &ax, float &ay, float &az);
  bool wasTiltedForward() { return false; }
  bool wasTiltedBack() { return false; }
  bool hadActivity() { return false; }
  void clearPendingEvents() {}
};
