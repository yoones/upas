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
#include "include/IAudio.hpp"
#include "include/AudioStreamPortAudio.hpp"
#include "include/AudioCodecSpeex.hpp"
#include "include/AudioManager.hpp"

#define NBR_SECONDS	3

#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif

int main()
{
  Audio::IStream *stream = new Audio::StreamPortAudio();
  Audio::ICodec *codec = new Audio::CodecSpeex();
  Audio::IAudio *audio = new Audio::AudioManager(stream, codec);

  Audio::EncodedFrames encFrames;
  short *data = NULL;
  unsigned long size = 0;

  // record
  audio->startStream(true, false);
  for (int i = NBR_SECONDS; i > 0; i--)
    sleep(1);
  audio->stopStream();

  // read/encode
  audio->read(&data, &size);
  audio->encode(encFrames, data, size);
  delete (data);

  // decode/write
  audio->decode(encFrames, &data, &size);
  audio->write(data, size);
  delete (data);

  // play  
  audio->startStream(false, true);
  while (audio->isStreamActive() == true)
    sleep(1);
  audio->stopStream();

  delete audio;
  delete codec;
  delete stream;

  return (0);
}
