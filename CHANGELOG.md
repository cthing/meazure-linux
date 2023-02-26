# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

The changes described below are relative to version
[4.0.0 of Meazure for Windows](https://github.com/cthing/meazure/tree/4.0.0).

### Added

- First release of Meazure on Linux.
- The thickness of the measurement tool lines can be adjusted.
- The screen grid spacing can now be set in any units supported by Meazure.
- The screen grid size, origin offset, and angular orientation can now be set in addition to the grid spacing.
- The Tool and Ruler preference pages show sample lines, crosshairs and rulers on light and dark backgrounds
  to help when changing colors and opacities.

### Changed

- The Ruler tool has been completely redesigned to provide a pair of orthogonal rulers that can be dragged, resized
  and rotated.
- The zoom controls and color display on the main user interface have been reorganized for clarity.
- The hard reset button has been moved from a dedicated preference page to a button on the preference dialog.
- The popup data window and origin marker visibility controls have been moved from the Tools preference page to
  the View menu.
- The shortcut for setting the origin has been changed Ctrl+N from Ctrl+A.
- The Hide Crosshairs item has been moved from the Tools menu to the View menu.
- The Find Crosshairs item has been moved from the Tools menu to the Edit menu.
- The Tool information section is no longer labeled with the selected tool.
- The Replace button on the position management dialog has been removed and an Insert button has been added
- "Profiles" are now called "Configurations".

### Removed

- Sample profiles (configurations) are no longer included with Meazure.
- The ability to specify a startup profile has been removed.

[unreleased]: https://github.com/cthing/meazure-linux/compare/bb3d96a0518989782d680451fff8cc69d1005d3f...HEAD
