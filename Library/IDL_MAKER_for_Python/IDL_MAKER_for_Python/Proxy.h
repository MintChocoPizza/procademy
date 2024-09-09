#ifndef __PROXY_H__ 
#define __PROXY_H__ 

namespace OreoPizza 
{ 
	class Proxy 
	{ 
		public: 		void packet_SC_Create_My_Character(int destID, int ID, char Direction, short X, short Y, char HP); 
		public: 		void packet_SC_Create_Other_Character(int destID, int ID, char Direction, short X, short Y, char HP); 
		public: 		void packet_SC_Delete_Chatacter(int destID, int ID); 
		public: 		void packet_CS_Move_Start(int destID, char Direction, short X, short Y); 
		public: 		void packet_SC_Move_Start(int destID, int ID, char Direction, short X, short Y); 
		public: 		void packet_CS_Move_Stop(int destID, char Direction, short X, short Y); 
		public: 		void packet_SC_Move_Stop(int destID, int ID, char Direction, short X, short Y); 
		public: 		void packet_CS_Attack1(int destID, char Direction, short X, short Y); 
		public: 		void pakcet_SC_Attack1(int destID, int ID, char Direction, short X, short Y); 
		public: 		void packet_CS_Attack2(int destID, char Direction, short X, short Y); 
		public: 		void pakcet_SC_Attack2(int destID, int ID, char Direction, short X, short Y); 
		public: 		void packet_CS_Attack3(int destID, char Direction, short X, short Y); 
		public: 		void pakcet_SC_Attack3(int destID, int ID, char Direction, short X, short Y); 
		public: 		void packet_SC_Damage(int destID, int AttackID, int DamageID, char DamageHP); 
		public: 		void packet_SC_Sync(int destID, int ID, short X, short Y); 
		public: 		void packet_CS_Echo(int destID, int Time); 
		public: 		void packet_SC_Echo(int destID, int Time); 
	}; 
} 

extern OreoPizza::Proxy proxy; 

#endif