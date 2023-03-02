#ifdef _NUT_LOGGING
#include <time.h>
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../NFlags.h"

#include "NUT_Core.h"
#include "NUT_Logging.h"

extern Nu32 NUtilitiesCoreFlags;

Nu64 NUTLogChannels = 0;
Nchar NUTLogFileName[CONSTANT_NUT_LOGGING_PATH_SIZE_MAX + CONSTANT_NUT_LOGGING_FILENAME_SIZE_MAX];

void NUT_EnableLoggingChannel(Nu8 c, Nbool b)
{
	if (c < 64)
	{
		if (b)
			NUTLogChannels |= (1 << c);
		else
			NUTLogChannels &= ~(1 << c);
	}
}

void NUT_InitLogging(const Nu32 setup_flags, const Nchar *plogpath)
{
	SET_FLAGS(NUtilitiesCoreFlags, MASK_NUT_CORE_LOGGING, 0);
	NUTLogFileName[0] = 0;
	NUtilitiesCoreFlags |= setup_flags & MASK_NUT_CORE_LOGGING;

	// logpath
	if (plogpath)
	{
		if (strlen(plogpath) < CONSTANT_NUT_LOGGING_PATH_SIZE_MAX)
		{
			strcpy(NUTLogFileName, plogpath);
			FLAG_ON(NUtilitiesCoreFlags, FLAG_NUT_CORE_LOGGING_STREAM_FILEOK);
		}
	}
	else
	{
		strcpy(NUTLogFileName, "./");
		FLAG_ON(NUtilitiesCoreFlags, FLAG_NUT_CORE_LOGGING_STREAM_FILEOK);
	}

	// logfilename:
	Nchar buf[CONSTANT_NUT_LOGGING_FILENAME_SIZE_MAX];
	Nu64 sec = (Nu64)time(NULL);
	sprintf(buf, "LogFile[%lld].txt", sec);
	strcat(NUTLogFileName, buf);
}

void NUT_Logging(const Nu8 channel, const Nchar *format, ...)
{
	if (channel > 63 || (NUTLogChannels >> channel) & 1)
	{
		va_list args;
		Nchar buffer[CONSTANT_NUT_LOGGING_BUFFER_SIZE];
		buffer[0] = 0;

		va_start(args, format);
		vsnprintf(buffer, sizeof buffer, format, args);
		va_end(args);

		if (ISFLAG_ON(NUtilitiesCoreFlags, FLAG_NUT_CORE_LOGGING_STDOUT))
			printf(buffer);

		if (FLAGS_TEST(NUtilitiesCoreFlags, MASK_NUT_CORE_LOGGING_STREAM, FLAG_NUT_CORE_LOGGING_STREAM | FLAG_NUT_CORE_LOGGING_STREAM_FILEOK))
		{
			FILE *pfile = fopen(NUTLogFileName, "a");
			if (pfile)
			{
				fprintf(pfile, buffer);
				fprintf(pfile, "\n");
				fclose(pfile);
			}
		}
	}
}
#endif //_NUT_LOGGING