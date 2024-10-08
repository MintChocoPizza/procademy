#include "SerializeBuffer.h" 
#include "Protocol.h" 
#include "Stub.h" 
OreoPizza::Stub stub; 

void OreoPizza::Stub::ProcessRecvMessage(int SrcID, char Packet_Type, SerializeBuffer* p_Message) 
{ 
	switch(Packet_Type) 
	{ 
	case dfPACKET_SC_CREATE_MY_CHARACTER: 
		Message_packet_SC_Create_My_Character(SrcID, p_Message); 
		break; 
	case dfPACKET_SC_CREATE_OTHER_CHARACTER: 
		Message_packet_SC_Create_Other_Character(SrcID, p_Message); 
		break; 
	case dfPACKET_SC_DELETE_CHATACTER: 
		Message_packet_SC_Delete_Chatacter(SrcID, p_Message); 
		break; 
	case dfPACKET_CS_MOVE_START: 
		Message_packet_CS_Move_Start(SrcID, p_Message); 
		break; 
	case dfPACKET_SC_MOVE_START: 
		Message_packet_SC_Move_Start(SrcID, p_Message); 
		break; 
	case dfPACKET_CS_MOVE_STOP: 
		Message_packet_CS_Move_Stop(SrcID, p_Message); 
		break; 
	case dfPACKET_SC_MOVE_STOP: 
		Message_packet_SC_Move_Stop(SrcID, p_Message); 
		break; 
	case dfPACKET_CS_ATTACK1: 
		Message_packet_CS_Attack1(SrcID, p_Message); 
		break; 
	case dfPAKCET_SC_ATTACK1: 
		Message_pakcet_SC_Attack1(SrcID, p_Message); 
		break; 
	case dfPACKET_CS_ATTACK2: 
		Message_packet_CS_Attack2(SrcID, p_Message); 
		break; 
	case dfPAKCET_SC_ATTACK2: 
		Message_pakcet_SC_Attack2(SrcID, p_Message); 
		break; 
	case dfPACKET_CS_ATTACK3: 
		Message_packet_CS_Attack3(SrcID, p_Message); 
		break; 
	case dfPAKCET_SC_ATTACK3: 
		Message_pakcet_SC_Attack3(SrcID, p_Message); 
		break; 
	case dfPACKET_SC_DAMAGE: 
		Message_packet_SC_Damage(SrcID, p_Message); 
		break; 
	case dfPACKET_SC_SYNC: 
		Message_packet_SC_Sync(SrcID, p_Message); 
		break; 
	case dfPACKET_CS_ECHO: 
		Message_packet_CS_Echo(SrcID, p_Message); 
		break; 
	case dfPACKET_SC_ECHO: 
		Message_packet_SC_Echo(SrcID, p_Message); 
		break; 
	default: 
		break; 
	} 
} 

void OreoPizza::Stub::Message_packet_SC_Create_My_Character(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 
	char Direction; 
	short X; 
	short Y; 
	char HP; 

	*p_Message >> ID >> Direction >> X >> Y >> HP; 

	packet_SC_Create_My_Character(SrcID, ID, Direction, X, Y, HP); 
} 

void OreoPizza::Stub::packet_SC_Create_My_Character(int SrcID, int ID, char Direction, short X, short Y, char HP) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_SC_Create_Other_Character(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 
	char Direction; 
	short X; 
	short Y; 
	char HP; 

	*p_Message >> ID >> Direction >> X >> Y >> HP; 

	packet_SC_Create_Other_Character(SrcID, ID, Direction, X, Y, HP); 
} 

void OreoPizza::Stub::packet_SC_Create_Other_Character(int SrcID, int ID, char Direction, short X, short Y, char HP) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_SC_Delete_Chatacter(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 

	*p_Message >> ID; 

	packet_SC_Delete_Chatacter(SrcID, ID); 
} 

void OreoPizza::Stub::packet_SC_Delete_Chatacter(int SrcID, int ID) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_CS_Move_Start(int SrcID, SerializeBuffer* p_Message) 
{ 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> Direction >> X >> Y; 

	packet_CS_Move_Start(SrcID, Direction, X, Y); 
} 

void OreoPizza::Stub::packet_CS_Move_Start(int SrcID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_SC_Move_Start(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> ID >> Direction >> X >> Y; 

	packet_SC_Move_Start(SrcID, ID, Direction, X, Y); 
} 

void OreoPizza::Stub::packet_SC_Move_Start(int SrcID, int ID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_CS_Move_Stop(int SrcID, SerializeBuffer* p_Message) 
{ 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> Direction >> X >> Y; 

	packet_CS_Move_Stop(SrcID, Direction, X, Y); 
} 

void OreoPizza::Stub::packet_CS_Move_Stop(int SrcID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_SC_Move_Stop(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> ID >> Direction >> X >> Y; 

	packet_SC_Move_Stop(SrcID, ID, Direction, X, Y); 
} 

void OreoPizza::Stub::packet_SC_Move_Stop(int SrcID, int ID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_CS_Attack1(int SrcID, SerializeBuffer* p_Message) 
{ 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> Direction >> X >> Y; 

	packet_CS_Attack1(SrcID, Direction, X, Y); 
} 

void OreoPizza::Stub::packet_CS_Attack1(int SrcID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_pakcet_SC_Attack1(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> ID >> Direction >> X >> Y; 

	pakcet_SC_Attack1(SrcID, ID, Direction, X, Y); 
} 

void OreoPizza::Stub::pakcet_SC_Attack1(int SrcID, int ID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_CS_Attack2(int SrcID, SerializeBuffer* p_Message) 
{ 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> Direction >> X >> Y; 

	packet_CS_Attack2(SrcID, Direction, X, Y); 
} 

void OreoPizza::Stub::packet_CS_Attack2(int SrcID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_pakcet_SC_Attack2(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> ID >> Direction >> X >> Y; 

	pakcet_SC_Attack2(SrcID, ID, Direction, X, Y); 
} 

void OreoPizza::Stub::pakcet_SC_Attack2(int SrcID, int ID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_CS_Attack3(int SrcID, SerializeBuffer* p_Message) 
{ 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> Direction >> X >> Y; 

	packet_CS_Attack3(SrcID, Direction, X, Y); 
} 

void OreoPizza::Stub::packet_CS_Attack3(int SrcID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_pakcet_SC_Attack3(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 
	char Direction; 
	short X; 
	short Y; 

	*p_Message >> ID >> Direction >> X >> Y; 

	pakcet_SC_Attack3(SrcID, ID, Direction, X, Y); 
} 

void OreoPizza::Stub::pakcet_SC_Attack3(int SrcID, int ID, char Direction, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_SC_Damage(int SrcID, SerializeBuffer* p_Message) 
{ 
	int AttackID; 
	int DamageID; 
	char DamageHP; 

	*p_Message >> AttackID >> DamageID >> DamageHP; 

	packet_SC_Damage(SrcID, AttackID, DamageID, DamageHP); 
} 

void OreoPizza::Stub::packet_SC_Damage(int SrcID, int AttackID, int DamageID, char DamageHP) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_SC_Sync(int SrcID, SerializeBuffer* p_Message) 
{ 
	int ID; 
	short X; 
	short Y; 

	*p_Message >> ID >> X >> Y; 

	packet_SC_Sync(SrcID, ID, X, Y); 
} 

void OreoPizza::Stub::packet_SC_Sync(int SrcID, int ID, short X, short Y) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_CS_Echo(int SrcID, SerializeBuffer* p_Message) 
{ 
	int Time; 

	*p_Message >> Time; 

	packet_CS_Echo(SrcID, Time); 
} 

void OreoPizza::Stub::packet_CS_Echo(int SrcID, int Time) 
{ 
	 return ; 
} 

void OreoPizza::Stub::Message_packet_SC_Echo(int SrcID, SerializeBuffer* p_Message) 
{ 
	int Time; 

	*p_Message >> Time; 

	packet_SC_Echo(SrcID, Time); 
} 

void OreoPizza::Stub::packet_SC_Echo(int SrcID, int Time) 
{ 
	 return ; 
} 

