/*
 * Pixy.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: root
 */

#include "Pixy.h"

void LinkSPI::init()
{
   outLen = 0;
   spi = new SPI(SPI::kMXP);

}
uint8_t LinkSPI::getByte()
{
	uint8_t * ret;
	spi.Read(false, ret, (uint8_t)1);
	return *ret;
}

uint16_t LinkSPI::getWord()
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
int8_t LinkSPI::send(uint8_t *data, uint8_t len)
{
      if (len>PIXY_OUTBUF_SIZE || outLen!=0)
        return -1;
      memcpy(outBuf, data, len);
      outLen = len;
      outIndex = 0;
      return len;
}
uint8_t LinkSPI::transfer(uint8_t data)
{
	spi.Write(&data, 1);
	uint8_t * ret;
	spi.Read(false, ret, 1);
	return *ret;
}
uint16_t LinkSPI::transfer(uint16_t data)
{
	union{uint16_t word; uint8_t * bytes;} input, recieved;
	input.word = data;
	spi.Write(input.bytes, sizeof(uint16_t));
	spi.Read(false, recieved.bytes, sizeof(uint16_t));
	return recieved.word;

}
