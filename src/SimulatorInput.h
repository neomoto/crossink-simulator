#pragma once

#include <cstdint>

// Synthetic gravity in g, held until replaced or cleared. HalTiltSensor still
// applies its reader/enable/sleep gates and 50 ms sampling cadence.
void simulatorSetAcceleration(float ax, float ay, float az);
void simulatorClearAcceleration();

// Queue a physical button edge for HalGPIO's next update. Holds use the same
// SDL clock and production mapped-input/global routing as scripted keyboard
// input.
void simulatorSetButtonPressed(uint8_t button, bool pressed);

// Queue a physical screen-contact sample for HalGPIO's next update. Coordinates
// are normalized panel-native values, shared by the injected contacts. Zero
// releases all contacts; positive counts retain raw contact state through
// partial releases. No pinch/rotation gesture geometry is synthesized.
void simulatorSetTouchContacts(uint8_t count, float nx, float ny);

// Convert SDL window coordinates back into the panel-native normalized frame
// expected by the firmware's GfxRenderer::tapToLogical().
void simulatorWindowPointToPanelNormalized(int windowX, int windowY, float &nx,
                                           float &ny);
