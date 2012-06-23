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

#ifndef AUDIO_MANAGER_HPP_
# define AUDIO_MANAGER_HPP_

# include "IAudio.hpp"

namespace Audio
{
  class AudioManager
    : public IAudio
  {
  protected:
    IStream *audioStream;
    ICodec *audioCodec;

  public:
    AudioManager(IStream *as, ICodec *ac);
    ~AudioManager();

    virtual bool startStream(bool record, bool play);
    virtual bool stopStream();
    virtual bool isStreamActive();

    virtual bool read(short **buffer, unsigned long *size);
    virtual bool write(short const *buffer, unsigned long size);

    virtual bool encode(EncodedFrames &encodedFrames,
			short const *buffer, int size);
    virtual bool decode(EncodedFrames const &encodedFrames,
			short **buffer, unsigned long *size);
  };
};

#endif // AUDIO_MANAGER_HPP_
