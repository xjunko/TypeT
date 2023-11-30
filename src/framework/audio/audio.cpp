#pragma once

// OK
#include <memory>
#include <stdbool.h>
#define BOOL bool

#include "bass.h"
#include "bass_fx.h"
#include "bassmix.h"

namespace Audio {

const int PLAYBACK_BUFFER_LENGTH = 100;
const int DEVICE_BUFFER_LENGTH = 10;
const int UPDATE_PERIOD = 5;
const int DEV_UPDATE_PERIOD = 10;

class BASS {
public:
  HSTREAM mixer;

  // FNs
  void initialize();
};

void BASS::initialize() {
  // Initialize BASS the same way osu! does it.
  BASS_SetConfig(BASS_CONFIG_DEV_NONSTOP, 1);
  BASS_SetConfig(BASS_CONFIG_VISTA_TRUEPOS, 0);
  BASS_SetConfig(BASS_CONFIG_BUFFER, PLAYBACK_BUFFER_LENGTH);
  BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, UPDATE_PERIOD);
  BASS_SetConfig(BASS_CONFIG_DEV_BUFFER, DEVICE_BUFFER_LENGTH);
  BASS_SetConfig(BASS_CONFIG_DEV_PERIOD, DEV_UPDATE_PERIOD);
  BASS_SetConfig(68, 1);

  int DEVICE_ID = -1;
  int MIXER_FLAGS = BASS_MIXER_NONSTOP;

  int BASS_RATE = 48000;

  if (BASS_Init(DEVICE_ID, BASS_RATE, 0, 0, 0) != 0) {
    std::printf("[Audio] Bass started!");

    // Mixer
    auto MASTER_MIXER = BASS_Mixer_StreamCreate(BASS_RATE, 2, MIXER_FLAGS);
    BASS_ChannelSetAttribute(MASTER_MIXER, BASS_ATTRIB_BUFFER, 0);
    BASS_ChannelSetDevice(MASTER_MIXER, BASS_GetDevice());

    BASS_ChannelPlay(MASTER_MIXER, 0);

    mixer = MASTER_MIXER;
  } else {
    std::printf("[Audio] Error, failed to start BASS!");
  }
}

class Track {
private:
  HSTREAM channel;

public:
  std::shared_ptr<BASS> bass;
  std::string path;

  void initialize();
  void play();
  void pause();
  void resume();
};

void Track::initialize() {
  channel = BASS_StreamCreateFile(0, path.c_str(), 0, 0,
                                  BASS_STREAM_DECODE | BASS_STREAM_PRESCAN |
                                      BASS_ASYNCFILE);

  // FX
  channel =
      BASS_FX_TempoCreate(channel, BASS_FX_FREESOURCE | BASS_STREAM_DECODE);

  BASS_ChannelSetAttribute(channel, BASS_ATTRIB_TEMPO_OPTION_USE_QUICKALGO, 1);
  BASS_ChannelSetAttribute(channel, BASS_ATTRIB_TEMPO_OPTION_OVERLAP_MS, 4.0);
  BASS_ChannelSetAttribute(channel, BASS_ATTRIB_TEMPO_OPTION_SEQUENCE_MS, 30.0);

  // HACK: volume
  BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, 0.2f);
}

void Track::play() {
  BASS_Mixer_StreamAddChannel(
      bass->mixer, channel, BASS_MIXER_CHAN_NORAMPIN | BASS_MIXER_CHAN_BUFFER);
}

void Track::pause() {
  BASS_Mixer_ChannelFlags(channel, BASS_MIXER_CHAN_PAUSE,
                          BASS_MIXER_CHAN_PAUSE);
}

void Track::resume() {
  BASS_Mixer_ChannelFlags(channel, 0, BASS_MIXER_CHAN_PAUSE);
}

std::shared_ptr<Track> new_track(std::string path, std::shared_ptr<BASS> bass) {
  auto track = std::make_shared<Track>();

  track->path = path;
  track->bass = bass;

  track->initialize();

  return track;
}

} // namespace Audio