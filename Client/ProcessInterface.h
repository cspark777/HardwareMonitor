#pragma once
class ProcessInterface
{
public:
	virtual ~ProcessInterface() {}
	virtual void ProcessPacket(LPBYTE pData,DWORD dwCount) = 0;
};

