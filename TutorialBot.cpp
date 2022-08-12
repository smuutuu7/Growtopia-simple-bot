#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <windows.h>

#include "Struct.h"
#include "Utils.h"
#include "TutorialBot.h"


void ClientData::Reset() {
	ip = "";
	port = 0;
	
	lmode = 0;
	
	meta = "";
	
	user = 0;
	token = 0;
	
	uuid = "";
	doorid = "";
}

void ClientData::Set(variantlist_t& var) {
	port = var[1].get_int32();
	
	if (var[2].get_int32() != -1)
		token = var[2].get_int32();
	
	user = var[3].get_int32();
	
	auto split = Utils::Split(var[4].get_string(), "|");
	if (split.size() == 3) {
		ip = split[0];
		doorid = split[1];
		if (split[2] != "-1")
			uuid = split[2];
	}
	
	lmode = var[5].get_int32();
}


std::string ClientData::Create() {
	std::stringstream m_ret;
	
	if (name != "" && pass != "") {
		m_ret << "tankIDName|" << name << "\n";
		m_ret << "tankIDPass|" << pass << "\n";
	}
	
	m_ret << "requestedName|" << "UmbrellaBot" << "\n";
	m_ret << "f|" << personal.filter_innapropriate << "\n";
	m_ret << "protocol|" << protocol << "\n";
	m_ret << "game_version|" << game_version << "\n";
	m_ret << "lmode|" << lmode << "\n";
	m_ret << "cbits|" << personal.GetCbits() << "\n";
	m_ret << "player_age|" << player_age << "\n";
	m_ret << "GDPR|" << (int)1 << "\n";
	m_ret << "category|" << "_-5100gid" << "\n";
	m_ret << "tr|" << (int)4322 << "\n";
	m_ret << "meta|" << meta << "\n";
	m_ret << "fhash|" << (int)-716928004 << "\n";
	m_ret << "rid|" << rid << "\n";
	m_ret << "platformID|" << (int)4 << "\n";
	m_ret << "deviceVersion|" << (int)0 << "\n";
	m_ret << "country|" << "id" << "\n";
	m_ret << "hash|" << (int)1225092726 << "\n";
	m_ret << "mac|" << mac << "\n";
	
	if (user != 0 && token != 0) {
		m_ret << "user|" << user << "\n";
		m_ret << "token|" << token << "\n";
	}
	
	if (uuid != "")
		m_ret << "UUIDToken|" << uuid << "\n";
	
	if (doorid != "")
		m_ret << "doorID|" << doorid << "\n";
	
	m_ret << "wk|NONE0" << "\n";
	
	return m_ret.str();
}

void TutorialBot::SendPacketRaw(int type, uint8_t* ptr, int flags) {
	if (!peer)
		return;
	if (peer->state != ENET_PEER_STATE_CONNECTED)
		return;
	
	int len = 4 + 56;
	
	char* packet = new char[len];
	
	memset(packet, 0, len);
	
	memcpy(packet, &type, 4);
	memcpy(packet + 4, ptr, 56);
	
	ENetPacket* nPacket = enet_packet_create(packet, len, flags);
	enet_peer_send(peer, 0, nPacket);
	
	delete[] packet;
}


void TutorialBot::SendPacket(int type, std::string const& text) {
	if (!peer)
		return;
	if (peer->state != ENET_PEER_STATE_CONNECTED)
		return;
	
	int len = 5 + text.length();
	
	char* packet = new char[len];
	
	memset(packet, 0, len);
	
	memcpy(packet, &type, 4);
	memcpy(packet + 4, text.c_str(), text.length());
	
	ENetPacket* nPacket = enet_packet_create(packet, len, 1);
	enet_peer_send(peer, 0, nPacket);
	
	delete[] packet;
}

void TutorialBot::ConnectClient(std::string IP, int Port) {
          
}

void TutorialBot::OnConnected() {
     std::cout << "Client Connected, Succesfully" << endl;
}

void TutorialBot::OnDisconnected() {
	std::cout << "Disconnected!.. Reconnecting..." << endl;
	ConnectClient(IP, PORT);
}

void TutorialBot::OnReceive(ENetPacket* packet) {
	if (packet->dataLength < 4 || !packet->data)
		return;
	int type = *(int*)packet->data;
	switch (type) {
		case 1: {
			SendPacket(2, data.Create());
		} break;
		case 3:
		case 5:
		case 6: {
			packet->data[packet->dataLength - 1] = '\x00';
			OnTextPacket(type, (char*)(packet->data + 4));
		} break;
		case 4: {
			OnTankPacket(type, packet->data + 4, packet->dataLength - 4);
		} break;
		default: {
		} break;
	}
}

void TutorialBot::OnTextPacket(int type, std::string text) {
    if (text.find("action|logon_fail") != std::string::npos){
		// disconnect
    }
}



void TutorialBot::OnTankPacket(int type, uint8_t* ptr, int size) {
	TankPacketStruct* packet = (TankPacketStruct*)ptr;
	switch (packet->type) {
		case 0: {
			// state
		} break;
		case 1: {
			// call
			variantlist_t var{};
			var.serialize_from_mem(ptr + 56);
			std::string func = var[0].get_string();
			
			
			if (func == "OnSendToServer") {
				data.Set(var); // data = clientdata
				TankPacketStruct tank;
				tank.type = 26;
				tank.netid = -1;
				SendPacketRaw(type, (uint8_t*)&tank);
				Disconnect();
			}
            
		} break;
		case 22: {
			TankPacketStruct tank;
			tank.type = 21;
			tank.x = 64;
			tank.y = 64;
			tank.value = packet->value + 5000;
			SendPacketRaw(type, (uint8_t*)&tank);
		} break;
		default: {
			
		} break;
	}
}
void TutorialBot::Event() {
    ENetEvent Event;
	while (enet_host_service(host, &Event, 0) > 0) {
		switch (Event.type) {
				
		case ENET_EVENT_TYPE_CONNECT: break;
		case ENET_EVENT_TYPE_DISCONNECT:  return;
		case ENET_EVENT_TYPE_RECEIVE: break;
		default: break;
				
		}
	}
}
