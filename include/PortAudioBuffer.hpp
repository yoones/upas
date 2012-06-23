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

#ifndef PORTAUDIO_BUFFER_HPP_
# define PORTAUDIO_BUFFER_HPP_

# include <deque>
# include "PortAudio.hpp"

template <typename SAMPLE>
class PortAudioBuffer
{
protected:
  PortAudio *paInstance;
  unsigned long chunkSize;
  SAMPLE silenceSample;
  std::deque< SAMPLE * > recordedSamples;
  unsigned int recordIdx;
  unsigned int playIdx;

protected:
  bool addChunkBack()
  {
    SAMPLE *s = new SAMPLE[this->chunkSize];
    if (!s)
      {
	std::cerr << "Error: Out of memory" << std::endl;
	return (false);
      }
    this->recordedSamples.push_back(s);
    this->recordIdx = 0;
    return (true);
  }

  bool delChunkFront()
  {
    if (this->recordedSamples.size() > 0)
      {
	if (this->recordedSamples.front())
	  delete (this->recordedSamples.front());
	this->recordedSamples.pop_front();
	return (true);
      }
    return (false);
  }

  void delPlayedChunk()
  {
    if (this->playIdx == this->chunkSize)
      {
	this->playIdx = 0;
	this->delChunkFront();
      }
  }

public:

  PortAudioBuffer(PortAudio *pa, unsigned long chunkSize = 1024)
    : paInstance(pa),
      chunkSize(chunkSize),
      silenceSample((SAMPLE)0),
      recordIdx(0), playIdx(0)
  {}

  ~PortAudioBuffer()
  {
    this->clearBuffer();
  }

  unsigned long getNbrSamples() const
  {
    if (this->recordedSamples.size() == 0)
      return (0);

    unsigned long nbr = 0;
    nbr = (this->recordedSamples.size() - 1) * this->chunkSize;
    nbr += this->recordIdx;
    nbr -= this->playIdx;
    return (nbr);
  }

  void clearBuffer()
  {
    while (this->recordedSamples.size() > 0)
      this->delChunkFront();
    this->recordIdx = 0;
    this->playIdx = 0;
  }

  bool setRecordedSamples(SAMPLE *samples, unsigned long total)
  {
    if (total == 0)
      return (false);

    this->clearBuffer();

    SAMPLE *w = samples;
    unsigned long i = 0;
    unsigned long nbrChunks = total / this->chunkSize;

    if (nbrChunks == 0)
      nbrChunks = 1;
    for (unsigned long c = 0; c < nbrChunks; c++)
      {
	this->addChunkBack();
	for (unsigned long j = 0; j < this->chunkSize && i < total; j++, w++, i++)
	  this->recordedSamples.back()[j] = *w;
      }
    this->playIdx = 0;
    this->recordIdx = total % this->chunkSize;
    return (true);
  }

  SAMPLE *extractRawSamples(void)
  {
    unsigned long nbrSamples = this->getNbrSamples();
    SAMPLE *rawSamples = new SAMPLE[nbrSamples];
    SAMPLE *w = rawSamples;

    if (!rawSamples)
      {
	std::cerr << "Error: Out of memory" << std::endl;
	return (NULL);
      }

    unsigned long i = this->playIdx;
    for (unsigned long c = 0; c < this->recordedSamples.size(); c++)
      {
	SAMPLE *chunk = this->recordedSamples.at(c);
  	for (; i < this->chunkSize && nbrSamples > 0; i++, w++, nbrSamples--)
  	  *w = chunk[i];
	i = 0;
      }
    this->clearBuffer();
    return (rawSamples);
  }

  void recordRawData(const void *inputBuffer,
		     unsigned long framesPerBuffer)
  {
    const SAMPLE *w = (const SAMPLE *)inputBuffer;
    unsigned long i = 0;

    if (this->recordedSamples.size() == 0)
      this->addChunkBack();
    for (i = 0; i < framesPerBuffer; i++, w++)
      {
	if (this->recordIdx == this->chunkSize)
	  {
	    this->addChunkBack();
	    this->recordIdx = 0;
	  }
	this->recordedSamples.back()[this->recordIdx] = *w;
	this->recordIdx++;
      }
  }

  int recordSample(const void *inputBuffer,
		   unsigned long framesPerBuffer)
  {
    const SAMPLE *w = (const SAMPLE *)inputBuffer;
    unsigned long i = 0;

    if (this->recordedSamples.size() == 0)
      this->addChunkBack();

    framesPerBuffer *= NUM_CHANNELS;
    if (w == NULL || this->paInstance->isMute() == true)
      {
	for (i = 0; i < framesPerBuffer; i++, w++)
	  {
	    if (this->recordIdx == this->chunkSize)
	      {
		this->addChunkBack();
		this->recordIdx = 0;
	      }
	    this->recordedSamples.back()[this->recordIdx] = this->silenceSample;
	    this->recordIdx++;
	  }
      }
    else
      {
	for (i = 0; i < framesPerBuffer; i++, w++)
	  {
	    if (this->recordIdx == this->chunkSize)
	      {
		this->addChunkBack();
		this->recordIdx = 0;
	      }
	    this->recordedSamples.back()[this->recordIdx] = *w;
	    this->recordIdx++;
	  }
      }
    return (paContinue);
  }

  int playSample(void *outputBuffer,
		 unsigned long framesPerBuffer)
  {
    SAMPLE *w = (SAMPLE *)outputBuffer;
    unsigned int i = 0;
    unsigned int framesLeft = 0;

    framesLeft = this->getNbrSamples();
    framesPerBuffer *= NUM_CHANNELS;
    if (this->paInstance->isMute() == false)
      {
	for (i = 0; i < framesPerBuffer && framesLeft > 0; i++, w++, framesLeft--)
	  {
	    this->delPlayedChunk();
	    *w = this->recordedSamples.front()[this->playIdx];
	    this->playIdx++;
	  }
      }
    for (; i < framesPerBuffer; i++, w++)
      *w = this->silenceSample;
    this->delPlayedChunk();
    if (framesLeft == 0)
      {
	this->clearBuffer();
	return (paComplete);
      }
    else
      return (paContinue);
  }
};

template <typename T>
struct Pa2Buff
{
  PortAudioBuffer<T> *input;
  PortAudioBuffer<T> *output;
};

#endif // PORTAUDIO_BUFFER_HPP_
