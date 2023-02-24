#pragma once

#include "lib/NL/NLType.h"

typedef struct
{
	nlf32 x;
	nlf32 y;
} NL2f32;

typedef struct
{
	nlf64 x;
	nlf64 y;
} NL2f64;

class NLBEZIERf32
{
public:
	NLBEZIERf32();
	~NLBEZIERf32();

	NL2f32 getPoint(const nlf32 t);

	NL2f32 m_p0;
	NL2f32 m_p1;
	NL2f32 m_p2;
	NL2f32 m_p3;
};

class NLSPLINEf32
{
public:
	NLSPLINEf32();
	~NLSPLINEf32();
};
