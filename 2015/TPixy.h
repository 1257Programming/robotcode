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
#include <RobotBase.h>
struct Block
{
  uint16_t signature;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
};
typedef void (RobotBase::*BlockProcessCallbackPtr)(Block*);
#define GET_CALL_PTR(clss, func) static_cast<BlockProcessCallbackPtr>(&clss::func)
void defaultBlockProcess(Block*);
template <class LinkType> class TPixy {
public:
  TPixy(uint8_t addr=PIXY_DEFAULT_ADDR, void* procCall(Block*) = defaultBlockProcess);
  TPixy(BlockProcessCallbackPtr, RobotBase, uint8_t addr=PIXY_DEFAULT_ADDR);
  ~TPixy();
  void processBlocks(Block *);
  uint16_t getBlocks(uint16_t maxBlocks=1000);
  void init();
  void update();
  Block *blocks;

private:
  bool getStart();
  void resize();
  bool m_classbased_proc = false;
  RobotBase m_bot;
  BlockProcessCallbackPtr m_process;
  LinkType link;
  bool  skipStart;
  uint16_t blockCount;
  uint16_t blockArraySize;
};

#endif /* AUTONOMOUS_TPIXY_H_ */
