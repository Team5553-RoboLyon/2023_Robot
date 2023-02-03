#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define NLCSVITEM_LABEL_MAXSIZE		32
#define NLCSVITEM_FORMAT_MAXSIZE	8
#define NLCSVITEM_FILENAME_MAXSIZE	256

#define NLCSVITEM_TYPE_FLOAT		1
#define NLCSVITEM_TYPE_DOUBLE		2
#define NLCSVITEM_TYPE_INT			3

typedef struct NLCSVITEM	NLCSVITEM;
struct NLCSVITEM
{
	unsigned long	m_type;
	char			m_label[NLCSVITEM_LABEL_MAXSIZE];
	char			m_format[NLCSVITEM_FORMAT_MAXSIZE];
	void*			m_ptr;
};

class NLCSV
{
public:
	NLCSV() :m_pFile(nullptr), m_pItems(nullptr), m_itemListSize(0) {};
	NLCSV(unsigned long itemlistsize);
	~NLCSV() { close(); if (m_pItems)free(m_pItems); };


	void setItem(const unsigned long idx, const char* label, const unsigned char precision, double* pdblptr);
	void setItem(const unsigned long idx, const char* label, const unsigned char precision, float* pfltptr);
	void setItem(const unsigned long idx, const char* label, int* pintptr);

	bool open(const char* logname = nullptr);
	bool open(const char* logname, const char* headline);
	bool open(const char* logname, const bool bautoheadline);
	bool isOpen() { return m_pFile ? true : false; }

	void write(const char* format, ...);
	void write();
	void close(){if (m_pFile) { fclose(m_pFile); m_pFile = nullptr; }};


private:
	FILE*			m_pFile;
	NLCSVITEM*		m_pItems;
	unsigned long	m_itemListSize;
};