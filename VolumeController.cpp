#include "pch.h"
#include <mmdeviceapi.h>
#include "VolumeController.h"

// Pulled from:
// https://www.codeproject.com/Tips/233484/Change-Master-Volume-in-Visual-Cplusplus
// A little fixing up and nudging around, but mostly whole hog a copy

VolumeController::VolumeController():
    endpointVolume(nullptr)
{
    HRESULT hr = NULL;

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    IMMDeviceEnumerator* deviceEnumerator = nullptr;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER,
        __uuidof(IMMDeviceEnumerator), reinterpret_cast<LPVOID*>(&deviceEnumerator));
    IMMDevice* defaultDevice = nullptr;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = nullptr;

    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
        CLSCTX_INPROC_SERVER, nullptr, reinterpret_cast<LPVOID*>(&endpointVolume));
    defaultDevice->Release();
    defaultDevice = nullptr;
}

VolumeController::~VolumeController()
{
    endpointVolume->Release();

    CoUninitialize();
}

void VolumeController::volume_up() const
{
    endpointVolume->VolumeStepUp(nullptr);
}

void VolumeController::volume_down() const
{
    endpointVolume->VolumeStepDown(nullptr);
}

void VolumeController::mute() const
{
    HRESULT hr { NULL };

    BOOL muteState{ false };
    hr = endpointVolume->GetMute(&muteState);
    hr = endpointVolume->SetMute(!muteState, nullptr);
}

double VolumeController::get_volume(const bool scalar) const
{
    HRESULT hr { NULL };

    float current_volume { 0 };
    if(scalar)
    {
        hr = endpointVolume->GetMasterVolumeLevelScalar(&current_volume);
    }
    else
    {
        hr = endpointVolume->GetMasterVolumeLevel(&current_volume);
    }
       
    return current_volume;
}

void VolumeController::set_volume(const double value, const bool scalar) const
{
    HRESULT hr { NULL };
    if (scalar)
    {
        hr = endpointVolume->SetMasterVolumeLevelScalar(static_cast<float>(value), nullptr);
    }
    else
    {
        hr = endpointVolume->SetMasterVolumeLevel(static_cast<float>(value), nullptr);
    }
}
