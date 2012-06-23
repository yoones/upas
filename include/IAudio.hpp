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

#ifndef I_AUDIO_HPP_
# define I_AUDIO_HPP_

# include "IAudioStream.hpp"
# include "IAudioCodec.hpp"

namespace Audio
{
  class IAudio
  {
  public:
    virtual ~IAudio() {}

    virtual bool startStream(bool record, bool play) = 0;
    virtual bool stopStream() = 0;
    virtual bool isStreamActive() = 0;

    virtual bool read(short **buffer, unsigned long *size) = 0;
    virtual bool write(short const *buffer, unsigned long size) = 0;

    virtual bool encode(EncodedFrames &encodedFrames,
			short const *buffer, int size) = 0;
    virtual bool decode(EncodedFrames const &encodedFrames,
			short **buffer, unsigned long *size) = 0;
  };
}

#endif // I_AUDIO_HPP_
