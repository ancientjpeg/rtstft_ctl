# rtstft\_ctl

A companion plugin to the [RTSTFT](https://github.com/ancientjpeg/RTSTFT) library, built in JUCE. 

# Compilation


You will require an installation of [JUCE](https://juce.com/get-juce), including the Projucer, to build this project. Further, you will need to initialize this project's submodules. A full setup of the rtstft\_ctl build folder can be done by executing these commands:

```
git clone "https://github.com/ancientjpeg/rtstft_ctl"
cd rtstft_ctl
git submodule init && git submodule update
cd RTSTFT
git submodule init && git submodule update
```

At this point, RTSTFT should be fully set up along with its dependency, PFFFT. You can then open the .jucer file and build the project using your favorite exporter (This has only been tested on macOS, however–installers and Windows verification coming soon!).
