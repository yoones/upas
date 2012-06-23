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

#ifndef I_AUDIO_CODEC_HPP_
# define I_AUDIO_CODEC_HPP_

# include <deque>

namespace Audio
{
  template <typename T>
  struct CodecFrame
  {
    unsigned long size;
    T *data;
  };

  typedef CodecFrame<char> EncodedFrame;
  typedef CodecFrame<short> DecodedFrame;
  typedef std::deque<EncodedFrame *> EncodedFrames;
  typedef std::deque<DecodedFrame *> DecodedFrames;

  class ICodec
  {
  public:
    virtual ~ICodec() {}

    virtual unsigned long getEncFrameSize() const = 0;
    virtual unsigned long getDecFrameSize() const = 0;

    virtual EncodedFrames encode(short const *frames, int nbrFrames) = 0;
    virtual short *decode(EncodedFrames const encodedFrames,
			  unsigned long *decodedFramesSize) = 0;
  };
}

#endif // I_AUDIO_CODEC_HPP_
