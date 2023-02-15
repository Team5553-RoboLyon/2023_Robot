#pragma once

#include <cstdlib>
#include <math.h>

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NRollingAverage.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#define NMA_DEFAULT_SAMPLE_NB		8
class NdoubleMovingAverage
{
public:
	NdoubleMovingAverage();
	NdoubleMovingAverage(const int table_size, const double initial_average = 0.0 );
	~NdoubleMovingAverage();

	void reset(const int table_size = 0, const double initial_average = 0.0 );
	const double addSample(const double value);
	inline const double getMean(){return m_mean;}
	inline const double getVariance(){return m_variance;}
	inline const double getStdDeviation(){return sqrt(m_variance);}

	double			m_sum2;			// somme du carré des samples
	double			m_sum;			// Somme algérique des samples
	double*			m_pSamples;		// Table des samples
	int				m_last;			// index du dernier sample ( = tablesize - 1)	
	int				m_index;		// index du sample "à écrire"

	double			m_mean;			// moyenne
	double 			m_variance;		// variance

};

class NfloatRollingAverage
{
public:
	NfloatRollingAverage(const unsigned short table_size, const float initial_average = 0.0f );
	~NfloatRollingAverage();

	void reset(const float initial_average = 0.0f);
	const float add(const float value);
	inline const float get(){return m_average;}
	
	unsigned short	m_last;
	unsigned short	m_index;
	float			m_average;
	float			m_sum;
	float		   *m_pfloat;
};

class NlongRollingAverage
{
public:
	NlongRollingAverage(const unsigned short table_size, const long initial_average = 0.0f );
	~NlongRollingAverage();

	void reset(const long initial_average = 0.0f);
	const long add(const long value);
	inline const long get(){return m_average;}
	
	unsigned short	m_last;
	unsigned short	m_index;
	long			m_average;
	long			m_sum;
	long		   *m_plong;
};


