#include "proxy.h" 
#include "SerializeBuffer.h" 

bool OreoPizza::Proxy::SerializeBuffer_netPacketProc_SC_CREATE_MY_CHARACTER(char* ID, DEST X, int Y) 
{ 
	SerializeBuffer sb << ID << X << Y;
} 

bool OreoPizza::Proxy::SerializeBuffer_netPacketProc_CS_ATTACK1(st_SESSION* p_Session, SerializeBuffer* clpPacket) 
{ 
	SerializeBuffer sb << p_Session << clpPacket;
} 

