#ifndef __NROLLINGAVERAGE_H
#define __NROLLINGAVERAGE_H

#include <cstdlib>
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NRollingAverage.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
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



#endif // __NROLLINGAVERAGE_H 

