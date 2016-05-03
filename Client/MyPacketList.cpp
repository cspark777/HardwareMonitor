#include "stdafx.h"
#include "MyPacketList.h"


CMyPacketList::CMyPacketList(void)
{
	InitPacketList();
}


CMyPacketList::~CMyPacketList(void)
{
}

void CMyPacketList::InitPacketList()
{
	head = NULL;
	tail = NULL;
}

void CMyPacketList::AddPacket(BYTE* buf, int len)
{
	PACKET_NODE *node = new PACKET_NODE;
	node->nLen = len;
	node->buf = new BYTE[len];
	memcpy(node->buf,buf,len);
	node->next = NULL;

	if(head == NULL)
	{
		head = node;
		tail = node;
	}
	else
	{
		tail->next = node;
		tail = node;
	}	
}

void CMyPacketList::PopPacket() //pop and delete packet
{
	PACKET_NODE* node = NULL;
	if(head != NULL)		
	{
		node = head;
		head = head->next;
		DeletePacket(node);
	}
}
PACKET_NODE* CMyPacketList::GetPacket()
{
	return head;
}

void CMyPacketList::DeletePacket(PACKET_NODE* node)
{
	delete node->buf;
	delete node;
}
void CMyPacketList::DeleteAllPacket()
{
	PACKET_NODE* node;
	while(head!=NULL)
	{
		node = head;
		head = head->next;
		DeletePacket(node);
	}
}