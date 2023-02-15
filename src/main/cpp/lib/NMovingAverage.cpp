#include "lib/NMovingAverage.h"

NdoubleMovingAverage::NdoubleMovingAverage()
{
	m_last = 0;
	m_pSamples = NULL;
	m_sum = 0.0;
	m_sum2 = 0.0;
	m_mean = 0.0;
	m_variance = 0.0;
	m_index = 0;
}

NdoubleMovingAverage::NdoubleMovingAverage(const int table_size, const double initial_average)
{
	m_last = table_size - 1;
	m_pSamples = (double *)malloc(sizeof(double) * table_size);
	m_sum = initial_average * table_size;
	m_sum2 = m_sum * initial_average;
	m_mean = initial_average;
	m_variance = 0.0;
	m_index = 0;

	double *pd = m_pSamples;
	for (int i = 0; i < table_size; i++, pd++)
		*pd = initial_average;
}

NdoubleMovingAverage::~NdoubleMovingAverage()
{
	m_pSamples -= m_index; // replace le pointeur en position initiale
	free(m_pSamples);	   // lib�re la m�moire
}

const double NdoubleMovingAverage::addSample(const double value)
{
	m_sum += (value - *m_pSamples);
	m_sum2 += (value * value - (*m_pSamples) * (*m_pSamples));
	m_mean = m_sum / (m_last + 1);
	m_variance = (m_sum2 - (m_sum * m_sum) / (m_last + 1)) / (m_last);
	//m_variance = (m_sum2 / (m_last + 1) - (m_mean * m_mean));
	*m_pSamples = value;

	if (m_index == m_last)
	{
		m_pSamples -= m_last; // Le pointeur "re-pointe" sur l'adresse du premier sample
		m_index = 0;		  // L'index "re-designe" le premier sample
	}
	else
	{
		m_pSamples++; // On décale le pointeur sur le sample suivant
		m_index++;	  // L'index designe le sample suivant
	}

	return m_mean;
}

void NdoubleMovingAverage::reset(const int table_size = 0, const double initial_average = 0.0)
{
	if (table_size < 2)
	{
		if (!m_pSamples)
		{
			m_last = NMA_DEFAULT_SAMPLE_NB - 1;
			m_pSamples = (double *)malloc(sizeof(double) * NMA_DEFAULT_SAMPLE_NB);
			m_index = 0;
		}
		// else
		//  on ne fait rien. en faisant un reset avec une valeur <= 2, l'utilisateur sous entend ne pas vouloir réallouer.
	}
	else // table_size > 2
	{
		if (m_last != (table_size - 1))
		{
			m_last = table_size - 1;
			m_pSamples = (double *)realloc(m_pSamples, sizeof(double) * table_size);
			m_index = 0;
		}
	}

	m_sum = initial_average * (m_last + 1);
	m_sum2 = m_sum * initial_average;
	m_mean = initial_average;
	m_variance = 0.0;
	m_pSamples -= m_index; // replace le pointeur en position initiale
	m_index = 0;

	double *pd = m_pSamples;
	for (int i = 0; i <= m_last; i++, pd++)
		*pd = initial_average;
}

//

NfloatRollingAverage::NfloatRollingAverage(const unsigned short table_size, const float initial_average)
{
	m_last = table_size - 1;
	m_pfloat = (float *)malloc(sizeof(float) * table_size);
	m_sum = initial_average * table_size;
	m_average = initial_average;
	m_index = 0;

	float *pf = m_pfloat;
	for (unsigned short i = 0; i < table_size; i++, pf++)
		*pf = initial_average;
}

NfloatRollingAverage::~NfloatRollingAverage()
{
	m_pfloat -= m_index; // replace le pointeur en position initiale
	free(m_pfloat);		 // lib�re la m�moire
}

const float NfloatRollingAverage::add(const float value)
{
	m_sum += (value - *m_pfloat);
	m_average = m_sum / (m_last + 1);
	*m_pfloat = value;

	if (m_index == m_last)
	{
		m_pfloat -= m_last;
		m_index = 0;
	}
	else
	{
		m_pfloat++;
		m_index++;
	}

	return m_average;
}

void NfloatRollingAverage::reset(const float initial_average)
{
	m_sum = initial_average * (m_last + 1);
	m_average = initial_average;
	m_pfloat -= m_index; // replace le pointeur en position initiale
	m_index = 0;
	float *pf = m_pfloat;
	for (unsigned short i = 0; i <= m_last; i++, pf++)
		*pf = initial_average;
}

// NlongRollingAverage
NlongRollingAverage::NlongRollingAverage(const unsigned short table_size, const long initial_average)
{
	m_last = table_size - 1;
	m_plong = (long *)malloc(sizeof(long) * table_size);
	m_average = initial_average;
	m_sum = initial_average * table_size;
	m_index = 0;

	long *pl = m_plong;
	for (unsigned short i = 0; i < table_size; i++, pl++)
		*pl = initial_average;
}

NlongRollingAverage::~NlongRollingAverage()
{
	m_plong -= m_index; // replace le pointeur en position initiale
	free(m_plong);		// lib�re la m�moire
}

const long NlongRollingAverage::add(const long value)
{
	m_sum += (value - *m_plong);
	m_average = m_sum / (m_last + 1);
	*m_plong = value;

	if (m_index == m_last)
	{
		m_plong -= m_last;
		m_index = 0;
	}
	else
	{
		m_plong++;
		m_index++;
	}

	return m_average;
}

void NlongRollingAverage::reset(const long initial_average)
{
	m_sum = initial_average * (m_last + 1);
	m_average = initial_average;
	m_plong -= m_index; // replace le pointeur en position initiale
	m_index = 0;
	long *pl = m_plong;
	for (unsigned short i = 0; i <= m_last; i++, pl++)
		*pl = initial_average;
}
