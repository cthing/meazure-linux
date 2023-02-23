# Meazure<sup>TM</sup> for Linux

Meazure is a tool for easily measuring and capturing portions of the screen. Tasks such as measuring the size of a window,
capturing an image, determining the color of a pixel, and determining the screen size and resolution can all be
easily accomplished using Meazure.

This is the **Linux** version of Meazure. For the Windows version, go to [github.com/cthing/meazure](https://github.com/cthing/meazure).

![Meazure screenshot](meazure/resources/external/help/images/MeazureLayout.png)

## Linux Compatibility

This version of Meazure has been tested on Ubuntu 20.04 using the GNOME and KDE desktops. Meazure requires the X11
Window system and **is not supported on Wayland or Xwayland**.

## Downloading and Installation

The Debian package for Meazure can be [downloaded from GitHub](https://github.com/cthing/meazure-linux/releases).
As part of the installation, the file suffixes `.mea` and `.mpl` with the Meazure program.

## Known Issues

- Meazure is not supported on Wayland

- Certain Window Managers (e.g. Gnome) have areas into which the crosshair can be dragged but cannot be dragged out.
  Typically, these are areas on the side, bottom or top edges of the screen where the Window Manager provides its
  icon bars and decoration. If a crosshair is dragged into one of these dead zones, use the text fields to move the
  crosshair out.

## Feedback

Please [open an issue on GitHub](https://github.com/cthing/meazure-linux/issues) for feature requests and to report
bugs.

## Financial Contributions

We hope Meazure is a valuable addition to your software toolbox. Please consider
making a [financial contribution](https://github.com/sponsors/baron1405). Thank you!
