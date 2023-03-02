
#include "../NLKin.h"
#include "NLSMotionProfile.h"


Nu32 promoteNull(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff)
{
	return 0;
}

Nu32 promoteFlat(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff)
{
	return psmp->m_fUp.promote(pkbuff, psmp->m_sUp.m_v1);
}

Nu32 promoteSup(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff)
{

	return psmp->m_sUp.promote(pkbuff, psmp->m_cutInOffsets.m_dt, &psmp->m_cutOutOffsets);
}

Nu32 promoteSdown(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff)
{
	return psmp->m_sDown.promote(pkbuff, psmp->m_cutInOffsets.m_dt, &psmp->m_cutOutOffsets);
}

Nu32 promoteSupFlat(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff)
{
	Nu32 size = psmp->m_sUp.promote(pkbuff, psmp->m_cutInOffsets.m_dt);
	return ( size + psmp->m_fUp.promote(pkbuff + size ,psmp->m_sUp.m_v1) );
}

Nu32 promoteFlatSdown(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff)
{
	Nu32 size = psmp->m_fUp.promote(pkbuff, psmp->m_sDown.m_v0);
	return ( size + psmp->m_sDown.promote(pkbuff + size, &psmp->m_cutOutOffsets));
}

Nu32 promoteSupSdown(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff)
{
	Nu32 size = psmp->m_sUp.promote(pkbuff, psmp->m_cutInOffsets.m_dt);
	return ( size + psmp->m_sDown.promote(pkbuff + size, &psmp->m_cutOutOffsets));
}

Nu32 promoteSupFlatSdown(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff)
{
	Nu32 size =	psmp->m_sUp.promote(pkbuff, psmp->m_cutInOffsets.m_dt);
	size = size + psmp->m_fUp.promote(pkbuff + size, psmp->m_sUp.m_v1);
	return ( size  + psmp->m_sDown.promote(pkbuff + size, &psmp->m_cutOutOffsets));
}
