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

#include "include/AudioManager.hpp"

namespace Audio
{
  AudioManager::AudioManager(IStream *as, ICodec *ac)
    : audioStream(as), audioCodec(ac)
  {
  }

  AudioManager::~AudioManager()
  {
  }

  bool AudioManager::startStream(bool record, bool play)
  {
    if (this->audioStream->getStatus() != AS_AVAILABLE
	|| (record == false && play == false))
      return (false);

    bool ret = false;
    if (record == true && play == true)
      ret = this->audioStream->startTwoWaysStream();
    else if (record == true)
      ret = this->audioStream->startRecordStream();
    else
      ret = this->audioStream->startPlayStream();
    return (ret);
  }

  bool AudioManager::stopStream()
  {
    return (this->audioStream->stopStream());
  }

  bool AudioManager::isStreamActive()
  {
    return (this->audioStream->isStreamActive());
  }

  bool AudioManager::read(short **buffer, unsigned long *size)
  {
    return (this->audioStream->read(buffer, size));
  }

  bool AudioManager::write(short const *buffer, unsigned long size)
  {
    return (this->audioStream->write(buffer, size));
  }

  bool AudioManager::encode(EncodedFrames &encodedFrames,
			    short const *buffer, int size)
  {
    unsigned int nbrFrames = (size / this->audioCodec->getEncFrameSize());

    if (!buffer || nbrFrames == 0)
      return (false);
    encodedFrames = this->audioCodec->encode(buffer, nbrFrames);
    return (true);
  }

  bool AudioManager::decode(EncodedFrames const &encodedFrames,
			    short **buffer, unsigned long *size)
  {
    *buffer = this->audioCodec->decode(encodedFrames, size);
    if (!(*buffer))
      return (false);
    return (true);
  }
}
