/*
 * VTPArduino.h
 *
 *  Created on: Feb 3, 2015
 *      Author: Team1257
 */

#ifndef SRC_AUTONOMOUS_VTPARDUINO_H_
#define SRC_AUTONOMOUS_VTPARDUINO_H_
#include <SerialPort.h>
typedef struct{
  unsigned short vs_magic;
  unsigned char vs_dat_x;
  unsigned char vs_dat_y;
}vtp_slave_datagram;
typedef struct{
  unsigned short vm_magic;
  unsigned char vm_enable;
}vtp_master_datagram;
class VTPArduino {
private:
	SerialPort m_serial;
	bool m_enabled = false;
public:
	VTPArduino();
	vtp_slave_datagram getData();
	void Enable();
	void Disable();
	bool IsEnabled();
	void sendData();
	virtual ~VTPArduino();
};

#endif /* SRC_AUTONOMOUS_VTPARDUINO_H_ */
