#include "lib/NRollingAverage.h"

NdoubleRollingAverage::NdoubleRollingAverage()
{
	m_last			= 0;
	m_pdouble		= NULL;
	m_sum			= 0.0;
	m_average		= 0.0;	
	m_index			= 0;
}

NdoubleRollingAverage::NdoubleRollingAverage(const int table_size, const double initial_average)
{
	m_last			= table_size - 1;
	m_pdouble		= (double*)malloc(sizeof(double)*table_size);
	m_sum			= initial_average*table_size;
	m_average		= initial_average;	
	m_index			= 0;

	double *pd = m_pdouble;
	for(int i= 0;i<table_size;i++,pd++)
		*pd = initial_average;
}

NdoubleRollingAverage::~NdoubleRollingAverage()
{
	m_pdouble -= m_index;	// replace le pointeur en position initiale
	free(m_pdouble);			// lib�re la m�moire
}

const double NdoubleRollingAverage::add(const double value)
{
	m_sum += (value - *m_pdouble);
	m_average = m_sum/(m_last+1);
	*m_pdouble  = value;
	
	if(m_index == m_last)
	{
		m_pdouble -= m_last;
		m_index	  = 0;
	}
	else
	{
		m_pdouble ++;
		m_index	 ++;
	}

	return m_average;
}

void NdoubleRollingAverage::reset(const double initial_average)
{
	m_sum	  = initial_average*(m_last+1);
	m_average = initial_average;
	m_pdouble -= m_index;	// replace le pointeur en position initiale
	m_index	  = 0;
	double *pd = m_pdouble;
	for(int i= 0;i<=m_last;i++,pd++)
		*pd = initial_average;
}

// 



NfloatRollingAverage::NfloatRollingAverage(const unsigned short table_size, const float initial_average)
{
	m_last			= table_size - 1;
	m_pfloat		= (float*)malloc(sizeof(float)*table_size);
	m_sum			= initial_average*table_size;
	m_average		= initial_average;	
	m_index			= 0;

	float *pf = m_pfloat;
	for(unsigned short i= 0;i<table_size;i++,pf++)
		*pf = initial_average;
}

NfloatRollingAverage::~NfloatRollingAverage()
{
	m_pfloat -= m_index;	// replace le pointeur en position initiale
	free(m_pfloat);			// lib�re la m�moire
}

const float NfloatRollingAverage::add(const float value)
{
	m_sum += (value - *m_pfloat);
	m_average = m_sum/(m_last+1);
	*m_pfloat  = value;
	
	if(m_index == m_last)
	{
		m_pfloat -= m_last;
		m_index	  = 0;
	}
	else
	{
		m_pfloat ++;
		m_index	 ++;
	}

	return m_average;
}

void NfloatRollingAverage::reset(const float initial_average)
{
	m_sum	  = initial_average*(m_last+1);
	m_average = initial_average;
	m_pfloat -= m_index;	// replace le pointeur en position initiale
	m_index	  = 0;
	float *pf = m_pfloat;
	for(unsigned short i= 0;i<=m_last;i++,pf++)
		*pf = initial_average;
}

// NlongRollingAverage
NlongRollingAverage::NlongRollingAverage(const unsigned short table_size, const long initial_average)
{
	m_last			= table_size - 1;
	m_plong			= (long*)malloc(sizeof(long)*table_size);
	m_average		= initial_average;
	m_sum			= initial_average*table_size;
	m_index			= 0;

	long *pl = m_plong;
	for(unsigned short i= 0;i<table_size;i++,pl++)
		*pl = initial_average;
}

NlongRollingAverage::~NlongRollingAverage()
{
	m_plong -= m_index;		// replace le pointeur en position initiale
	free(m_plong);			// lib�re la m�moire
}

const long NlongRollingAverage::add(const long value)
{
	m_sum += (value - *m_plong);
	m_average = m_sum/(m_last+1);
	*m_plong  = value;
	
	if(m_index == m_last)
	{
		m_plong -= m_last;
		m_index	  = 0;
	}
	else
	{
		m_plong ++;
		m_index	 ++;
	}

	return m_average;
}

void NlongRollingAverage::reset(const long initial_average)
{
	m_sum		= initial_average*(m_last+1);
	m_average	= initial_average;
	m_plong    -= m_index;	// replace le pointeur en position initiale
	m_index		= 0;
	long *pl	= m_plong;
	for(unsigned short i= 0;i<=m_last;i++,pl++)
		*pl = initial_average;
}
