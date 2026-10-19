#include "HalTiltSensor.h"

#include <BoardConfig.h>

#include "SimulatorInput.h"

namespace {
bool accelerationAvailable = false;
float injectedAx = 0.0f;
float injectedAy = 0.0f;
float injectedAz = 0.0f;
} // namespace

HalTiltSensor halTiltSensor;

void simulatorSetAcceleration(float ax, float ay, float az) {
  injectedAx = ax;
  injectedAy = ay;
  injectedAz = az;
  accelerationAvailable = true;
}

void simulatorClearAcceleration() { accelerationAvailable = false; }

void HalTiltSensor::begin() {
  _available = BoardConfig::ACTIVE.board == BoardConfig::Board::XteinkX3 ||
               BoardConfig::isSticky();
  _isAwake = false;
  _accelerationFresh = false;
  _lastPollMs = millis();
}

bool HalTiltSensor::wake() {
  _accelerationFresh = false;
  if (!_available)
    return false;
  _wakeMs = millis();
  _lastPollMs = _wakeMs;
  _isAwake = true;
  return true;
}

bool HalTiltSensor::deepSleep() {
  _accelerationFresh = false;
  _isAwake = false;
  return _available;
}

void HalTiltSensor::update(const uint8_t enabled, const uint8_t /*direction*/,
                           const uint8_t /*orientation*/, const bool inReader,
                           const bool autoRotateEnabled) {
  if (!autoRotateEnabled)
    _accelerationFresh = false;
  if (!_available)
    return;

  const bool shouldBeAwake =
      inReader &&
      (enabled != CrossPointTiltPageTurn::TILT_OFF || autoRotateEnabled);
  if (shouldBeAwake && !_isAwake) {
    wake();
    return;
  }
  if (!shouldBeAwake) {
    deepSleep();
    return;
  }

  const unsigned long now = millis();
  if (now - _wakeMs < WAKE_STABILIZE_MS || now - _lastPollMs < POLL_INTERVAL_MS)
    return;
  _lastPollMs = now;
  _accelerationFresh = false;

  // This is synthetic gravity, not a model of a physical IMU or its gyro. Updating
  // the input does not publish a sample until the production poll cadence
  // fires.
  if (autoRotateEnabled && accelerationAvailable) {
    _ax = injectedAx;
    _ay = injectedAy;
    _az = injectedAz;
    _accelerationFresh = true;
  }
}

bool HalTiltSensor::takeAcceleration(float &ax, float &ay, float &az) {
  if (!_isAwake || !_accelerationFresh)
    return false;
  _accelerationFresh = false;
  if (millis() - _lastPollMs > ACCELERATION_MAX_AGE_MS)
    return false;
  ax = _ax;
  ay = _ay;
  az = _az;
  return true;
}
