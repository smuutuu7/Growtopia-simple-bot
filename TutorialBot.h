#pragma once
#include <string>
#include <enet/include/enet.h>
#include <proton/variant.hpp>
#include "GameUpdatePacket.h"

struct ClientData {
	std::string name = "";
	std::string pass = "";
	
	std::string ip = "";
	int port = 0;
	
	std::string meta = "";
	std::string rid = "";
	std::string mac = "";
	
	int user = 0;
	int token = 0;
	
	std::string uuid = "";
	std::string doorid = "";
	
	void Set(variantlist_t& var);
	void Reset();
	
	std::string Create();
}


class TutorialBot {
private:
	ENetPeer* peer = NULL;
	ENetHost* host = NULL;
public:
	void SendPacket(int type, std::string const& text);
	void SendPacketRaw(int type, uint8_t* ptr, int flags = 1);
	
	void ConnectClient(std::string IP, int Port);
	void OnConnected();
	void OnDisconnected();
	void OnReceive(ENetPacket* packet);
	
	void OnTextPacket(int type, std::string text);
	void OnTankPacket(int type, uint8_t* ptr, int size);
	
	void Event();
};
extern std::vector<TutorialBot*> bots;
