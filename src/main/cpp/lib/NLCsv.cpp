
#include "lib/NLCsv.h"

NLCSV::NLCSV(unsigned long itemlistsize)
{
	m_pFile = nullptr;
	if (itemlistsize)
	{
		m_itemListSize = itemlistsize;
		m_pItems = (NLCSVITEM*)malloc(sizeof(NLCSVITEM) * itemlistsize);
		memset(m_pItems, 0, sizeof(NLCSVITEM) * itemlistsize);
	}
	else
	{
		m_itemListSize = 0;
		m_pItems = nullptr;
	}
}


void NLCSV::setItem(const unsigned long idx, const char* label, const unsigned char precision, double* pdblptr)
{
	assert(idx < m_itemListSize);
	assert(strlen(label) < (NLCSVITEM_LABEL_MAXSIZE - 1));
	assert(pdblptr);

	strcpy(m_pItems[idx].m_label, label);
	if (precision)
		sprintf(m_pItems[idx].m_format, "%%.%dlf", precision);
	else
		sprintf(m_pItems[idx].m_format, "%.0lf");
	
	m_pItems[idx].m_type	= NLCSVITEM_TYPE_DOUBLE;
	m_pItems[idx].m_ptr		= (void*)pdblptr;
}

void NLCSV::setItem(const unsigned long idx, const char* label, const unsigned char precision, float* pfltptr)
{
	assert(idx < m_itemListSize);
	assert(strlen(label) < (NLCSVITEM_LABEL_MAXSIZE - 1));
	assert(pfltptr);

	strcpy(m_pItems[idx].m_label, label);
	if (precision)
		sprintf(m_pItems[idx].m_format, "%%.%df", precision);
	else
		sprintf(m_pItems[idx].m_format, "%.0f");

	m_pItems[idx].m_type = NLCSVITEM_TYPE_FLOAT;
	m_pItems[idx].m_ptr = (void*)pfltptr;
}

void NLCSV::setItem(const unsigned long idx, const char* label, int* pintptr)
{
	assert(idx < m_itemListSize);
	assert(strlen(label) < (NLCSVITEM_LABEL_MAXSIZE - 1));
	assert(pintptr);

	strcpy(m_pItems[idx].m_label, label);
	sprintf(m_pItems[idx].m_format, "%d");

	m_pItems[idx].m_type = NLCSVITEM_TYPE_INT;
	m_pItems[idx].m_ptr = (void*)pintptr;
}

bool NLCSV::open(const char* logname)
{
	// fichier dej� ouvert ! :(
	assert(!m_pFile);
	if (m_pFile)
		return false;

	char	filename[NLCSVITEM_FILENAME_MAXSIZE];
	time_t	t;

	time(&t);
	char*strt = ctime(&t);
	if (strt[strlen(strt) - 1] == '\n') strt[strlen(strt) - 1] = '\0';

	if (logname)
	{
		sprintf(filename, "%s", logname, strt);
		strftime(&filename[strlen(logname)], NLCSVITEM_FILENAME_MAXSIZE - strlen(logname), "log[%d_%m_%Y__%H_%M_%S].csv", localtime(&t));
	}
	else
		strftime(filename, NLCSVITEM_FILENAME_MAXSIZE, "log[%d_%m_%Y__%H_%M_%S].csv", localtime(&t));

	m_pFile = fopen(filename, "w, ccs=UTF-8");
	if (m_pFile)
		return true;
	else
		return false;
}

bool NLCSV::open(const char* logname, const char* headline )
{
	if (open(logname))
	{
		if (headline)
			fprintf(m_pFile, headline);
		return true;
	}
	else
		return false;
}

bool NLCSV::open(const char* logname, const bool bautoheadline)
{
	if (open(logname))
	{
		if (bautoheadline && m_pItems)
		{
			NLCSVITEM* pitem = m_pItems;
			for (unsigned long i = 0; i < m_itemListSize; i++, pitem++)
			{
				if(pitem->m_type != 0)
					fprintf(m_pFile, "%s;", pitem->m_label);
				else
					fprintf(m_pFile, "?;");
			}
			fprintf(m_pFile, "\n");
		}
		return true;
	}
	else
		return false;
}

void NLCSV::write(const char* format, ...)
{
	assert(m_pFile);

	if (m_pFile)
	{
		va_list args;
		va_start(args, format);
		vfprintf(m_pFile, format, args);
		va_end(args);
	}
}

void NLCSV::write()
{
	if (m_pFile && m_pItems)
	{
		NLCSVITEM* pitem = m_pItems;
		for (unsigned long i = 0; i < m_itemListSize; i++, pitem++)
		{
			switch (pitem->m_type)
			{
				case NLCSVITEM_TYPE_DOUBLE:
					fprintf(m_pFile, pitem->m_format, *((double*)pitem->m_ptr));
					break;
				case NLCSVITEM_TYPE_FLOAT:
					fprintf(m_pFile, pitem->m_format, *((float*)pitem->m_ptr));
					break;
				case NLCSVITEM_TYPE_INT:
					fprintf(m_pFile, pitem->m_format, *((int*)pitem->m_ptr));
					break;
				default:
					fprintf(m_pFile," ? ");
					break;
			}
			fprintf(m_pFile, ";");
		}
		fprintf(m_pFile, "\n");
	}
}
