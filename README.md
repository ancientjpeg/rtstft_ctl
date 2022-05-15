# rtstft_ctl

A companion plugin to the [RTSTFT](https://github.com/ancientjpeg/RTSTFT) library, built in JUCE.

# Compilation

You will require an installation of [JUCE](https://juce.com/get-juce), including the Projucer, to build this project. Further, you will need to initialize this project's submodules. A full setup of the rtstft_ctl build folder can be done by executing these commands:

```
git clone "https://github.com/ancientjpeg/rtstft_ctl"
cd rtstft_ctl
git submodule init && git submodule update
cd RTSTFT
git submodule init && git submodule update
```

At this point, RTSTFT should be fully set up along with its dependency, PFFFT. You can then open the .jucer file and build the project using your favorite exporter (This has only been tested on macOS, however–installers and Windows verification coming soon!).

## License

See LICENSE for more details.

```
rtstft_ctl is an audio plug-in based off JUCE and the RTSTFT Library.
Copyright (C) 2022 Jackson Kaplan

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
```
