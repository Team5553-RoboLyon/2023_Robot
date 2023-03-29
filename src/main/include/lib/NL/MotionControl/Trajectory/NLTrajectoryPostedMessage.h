#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"
#include "../../../N/NMemory.h"
typedef struct NLTRJ_POSTED_MESSAGE NLTRJ_POSTED_MESSAGE;
struct NLTRJ_POSTED_MESSAGE
{
	NLTRJ_POSTED_MESSAGE():m_timeStamp(0.0f),m_id(NVOID),m_user(0) {};
	~NLTRJ_POSTED_MESSAGE(){};
	void null();

	Nf32	m_timeStamp;
	Nu32	m_id;
	Nu32	m_user;
};
#define NLclearNLPostedMessageInArrayCallBack		NULL
#define NLclearNLPostedMessagePtrInArrayCallBack	NULL
