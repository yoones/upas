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

#ifndef SPEEX_HPP_
# define SPEEX_HPP_

# include <deque>
# include "speex/speex.h"
# include "IAudioCodec.hpp"

class Speex
{
protected:
  void *encoderState; 
  void *decoderState; 
  int quality;
  int encFrameSize;
  int decFrameSize;

public:
  Speex();
  ~Speex();

  unsigned long getEncFrameSize() const;
  unsigned long getDecFrameSize() const;

  Audio::EncodedFrames encode(short const *frames, int nbrFrames);
  short *decode(Audio::EncodedFrames const encodedFrames, unsigned long *size);
};

#endif // SPEEX_HPP_
