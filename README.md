dwm - dynamic window manager
============================
dwm is an extremely fast, small, and dynamic window manager for X.

This is my personal fork of [dwm](https://dwm.suckless.org) from suckless.

Why fork?
---------

Some patches on the wiki don't work together by default. They have to
be modified to be used at the same time. Some features work better when
they are implemented together such as the ability to have a fake fullscreen
mode and the ability to fullscreen any window.

Changes
-------

The following features have been added/changed:
* Fake fullscreen: "fullscreen" a window but keep it constrained to its area
* Fullscreen any window (monocle mode still has borders and the top bar)
* Control bar padding via config.h
* Patches taken from the wiki:
	- bottomstack layout
	- grid layout
	- scratchpad

Configuration
-------------

`config.def.h` contains all keybind changes and corresponds with the
manpage.

`config.h` is provided as an example of what I do in terms of cosmetics
and contains my bindings for spawning programs. It should be removed or
modified to suit your needs.

Why use suckless?
-----------------

Suckless programs are by designed to be simple and easy to
understand. This means that you can easily change and modify core
features to suit your needs. Using a packaged version of these programs
is pointless, to truly understand why they are so great you need to use
them to solve your problems.
