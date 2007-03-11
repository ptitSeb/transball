#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif


#ifdef _WIN32
#include "windows.h"
#endif

#include "stdio.h"
#include "stdarg.h"


FILE *fp=0;

void output_debug_message(const char *fmt, ...)						
{
	char		text[256];
	va_list		ap;

	if (fmt==0) return;

	va_start(ap, fmt);									
	vsprintf(text, fmt, ap);							
	va_end(ap);			
	
	if (fp==0) fp=fopen("output.txt","w");

	fprintf(fp,text);
	fflush(fp);
} /* glprintf */ 




void close_debug_messages(void)
{
	fclose(fp);
	fp=0;
} /* close_debug_messages */ 
