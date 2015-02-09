/*
 * TPixy.h
 *
 *  Created on: Feb 8, 2015
 *      Author: root
 */

#ifndef AUTONOMOUS_TPIXY_H_
#define AUTONOMOUS_TPIXY_H_
#define PIXY_INITIAL_ARRAYSIZE      30
#define PIXY_MAXIMUM_ARRAYSIZE      130
#define PIXY_START_WORD             0xaa55
#define PIXY_START_WORDX            0x55aa
#define PIXY_DEFAULT_ADDR           0x54  // I2C
#include <Timer.h>
#include <string.h>
#include <cstdlib>
struct Block
{
  uint16_t signature;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
};


void defaultBlockProcess(Block * blocks)
{
	//Do nothing
}
template <class LinkType, class RobotType> class TPixy {
public:
	typedef void (RobotType::*BlockProcessCallbackPtr)(Block*);
  TPixy(uint8_t addr=PIXY_DEFAULT_ADDR, void* procCall(Block*) = defaultBlockProcess)
	{
	  skipStart = false;
	  blockCount = 0;
	  blockArraySize = PIXY_INITIAL_ARRAYSIZE;
	  blocks = (Block *)malloc(sizeof(Block)*blockArraySize);
	  link.setAddress(addr);
	  processBlocks = procCall;
	  m_process = 0;
	  m_bot = 0;
	}
  TPixy(void (RobotType::*ptr)(Block*), RobotType* bot, uint8_t addr=PIXY_DEFAULT_ADDR)
  {
    skipStart = false;
    blockCount = 0;
    blockArraySize = PIXY_INITIAL_ARRAYSIZE;
    blocks = (Block *)malloc(sizeof(Block)*blockArraySize);
    link.setAddress(addr);
    m_classbased_proc = true;
    m_process = ptr;
    m_bot = bot;
  }
  ~TPixy()
  {
    free(blocks);
  }

  void processBlocks(Block *);
  uint16_t getBlocks(uint16_t maxBlocks=1000)
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
  void init()
  {
      link.init();
    }
  void update()
  {
  	if(getBlocks())
  	{
  		if(!m_classbased_proc)
  			{

  			}
  		else
  			(m_bot->*m_process)(blocks);
  	}
  }
  Block *blocks;

private:
  bool getStart()
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
  void resize()
  {
    Block *newBlocks;
    blockArraySize += PIXY_INITIAL_ARRAYSIZE;
    newBlocks = (Block *)malloc(sizeof(Block)*blockArraySize);
    memcpy(newBlocks, blocks, sizeof(Block)*blockCount);
    free(blocks);
    blocks = newBlocks;
  }
  bool m_classbased_proc = false;
  RobotType *m_bot;
  BlockProcessCallbackPtr m_process;
  LinkType link;
  bool  skipStart;
  uint16_t blockCount;
  uint16_t blockArraySize;
};

#endif /* AUTONOMOUS_TPIXY_H_ */
