#ifndef TRACKAUDIO_H
#define TRACKAUDIO_H

#include "audiomanager.h"

using namespace lab;


class TrackAudio
{
public:
    TrackAudio(std::shared_ptr<AudioContext> _context, std::shared_ptr<GainNode> _outputNode, int _trackID);
    std::shared_ptr<GainNode> getTrackNode();
private:
    std::shared_ptr<GainNode> outputNode;
    std::shared_ptr<AudioContext> context;
};

#endif // TRACKAUDIO_H
