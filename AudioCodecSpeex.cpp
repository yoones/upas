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

#include "include/AudioCodecSpeex.hpp"

namespace Audio
{
  CodecSpeex::CodecSpeex()
  {
  }

  CodecSpeex::~CodecSpeex()
  {
  }

  unsigned long CodecSpeex::getEncFrameSize() const
  {
    return (this->s.getEncFrameSize());
  }

  unsigned long CodecSpeex::getDecFrameSize() const
  {
    return (this->s.getDecFrameSize());
  }

  EncodedFrames CodecSpeex::encode(short const *frames, int nbrFrames)
  {
    return (this->s.encode(frames, nbrFrames));
  }

  short *CodecSpeex::decode(EncodedFrames const encodedFrames,
			    unsigned long *totalSize)
  {
    return (this->s.decode(encodedFrames, totalSize));
  }
}
