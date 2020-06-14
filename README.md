<img src="http://tom-butcher.ml:2233/git/tom/Horizon/raw/branch/master/main_brand.png" width="500"></img>
#
A digital audio workstation written in c++ and using the Labsound library for audio playback.

## Getting started
First clone the git repository to a directory with the following git command:
```
git clone http://tom-butcher.ml:2233/git/tom/Horizon.git
cd Horizon/
```
Then, you need to init the submodule(s):
```
git submodule init
```
And then update the submodules with the recursive and init flags:
```
git submodule update --init --recursive
```
Then run cmake to build a makefile:
```
cmake .
```
Finally to bulld Horizon, run:
```
make
```

## TODO List:
- [ ] Ensure it builds on Windows and not just macOS
- [x] Make the UI look SEXCCCCC.
- [x] Theme management using JSON
- [x] Getting audio into a region
- [ ] Library / file browser functionality
- [ ] MIDI / Software instruments
- [ ] Audio Effects (Reverb etc...)
- [ ] The 'node' editor

Trello:
https://trello.com/b/yox3T1Iv/horizon-daw