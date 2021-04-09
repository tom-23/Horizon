![Alt text](misc/assets/appicon.png?raw=true "Horizon")
# Horizon Digital Audio Workstation

[![Build status](https://ci.appveyor.com/api/projects/status/ql6h8cdtmjyqm5ag?svg=true)](https://ci.appveyor.com/project/tom-23/horizon)

A digital audio workstation written in c++ and using the Labsound library for audio playback.

## Getting started
First clone the git repository to a directory with the following git command:
Then update the submodules with the recursive and init flags:
```
git submodule update --init --recursive
```
Then run cmake to create a makefile:
```
cmake .
```
Finally to bulld Horizon, run:
```
make
```

## On macOS:
Once Horizon as built, you should be left with an app bundle (Horizon.app) which, when opened, should crash. This is because there are no core files within the app bundle itself causing Horizon to crash when it trys to use them

To solve this, copy the contents of: ``./assets/bundle`` to the application's resource folder: ``Horizon.app/Contents/Resources``. 

## TODO List:
- [x] Ensure it builds on Windows and not just macOS
- [x] Theme management using JSON
- [x] Getting audio into a region
- [x] Library / file browser functionality
- [ ] MIDI / Software instruments
- [ ] Audio Effects (Reverb etc...)
- [ ] The 'node' editor

Trello:
https://trello.com/b/yox3T1Iv/horizon-daw
