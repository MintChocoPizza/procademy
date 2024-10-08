#include "Protocol.h" 
#include "SerializeBuffer.h" 
#include "proxy.h" 

#pragma warning( disable : 4309 ) 
#pragma warning( disable : 4267 )

void ForwardDecl(int DestID, SerializeBuffer *sb);
OreoPizza::Proxy proxy; 

void OreoPizza::Proxy::packet_SC_Create_My_Character(int DestID, int ID, char Direction, short X, short Y, char HP) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID << Direction << X << Y << HP; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_SC_CREATE_MY_CHARACTER; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_SC_Create_Other_Character(int DestID, int ID, char Direction, short X, short Y, char HP) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID << Direction << X << Y << HP; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_SC_CREATE_OTHER_CHARACTER; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_SC_Delete_Chatacter(int DestID, int ID) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_SC_DELETE_CHATACTER; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_CS_Move_Start(int DestID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_CS_MOVE_START; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_SC_Move_Start(int DestID, int ID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_SC_MOVE_START; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_CS_Move_Stop(int DestID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_CS_MOVE_STOP; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_SC_Move_Stop(int DestID, int ID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_SC_MOVE_STOP; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_CS_Attack1(int DestID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_CS_ATTACK1; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::pakcet_SC_Attack1(int DestID, int ID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPAKCET_SC_ATTACK1; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_CS_Attack2(int DestID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_CS_ATTACK2; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::pakcet_SC_Attack2(int DestID, int ID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPAKCET_SC_ATTACK2; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_CS_Attack3(int DestID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_CS_ATTACK3; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::pakcet_SC_Attack3(int DestID, int ID, char Direction, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID << Direction << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPAKCET_SC_ATTACK3; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_SC_Damage(int DestID, int AttackID, int DamageID, char DamageHP) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << AttackID << DamageID << DamageHP; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_SC_DAMAGE; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_SC_Sync(int DestID, int ID, short X, short Y) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << ID << X << Y; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_SC_SYNC; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_CS_Echo(int DestID, int Time) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << Time; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_CS_ECHO; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

void OreoPizza::Proxy::packet_SC_Echo(int DestID, int Time) 
{ 
	SerializeBuffer sb; 
	st_PACKET_HEADER header; 

	sb.PutData((char *)&header, sizeof(st_PACKET_HEADER)); 
	sb << Time; 

	header.byCode = dfPACKET_CODE; 
	header.byType = dfPACKET_SC_ECHO; 
	header.bySize = sb.GetDataSize() - sizeof(st_PACKET_HEADER); 

	sb.ReWrite(); 
	sb.PutData((char*)&header, sizeof(st_PACKET_HEADER)); 
	sb.ReturnPos(); 

 	ForwardDecl(DestID, &sb); 

} 

