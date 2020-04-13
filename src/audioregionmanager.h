#ifndef AUDIOREGIONMANAGER_H
#define AUDIOREGIONMANAGER_H

#include "trackaudio.h"

using namespace lab;

class AudioRegionManager
{
public:
    AudioRegionManager(std::shared_ptr<AudioContext> _context, std::shared_ptr<GainNode> _trackNode, int _trackID, int _regionID);
    float loadFile(std::string _audioFileName);

    float duration;



    void setStartTime(float _time);
    void startFrom(float _time);
private:
    std::shared_ptr<AudioContext> context;
    std::shared_ptr<GainNode> trackNode;

    std::shared_ptr<AudioBus> audioClip;
    std::shared_ptr<SampledAudioNode> audioClipNode;
    std::shared_ptr<GainNode> regionGainNode;



    int trackID;
    int regionID;
};

#endif // AUDIOREGIONMANAGER_H
