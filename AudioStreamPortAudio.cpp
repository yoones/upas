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

#include <iostream>
#include "include/AudioStreamPortAudio.hpp"

namespace Audio
{
  StreamPortAudio::StreamPortAudio()
  {
    this->pa = new PortAudio(paInt16, 8000);
    if (!this->pa)
      std::cerr << "Error: Out of memory" << std::endl;
    else
      {
	this->pa->unmute();

	this->paBuff.input = new PortAudioBuffer<short>(this->pa);
	this->paBuff.output = new PortAudioBuffer<short>(this->pa);

	this->status = AS_AVAILABLE;
      }
  }

  StreamPortAudio::~StreamPortAudio()
  {
    if (this->pa)
      {
	this->stopStream();
	if (this->paBuff.input)
	  delete (this->paBuff.input);
	if (this->paBuff.output)
	  delete (this->paBuff.output);
	delete (this->pa);
      }
  }

  bool StreamPortAudio::read(short **data, unsigned long *size)
  {
    *size = this->paBuff.input->getNbrSamples();
    *data = this->paBuff.input->extractRawSamples();
    if (*data == NULL)
      return (false);
    return (true);
  }

  bool StreamPortAudio::write(short const *data, unsigned long size)
  {
    this->paBuff.output->recordRawData(data, size);
    return (true);
  }

  bool StreamPortAudio::startRecordStream()
  {
    if (this->status != AS_AVAILABLE)
      return (false);

    this->pa->setInputCallback(StreamPA_RecordCallback<short>);
    if (this->pa->openInputStream(this->paBuff.input) == false
	|| this->pa->startStream() == false)
      return (false);
    this->status = AS_RECORDING;
    return (true);
  }

  bool StreamPortAudio::startPlayStream()
  {
    if (this->status != AS_AVAILABLE)
      return (false);

    this->pa->setOutputCallback(StreamPA_PlayCallback<short>);
    if (this->pa->openOutputStream(this->paBuff.output) == false
	|| this->pa->startStream() == false)
      return (false);
    this->status = AS_PLAYING;
    return (true);
  }

  bool StreamPortAudio::startTwoWaysStream()
  {
    if (this->status != AS_AVAILABLE)
      return (false);

    this->pa->setInputOutputCallback(StreamPA_TwoWaysCallback<short>);
    if (this->pa->openInputOutputStream(&(this->paBuff)) == false
	|| this->pa->startStream() == false)
      return (false);
    this->status = AS_REC_AND_PLAY;
    return (true);
  }

  void StreamPortAudio::sleep(int ms)
  {
    this->pa->sleep(ms);
  }

  bool StreamPortAudio:: stopStream()
  {
    if (this->status == AS_AVAILABLE)
      return (false);
    this->status = AS_AVAILABLE;
    return (this->pa->closeStream());
  }

  bool StreamPortAudio::isStreamActive()
  {
    return (this->pa->isStreamActive());
  }

  e_StreamStatus StreamPortAudio::getStatus() const
  {
    return (this->status);
  }
}
