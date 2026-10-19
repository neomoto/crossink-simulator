# Changelog

## Unreleased

### Added

- Inject persistent X3 accelerometer samples with scripted `ACCEL:x,y,z` actions
  in g, or missing samples with `ACCEL:OFF`.
- Supply consume-once acceleration samples for firmware auto-rotation, with
  reader/feature gating, wake stabilization, polling cadence, and freshness.

### Fixed

- Restore CrossInk simulator builds when renderer font headers are not discovered.
- Avoid duplicate serial wrapper definitions when the firmware supplies them.
- Retain SDL and scripted button hold durations through release, matching the
  hardware input contract for short- and long-press actions.
