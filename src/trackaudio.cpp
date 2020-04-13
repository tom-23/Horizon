#include "trackaudio.h"

TrackAudio::TrackAudio(std::shared_ptr<AudioContext> _context, std::shared_ptr<GainNode> _outputNode, int trackID)
{
    context = _context;
    outputNode = _outputNode;
}

std::shared_ptr<GainNode> TrackAudio::getTrackNode() {
    return outputNode;
}
