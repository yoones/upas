/*
** This file is part of upas.
** 
** upas is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** upas is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PORTAUDIO_HPP_
# define PORTAUDIO_HPP_

# include "portaudio/portaudio.h"

# define NUM_CHANNELS 2

// paFloat32	float
// paInt32	int
// paInt16	short

class PortAudio
{
protected:
  PaStream* stream;
  PaStreamParameters inputParameters;
  PaStreamParameters outputParameters;

  PaSampleFormat sampleFormat;
  int sampleRate;
  int framesPerBuffer;

  PaStreamCallback *inputCallback;
  PaStreamCallback *outputCallback;
  PaStreamCallback *inputOutputCallback;

  bool muteState;

private:
  bool checkReturn(PaError err) const;

public:
  PortAudio(PaSampleFormat format,
	    int sampleRate = 32000,
	    int framesPerBuffer = 512);
  ~PortAudio();

  // Getters / Setters
  void mute();
  void unmute();
  bool isMute() const;

  void setSampleFormat(PaSampleFormat format);
  PaSampleFormat getSampleFormat() const;

  void setInputCallback(PaStreamCallback *);
  PaStreamCallback *getInputCallback(void);

  void setOutputCallback(PaStreamCallback *);
  PaStreamCallback *getOutputCallback(void);

  void setInputOutputCallback(PaStreamCallback *);
  PaStreamCallback *getInputOutputCallback(void);

  // Device-related methods
  PaDeviceIndex getDefaultInputDevice();
  PaDeviceIndex getDefaultOutputDevice();

  PaDeviceInfo const *getInputDeviceInfo() const;
  PaDeviceInfo const *getOutputDeviceInfo() const;

  // Stream-related methods
  bool startStream() const;
  bool closeStream() const;

  bool isStreamActive() const;

  bool openInputStream(void *callbackArg = NULL);
  bool openOutputStream(void *callbackArg = NULL);
  bool openInputOutputStream(void *callbackArg = NULL);

  // tools
  void sleep(int) const;
};

#endif // PORTAUDIO_HPP_
