/*
 * TPixy.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: root
 */

#include "TPixy.h"


template <class LinkType> TPixy<LinkType>::TPixy(uint8_t addr, void * procCall(Block*))
{
  skipStart = false;
  blockCount = 0;
  blockArraySize = PIXY_INITIAL_ARRAYSIZE;
  blocks = (Block *)malloc(sizeof(Block)*blockArraySize);
  link.setAddress(addr);
  processBlocks = procCall;
}
template <class LinkType> TPixy<LinkType>::TPixy(uint8_t addr, BlockProcessCallbackPtr ptr, RobotBase bot)
{
  skipStart = false;
  blockCount = 0;
  blockArraySize = PIXY_INITIAL_ARRAYSIZE;
  blocks = (Block *)malloc(sizeof(Block)*blockArraySize);
  link.setAddress(addr);
  m_classbased_proc = true;
  m_process = ptr;
  m_bot = bot;
  processBlocks = defaultBlockProcess;
}
template <class LinkType> void TPixy<LinkType>::init()
{
  link.init();
}

template <class LinkType> TPixy<LinkType>::~TPixy()
{
  free(blocks);
}

template <class LinkType> bool TPixy<LinkType>::getStart()
{
  uint16_t w, lastw;

  lastw = 0xffff;

  while(true)
  {
    w = link.getWord();
    if (w==0 && lastw==0)
	{
      Wait(.00000010);
	  return false;
	}
    else if (w==PIXY_START_WORD && lastw==PIXY_START_WORD)
      return true;
	else if (w==PIXY_START_WORDX)
	{
	  link.getByte(); // resync
	}
	lastw = w;
  }
}

template <class LinkType> void TPixy<LinkType>::resize()
{
  Block *newBlocks;
  blockArraySize += PIXY_INITIAL_ARRAYSIZE;
  newBlocks = (Block *)malloc(sizeof(Block)*blockArraySize);
  memcpy(newBlocks, blocks, sizeof(Block)*blockCount);
  free(blocks);
  blocks = newBlocks;
}

template <class LinkType> uint16_t TPixy<LinkType>::getBlocks(uint16_t maxBlocks)
{
  uint8_t i;
  uint16_t w, checksum, sum;
  Block *block;

  if (!skipStart)
  {
    if (getStart()==false)
      return 0;
  }
  else
	skipStart = false;

  for(blockCount=0; blockCount<maxBlocks && blockCount<PIXY_MAXIMUM_ARRAYSIZE;)
  {
    checksum = link.getWord();
    if (checksum==PIXY_START_WORD) // we've reached the beginning of the next frame
    {
      skipStart = true;
	  //Serial.println("skip");
      return blockCount;
    }
    else if (checksum==0)
      return blockCount;

	if (blockCount>blockArraySize)
		resize();

	block = blocks + blockCount;

    for (i=0, sum=0; i<sizeof(Block)/sizeof(uint16_t); i++)
    {
      w = link.getWord();
      sum += w;
      *((uint16_t *)block + i) = w;
    }

    if (checksum==sum)
      blockCount++;
    else
    {
    	//handle error
    }

	w = link.getWord();
    if (w!=PIXY_START_WORD)
      return blockCount;
  }
}
template <class LinkType> void TPixy<LinkType>::update()
{
	if(getBlocks())
	{
		if(!m_classbased_proc)
			processBlocks(blocks);
		else
			(m_bot.*m_process)(blocks);
	}
}
void defaultBlockProcess(Block * blocks)
{
	//Do nothing
}
