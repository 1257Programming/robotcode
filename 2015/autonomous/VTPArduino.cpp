/*
 * VTPArduino.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: Team1257
 */

#include <autonomous/VTPArduino.h>

VTPArduino::VTPArduino() :
m_serial(9600,SerialPort::kUSB)
{
	// TODO Auto-generated constructor stub

}
vtp_slave_datagram VTPArduino::getData()
{
	char buffer[sizeof(vtp_slave_datagram)];
	m_serial.Read(buffer, sizeof(vtp_slave_datagram));
	vtp_slave_datagram *gram;
	memcpy((void*)gram, buffer, sizeof(vtp_slave_datagram));
	if(gram->vs_magic != 413) return NULL;
	return *gram;
}
void VTPArduino::Enable()
{
	m_enabled = true;
}
void VTPArduino::Disable()
{
	m_enabled = false;
}
bool VTPArduino::IsEnabled()
{
	return m_enabled;
}
void VTPArduino::sendData()
{
	char buffer[sizeof(vtp_master_datagram)];
	vtp_master_datagram *gram;
	gram->vm_magic = 437;
	gram->vm_enable = m_enabled;
	memcpy(buffer, (void*)gram, sizeof(vtp_master_datagram));
	m_serial.Write(buffer, sizeof(vtp_master_datagram));
}
VTPArduino::~VTPArduino() {
	// TODO Auto-generated destructor stub
}

