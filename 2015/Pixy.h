/*
 * Pixy.h
 *
 *  Created on: Feb 8, 2015
 *      Author: root
 */

#ifndef AUTONOMOUS_PIXY_H_
#define AUTONOMOUS_PIXY_H_
#include "TPixy.h"
#include </home/nathan/wpilib/cpp/current/include/SPI.h>


#define PIXY_SYNC_BYTE              0x5a
#define PIXY_SYNC_BYTE_DATA         0x5b
#define PIXY_OUTBUF_SIZE            6

class LinkSPI
{
  public:
    void init();

    uint16_t getWord();

    uint8_t getByte();

    int8_t send(uint8_t *data, uint8_t len);

    void setAddress(uint8_t addr);

  private:
    uint8_t outBuf[PIXY_OUTBUF_SIZE];
    uint8_t outLen;
    uint8_t outIndex;
    uint8_t addr_;
    SPI spi;
    uint16_t transfer(uint16_t);
    uint8_t transfer(uint8_t);
};


typedef TPixy<LinkSPI> Pixy;
