# Benoe's Terminal Launcher (BTL)
A terminal-based app launcher that makes use of my previously made libraries.

If you do not see any recent commits that does not mean that this project is abandoned. All it means is that it works well, because this will be my daily driver launcher for now, and thus I will most likely find any bugs it has. Should you find any issues, do not hesitate to report them, I will most likely fix them.

I decided to make this project because I have had enough of not having a very simple app launcher on wayland. Considering that this app uses only the terminal it should work on every linux distribution. It scans /usr/bin only by default, so you should either have every application you wish to launch installed or symlinked there.

Its intended use case is creating a keybind in your window manager or desktop environment which launches your favorite terminal emulator with an instance of btl. You can enhance the experience by using a custom title for this instance and adding a rule to your compositor (or window manager or desktop environment) which moves it wherever you prefer it (the left side for me) and resizes it to a reasonable size (a tall window width small width in my case). This programs adapts automatically to whatever size you give it, so no problems should appear there (With a small width you risk overflowing characters into new lines which looks messy, so a width of at least 15-20 characters is recommended).

An example of doing this with Alacritty (with btl being installed in /usr/bin):

```alacritty --title Btl -e /usr/bin/btl```
