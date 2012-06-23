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

#ifndef AUDIO_CODEC_SPEEX_HPP_
# define AUDIO_CODEC_SPEEX_HPP_

# include "IAudioCodec.hpp"
# include "Speex.hpp"

namespace Audio
{
  class CodecSpeex
    : public ICodec
  {
  protected:
    Speex s;

  public:
    CodecSpeex();
    ~CodecSpeex();

    virtual unsigned long getEncFrameSize() const;
    virtual unsigned long getDecFrameSize() const;

    virtual EncodedFrames encode(short const *frames, int nbrFrames);
    virtual short *decode(EncodedFrames const encodedFrames,
			  unsigned long *decodedFramesSize);
  };
};

#endif // AUDIO_CODEC_SPEEX_HPP_
