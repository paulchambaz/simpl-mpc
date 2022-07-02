SIMPL-MPC
=

About
-
This project is a continuation of another project - a music program called simpl.
It takes its interface inspired by the musikcube interface and connects it to a mpd server.
Do not expect this program to be a "complete" mpd client: its only use is to listen to album one at a time.
Uses vim binding and all shortcut are enable without pressing a modkey.

For more information read the man page.

Installation
-

To build this program you will need a couple dependencies:
- [ncurses](https://invisible-island.net/ncurses/)
- [libmpdclient](https://www.musicpd.org/libs/libmpdclient/)

If you use linux, these are most likely in the default repository of your package manager.

```
git clone https://git.chambaz.xyz/simpl-mpc
cd simpl-mpc/build
sudo make clean install
```

Configuration
-
You can choose to configure this program.
All modifications are done in a `include/config.h`.
Of course you can also edit in `include/config.def.h` for better organization.

You can configure the command sent after each mpd command (useful for updating status bar).
You can configure the volume and seek increments.
Finally, you can configure each shortcut of the program.

Licence
-

This project is licenced under the GPLv2 licence.
For more information, read the LICENSE file.
