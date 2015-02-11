/*
 * Pixy.h
 *
 *  Created on: Feb 8, 2015
 *      Author: root
 */

#ifndef AUTONOMOUS_PIXY_H_
#define AUTONOMOUS_PIXY_H_
#include <SPI.h>
#include <cstring>
#include "TPixy.h"
#define PIXY_SYNC_BYTE              0x5a
#define PIXY_SYNC_BYTE_DATA         0x5b
#define PIXY_OUTBUF_SIZE            6

class LinkSPI
{
  public:
    void init()
    {
       outLen = 0;
    }

    LinkSPI():spi(SPI::kMXP){
    	init();
    }
    uint8_t getByte()
    {
    	uint8_t * ret = new uint8_t;
    	spi.Read(false, ret, (uint8_t)1);
    	return *ret;
    }
    uint16_t getWord()
    {
    	// ordering is different because Pixy is sending 16 bits through SPI
    	      // instead of 2 bytes in a 16-bit word as with I2C
    	      uint16_t w;
    	      uint8_t c, cout = 0;

    	      if (outLen)
    	      {
    	        w = transfer((uint16_t)PIXY_SYNC_BYTE_DATA);
    	        cout = outBuf[outIndex++];
    	        if (outIndex==outLen)
    	          outLen = 0;
    	      }
    	      else
    	        w = transfer((uint16_t)PIXY_SYNC_BYTE);
    	      w <<= 8;
    	      c = transfer(cout);
    	      w |= c;

    	      return w;
    }
    int8_t send(uint8_t *data, uint8_t len)
    {
          if (len>PIXY_OUTBUF_SIZE || outLen!=0)
            return -1;
          memcpy(outBuf, data, len);
          outLen = len;
          outIndex = 0;
          return len;
    }

    void setAddress(uint8_t addr)
    {
    	addr_ = addr;
    }

  private:
    uint8_t outBuf[PIXY_OUTBUF_SIZE];
    uint8_t outLen;
    uint8_t outIndex;
    uint8_t addr_;
    SPI spi;
    uint16_t transfer(uint16_t data)
    {
    	union{uint16_t word; uint8_t * bytes;} input, recieved;
    	input.word = data;
    	spi.Write(input.bytes, sizeof(uint16_t));
    	spi.Read(false, recieved.bytes, sizeof(uint16_t));
    	return recieved.word;

    }
    uint8_t transfer(uint8_t data)
    {
    	spi.Write(&data, 1);
    	uint8_t * ret = new uint8_t;
    	spi.Read(false, ret, 1);
    	return *ret;
    }
};

#define DECL_PIXY_TYPE(cls) typedef TPixy<LinkSPI, cls> Pixy
#endif
