
Debian
====================
This directory contains files used to package bluecoind/bluecoin-qt
for Debian-based Linux systems. If you compile bluecoind/bluecoin-qt yourself, there are some useful files here.

## bluecoin: URI support ##


bluecoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install bluecoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your bluecoinqt binary to `/usr/bin`
and the `../../share/pixmaps/bluecoin128.png` to `/usr/share/pixmaps`

bluecoin-qt.protocol (KDE)

