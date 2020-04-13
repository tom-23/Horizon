#ifndef AUDIOGRID_H
#define AUDIOGRID_H
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

#include <QtGui>

struct GridTime {
    int bar;
    int beat;
    int div;
    int tick;
};

class AudioGrid
{
public:
    AudioGrid();
    int bpm;
    float barLength;
    float beatLength;
    GridTime playHeadLocation;


private:


};

#endif // AUDIOGRID_H
