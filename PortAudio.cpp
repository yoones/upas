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

#include <cstdlib>
#include <iostream>
#include "include/PortAudio.hpp"

bool PortAudio::checkReturn(PaError err) const
{
  if (err != paNoError)
    {
      std::cerr << "Error #" << err << ": " << Pa_GetErrorText(err) << std::endl;
      return (false);
    }
  return (true);
}

PortAudio::PortAudio(PaSampleFormat format, int sampleRate, int framesPerBuffer)
  : stream(NULL),
    sampleFormat(format), sampleRate(sampleRate), framesPerBuffer(framesPerBuffer),
    inputCallback(NULL), outputCallback(NULL), inputOutputCallback(NULL),
    muteState(false)
{
  if (this->checkReturn(Pa_Initialize()) == false)
    std::cerr << "Critical Error: Failed to initialize PortAudio." << std::endl;

  // configure inputParameters
  this->inputParameters.device = this->getDefaultInputDevice();
  if (this->inputParameters.device == paNoDevice)
    std::cerr << "Error: No default input device." << std::endl;
  else
    {
      this->inputParameters.channelCount = NUM_CHANNELS;
      this->inputParameters.sampleFormat = this->sampleFormat;
      this->inputParameters.suggestedLatency =
	this->getInputDeviceInfo()->defaultLowInputLatency;
      this->inputParameters.hostApiSpecificStreamInfo = NULL;
    }

  // configure outputParameters
  this->outputParameters.device = this->getDefaultOutputDevice();
  if (this->outputParameters.device == paNoDevice)
    std::cerr << "Error: No default output device." << std::endl;
  else
    {
      this->outputParameters.channelCount = NUM_CHANNELS;
      this->outputParameters.sampleFormat = this->sampleFormat;
      this->outputParameters.suggestedLatency =
	this->getOutputDeviceInfo()->defaultLowOutputLatency;
      this->outputParameters.hostApiSpecificStreamInfo = NULL;
    }
}

PortAudio::~PortAudio()
{
  Pa_Terminate();
}

// Getters / Setters
void PortAudio::mute()
{
  this->muteState = true;
}

void PortAudio::unmute()
{
  this->muteState = false;
}

bool PortAudio::isMute() const
{
  return (this->muteState);
}

void PortAudio::setSampleFormat(PaSampleFormat format)
{
  this->sampleFormat = format;
}

PaSampleFormat PortAudio::getSampleFormat() const
{
  return (this->sampleFormat);
}

void PortAudio::setInputCallback(PaStreamCallback *ptr)
{
  this->inputCallback = ptr;
}

PaStreamCallback *PortAudio::getInputCallback(void)
{
  return (this->inputCallback);
}

void PortAudio::setOutputCallback(PaStreamCallback *ptr)
{
  this->outputCallback = ptr;
}

PaStreamCallback *PortAudio::getOutputCallback(void)
{
  return (this->outputCallback);
}

void PortAudio::setInputOutputCallback(PaStreamCallback *ptr)
{
  this->inputOutputCallback = ptr;
}

PaStreamCallback *PortAudio::getInputOutputCallback(void)
{
  return (this->inputOutputCallback);
}

// Device-related methods
PaDeviceIndex PortAudio::getDefaultInputDevice()
{
  this->inputParameters.device = Pa_GetDefaultInputDevice();
  return (this->inputParameters.device);
}

PaDeviceIndex PortAudio::getDefaultOutputDevice()
{
  this->outputParameters.device = Pa_GetDefaultOutputDevice();
  return (this->outputParameters.device);
}

PaDeviceInfo const *PortAudio::getInputDeviceInfo() const
{
  return (Pa_GetDeviceInfo(this->inputParameters.device));
}

PaDeviceInfo const *PortAudio::getOutputDeviceInfo() const
{
  return (Pa_GetDeviceInfo(this->outputParameters.device));
}

// Stream-related methods
bool PortAudio::startStream() const
{
  return (this->checkReturn(Pa_StartStream(this->stream)));
}

bool PortAudio::closeStream() const
{
  return (this->checkReturn(Pa_CloseStream(this->stream)));
}

bool PortAudio::isStreamActive() const
{
  PaError err;

  if ((err = Pa_IsStreamActive(this->stream)) == 1)
    return (true);
  this->checkReturn(err);
  return (false);
}

bool PortAudio::openInputStream(void *callbackArg)
{
  if (this->inputParameters.device == paNoDevice)
    return (false);
  return (this->checkReturn(Pa_OpenStream(&(this->stream),
					  &(this->inputParameters),
					  NULL,
					  this->sampleRate,
					  this->framesPerBuffer,
					  paClipOff,
					  this->inputCallback,
					  callbackArg)));
}

bool PortAudio::openOutputStream(void *callbackArg)
{
  if (this->outputParameters.device == paNoDevice)
    return (false);
  return (this->checkReturn(Pa_OpenStream(&(this->stream),
					  NULL,
					  &(this->outputParameters),
					  this->sampleRate,
					  this->framesPerBuffer,
					  paClipOff,
					  this->outputCallback,
					  callbackArg)));
}

bool PortAudio::openInputOutputStream(void *callbackArg)
{
  if (this->inputParameters.device == paNoDevice
      || this->outputParameters.device == paNoDevice)
    return (false);
  return (this->checkReturn(Pa_OpenStream(&(this->stream),
					  &(this->inputParameters),
					  &(this->outputParameters),
					  this->sampleRate,
					  this->framesPerBuffer,
					  paClipOff,
					  this->inputOutputCallback,
					  callbackArg)));
}

// tools
void PortAudio::sleep(int t) const
{
  Pa_Sleep(t);
}
