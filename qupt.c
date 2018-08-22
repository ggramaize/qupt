#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char **argv)
{
	FILE* fIn;
	char c, c_old = 0;
	unsigned int linepos = 0;

	getopt( argc, argv, ":");

	if( optind < argc && strncmp( argv[optind], "-", 2) != 0)
	{
		/* Open file */
		fIn = fopen( argv[optind], "rb");
		if( fIn == NULL )
		{
			fprintf( stderr, "Cannot open file %s.\n", argv[optind]);
			exit( EXIT_FAILURE);
		}
	}
	else
	{
		/* Reopen STDIN in binary */
		fIn = freopen( NULL, "rb", stdin);
	}

	while( !feof( fIn) )
	{
		c = fgetc( fIn);
		++linepos;

		/* Quit loop if we hit the end of file */
		if( feof( fIn) )
			break; 

		if( c == 0x0d || c == 0x0a )
		{
			/* CR or LF */

			/* If we have a LF following a CR, then LF already has
			 * been sent on stdout. Skip. */
			if( c_old == 0x0d && c == 0x0a )
			{
				c_old = c;
				break;
			}

			if( c_old == 0x20 )
			{
				/* Space or tab may be represented as themselves unless
				 * they're preceding a meaning line break. In that case
				 * we must output a trailing '=' before the line break.
				 *
				 * No check for tabs, which we decided to explicitely 
				 * escape */
				fputc( 0x3d, stdout);
			}
			
			/* Any meaning line break must be encoded as CR-LF */
			fprintf( stdout, "\r\n");
			linepos = 0;
		}
		else if ( c != 0x3d && c >= 0x20 && c <= 0x7e )
		{
			/* printable character, except '=' which must be escaped */
			if( linepos < 76 )
				fputc( c, stdout);
			else
			{
				/* wrap line before printing */
				fprintf( stdout, "%s", "=\r\n");
				fputc( c, stdout);
				linepos = 0;
			}
		}
		else
		{
			/* Non-printable character */

			/* In this implementation, we make the choice to 
			 * explicitely escape tabs, as knowing the tab 
			 * effective width is a PITA */

			if( linepos < 74 )
			{
				fprintf( stdout, "=%02X", (unsigned char)c);
				linepos += 2; /* effectively += 3, because of line 33 */
			}
			else
			{
				/* wrap line before printing */
				fprintf( stdout, "%s", "=\r\n");
				fprintf( stdout, "=%X", (unsigned char)c);
				linepos = 3; /* because the escape sequence writes 3 characters*/ 
			}
		}

		/* Shift current char in past char, used to handle CR-LF */
		c_old = c;
	}
	
	fflush( stdout);
	fclose( fIn);
	return 0;
}
