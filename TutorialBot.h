#pragma once
#include <string>
#include <enet/include/enet.h>
#include <proton/variant.hpp>
#include "GameUpdatePacket.h"

class TutorialBot {
public:

	void SendPacket(int type, std::string packet, ENetPeer* peer);
	void SendPacket2(int32_t type, std::string packet, ENetPeer* peer, ENetHost* host);
	void SendVarList(variantlist_t& varlist, ENetPeer* peer, ENetHost* host);
	void SendPacketRaw(int a1, void* PacketData, size_t PacketDataSize, void* a4, ENetPeer* peer, int PacketFlag);
	void SendPacketRaw2(int32_t type, gameupdatepacket_t* updatepacket, ENetPeer* peer, ENetHost* host);
	PBYTE GetExtended(gameupdatepacket_t* packet);
	int8_t GetPacketType(ENetPacket* packet);
	std::string GetString(ENetPacket* packet);
	gameupdatepacket_t* GetStruct(ENetPacket* packet);
};
