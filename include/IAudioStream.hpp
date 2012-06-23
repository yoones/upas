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

#ifndef I_AUDIO_STREAM_HPP_
# define I_AUDIO_STREAM_HPP_

namespace Audio
{
  enum e_StreamStatus
    {
      AS_AVAILABLE,
      AS_RECORDING,
      AS_PLAYING,
      AS_REC_AND_PLAY
    };

  class IStream
  {
  public:
    virtual ~IStream() {}

    virtual bool read(short **data, unsigned long *size) = 0;
    virtual bool write(short const *data, unsigned long size) = 0;

    virtual bool startRecordStream() = 0;
    virtual bool startPlayStream() = 0;
    virtual bool startTwoWaysStream() = 0;
    virtual bool stopStream() = 0;
    virtual bool isStreamActive() = 0;

    virtual e_StreamStatus getStatus() const = 0;
  };
}

#endif // I_AUDIO_STREAM_HPP_
