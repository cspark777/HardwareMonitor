#pragma once

struct PACKET_NODE
{
	PACKET_NODE *next;
	int nLen;
	BYTE *buf;
};

class CMyPacketList
{
	PACKET_NODE *head;
	PACKET_NODE *tail;
public:
	CMyPacketList(void);
	~CMyPacketList(void);
public:
	void AddPacket(BYTE* buf, int len);
	void PopPacket();
	PACKET_NODE* GetPacket();
	void InitPacketList();
	void DeleteAllPacket();
	void DeletePacket(PACKET_NODE* node);
	
};

