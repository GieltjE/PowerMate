#pragma once
#include <endpointvolume.h>

class VolumeController
{
public:
    VolumeController();
    ~VolumeController();
    void volume_up() const;
    void volume_down() const;
    void mute() const;

    double get_volume(bool scalar = false) const;
    void set_volume(double value, bool scalar = false) const;
private:
    IAudioEndpointVolume* endpointVolume;
};
