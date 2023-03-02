// #include "lib/N/NCStandard.h"

#include "lib/N/NErrorHandling.h"
#include "NUT_LUT.h"

void NUT_LUTBuild_NVec3FromConstant(NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NVEC3 *pvector)
{
	Nu32 i;
	NVEC3 *pv;

	NErrorIf(!stride, NERROR_NULL_VALUE);

	pv = plut;
	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			*pv = *pvector;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			pv->x += pvector->x;
			pv->y += pvector->y;
			pv->z += pvector->z;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			pv->x -= pvector->x;
			pv->y -= pvector->y;
			pv->z -= pvector->z;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			pv->x *= pvector->x;
			pv->y *= pvector->y;
			pv->z *= pvector->z;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			pv->x = (pv->x + pvector->x) * 0.5f;
			pv->y = (pv->x + pvector->y) * 0.5f;
			pv->z = (pv->x + pvector->z) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NVec3FromConstantf(NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const Nf32 x, const Nf32 y, const Nf32 z)
{
	Nu32 i;
	NVEC3 *pv;

	NErrorIf(!stride, NERROR_NULL_VALUE);

	pv = plut;
	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			pv->x = x;
			pv->y = y;
			pv->z = z;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			pv->x += x;
			pv->y += y;
			pv->z += z;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			pv->x -= x;
			pv->y -= y;
			pv->z -= z;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			pv->x *= x;
			pv->y *= y;
			pv->z *= z;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			pv->x = (pv->x + x) * 0.5f;
			pv->y = (pv->x + y) * 0.5f;
			pv->z = (pv->x + z) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NVec3FromFastRand(NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NVEC3 *pscale)
{
	Nu32 i;
	NVEC3 *pv;

	NErrorIf(!stride, NERROR_NULL_VALUE);

	pv = plut;
	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			pv->x = NFastRand() * pscale->x;
			pv->y = NFastRand() * pscale->y;
			pv->z = NFastRand() * pscale->z;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			pv->x += NFastRand() * pscale->x;
			pv->y += NFastRand() * pscale->y;
			pv->z += NFastRand() * pscale->z;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			pv->x -= NFastRand() * pscale->x;
			pv->y -= NFastRand() * pscale->y;
			pv->z -= NFastRand() * pscale->z;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			pv->x *= NFastRand() * pscale->x;
			pv->y *= NFastRand() * pscale->y;
			pv->z *= NFastRand() * pscale->z;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			pv->x = (pv->x + NFastRand() * pscale->x) * 0.5f;
			pv->y = (pv->x + NFastRand() * pscale->y) * 0.5f;
			pv->z = (pv->x + NFastRand() * pscale->z) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NVec3FromFastRandf(NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const Nf32 xscale, const Nf32 yscale, const Nf32 zscale)
{
	Nu32 i;
	NVEC3 *pv;

	NErrorIf(!stride, NERROR_NULL_VALUE);

	pv = plut;

	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			pv->x = xscale * NFastRand();
			pv->y = yscale * NFastRand();
			pv->z = zscale * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			pv->x += xscale * NFastRand();
			pv->y += yscale * NFastRand();
			pv->z += zscale * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			pv->x -= xscale * NFastRand();
			pv->y -= yscale * NFastRand();
			pv->z -= zscale * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			pv->x *= xscale * NFastRand();
			pv->y *= yscale * NFastRand();
			pv->z *= zscale * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			pv->x = (pv->x + xscale * NFastRand()) * 0.5f;
			pv->y = (pv->x + yscale * NFastRand()) * 0.5f;
			pv->z = (pv->x + zscale * NFastRand()) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NVec3FromFastRandEx(NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NVEC3 *pscale, const Nu32 key_every)
{
	Nu32 i;
	NVEC3 *pv;

	NVEC3 v, va, vb, vmore;
	Nu32 k;
	Nf32 step;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(key_every < 2, NERROR_UNAUTHORIZED_REQUEST); // Use the primary function 'NUT_LUTBuild_Nf32FromFastRand' to fill up a table !

	k = 0;
	vb.x = NFastRand() * pscale->x;
	vb.y = NFastRand() * pscale->y;
	vb.z = NFastRand() * pscale->z;
	step = 1.0f / (Nf32)key_every;

	pv = plut;
	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		if (!k)
		{
			k = key_every - 1;
			v = va = vb;
			vb.x = NFastRand() * pscale->x;
			vb.y = NFastRand() * pscale->y;
			vb.z = NFastRand() * pscale->z;

			vmore.x = (vb.x - va.x) * step;
			vmore.y = (vb.y - va.y) * step;
			vmore.z = (vb.z - va.z) * step;
		}
		else
		{
			v.x += vmore.x;
			v.y += vmore.y;
			v.z += vmore.z;

			k--;
		}

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			*pv = v;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			pv->x += v.x;
			pv->y += v.y;
			pv->z += v.z;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			pv->x -= v.x;
			pv->y -= v.y;
			pv->z -= v.z;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			pv->x *= v.x;
			pv->y *= v.y;
			pv->z *= v.z;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			pv->x = (pv->x + v.x) * 0.5f;
			pv->y = (pv->x + v.y) * 0.5f;
			pv->z = (pv->x + v.z) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NVec3FromFastRandExf(NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const Nf32 xscale, const Nf32 yscale, const Nf32 zscale, const Nu32 key_every)
{
	Nu32 i;
	NVEC3 *pv;

	NVEC3 v, va, vb, vmore;
	Nu32 k;
	Nf32 step;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(key_every < 2, NERROR_UNAUTHORIZED_REQUEST); // Use the primary function 'NUT_LUTBuild_Nf32FromFastRand' to fill up a table !

	k = 0;
	vb.x = NFastRand() * xscale;
	vb.y = NFastRand() * yscale;
	vb.z = NFastRand() * zscale;
	step = 1.0f / (Nf32)key_every;

	pv = plut;
	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		if (!k)
		{
			k = key_every - 1;
			v = va = vb;
			vb.x = NFastRand() * xscale;
			vb.y = NFastRand() * yscale;
			vb.z = NFastRand() * zscale;

			vmore.x = (vb.x - va.x) * step;
			vmore.y = (vb.y - va.y) * step;
			vmore.z = (vb.z - va.z) * step;
		}
		else
		{
			v.x += vmore.x;
			v.y += vmore.y;
			v.z += vmore.z;

			k--;
		}

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			*pv = v;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			pv->x += v.x;
			pv->y += v.y;
			pv->z += v.z;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			pv->x -= v.x;
			pv->y -= v.y;
			pv->z -= v.z;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			pv->x *= v.x;
			pv->y *= v.y;
			pv->z *= v.z;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			pv->x = (pv->x + v.x) * 0.5f;
			pv->y = (pv->x + v.y) * 0.5f;
			pv->z = (pv->x + v.z) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NVec3FromSplines(NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NSPLINE *px_spline, const NSPLINE *py_spline, const NSPLINE *pz_spline)
{
	Nu32 i;
	Nf32 step;
	NVEC2 C;
	NUT_2DSPLINE_PERSISTENT_XRESULT persistentx, persistenty, persistentz;
	Nf32 x, y, z;
	NVEC3 *pv;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!px_spline, NERROR_NULL_POINTER);
	NErrorIf(!py_spline, NERROR_NULL_POINTER);
	NErrorIf(!pz_spline, NERROR_NULL_POINTER);

	// Computing values
	NUT_Setup2DSplinePersistentXResult(&persistentx, px_spline);
	NUT_Setup2DSplinePersistentXResult(&persistenty, py_spline);
	NUT_Setup2DSplinePersistentXResult(&persistentz, pz_spline);

	if (element_nb > 1)
		step = 1.0f / (Nf32)(element_nb - 1);
	else
		step = 0.0f;

	pv = plut;
	C.x = 0.0f;
	C.y = 0.0f;
	x = y = z = 0.0f;
	for (i = 0; i < element_nb; i++, C.x = (Nf32)i * step, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistentx))
			x = persistentx.XResult.I.y;
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistenty))
			y = persistenty.XResult.I.y;
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistentz))
			z = persistentz.XResult.I.y;

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			pv->x = x;
			pv->y = y;
			pv->z = z;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			pv->x += x;
			pv->y += y;
			pv->z += z;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			pv->x -= x;
			pv->y -= y;
			pv->z -= z;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			pv->x *= x;
			pv->y *= y;
			pv->z *= z;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			pv->x = (pv->x + x) * 0.5f;
			pv->y = (pv->x + y) * 0.5f;
			pv->z = (pv->x + z) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NVec3Clamp(NVEC3 *plut, const Nu32 element_nb, const Nu32 stride, const NVEC3 *pvnewmin, const NVEC3 *pvnewmax)
{
	Nu32 i;
	NVEC3 *pv;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!pvnewmin, NERROR_NULL_VALUE);
	NErrorIf(!pvnewmax, NERROR_NULL_VALUE);

	pv = plut;
	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		if (pv->x < pvnewmin->x)
			pv->x = pvnewmin->x;
		else if (pv->x > pvnewmax->x)
			pv->x = pvnewmax->x;

		if (pv->y < pvnewmin->y)
			pv->y = pvnewmin->y;
		else if (pv->y > pvnewmax->y)
			pv->y = pvnewmax->y;

		if (pv->z < pvnewmin->z)
			pv->z = pvnewmin->z;
		else if (pv->z > pvnewmax->z)
			pv->z = pvnewmax->z;
	}
}

void NUT_LUTBuild_NVec3Warp(NVEC3 *plut, const Nu32 element_nb, const Nu32 stride, const NVEC3 *pvnewmin, const NVEC3 *pvnewmax)
{
	Nu32 i;
	NVEC3 *pv;
	NVEC3 curmin, curmax;
	NVEC3 ratio;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!pvnewmin, NERROR_NULL_VALUE);
	NErrorIf(!pvnewmax, NERROR_NULL_VALUE);

	// Find LUT current min and max
	pv = plut;
	curmin = curmax = *pv;
	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		if (pv->x < curmin.x)
			curmin.x = pv->x;
		else if (pv->x > curmax.x)
			curmax.x = pv->x;

		if (pv->y < curmin.y)
			curmin.y = pv->y;
		else if (pv->y > curmax.y)
			curmax.y = pv->y;

		if (pv->z < curmin.z)
			curmin.z = pv->z;
		else if (pv->z > curmax.z)
			curmax.z = pv->z;
	}

	// incoming min/max values constraint
	pv = plut;
	ratio.x = (pvnewmax->x - pvnewmin->x) / (curmax.x - curmin.x);
	ratio.y = (pvnewmax->y - pvnewmin->y) / (curmax.y - curmin.y);
	ratio.z = (pvnewmax->z - pvnewmin->z) / (curmax.z - curmin.z);

	for (i = element_nb; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		pv->x = pvnewmin->x + (pv->x - curmin.x) * ratio.x;
		pv->y = pvnewmin->y + (pv->y - curmin.y) * ratio.y;
		pv->z = pvnewmin->z + (pv->z - curmin.z) * ratio.z;
	}
}

void NUT_LUTBuild_NVec3Smooth(NVEC3 *plut, const Nu32 element_nb, const Nu32 stride, const Nu16 range, const Nu16 pass_nb)
{
	Nu32 i;
	Nf32 inv_coefsum;
	NVEC3 *pv, *pdata, *pd, *prevpd;
	NVEC3 tmp;
	Nu16 p;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!range, NERROR_NULL_VALUE);
	NErrorIf(!pass_nb, NERROR_NULL_VALUE);

	pdata = (NVEC3 *)Nmalloc(sizeof(NVEC3) * (element_nb + 2 * range));
	inv_coefsum = 1.0f / ((Nf32)range * 2.0f + 1.0f);

	for (p = 0; p < pass_nb; p++)
	{
		pv = plut;
		pd = pdata;
		// All divided in one pass !
		// 0) The first
		pd->x = tmp.x = pv->x * inv_coefsum;
		pd->y = tmp.y = pv->y * inv_coefsum;
		pd->z = tmp.z = pv->z * inv_coefsum;
		prevpd = pd;
		pd++;
		// 1) The left "out of range" part !
		for (i = range - 1; i != 0; i--, pd++, prevpd++)
		{
			pd->x = prevpd->x + tmp.x;
			pd->y = prevpd->y + tmp.y;
			pd->z = prevpd->z + tmp.z;
		}
		// 2) The main part !
		for (i = element_nb; i != 0; i--, pd++, prevpd++, pv = (NVEC3 *)((NBYTE *)pv + stride))
		{
			pd->x = prevpd->x + pv->x * inv_coefsum;
			pd->y = prevpd->y + pv->y * inv_coefsum;
			pd->z = prevpd->z + pv->z * inv_coefsum;
		}
		// 3) The right "out of range" part !
		tmp.x = prevpd->x - ((NVEC3 *)(prevpd - 1))->x;
		tmp.y = prevpd->y - ((NVEC3 *)(prevpd - 1))->y;
		tmp.z = prevpd->z - ((NVEC3 *)(prevpd - 1))->z;

		for (i = range; i != 0; i--, pd++, prevpd++)
		{
			pd->x = prevpd->x + tmp.x;
			pd->y = prevpd->y + tmp.y;
			pd->z = prevpd->z + tmp.z;
		}
		// 4)Final pass: start !
		pv = plut;
		prevpd = pdata;
		pd = &pdata[2 * range];

		pv->x = pd->x;
		pv->y = pd->y;
		pv->z = pd->z;

		// 5)Final pass, loop !
		pv = (NVEC3 *)((NBYTE *)pv + stride);
		pd++;
		for (i = element_nb - 1; i != 0; i--, pd++, prevpd++, pv = (NVEC3 *)((NBYTE *)pv + stride))
		{
			pv->x = pd->x - prevpd->x;
			pv->y = pd->y - prevpd->y;
			pv->z = pd->z - prevpd->z;
		}
	}

	Nfree(pdata);
}
void NUT_LUTBuild_Vector3Lerp(NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NVEC3 *pva, const NVEC3 *pvb)
{
	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!element_nb, NERROR_NULL_VALUE);

	Nu32 i;
	NVEC3 v;
	NVEC3 *pv;
	Nf32 t;

	if (element_nb < 2)
	{
		t = 0.0f;
	}
	else
	{
		t = 1.0f / (Nf32)(element_nb - 1);
	}

	pv = plut;
	for (i = 0; i < element_nb; i++, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			NVec3Lerp(pv, pva, pvb, (Nf32)i * t);
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			NVec3Lerp(&v, pva, pvb, (Nf32)i * t);
			pv->x += v.x;
			pv->y += v.y;
			pv->z += v.z;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			NVec3Lerp(&v, pva, pvb, (Nf32)i * t);
			pv->x *= v.x;
			pv->y *= v.y;
			pv->z *= v.z;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			NVec3Lerp(&v, pva, pvb, (Nf32)i * t);
			pv->x -= v.x;
			pv->y -= v.y;
			pv->z -= v.z;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			NVec3Lerp(&v, pva, pvb, (Nf32)i * t);
			pv->x = (pv->x + v.x) * 0.5f;
			pv->y = (pv->y + v.y) * 0.5f;
			pv->z = (pv->z + v.z) * 0.5f;
			break;

		default:
			break;
		}
	}
}
