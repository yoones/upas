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

#ifndef AUDIO_STREAM_PA_HPP_
# define AUDIO_STREAM_PA_HPP_

# include "IAudioStream.hpp"
# include "PortAudio.hpp"
# include "PortAudioBuffer.hpp"

namespace Audio
{
  class StreamPortAudio
    : public IStream
  {
  protected:
    PortAudio *pa;
    Pa2Buff<short> paBuff;
    e_StreamStatus status;

  public:
    StreamPortAudio();
    ~StreamPortAudio();

    virtual bool read(short **data, unsigned long *size);
    virtual bool write(short const *data, unsigned long size);

    virtual bool startRecordStream();
    virtual bool startPlayStream();
    virtual bool startTwoWaysStream();

    virtual void sleep(int ms);
    virtual bool stopStream();

    virtual bool isStreamActive();
    virtual  e_StreamStatus getStatus() const;
  };

  // Internal Callbacks
  // (using PortAudioBuffer)
  template <typename SAMPLE>
  int StreamPA_RecordCallback(const void *inputBuffer, void *outputBuffer,
			      unsigned long framesPerBuffer,
			      const PaStreamCallbackTimeInfo* timeInfo,
			      PaStreamCallbackFlags statusFlags,
			      void *userData)
  {
    PortAudioBuffer<SAMPLE> *buffer = (PortAudioBuffer<SAMPLE> *)userData;

    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;

    return (buffer->recordSample(inputBuffer, framesPerBuffer));
  }

  template <typename SAMPLE>
  int StreamPA_PlayCallback(const void *inputBuffer, void *outputBuffer,
			    unsigned long framesPerBuffer,
			    const PaStreamCallbackTimeInfo* timeInfo,
			    PaStreamCallbackFlags statusFlags,
			    void *userData)
  {
    PortAudioBuffer<SAMPLE> *buffer = (PortAudioBuffer<SAMPLE> *)userData;

    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;

    return (buffer->playSample(outputBuffer, framesPerBuffer));
  }

  template <typename SAMPLE>
  int StreamPA_TwoWaysCallback(const void *inputBuffer, void *outputBuffer,
			       unsigned long framesPerBuffer,
			       const PaStreamCallbackTimeInfo* timeInfo,
			       PaStreamCallbackFlags statusFlags,
			       void *userData)
  {
    Pa2Buff<SAMPLE> *buffer = (Pa2Buff<SAMPLE> *)userData;

    (void) timeInfo;
    (void) statusFlags;

    buffer->input->recordSample(inputBuffer, framesPerBuffer);
    buffer->output->playSample(outputBuffer, framesPerBuffer);

    return (paContinue);
  }
};

#endif // AUDIO_STREAM_PA_HPP_
