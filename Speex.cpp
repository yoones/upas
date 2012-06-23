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
#include "include/Speex.hpp"

Speex::Speex()
{
  int activate = 1;
  int desactivate = 0;

  this->quality = 6;

  // We get the speex_nb_mode this way
  // for windows compatibility
  const SpeexMode * mode;
  mode = speex_lib_get_mode(SPEEX_MODEID_NB);
  this->encoderState = speex_encoder_init(mode); 
  this->decoderState = speex_decoder_init(mode);

  // encoder configuration
  speex_encoder_ctl(this->encoderState,
		    SPEEX_GET_FRAME_SIZE,
		    &(this->encFrameSize));
  speex_encoder_ctl(this->encoderState,
		    SPEEX_SET_HIGHPASS,
		    &desactivate);
  speex_encoder_ctl(this->encoderState,
		    SPEEX_SET_QUALITY,
		    &(this->quality));

  // decoder configuration
  speex_decoder_ctl(this->decoderState,
		    SPEEX_GET_FRAME_SIZE,
		    &(this->decFrameSize));
  speex_decoder_ctl(this->decoderState,
		    SPEEX_SET_HIGHPASS,
		    &desactivate);
  speex_decoder_ctl(this->decoderState,
		    SPEEX_SET_ENH,
		    &activate);
}

Speex::~Speex()
{
  speex_encoder_destroy(this->encoderState);
}

unsigned long Speex::getEncFrameSize() const
{
  return (this->encFrameSize);
}

unsigned long Speex::getDecFrameSize() const
{
  return (this->decFrameSize);
}

Audio::EncodedFrames Speex::encode(short const *frames, int nbrFrames)
{
  Audio::EncodedFrames encodedFrames;
  SpeexBits bits; 
  int size;

  short *w = (short *)frames;
  speex_bits_init(&bits);
  for (int i = 0; i < nbrFrames; i++)
    {
      speex_bits_reset(&bits);
      speex_encode_int(this->encoderState, w, &bits);
      size = speex_bits_nbytes(&bits);
      Audio::EncodedFrame *frame = new Audio::EncodedFrame;
      frame->size = size;
      frame->data = new char[size];
      speex_bits_write(&bits, frame->data, size);
      encodedFrames.push_back(frame);
      w += this->encFrameSize;
    }
  speex_bits_destroy(&bits);
  return (encodedFrames);
}

short *Speex::decode(Audio::EncodedFrames const encodedFrames,
		     unsigned long *totalSize)
{
  SpeexBits bits;
  short *rawData = NULL;
  short *frame = NULL;

  *totalSize = this->decFrameSize * encodedFrames.size();
  rawData = new short[*totalSize];
  frame = rawData;
  speex_bits_init(&bits);
  for (unsigned long i = 0;
       i < encodedFrames.size();
       i++, frame += this->decFrameSize)
    {
      speex_bits_reset(&bits);
      speex_bits_read_from(&bits,
			   encodedFrames.at(i)->data,
			   encodedFrames.at(i)->size);
      speex_decode_int(this->decoderState, &bits, frame);
    }
  speex_bits_destroy(&bits);
  
  return (rawData);
}  
