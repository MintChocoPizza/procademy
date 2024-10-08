#ifndef __STUB_H__ 
#define __STUB_H__ 

namespace OreoPizza 
{ 
	class Stub 
	{ 
	private: 	void ProcessRecvMessage(int SrcID, char Packet_Type, SerializeBuffer* p_Message); 

	private: 	void Message_packet_SC_Create_My_Character(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_SC_Create_My_Character(int SrcID, int ID, char Direction, short X, short Y, char HP); 

	private: 	void Message_packet_SC_Create_Other_Character(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_SC_Create_Other_Character(int SrcID, int ID, char Direction, short X, short Y, char HP); 

	private: 	void Message_packet_SC_Delete_Chatacter(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_SC_Delete_Chatacter(int SrcID, int ID); 

	private: 	void Message_packet_CS_Move_Start(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_CS_Move_Start(int SrcID, char Direction, short X, short Y); 

	private: 	void Message_packet_SC_Move_Start(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_SC_Move_Start(int SrcID, int ID, char Direction, short X, short Y); 

	private: 	void Message_packet_CS_Move_Stop(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_CS_Move_Stop(int SrcID, char Direction, short X, short Y); 

	private: 	void Message_packet_SC_Move_Stop(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_SC_Move_Stop(int SrcID, int ID, char Direction, short X, short Y); 

	private: 	void Message_packet_CS_Attack1(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_CS_Attack1(int SrcID, char Direction, short X, short Y); 

	private: 	void Message_pakcet_SC_Attack1(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void pakcet_SC_Attack1(int SrcID, int ID, char Direction, short X, short Y); 

	private: 	void Message_packet_CS_Attack2(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_CS_Attack2(int SrcID, char Direction, short X, short Y); 

	private: 	void Message_pakcet_SC_Attack2(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void pakcet_SC_Attack2(int SrcID, int ID, char Direction, short X, short Y); 

	private: 	void Message_packet_CS_Attack3(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_CS_Attack3(int SrcID, char Direction, short X, short Y); 

	private: 	void Message_pakcet_SC_Attack3(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void pakcet_SC_Attack3(int SrcID, int ID, char Direction, short X, short Y); 

	private: 	void Message_packet_SC_Damage(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_SC_Damage(int SrcID, int AttackID, int DamageID, char DamageHP); 

	private: 	void Message_packet_SC_Sync(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_SC_Sync(int SrcID, int ID, short X, short Y); 

	private: 	void Message_packet_CS_Echo(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_CS_Echo(int SrcID, int Time); 

	private: 	void Message_packet_SC_Echo(int SrcID, SerializeBuffer* p_Message); 
	protected: 	virtual void packet_SC_Echo(int SrcID, int Time); 

	}; 
} 

extern OreoPizza::Stub stub; 
#endif 
