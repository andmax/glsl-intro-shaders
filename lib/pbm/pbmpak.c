/*  pbmpak.c  21 June 1999 */

#include <sstream>



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pbmpak.h"

#define FALSE 0
#define TRUE 1

#define DEBUG FALSE
#define MAX_LEN 256
#define MAX(a,b) ( (a)>(b) ? (a) : (b) ) 


char line[MAX_LEN];
char word[MAX_LEN];

/**********************************************************************/

void bitchr75 ( char c, int *pattern )

/**********************************************************************/

/*
  Purpose:

    BITCHR75 returns a 35 bit pattern for a given character.

  Discussion:

    The pattern should be interpreted as an array of 7 rows and 5 columns,
    with a value of 1 meaning that entry should be filled, and a zero
    meaning it should be blank.

    Only a limited set of characters is available.  

    Lowercase letters are NOT available.

  Examples:

    C = 'A'

    PATTERN =

      0 0 1 0 0
      0 1 0 1 0
      1 1 0 1 1
      1 0 0 0 1
      1 1 1 1 1
      1 0 0 0 1
      1 0 0 0 1

  Modified:

    21 June 1999

  Author:

    John Burkardt

  Parameters;

    Input, char C, a character whose bit pattern is desired.

    Output, int *PATTERN, the 7 by 5 bit pattern for the character,
    which will be all 0's if the character is not available.  
*/
{
  int i;
  int j;
  int *pattern_match;
/*
  The names of the variables below include the ASCII code of the 
  corresponding character.  In particular, ASCII 32 is blank.
*/
  static int pat032[35] =
    { 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0 };

  static int pat033[35] =
    { 0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 1, 0, 0 };

  static int pat034[35] =
    { 0, 1, 0, 1, 0,
      0, 1, 0, 1, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0 };

  static int pat035[35] =
    { 0, 0, 0, 0, 0,
      0, 1, 0, 1, 0,
      1, 1, 1, 1, 1,
      0, 1, 0, 1, 0,
      1, 1, 1, 1, 1,
      0, 1, 0, 1, 0,
      0, 0, 0, 0, 0 };

  static int pat036[35] =
    { 0, 0, 1, 0, 0,
      0, 1, 1, 1, 0,
      1, 0, 1, 0, 0,
      0, 1, 1, 1, 0,
      0, 0, 1, 0, 1,
      0, 1, 1, 1, 0,
      0, 0, 1, 0, 0 };

  static int pat039[35] =
    { 0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0 };

  static int pat040[35] =
    { 0, 0, 1, 0, 0,
      0, 1, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      0, 1, 0, 0, 0,
      0, 0, 1, 0, 0 };

  static int pat041[35] =
    { 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0,
      0, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 1, 0,
      0, 0, 1, 0, 0 };

  static int pat044[35] =
    { 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 1, 1, 0, 0,
      0, 1, 0, 0, 0 };

  static int pat045[35] =
    { 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 1, 1, 1, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0 };

  static int pat046[35] =
    { 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 1, 1, 0, 0,
      0, 1, 1, 0, 0 };

  static int pat047[35] =
    { 0, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 1, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0 };

  static int pat048[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 1, 1,
      1, 0, 1, 0, 1,
      1, 1, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 0, 0 };

  static int pat049[35] =
    { 0, 0, 1, 0, 0,
      0, 1, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0 };

  static int pat050[35] =
    { 0, 1, 1, 0, 0,
      1, 0, 0, 1, 0,
      0, 0, 0, 0, 1,
      0, 0, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 1, 0, 0, 0,
      1, 1, 1, 1, 1 };

  static int pat051[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 1, 1, 1,
      0, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat052[35] =
    { 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 0, 1 };

  static int pat053[35] =
    { 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      0, 1, 1, 1, 0,
      0, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat054[35] =
    { 0, 1, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 0 };

  static int pat055[35] =
    { 1, 1, 1, 1, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0 };

  static int pat056[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat057[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 1,
      0, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat058[35] =
    { 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 0, 0 };

  static int pat059[35] =
    { 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 1, 0 };

  static int pat060[35] =
    { 0, 0, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 1, 0, 0, 0,
      1, 0, 0, 0, 0,
      0, 1, 0, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 1, 0 };

  static int pat061[35] =
    { 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      1, 1, 1, 1, 1,
      0, 0, 0, 0, 0,
      1, 1, 1, 1, 1,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0 };

  static int pat062[35] =
    { 1, 0, 0, 0, 0,
      0, 1, 0, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 1, 0, 0, 0,
      1, 0, 0, 0, 0 };

  static int pat065[35] =
    { 0, 0, 1, 0, 0,
      0, 1, 0, 1, 0,
      1, 1, 0, 1, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1 };

  static int pat066[35] =
    { 1, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 0 };

  static int pat067[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat068[35] =
    { 1, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 0 };

  static int pat069[35] =
    { 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 1, 1, 1, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 1, 1, 1, 1 };

  static int pat070[35] =
    { 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 1, 1, 1, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0 };

  static int pat071[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 1, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat072[35] =
    { 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1 };

  static int pat073[35] =
    { 0, 1, 1, 1, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 1, 1, 1, 0 };

  static int pat074[35] =
    { 1, 1, 1, 1, 1,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      1, 0, 1, 0, 0,
      0, 1, 0, 0, 0 };

  static int pat075[35] =
    { 1, 0, 0, 0, 1,
      1, 0, 0, 1, 0,
      1, 0, 1, 0, 0,
      1, 1, 0, 0, 0,
      1, 0, 1, 0, 0,
      1, 0, 0, 1, 0,
      1, 0, 0, 0, 1 };

  static int pat076[35] =
    { 1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 1, 1, 1, 1 };

  static int pat077[35] =
    { 1, 1, 0, 1, 1,
      1, 1, 0, 1, 1,
      1, 0, 1, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1 };

  static int pat078[35] =
    { 1, 1, 0, 0, 1,
      1, 1, 0, 0, 1,
      1, 0, 1, 0, 1,
      1, 0, 1, 0, 1,
      1, 0, 1, 0, 1,
      1, 0, 0, 1, 1,
      1, 0, 0, 1, 1 };

  static int pat079[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat080[35] =
    { 1, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0 };

  static int pat081[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 1, 0, 1,
      1, 0, 0, 1, 0,
      0, 1, 1, 0, 1 };

  static int pat082[35] =
    { 1, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 0,
      1, 0, 1, 0, 0,
      1, 0, 0, 1, 0,
      1, 0, 0, 0, 1 };

  static int pat083[35] =
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 0,
      0, 1, 1, 1, 0,
      0, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat084[35] =
    { 1, 1, 1, 1, 1,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0 };

  static int pat085[35] =
    { 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 };

  static int pat086[35] =
    { 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 0, 1, 0,
      0, 1, 0, 1, 0,
      0, 1, 0, 1, 0,
      0, 0, 1, 0, 0 };

  static int pat087[35] =
    { 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 1, 0, 1,
      1, 0, 1, 0, 1,
      1, 1, 1, 1, 1,
      0, 1, 1, 1, 0,
      0, 1, 0, 1, 0 };

  static int pat088[35] =
    { 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 1, 0, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1 };

  static int pat089[35] =
    { 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0 };

  static int pat090[35] =
    { 1, 1, 1, 1, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 1, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 1, 1, 1, 1 };

  static int pat091[35] =
    { 1, 1, 1, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 0, 0, 0, 0,
      1, 1, 1, 0, 0 };

  static int pat093[35] =
    { 0, 0, 1, 1, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 0, 0, 1,
      0, 0, 1, 1, 1 };

  static int pat095[35] =
    { 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      1, 1, 1, 1, 1 };

  static int pat126[35] =
    { 0, 0, 1, 0, 0,
      1, 0, 1, 0, 1,
      0, 1, 0, 1, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0 };

  switch ( c ) {

    case ' ':
      pattern_match = pat032;
      break;
    case '!':
      pattern_match = pat033;
      break;
    case '"':
      pattern_match = pat034;
      break;
    case '#':
      pattern_match = pat035;
      break;
    case '$':
      pattern_match = pat036;
      break;
    case '\'':
      pattern_match = pat039;
      break;
    case '(':
      pattern_match = pat040;
      break;
    case ')':
      pattern_match = pat041;
      break;
    case ',':
      pattern_match = pat044;
      break;
    case '-':
      pattern_match = pat045;
      break;
    case '.':
      pattern_match = pat046;
      break;
    case '/':
      pattern_match = pat047;
      break;
    case '0':
      pattern_match = pat048;
      break;
    case '1':
      pattern_match = pat049;
      break;
    case '2':
      pattern_match = pat050;
      break;
    case '3':
      pattern_match = pat051;
      break;
    case '4':
      pattern_match = pat052;
      break;
    case '5':
      pattern_match = pat053;
      break;
    case '6':
      pattern_match = pat054;
      break;
    case '7':
      pattern_match = pat055;
      break;
    case '8':
      pattern_match = pat056;
      break;
    case '9':
      pattern_match = pat057;
      break;
    case ':':
      pattern_match = pat058;
      break;
    case ';':
      pattern_match = pat059;
      break;
    case '<':
      pattern_match = pat060;
      break;
    case '=':
      pattern_match = pat061;
      break;
    case '>':
      pattern_match = pat062;
      break;
    case 'A':
      pattern_match = pat065;
      break;
    case 'B':
      pattern_match = pat066;
      break;
    case 'C':
      pattern_match = pat067;
      break;
    case 'D':
      pattern_match = pat068;
      break;
    case 'E':
      pattern_match = pat069;
      break;
    case 'F':
      pattern_match = pat070;
      break;
    case 'G':
      pattern_match = pat071;
      break;
    case 'H':
      pattern_match = pat072;
      break;
    case 'I':
      pattern_match = pat073;
      break;
    case 'J':
      pattern_match = pat074;
      break;
    case 'K':
      pattern_match = pat075;
      break;
    case 'L':
      pattern_match = pat076;
      break;
    case 'M':
      pattern_match = pat077;
      break;
    case 'N':
      pattern_match = pat078;
      break;
    case 'O':
      pattern_match = pat079;
      break;
    case 'P':
      pattern_match = pat080;
      break;
    case 'Q':
      pattern_match = pat081;
      break;
    case 'R':
      pattern_match = pat082;
      break;
    case 'S':
      pattern_match = pat083;
      break;
    case 'T':
      pattern_match = pat084;
      break;
    case 'U':
      pattern_match = pat085;
      break;
    case 'V':
      pattern_match = pat086;
      break;
    case 'W':
      pattern_match = pat087;
      break;
    case 'X':
      pattern_match = pat088;
      break;
    case 'Y':
      pattern_match = pat089;
      break;
    case 'Z':
      pattern_match = pat090;
      break;
    case '[':
      pattern_match = pat091;
      break;
    case ']':
      pattern_match = pat093;
      break;
    case '_':
      pattern_match = pat095;
      break;
    case '~':
      pattern_match = pat126;
      break;

    default:
      pattern_match = pat032;
      break;
  }
/*
  Copy data from the pattern to the output.
*/
  for ( i = 0; i < 7; i++ ) {
    for ( j = 0; j < 5; j++ ) {
      *pattern = *pattern_match;
      pattern = pattern + 1;
      pattern_match = pattern_match + 1;
    }
  }

  return;
}

/******************************************************************************/

int pbm_check_data ( int xsize, int ysize, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PBM_CHECK_DATA checks the data for an ASCII portable bit map file.

  Example:

    P1
    # feep.pbm
    24 7
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 1 0
    0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0
    0 1 0 0 0 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

  Modified:

    28 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBM_CHECK_DATA, is
    1, if an error was detected, or
    0, if the data was legal.
*/
  int  i;
  int *indexb;
  int  j;

  if ( xsize <= 0 ) {
    printf ( "\n" );
    printf ( "PBM_CHECK_DATA: 0 >= XSIZE = %d.\n", xsize );
    return 1;
  }

  if ( ysize <= 0 ) {
    printf ( "\n" );
    printf ( "PBM_CHECK_DATA: 0 >= YSIZE = %d.\n", ysize );
    return 1;
  }

  if ( barray == NULL ) {
    printf ( "\n" );
    printf ( "PBM_CHECK_DATA: Null pointer to data.\n" );
    return 1;
  }

  indexb = barray;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      if ( *indexb != 0 && *indexb != 1 ) {
        printf ( "\n" );
        printf ( "PBM_CHECK_DATA: b(%d,%d) = %d.\n", i, j, *indexb );
        return 1;
      }

      indexb = indexb + 1;
    }
  }

  return 0;
}
/******************************************************************************/

int pbma_read ( char *filein_name, int *xsize, int *ysize, int **barray ) {

/******************************************************************************/

/*
  Purpose:

    PBMA_READ reads the header and data from an ASCII portable bit map file.
 
  Modified:
 
    27 September 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the ASCII
    portable bit map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int **BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBMA_READ, is
    1, if an error was detected, or
    0, if the file was read.
*/
  FILE *filein;
  int   numbytes;
  int   result;

  filein = fopen ( filein_name, "r" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "PBMA_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return 1;
  }
/*
  Read the header.
*/
  result = pbma_read_header ( filein, xsize, ysize );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMA_READ: Fatal error!\n" );
    printf ( "  PBMA_READ_HEADER failed.\n" );
    return 1;
  }
/*
  Allocate storage for the data.
*/
  numbytes = (*xsize) * (*ysize) * sizeof ( int );

  *barray = ( int * ) malloc ( numbytes );

  if ( *barray == NULL ) {
    printf ( "\n" );
    printf ( "PBMA_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }
/*
  Read the data.
*/
  result = pbma_read_data ( filein, *xsize, *ysize, *barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMA_READ: Fatal error!\n" );
    printf ( "  PBMA_READ_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( filein );

  return 0;
}
/******************************************************************************/

int pbma_read_data ( FILE *filein, int xsize, int ysize, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PBMA_READ_DATA reads the data in an ASCII portable bit map file.

  Example:

    P1
    # feep.pbm
    24 7
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 1 0
    0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0
    0 1 0 0 0 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

  Modified:

    27 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the ASCII
    portable bit map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, int *BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBMA_READ_DATA, is
    1, if an error was detected, or
    0, if the data was read.
*/
  int   count;
  int   i;
  int  *indexb;
  int   ival;
  int   j;
  int   need_line;
  int   need_value;
  char *next;
  int   width;

  indexb = barray;
  need_line = TRUE;
  next = NULL;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      need_value = TRUE;

      while ( need_value == TRUE ) {

        if ( need_line == TRUE ) {

          next = fgets ( line, MAX_LEN, filein );

          if ( next == NULL ) {
            printf ( "\n" );
            printf ( "PBMA_READ_DATA: Fatal error.\n" );
            printf ( "  End of input.\n" );
            return 1;
          }
          need_line = FALSE;
        }

        count = sscanf ( next, "%s%n", word, &width );

        if ( count == EOF ) {
          need_line = TRUE;
        }
        else {
          next = next + width;
          count = sscanf ( word, "%d", &ival );
          *indexb = ival;
          indexb = indexb + 1;
          need_value = FALSE;
        }

      }
    }
  }

  return 0;
}
/******************************************************************************/

int pbma_read_header ( FILE *filein, int *xsize, int *ysize ) {

/******************************************************************************/

/*
  Purpose:

    PBMA_READ_HEADER reads the header of an ASCII portable bit map file.

  Example:

    P1
    # feep.pbm
    24 7
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 1 0
    0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0
    0 1 0 0 0 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

  Modified:

    27 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the ASCII
    portable bit map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int PBMA_READ_HEADER, is
    1, if an error was detected, or
    0, if the header was read.
*/
  int   count;
  char *next;
  int   step;
  int   width;

  step = 0;

  while ( ( next = fgets ( line, MAX_LEN, filein ) ) != NULL ) {

    if ( line[0] == '#' ) {
      continue;
    }

    if ( step == 0 ) {
      count = sscanf ( next, "%s%n", word, &width );
      if ( count == EOF ) {
        continue;
      }
      next = next + width;
      if ( strcmp ( word, "P1" ) != 0 && strcmp ( word, "p1" ) != 0 ) {
        printf ( "\n" );
        printf ( "PBMA_READ_HEADER: Fatal error.\n" );
        printf ( "  Bad magic number = %s.\n", word );
        return 1;
      }
      step = 1;
    }

    if ( step == 1 ) {

      count = sscanf ( next, "%d%n", xsize, &width );
      next = next + width;
      if ( count == EOF ) {
        continue;
      }
      step = 2;
    }

    if ( step == 2 ) {

      count = sscanf ( next, "%d%n", ysize, &width );
      next = next + width;
      if ( count == EOF ) {
        continue;
      }
      return 0;
    }

  }

  printf ( "\n" );
  printf ( "PBMA_READ_HEADER: Fatal error.\n" );
  printf ( "  End of input.\n" );
  return 1;

}
/******************************************************************************/

int pbma_read_test ( char *filein_name ) {

/******************************************************************************/

/*
  Purpose:

    PBMA_READ_TEST tests the ASCII portable bit map read routines.

  Modified:

    28 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the ASCII
    portable bit map data.

    Output, int PBMA_READ_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/

  int *barray;
  int  result;
  int  xsize;
  int  ysize;

  barray = NULL;
/*
  Read the data.
*/
  result = pbma_read ( filein_name, &xsize, &ysize, &barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMA_READ_TEST: Fatal error!\n" );
    printf ( "  PBMA_READ failed.\n" );
    if ( barray != NULL ) {
      free ( barray );
    }
    return 1;
  }
/*
  Check the data.
*/
  result = pbm_check_data ( xsize, ysize, barray );

  if ( barray != NULL ) {
    free ( barray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "  PBM_CHECK_DATA reports bad data from the file.\n" );
    return 1;
  }

  printf ( "\n" );
  printf ( "  PBM_CHECK_DATA passes the data from the file.\n" );

  return 0;
}
/******************************************************************************/

int pbma_write ( char *fileout_name, int xsize, int ysize, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PBMA_WRITE writes the header and data for an ASCII portable bit map file.
 
  Modified:
 
    27 September 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the ASCII
    portable bit map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBMA_WRITE, is
    1, if an error was detected, or
    0, if the file was written.
*/
  FILE *fileout;
  int   result;

  fileout = fopen ( fileout_name, "w" );

  if ( fileout == NULL ) {
    printf ( "\n" );
    printf ( "PBMA_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", fileout_name );
    return 1;
  }
/*
  Write the header.
*/
  result = pbma_write_header ( fileout, fileout_name, xsize, ysize );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMA_WRITE: Fatal error!\n" );
    printf ( "  PBMA_WRITE_HEADER failed.\n" );
    return 1;
  }
/*
  Write the data.
*/
  result = pbma_write_data ( fileout, xsize, ysize, barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMA_WRITE: Fatal error!\n" );
    printf ( "  PBMA_WRITE_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( fileout );

  return 0;
}
/******************************************************************************/

int pbma_write_data ( FILE *fileout, int xsize, int ysize, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PBMA_WRITE_DATA writes the data for an ASCII portable bit map file.

  Example:

    P1
    # feep.pbm
    24 7
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 1 0
    0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0
    0 1 0 0 0 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

  Modified:

    27 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the ASCII
    portable bit map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBMA_WRITE_DATA, is
    1, if an error was detected, or
    0, if the data was written.
*/
  int  i;
  int *indexb;
  int  j;
  int  numval;

  indexb = barray;
  numval = 0;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      fprintf ( fileout, "%d", *indexb );

      numval = numval + 1;
      indexb = indexb + 1;

      if ( numval%35 == 0 || numval == xsize * ysize ) {
        fprintf ( fileout, "\n" );
      }
      else {
        fprintf ( fileout, " " );
      }

    }
  }
  return 0;
}
/******************************************************************************/

int pbma_write_header ( FILE *fileout, char *fileout_name, int xsize, 
  int ysize ) {

/******************************************************************************/

/*
  Purpose:

    PBMA_WRITE_HEADER writes the header of an ASCII portable bit map file.

  Example:

    P1
    # feep.pbm
    24 7
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 1 0
    0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 1 0
    0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0
    0 1 0 0 0 0 0 1 1 1 1 0 0 1 1 1 1 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

  Modified:

    27 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the ASCII
    portable bit map data.

    Input, char *FILEOUT_NAME, the name of the output file.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, int PBMA_WRITE_HEADER, is
    1, if an error was detected, or
    0, if the header was written.
*/

  fprintf ( fileout, "P1\n" );
  fprintf ( fileout, "# %s\n", fileout_name );
  fprintf ( fileout, "%d %d\n", xsize, ysize );

  return 0;
}
/******************************************************************************/

int pbma_write_test ( char *fileout_name ) {

/******************************************************************************/

/*
  Purpose:

    PBMA_WRITE_TEST tests the ASCII portable bit map write routines.

  Modified:

    27 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the ASCII
    portable bit map data.

    Output, int PBMA_WRITE_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/

  int *barray;
  int  i;
  int *indexb;
  int  j;
  int  k1;
  int  k2;
  int  numbytes;
  int  result;
  int  xsize;
  int  ysize;
/*
  Set the data.
*/
  xsize = 250;
  ysize = 150;

  barray = NULL;
  numbytes = xsize * ysize * sizeof ( int );  
  barray = ( int * ) malloc ( numbytes );

  if ( barray == NULL ) {
    printf ( "\n" );
    printf ( "PBMA_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  indexb = barray;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      k1 = i - xsize / 2;
      if ( k1 < 0 ) {
        k1 = - k1;
      }
      k2 = j - ysize / 2;
      if ( k2 < 0 ) {
        k2 = - k2;
      }
      if ( k1 > k2 ) {
        *indexb = k1%2;
      }
      else {
        *indexb = k2%2;
      }
      indexb = indexb + 1;
    }
  }

  result = pbma_write ( fileout_name, xsize, ysize, barray );

  if ( barray != NULL ) {
    free ( barray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMA_WRITE_TEST: Fatal error!\n" );
    printf ( "  PBMA_WRITE failed.\n" );
    return 1;
  }

  return 0;
}
/******************************************************************************/

int pbmb_read ( char *filein_name, int *xsize, int *ysize, int **barray ) {

/******************************************************************************/

/*
  Purpose:

    PBMB_READ reads the header and data from a binary portable bit map file.
 
  Modified:
 
    04 October 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the binary
    portable bit map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int **BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBMB_READ, is
    1, if an error was detected, or
    0, if the file was read.
*/
  FILE *filein;
  int   numbytes;
  int   result;

  filein = fopen ( filein_name, "rb" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "PBMB_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return 1;
  }
/*
  Read the header.
*/
  result = pbmb_read_header ( filein, xsize, ysize );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMB_READ: Fatal error!\n" );
    printf ( "  PBMB_READ_HEADER failed.\n" );
    return 1;
  }
/*
  Allocate storage for the data.
*/
  numbytes = (*xsize) * (*ysize) * sizeof ( int );

  *barray = ( int * ) malloc ( numbytes );

  if ( *barray == NULL ) {
    printf ( "\n" );
    printf ( "PBMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }
/*
  Read the data.
*/
  result = pbmb_read_data ( filein, *xsize, *ysize, *barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMB_READ: Fatal error!\n" );
    printf ( "  PBMB_READ_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( filein );

  return 0;
}
/******************************************************************************/

int pbmb_read_data ( FILE *filein, int xsize, int ysize, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PBMB_READ_DATA reads the data in a binary portable bit map file.

  Modified:

    29 April 1999

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the binary
    portable bit map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, int *BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBMB_READ_DATA, is
    1, if an error was detected, or
    0, if the data was read.
*/
  int           bit;
  int           c;
  unsigned char c2;
  int           i;
  int          *indexb;
  int           j;
  int           k;
  int           numbyte;

  indexb = barray;
  numbyte = 0;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      if ( i%8 == 0 ) {

        c = fgetc ( filein );

        if ( c == EOF ) {
          printf ( "\n" );
          printf ( "PBMB_READ_DATA: Failed reading data byte %d.\n", numbyte );
          return 1;
        }
        c2 = ( unsigned char ) c;
        numbyte = numbyte + 1;
      }

      k = 7 - i%8;
      bit = ( c2 >> k )%2;

      *indexb = bit;
      indexb = indexb + 1;
    }
  }
  return 0;
}
/******************************************************************************/

int pbmb_read_header ( FILE *filein, int *xsize, int *ysize ) {

/******************************************************************************/

/*
  Purpose:

    PBMB_READ_HEADER reads the header of a binary portable bit map file.

  Modified:

    29 April 1999

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the binary
    portable bit map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int PBMB_READ_HEADER, is
    1, if an error was detected, or
    0, if the header was read.
*/
  int   c_val;
  int   count;
  int   flag;
  int   nchar;
  int   state;
  char  string[80];

  state = 0;
  nchar = 0;

  for ( ;; ) {

    c_val = fgetc ( filein );

    if ( c_val == EOF ) {
      return 1;
    }
/*
  If not whitespace, add the character to the current string.
*/
    flag = isspace ( c_val );

    if ( !flag ) {
      string[nchar] = c_val;
      nchar = nchar + 1;
    }
/*
  See if we have finished an old item, or begun a new one.
*/
    if ( state == 0 ) {
      if ( !flag ) {
        state = 1;
      }
      else {
        return 1;
      }
    }
    else if ( state == 1 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        if ( strcmp ( string, "P4" ) != 0 && strcmp ( string, "p4" ) != 0 ) {
          printf ( "\n" );
          printf ( "PBMB_READ_HEADER: Fatal error.\n" );
          printf ( "  Bad magic number = %s.\n", string );
          return 1;
        }
        nchar = 0;
        state = 2;
      }
    }
    else if ( state == 2 ) {
      if ( !flag ) {
        state = 3;
      }
    }
    else if ( state == 3 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", xsize );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        state = 4;
      }
    }
    else if ( state == 4 ) {
      if ( !flag ) {
        state = 5;
      }
    }
    else if ( state == 5 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", ysize );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        return 0;
      }
    }
  }
}
/******************************************************************************/

int pbmb_read_test ( char *filein_name ) {

/******************************************************************************/

/*
  Purpose:

    PBMB_READ_TEST tests the binary portable bit map read routines.

  Modified:

    04 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the binary
    portable bit map data.

    Output, int PBMB_READ_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/

  int *barray;
  int  result;
  int  xsize;
  int  ysize;

  barray = NULL;
/*
  Read the data.
*/
  result = pbmb_read ( filein_name, &xsize, &ysize, &barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMB_READ_TEST: Fatal error!\n" );
    printf ( "  PBMB_READ failed.\n" );
    if ( barray != NULL ) {
      free ( barray );
    }
    return 1;
  }
/*
  Check the data.
*/
  result = pbm_check_data ( xsize, ysize, barray );

  if ( barray != NULL ) {
    free ( barray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "  PBM_CHECK_DATA reports bad data from the file.\n" );
    return 1;
  }

  printf ( "\n" );
  printf ( "  PBM_CHECK_DATA passes the data from the file.\n" );

  return 0;
}
/******************************************************************************/

int pbmb_write ( char *fileout_name, int xsize, int ysize, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PBMB_WRITE writes the header and data for a binary portable bit map file.
 
  Modified:
 
    04 October 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the binary
    portable bit map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBMB_WRITE, is
    1, if an error was detected, or
    0, if the file was written.
*/
  FILE *fileout;
  int   result;

  fileout = fopen ( fileout_name, "wb" );

  if ( fileout == NULL ) {
    printf ( "\n" );
    printf ( "PBMB_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", fileout_name );
    return 1;
  }
/*
  Write the header.
*/
  result = pbmb_write_header ( fileout, xsize, ysize );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMB_WRITE: Fatal error!\n" );
    printf ( "  PBMB_WRITE_HEADER failed.\n" );
    return 1;
  }
/*
  Write the data.
*/
  result = pbmb_write_data ( fileout, xsize, ysize, barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMB_WRITE: Fatal error!\n" );
    printf ( "  PBMB_WRITE_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( fileout );

  return 0;
}
/******************************************************************************/

int pbmb_write_data ( FILE *fileout, int xsize, int ysize, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PBMB_WRITE_DATA writes the data for a binary portable bit map file.

  Modified:

    04 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the binary
    portable bit map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *BARRAY, the array of XSIZE by YSIZE data values.

    Output, int PBMB_WRITE_DATA, is
    1, if an error was detected, or
    0, if the data was written.
*/
  int           bit;
  unsigned char c;
  int           i;
  int          *indexb;
  int           j;
  int           k;

  indexb = barray;
  c = 0;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      k = 7 - i%8;
      bit = (*indexb)%2;
      c = c | ( bit << k );

      indexb = indexb + 1;

      if ( (i+1)%8 == 0 || i == ( xsize - 1 ) ) {
        fputc ( c, fileout );
        c = 0;
      }

    }
  }
  return 0;
}
/******************************************************************************/

int pbmb_write_header ( FILE *fileout, int xsize, int ysize ) {

/******************************************************************************/

/*
  Purpose:

    PBMB_WRITE_HEADER writes the header of a binary portable bit map file.

  Modified:

    04 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the binary
    portable bit map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, int PBMB_WRITE_HEADER, is
    1, if an error was detected, or
    0, if the header was written.
*/

  fprintf ( fileout, "P4 %d %d ", xsize, ysize );

  return 0;
}
/******************************************************************************/

int pbmb_write_test ( char *fileout_name ) {

/******************************************************************************/

/*
  Purpose:

    PBMB_WRITE_TEST tests the binary portable bit map write routines.

  Modified:

    04 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the binary
    portable bit map data.

    Output, int PBMB_WRITE_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/

  int *barray;
  int  i;
  int *indexb;
  int  j;
  int  k1;
  int  k2;
  int  numbytes;
  int  result;
  int  xsize;
  int  ysize;
/*
  Set the data.
*/
  xsize = 250;
  ysize = 150;

  barray = NULL;
  numbytes = xsize * ysize * sizeof ( int );  
  barray = ( int * ) malloc ( numbytes );

  if ( barray == NULL ) {
    printf ( "\n" );
    printf ( "PBMB_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  indexb = barray;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      k1 = i - xsize / 2;
      if ( k1 < 0 ) {
        k1 = - k1;
      }
      k2 = j - ysize / 2;
      if ( k2 < 0 ) {
        k2 = - k2;
      }
      if ( k1 > k2 ) {
        *indexb = k1%2;
      }
      else {
        *indexb = k2%2;
      }
      indexb = indexb + 1;
    }
  }

  result = pbmb_write ( fileout_name, xsize, ysize, barray );

  if ( barray != NULL ) {
    free ( barray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PBMB_WRITE_TEST: Fatal error!\n" );
    printf ( "  PBMB_WRITE failed.\n" );
    return 1;
  }

  return 0;
}
/******************************************************************************/

int pgm_check_data ( int xsize, int ysize, int maxgray, int *garray ) {

/******************************************************************************/

/*
  Purpose:

    PGM_CHECK_DATA checks the data for an ASCII portable gray map file.

  Example:

    P2
    # feep.pgm
    24 7
    15
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    0  3  3  3  3  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0 15  0
    0  3  3  3  0  0  0  7  7  7  0  0  0 11 11 11  0  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0  0  0
    0  3  0  0  0  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15  0  0  0  0
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    
  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXGRAY, the maximum gray value.

    Input, int *GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGM_CHECK_DATA, is
    1, if an error was detected, or
    0, if the data was legal.
*/
  int  i;
  int *indexg;
  int  j;

  if ( xsize <= 0 ) {
    printf ( "\n" );
    printf ( "PGM_CHECK_DATA: 0 >= XSIZE = %d.\n", xsize );
    return 1;
  }
  if ( ysize <= 0 ) {
    printf ( "\n" );
    printf ( "PGM_CHECK_DATA: 0 >= YSIZE = %d.\n", ysize );
    return 1;
  }

  if ( garray == NULL ) {
    printf ( "\n" );
    printf ( "PGM_CHECK_DATA: Null pointer to data.\n" );
    return 1;
  }

  indexg = garray;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      if ( *indexg < 0  ) {
        printf ( "\n" );
        printf ( "PGM_CHECK_DATA: G(%d,%d) = %d < 0.\n", 
          i, j, *indexg );
        return 1;
      }
      else if ( *indexg > maxgray ) {
        printf ( "\n" );
        printf ( "PGM_CHECK_DATA: G(%d,%d) = %d > %d.\n", 
          i, j, *indexg, maxgray );
        return 1;
      }

      indexg = indexg + 1;
    }
  }

  return 0;
}
/******************************************************************************/

int pgma_read ( char *filein_name, int *xsize, int *ysize, int *maxgray,
  int **garray ) {

/******************************************************************************/

/*
  Purpose:

    PGMA_READ reads the header and data from an ASCII portable gray map file.
 
  Modified:
 
    29 September 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the ASCII
    portable gray map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int *MAXGRAY, the maximum gray value.

    Output, int **GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGMA_READ, is
    1, if an error was detected, or
    0, if the file was read.
*/
  FILE *filein;
  int   numbytes;
  int   result;

  filein = fopen ( filein_name, "r" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "PGMA_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return 1;
  }
/*
  Read the header.
*/
  result = pgma_read_header ( filein, xsize, ysize, maxgray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMA_READ: Fatal error!\n" );
    printf ( "  PGMA_READ_HEADER failed.\n" );
    return 1;
  }
/*
  Allocate storage for the data.
*/
  numbytes = (*xsize) * (*ysize) * sizeof ( int );

  *garray = ( int * ) malloc ( numbytes );

  if ( *garray == NULL ) {
    printf ( "\n" );
    printf ( "PGMA_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }
/*
  Read the data.
*/
  result = pgma_read_data ( filein, *xsize, *ysize, *garray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMA_READ: Fatal error!\n" );
    printf ( "  PGMA_READ_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( filein );

  return 0;
}
/******************************************************************************/

int pgma_read_data ( FILE *filein, int xsize, int ysize, int *garray ) {

/******************************************************************************/

/*
  Purpose:

    PGMA_READ_DATA reads the data in an ASCII portable gray map file.

  Example:

    P2
    # feep.pgm
    24 7
    15
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    0  3  3  3  3  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0 15  0
    0  3  3  3  0  0  0  7  7  7  0  0  0 11 11 11  0  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0  0  0
    0  3  0  0  0  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15  0  0  0  0
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    
  Modified:

    29 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the ASCII
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, int *GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGMA_READ_DATA, is
    1, if an error was detected, or
    0, if the data was read.
*/
  int   count;
  int   i;
  int  *indexg;
  int   ival;
  int   j;
  int   need_line;
  int   need_value;
  char *next;
  int   width;

  indexg = garray;
  need_line = TRUE;
  next = NULL;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      need_value = TRUE;

      while ( need_value == TRUE ) {

        if ( need_line == TRUE ) {

          next = fgets ( line, MAX_LEN, filein );

          if ( next == NULL ) {
            printf ( "\n" );
            printf ( "PGMA_READ_DATA: Fatal error.\n" );
            printf ( "  End of input.\n" );
            return 1;
          }
          need_line = FALSE;
        }

        count = sscanf ( next, "%s%n", word, &width );

        if ( count == EOF ) {
          need_line = TRUE;
        }
        else {
          next = next + width;
          count = sscanf ( word, "%d", &ival );
          *indexg = ival;
          indexg = indexg + 1;
          need_value = FALSE;
        }

      }
    }
  }

  return 0;
}
/******************************************************************************/

int pgma_read_header ( FILE *filein, int *xsize, int *ysize, int *maxgray ) {

/******************************************************************************/

/*
  Purpose:

    PGMA_READ_HEADER reads the header of an ASCII portable gray map file.

  Example:

    P2
    # feep.pgm
    24 7
    15
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    0  3  3  3  3  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0 15  0
    0  3  3  3  0  0  0  7  7  7  0  0  0 11 11 11  0  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0  0  0
    0  3  0  0  0  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15  0  0  0  0
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    
  Modified:

    29 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the ASCII
    portable gray map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int *MAXGRAY, the maximum gray value.

    Output, int PGMA_READ_HEADER, is
    1, if an error was detected, or
    0, if the header was read.
*/
  int   count;
  char *next;
  int   step;
  int   width;

  step = 0;

  while ( ( next = fgets ( line, MAX_LEN, filein ) ) != NULL ) {

    if ( line[0] == '#' ) {
      continue;
    }

    if ( step == 0 ) {
      count = sscanf ( next, "%s%n", word, &width );
      if ( count == EOF ) {
        continue;
      }
      next = next + width;
      if ( strcmp ( word, "P2" ) != 0 && strcmp ( word, "p2" ) != 0 ) {
        printf ( "\n" );
        printf ( "PGMA_READ_HEADER: Fatal error.\n" );
        printf ( "  Bad magic number = %s.\n", word );
        return 1;
      }
      step = 1;
    }

    if ( step == 1 ) {

      count = sscanf ( next, "%d%n", xsize, &width );
      next = next + width;
      if ( count == EOF ) {
        continue;
      }
      step = 2;
    }

    if ( step == 2 ) {

      count = sscanf ( next, "%d%n", ysize, &width );
      next = next + width;
      if ( count == EOF ) {
        continue;
      }
      step = 3;
    }
    if ( step == 3 ) {

      count = sscanf ( next, "%d%n", maxgray, &width );
      next = next + width;
      if ( count == EOF ) {
        continue;
      }
      return 0;
    }

  }

  printf ( "\n" );
  printf ( "PGMA_READ_HEADER: Fatal error.\n" );
  printf ( "  End of input.\n" );
  return 1;

}
/******************************************************************************/

int pgma_read_test ( char *filein_name ) {

/******************************************************************************/

/*
  Purpose:

    PGMA_READ_TEST tests the ASCII portable gray map read routines.

  Modified:

    29 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the ASCII
    portable gray map data.

    Output, int PGMA_READ_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/
  int *garray;
  int  maxgray;
  int  result;
  int  xsize;
  int  ysize;

  garray = NULL;
/*
  Read the data.
*/
  result = pgma_read ( filein_name, &xsize, &ysize, &maxgray, &garray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMA_READ_TEST: Fatal error!\n" );
    printf ( "  PGMA_READ failed.\n" );
    if ( garray != NULL ) {
      free ( garray );
    }
    return 1;
  }
/*
  Check the data.
*/
  result = pgm_check_data ( xsize, ysize, maxgray, garray );

  if ( garray != NULL ) {
    free ( garray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "  PGM_CHECK_DATA reports bad data from the file.\n" );
    return 1;
  }

  printf ( "\n" );
  printf ( "  PGM_CHECK_DATA passes the data from the file.\n" );

  return 0;
}
/******************************************************************************/

int pgma_write ( char *fileout_name, int xsize, int ysize, int maxgray,
  int *garray ) {

/******************************************************************************/

/*
  Purpose:

    PGMA_WRITE writes the header and data for an ASCII portable gray map file.
 
  Modified:
 
    28 September 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the ASCII
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXGRAY, the maximum gray value.

    Input, int *GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGMA_WRITE, is
    1, if an error was detected, or
    0, if the file was written.
*/
  FILE *fileout;
  int   result;

  fileout = fopen ( fileout_name, "w" );

  if ( fileout == NULL ) {
    printf ( "\n" );
    printf ( "PGMA_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", fileout_name );
    return 1;
  }
/*
  Write the header.
*/
  result = pgma_write_header ( fileout, fileout_name, xsize, ysize, maxgray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMA_WRITE: Fatal error!\n" );
    printf ( "  PGMA_WRITE_HEADER failed.\n" );
    return 1;
  }
/*
  Write the data.
*/
  result = pgma_write_data ( fileout, xsize, ysize, garray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMA_WRITE: Fatal error!\n" );
    printf ( "  PGMA_WRITE_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( fileout );

  return 0;
}
/******************************************************************************/

int pgma_write_data ( FILE *fileout, int xsize, int ysize, int *garray ) {

/******************************************************************************/

/*
  Purpose:

    PGMA_WRITE_DATA writes the data for an ASCII portable gray map file.

  Example:

    P2
    # feep.pgm
    24 7
    15
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    0  3  3  3  3  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0 15  0
    0  3  3  3  0  0  0  7  7  7  0  0  0 11 11 11  0  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0  0  0
    0  3  0  0  0  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15  0  0  0  0
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    
  Modified:

    28 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the ASCII
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGMA_WRITE_DATA, is
    1, if an error was detected, or
    0, if the data was written.
*/
  int  i;
  int *indexg;
  int  j;
  int  numval;

  indexg = garray;
  numval = 0;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      fprintf ( fileout, "%d", *indexg );

      numval = numval + 1;
      indexg = indexg + 1;

      if ( numval%10 == 0 || i == xsize - 1 || numval == xsize * ysize ) {
        fprintf ( fileout, "\n" );
      }
      else {
        fprintf ( fileout, " " );
      }

    }
  }
  return 0;
}
/******************************************************************************/

int pgma_write_header ( FILE *fileout, char *fileout_name, int xsize, 
  int ysize, int maxgray ) {

/******************************************************************************/

/*
  Purpose:

    PGMA_WRITE_HEADER writes the header of an ASCII portable gray map file.

  Example:

    P2
    # feep.pgm
    24 7
    15
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    0  3  3  3  3  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0 15  0
    0  3  3  3  0  0  0  7  7  7  0  0  0 11 11 11  0  0  0 15 15 15 15  0
    0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0  0  0
    0  3  0  0  0  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15  0  0  0  0
    0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
    
  Modified:

    27 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the ASCII
    portable gray map data.

    Input, char *FILEOUT_NAME, the name of the file to contain the ASCII
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXGRAY, the maximum gray value.

    Output, int PGMA_WRITE_HEADER, is
    1, if an error was detected, or
    0, if the header was written.
*/

  fprintf ( fileout, "P2\n" );
  fprintf ( fileout, "# %s\n", fileout_name );
  fprintf ( fileout, "%d %d\n", xsize, ysize );
  fprintf ( fileout, "%d\n", maxgray );

  return 0;
}
/******************************************************************************/

int pgma_write_test ( char *fileout_name ) {

/******************************************************************************/

/*
  Purpose:

    PGMA_WRITE_TEST tests the ASCII portable gray map write routines.

  Modified:

    09 April 2001

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the ASCII
    portable gray map data.

    Output, int PGMA_WRITE_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/

  int *garray;
  int  i;
  int *indexg;
  int  j;
  int  maxgray;
  int  numbytes;
  int  result;
  int  xsize;
  int  ysize;
/*
  Set the data.
*/
  xsize = 300;
  ysize = 200;

  garray = NULL;
  numbytes = xsize * ysize * sizeof ( int );  
  garray = ( int * ) malloc ( numbytes );

  if ( garray == NULL ) {
    printf ( "\n" );
    printf ( "PGMA_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  indexg = garray;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      *indexg = (255/(j+1) ) * ( (i+1)%(j+1) );
      indexg = indexg + 1;
    }
  }
/*
  Compute the maximum.
*/
  maxgray = 0;
  indexg = garray;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      maxgray = MAX ( maxgray, *indexg );
      indexg = indexg + 1;
    }
  }

  result = pgma_write ( fileout_name, xsize, ysize, maxgray, garray );

  if ( garray != NULL ) {
    free ( garray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMA_WRITE_TEST: Fatal error!\n" );
    printf ( "  PGMA_WRITE failed.\n" );
    return 1;
  }

  return 0;
}
/******************************************************************************/

int pgmb_read ( char *filein_name, int *xsize, int *ysize, int *maxgray,
  int **garray ) {

/******************************************************************************/

/*
  Purpose:

    PGMB_READ reads the header and data from a binary portable gray map file.
 
  Modified:
 
    29 September 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the binary
    portable gray map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int *MAXGRAY, the maximum gray value.

    Output, int **GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGMB_READ, is
    1, if an error was detected, or
    0, if the file was read.
*/
  FILE *filein;
  int   numbytes;
  int   result;

  filein = fopen ( filein_name, "rb" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "PGMB_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return 1;
  }
/*
  Read the header.
*/
  result = pgmb_read_header ( filein, xsize, ysize, maxgray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMB_READ: Fatal error!\n" );
    printf ( "  PGMB_READ_HEADER failed.\n" );
    return 1;
  }
/*
  Allocate storage for the data.
*/
  numbytes = ( *xsize ) * ( *ysize ) * sizeof ( int );

  *garray = ( int * ) malloc ( numbytes );

  if ( *garray == NULL ) {
    printf ( "\n" );
    printf ( "PGMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }
/*
  Read the data.
*/
  result = pgmb_read_data ( filein, *xsize, *ysize, *garray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMB_READ: Fatal error!\n" );
    printf ( "  PGMB_READ_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( filein );

  return 0;
}
/******************************************************************************/

int pgmb_read_data ( FILE *filein, int xsize, int ysize, int *garray ) {

/******************************************************************************/

/*
  Purpose:

    PGMB_READ_DATA reads the data in a binary portable gray map file.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the binary
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, int *GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGMB_READ_DATA, is
    1, if an error was detected, or
    0, if the data was read.
*/
  int   i;
  int   int_val;
  int  *indexg;
  int   j;
  int   numval;

  indexg = garray;
  numval = 0;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      int_val = fgetc ( filein );

      if ( int_val == EOF ) {
        printf ( "\n" );
        printf ( "PGMB_READ_DATA: Failed reading data byte %d.\n", numval );
        return 1;
      }
      else {
        *indexg = int_val;
        indexg = indexg + 1;
      }
      numval = numval + 1;
    }
  }
  return 0;
}
/******************************************************************************/

int pgmb_read_header ( FILE *filein, int *xsize, int *ysize, int *maxgray ) {

/******************************************************************************/

/*
  Purpose:

    PGMB_READ_HEADER reads the header of a binary portable gray map file.

  Modified:

    29 April 1999

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the binary
    portable gray map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int *MAXGRAY, the maximum gray value.

    Output, int PGMB_READ_HEADER, is
    1, if an error was detected, or
    0, if the header was read.
*/
  int   c_val;
  int   count;
  int   flag;
  int   nchar;
  int   state;
  char  string[80];

  state = 0;
  nchar = 0;

  for ( ;; ) {

    c_val = fgetc ( filein );

    if ( c_val == EOF ) {
      return 1;
    }
/*
  If not whitespace, add the character to the current string.
*/
    flag = isspace ( c_val );

    if ( !flag ) {
      string[nchar] = c_val;
      nchar = nchar + 1;
    }
/*
  See if we have finished an old item, or begun a new one.
*/
    if ( state == 0 ) {
      if ( !flag ) {
        state = 1;
      }
      else {
        return 1;
      }
    }
    else if ( state == 1 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        if ( strcmp ( string, "P5" ) != 0 && strcmp ( string, "p5" ) != 0 ) {
          printf ( "\n" );
          printf ( "PGMB_READ_HEADER: Fatal error.\n" );
          printf ( "  Bad magic number = %s.\n", string );
          return 1;
        }
        nchar = 0;
        state = 2;
      }
    }
    else if ( state == 2 ) {
      if ( !flag ) {
        state = 3;
      }
    }
    else if ( state == 3 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", xsize );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        state = 4;
      }
    }
    else if ( state == 4 ) {
      if ( !flag ) {
        state = 5;
      }
    }
    else if ( state == 5 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", ysize );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        state = 6;
      }
    }
    else if ( state == 6 ) {
      if ( !flag ) {
        state = 7;
      }
    }
    else if ( state == 7 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", maxgray );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        return 0;
      }
    }
  }
}
/******************************************************************************/

int pgmb_read_test ( char *filein_name ) {

/******************************************************************************/

/*
  Purpose:

    PGMB_READ_TEST tests the binary portable gray map read routines.

  Modified:

    29 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the binary
    portable gray map data.

    Output, int PGMB_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/
  int *garray;
  int  maxgray;
  int  result;
  int  xsize;
  int  ysize;

  garray = NULL;
/*
  Read the data.
*/
  result = pgmb_read ( filein_name, &xsize, &ysize, &maxgray, &garray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMB_READ_TEST: Fatal error!\n" );
    printf ( "  PGMB_READ failed.\n" );
    if ( garray != NULL ) {
      free ( garray );
    }
    return 1;
  }
/*
  Check the data.
*/
  result = pgm_check_data ( xsize, ysize, maxgray, garray );

  if ( garray != NULL ) {
    free ( garray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "  PGM_CHECK_DATA reports bad data from the file.\n" );
    return 1;
  }

  printf ( "\n" );
  printf ( "  PGM_CHECK_DATA passes the data from the file.\n" );

  return 0;
}
/******************************************************************************/

int pgmb_write ( char *fileout_name, int xsize, int ysize, int maxgray,
  int *garray ) {

/******************************************************************************/

/*
  Purpose:

    PGMB_WRITE writes the header and data for a binary portable gray map file.
 
  Modified:
 
    29 September 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the binary
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXGRAY, the maximum gray value.

    Input, int *GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGMB_WRITE, is
    1, if an error was detected, or
    0, if the file was written.
*/
  FILE *fileout;
  int   result;

  fileout = fopen ( fileout_name, "wb" );

  if ( fileout == NULL ) {
    printf ( "\n" );
    printf ( "PGMB_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", fileout_name );
    return 1;
  }
/*
  Write the header.
*/
  result = pgmb_write_header ( fileout, xsize, ysize, maxgray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMB_WRITE: Fatal error!\n" );
    printf ( "  PGMB_WRITE_HEADER failed.\n" );
    return 1;
  }
/*
  Write the data.
*/
  result = pgmb_write_data ( fileout, xsize, ysize, garray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMB_WRITE: Fatal error!\n" );
    printf ( "  PGMB_WRITE_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( fileout );

  return 0;
}
/******************************************************************************/

int pgmb_write_data ( FILE *fileout, int xsize, int ysize, int *garray ) {

/******************************************************************************/

/*
  Purpose:

    PGMB_WRITE_DATA writes the data for a binary portable gray map file.

  Modified:

    29 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the binary
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *GARRAY, the array of XSIZE by YSIZE data values.

    Output, int PGMB_WRITE_DATA, is
    1, if an error was detected, or
    0, if the data was written.
*/
  int  i;
  int *indexg;
  int  j;

  indexg = garray;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      fputc ( *indexg, fileout );
      indexg = indexg + 1;
    }
  }

  return 0;
}
/******************************************************************************/

int pgmb_write_header ( FILE *fileout, int xsize, int ysize, int maxgray ) {

/******************************************************************************/

/*
  Purpose:

    PGMB_WRITE_HEADER writes the header of a binary portable gray map file.

  Modified:

    29 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the binary
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXGRAY, the maximum gray value.

    Output, int PGMB_WRITE_HEADER, is
    1, if an error was detected, or
    0, if the header was written.
*/

  fprintf ( fileout, "P5 %d %d %d ", xsize, ysize, maxgray );

  return 0;
}
/******************************************************************************/

int pgmb_write_test ( char *fileout_name ) {

/******************************************************************************/

/*
  Purpose:

    PGMB_WRITE_TEST tests the binary portable gray map write routines.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the binary
    portable gray map data.

    Output, int PGMB_WRITE_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/

  int *garray;
  int  i;
  int *indexg;
  int  j;
  int  maxgray;
  int  numbytes;
  int  result;
  int  xsize;
  int  ysize;
/*
  Set the data.
*/
  xsize = 300;
  ysize = 200;

  garray = NULL;
  numbytes = xsize * ysize * sizeof ( int );  
  garray = ( int * ) malloc ( numbytes );

  if ( garray == NULL ) {
    printf ( "\n" );
    printf ( "PGMB_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  indexg = garray;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      *indexg = (i+1)%(j+1);
      indexg = indexg + 1;
    }
  }
/*
  Compute the maximum.
*/
  maxgray = 0;
  indexg = garray;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      maxgray = MAX ( maxgray, *indexg );
      indexg = indexg + 1;
    }
  }

  result = pgmb_write ( fileout_name, xsize, ysize, maxgray, garray );

  if ( garray != NULL ) {
    free ( garray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PGMB_WRITE_TEST: Fatal error!\n" );
    printf ( "  PGMB_WRITE failed.\n" );
    return 1;
  }

  return 0;
}
/******************************************************************************/

int ppm_check_data ( int xsize, int ysize, int maxrgb, int *rarray,
  int *garray, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PPM_CHECK_DATA checks the data for an ASCII portable pixel map file.

  Example:

    P3
    # feep.ppm
    4 4
    15
     0  0  0    0  0  0    0  0  0   15  0 15
     0  0  0    0 15  7    0  0  0    0  0  0
     0  0  0    0  0  0    0 15  7    0  0  0
    15  0 15    0  0  0    0  0  0    0  0  0

  Modified:

    29 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXRGB, the maximum RGB value.

    Input, int *RARRAY, *GARRAY, *BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPM_CHECK_DATA, is
    1, if an error was detected, or
    0, if the data was legal.
*/
  int  i;
  int *index;
  int  j;
  int  k;

  if ( xsize <= 0 ) {
    printf ( "\n" );
    printf ( "PPM_CHECK_DATA: 0 >= XSIZE = %d.\n", xsize );
    return 1;
  }

  if ( ysize <= 0 ) {
    printf ( "\n" );
    printf ( "PPM_CHECK_DATA: 0 >= YSIZE = %d.\n", ysize );
    return 1;
  }

  if ( rarray == NULL || garray == NULL || barray == NULL ) {
    printf ( "\n" );
    printf ( "PPM_CHECK_DATA: Null pointer to data.\n" );
    return 1;
  }

  for ( k = 0; k < 3; k++ ) {

    if ( k == 0 ) {
      index = rarray;
    }
    else if ( k == 1 ) {
      index = garray;
    }
    else if ( k == 2 ) {
      index = barray;
    }

    for ( j = 0; j < ysize; j++ ) {
      for ( i = 0; i < xsize; i++ ) {

        if ( *index < 0 ) {
          if ( k == 0 ) {
            printf ( "\n" );
            printf ( "PPM_CHECK_DATA: R(%d,%d) = %d < 0.\n", i, j, *index );
          }
          else if ( k == 1 ) {
            printf ( "\n" );
            printf ( "PPM_CHECK_DATA: G(%d,%d) = %d < 0.\n", i, j, *index );
          }
          else if ( k == 2 ) {
            printf ( "\n" );
            printf ( "PPM_CHECK_DATA: B(%d,%d) = %d < 0.\n", i, j, *index );
          }
          return 1;
        }
        else if ( *index > maxrgb ) {
          if ( k == 0 ) {
            printf ( "\n" );
            printf ( "PPM_CHECK_DATA: R(%d,%d) = %d > %d.\n", i, j, *index, 
              maxrgb );
          }
          else if ( k == 1 ) {
            printf ( "\n" );
            printf ( "PPM_CHECK_DATA: G(%d,%d) = %d > %d.\n", i, j, *index, 
              maxrgb );
          }
          else if ( k == 2 ) {
            printf ( "\n" );
            printf ( "PPM_CHECK_DATA: B(%d,%d) = %d > %d.\n", i, j, *index, 
              maxrgb );
          }
          return 1;
        }

        index = index + 1;
      }
    } 
  }

  return 0;
}
/******************************************************************************/

int ppma_read ( char *filein_name, int *xsize, int *ysize, int *maxrgb,
  int **rarray, int **garray, int **barray ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_READ reads the header and data from an ASCII portable pixel map file.
 
  Modified:
 
    03 October 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the ASCII
    portable pixel map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int *MAXRGB, the maximum RGB value.

    Output, int **RARRAY, **GARRAY, **BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMA_READ, is
    1, if an error was detected, or
    0, if the file was read.
*/
  FILE *filein;
  int   numbytes;
  int   result;

  filein = fopen ( filein_name, "r" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "PPMA_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return 1;
  }
/*
  Read the header.
*/
  result = ppma_read_header ( filein, xsize, ysize, maxrgb );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMA_READ: Fatal error!\n" );
    printf ( "  PPMA_READ_HEADER failed.\n" );
    return 1;
  }
/*
  Allocate storage for the data.
*/
  numbytes = (*xsize) * (*ysize) * sizeof ( int );

  *rarray = ( int * ) malloc ( numbytes );

  if ( *rarray == NULL ) {
    printf ( "\n" );
    printf ( "PPMA_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  *garray = ( int * ) malloc ( numbytes );

  if ( *garray == NULL ) {
    printf ( "\n" );
    printf ( "PPMA_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  *barray = ( int * ) malloc ( numbytes );

  if ( *barray == NULL ) {
    printf ( "\n" );
    printf ( "PPMA_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }
/*
  Read the data.
*/
  result = ppma_read_data ( filein, *xsize, *ysize, *rarray, *garray, *barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMA_READ: Fatal error!\n" );
    printf ( "  PPMA_READ_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( filein );

  return 0;
}
/******************************************************************************/

int ppma_read_data ( FILE *filein, int xsize, int ysize, int *rarray,
  int *garray, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_READ_DATA reads the data in an ASCII portable pixel map file.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the ASCII
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, int *RARRAY, *GARRAY, *BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMA_READ_DATA, is
    1, if an error was detected, or
    0, if the data was read.
*/
  int   count;
  int   i;
  int  *indexb;
  int  *indexg;
  int  *indexr;
  int   ival;
  int   j;
  int   k;
  int   need_line;
  int   need_value;
  char *next;
  int   width;

  indexr = rarray;
  indexg = garray;
  indexb = barray;
  need_line = TRUE;
  next = NULL;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      for ( k = 0; k < 3; k++ ) {

        need_value = TRUE;

        while ( need_value == TRUE ) {

          if ( need_line == TRUE ) {

            next = fgets ( line, MAX_LEN, filein );

            if ( next == NULL ) {
              printf ( "\n" );
              printf ( "PPMA_READ_DATA: Fatal error.\n" );
              printf ( "  End of input.\n" );
              return 1;
            }
            need_line = FALSE;
          }

          count = sscanf ( next, "%s%n", word, &width );

          if ( count == EOF ) {
            need_line = TRUE;
          }
          else {
            next = next + width;
            count = sscanf ( word, "%d", &ival );
            if ( k == 0 ) {
              *indexr = ival;
              indexr = indexr + 1;
            }
            else if ( k == 1 ) {
              *indexg = ival;
              indexg = indexg + 1;
            }
            else if ( k == 2 ) {
              *indexb = ival;
              indexb = indexb + 1;
            }
            need_value = FALSE;
          }

        }
      }
    }
  }

  return 0;
}
/******************************************************************************/

int ppma_read_data ( FILE *filein, int xsize, int ysize, unsigned char *array ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_READ_DATA reads the data in an ASCII portable pixel map file.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the ASCII
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, int *RARRAY, *GARRAY, *BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMA_READ_DATA, is
    1, if an error was detected, or
    0, if the data was read.
*/
  int   count;
  int   i;
  unsigned char ival;
  int   j;
  int   k;
  int   need_line;
  int   need_value;
  char *next;
  int   width;

  
  need_line = TRUE;
  next = NULL;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      for ( k = 0; k < 3; k++ ) {

        need_value = TRUE;

        while ( need_value == TRUE ) {

          if ( need_line == TRUE ) {

            next = fgets ( line, MAX_LEN, filein );

            if ( next == NULL ) {
              printf ( "\n" );
              printf ( "PPMA_READ_DATA: Fatal error.\n" );
              printf ( "  End of input.\n" );
              return 1;
            }
            need_line = FALSE;
          }

          count = sscanf ( next, "%s%n", word, &width );

          if ( count == EOF ) {
            need_line = TRUE;
          }
          else {
            next = next + width;
            count = sscanf ( word, "%d", &ival );			
			array[3*(i + j*xsize) + k] = ival;
            need_value = FALSE;
          }

        }
      }
    }
  }

  return 0;
}



/******************************************************************************/

int ppma_read_header ( FILE *filein, int *xsize, int *ysize, int *maxrgb ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_READ_HEADER reads the header of an ASCII portable pixel map file.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the ASCII
    portable pixel map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int *MAXRGB, the maximum RGB value.

    Output, int PPMA_READ_HEADER, is
    1, if an error was detected, or
    0, if the header was read.
*/
  int   count;
  char *next;
  int   step;
  int   width;

  step = 0;

  while ( ( next = fgets ( line, MAX_LEN, filein ) ) != NULL ) {

    if ( line[0] == '#' ) {
      continue;
    }

    if ( step == 0 ) {
      count = sscanf ( next, "%s%n", word, &width );
      if ( count == EOF ) {
        continue;
      }
      next = next + width;
      if ( strcmp ( word, "P3" ) != 0 && strcmp ( word, "p3" ) != 0 ) {
        printf ( "\n" );
        printf ( "PPMA_READ_HEADER: Fatal error.\n" );
        printf ( "  Bad magic number = %s.\n", word );
        return 1;
      }
      step = 1;
    }

    if ( step == 1 ) {

      count = sscanf ( next, "%d%n", xsize, &width );
      next = next + width;
      if ( count == EOF ) {
        continue;
      }
      step = 2;
    }

    if ( step == 2 ) {

      count = sscanf ( next, "%d%n", ysize, &width );
      next = next + width;
      if ( count == EOF ) {
        continue;
      }
      step = 3;
    }
    if ( step == 3 ) {

      count = sscanf ( next, "%d%n", maxrgb, &width );
      next = next + width;
      if ( count == EOF ) {
        continue;
      }
      return 0;
    }

  }

  printf ( "\n" );
  printf ( "PPMA_READ_HEADER: Fatal error.\n" );
  printf ( "  End of input.\n" );
  return 1;

}
/******************************************************************************/

int ppma_read_test ( char *filein_name ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_READ_TEST tests the ASCII portable pixel map read routines.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the ASCII
    portable pixel map data.

    Output, int PBMA_READ_TEST, is
    1, if an error was detected, or
    0, if the test was carried out.
*/
  int *barray;
  int *garray;
  int  maxrgb;
  int *rarray;
  int  result;
  int  xsize;
  int  ysize;

  rarray = NULL;
  garray = NULL;
  barray = NULL;
/*
  Read the data.
*/
  result = ppma_read ( filein_name, &xsize, &ysize, &maxrgb, &rarray,
    &garray, &barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMA_READ_TEST: Fatal error!\n" );
    printf ( "  PPMA_READ failed.\n" );
    if ( rarray != NULL ) {
      free ( rarray );
    }
    if ( garray != NULL ) {
      free ( garray );
    }
    if ( barray != NULL ) {
      free ( barray );
    }
    return 1;
  }
/*
  Check the data.
*/
  result = ppm_check_data ( xsize, ysize, maxrgb, rarray, garray, barray );

  if ( rarray != NULL ) {
    free ( rarray );
  }
  if ( garray != NULL ) {
    free ( garray );
  }
  if ( barray != NULL ) {
    free ( barray );
  }
  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "  PPM_CHECK_DATA reports bad data from the file.\n" );
    return 1;
  }

  printf ( "\n" );
  printf ( "  PPM_CHECK_DATA passes the data from the file.\n" );

  return 0;
}
/******************************************************************************/

int ppma_write ( char *fileout_name, int xsize, int ysize, int maxrgb,
  int *rarray, int *garray, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_WRITE writes the header and data for an ASCII portable pixel map file.
 
  Modified:
 
    28 September 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the ASCII
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXRGB, the maximum RGB value.

    Input, int *RARRAY, *GARRAY, *BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMA_WRITE, is
    1, if an error was detected, or
    0, if the file was written.
*/
  FILE *fileout;
  int   result;
/*
  Open the output file.
*/
  fileout = fopen ( fileout_name, "w" );

  if ( fileout == NULL ) {
    printf ( "\n" );
    printf ( "PPMA_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", fileout_name );
    return 1;
  }
/*
  Write the header.
*/
  result = ppma_write_header ( fileout, fileout_name, xsize, ysize, maxrgb );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMA_WRITE: Fatal error!\n" );
    printf ( "  PPMA_WRITE_HEADER failed.\n" );
    return 1;
  }
/*
  Write the data.
*/
  result = ppma_write_data ( fileout, xsize, ysize, rarray, garray, barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMA_WRITE: Fatal error!\n" );
    printf ( "  PPMA_WRITE_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( fileout );

  return 0;
}
/******************************************************************************/

int ppma_write_data ( FILE *fileout, int xsize, int ysize, int *rarray,
  int *garray, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_WRITE_DATA writes the data for an ASCII portable pixel map file.

  Example:

    P3
    # feep.ppm
    4 4
    15
     0  0  0    0  0  0    0  0  0   15  0 15
     0  0  0    0 15  7    0  0  0    0  0  0
     0  0  0    0  0  0    0 15  7    0  0  0
    15  0 15    0  0  0    0  0  0    0  0  0

  Modified:

    28 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the ASCII
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *RARRAY, *GARRAY, *BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMA_WRITE_DATA, is
    1, if an error was detected, or
    0, if the data was written.
*/
  int  i;
  int *indexb;
  int *indexg;
  int *indexr;
  int  j;
  int  numval;

  indexr = rarray;
  indexg = garray;
  indexb = barray;
  numval = 0;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      fprintf ( fileout, "%d %d %d", *indexr, *indexg, *indexb );
      numval = numval + 3;
      indexr = indexr + 1;
      indexg = indexg + 1;
      indexb = indexb + 1;

      if ( numval%12 == 0 || i == xsize - 1 || numval == 3 * xsize * ysize ) {
        fprintf ( fileout, "\n" );
      }
      else {
        fprintf ( fileout, " " );
      }

    }
  }
  return 0;
}
/******************************************************************************/

int ppma_write_header ( FILE *fileout, char *fileout_name, int xsize, 
  int ysize, int maxrgb ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_WRITE_HEADER writes the header of an ASCII portable pixel map file.

  Example:

    P3
    # feep.ppm
    4 4
    15
     0  0  0    0  0  0    0  0  0   15  0 15
     0  0  0    0 15  7    0  0  0    0  0  0
     0  0  0    0  0  0    0 15  7    0  0  0
    15  0 15    0  0  0    0  0  0    0  0  0

  Modified:

    28 September 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the ASCII
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXRGB, the maximum RGB value.

    Output, int PPMA_WRITE_HEADER, is
    1, if an error was detected, or
    0, if the header was written.
*/

  fprintf ( fileout, "P3\n" );
  fprintf ( fileout, "# %s\n", fileout_name );
  fprintf ( fileout, "%d %d\n", xsize, ysize );
  fprintf ( fileout, "%d\n", maxrgb );

  return 0;
}
/******************************************************************************/

int ppma_write_test ( char *fileout_name ) {

/******************************************************************************/

/*
  Purpose:

    PPMA_WRITE_TEST tests the ASCII portable pixel map write routines.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the ASCII
    portable pixel map data.

    Output, int PPMA_WRITE_TEST, equals
    1, if the test could not be carried out,
    0, if the test was carried out.
*/
  int *barray;
  int *garray;
  int  i;
  int *indexb;
  int *indexg;
  int *indexr;
  int  j;
  int  maxrgb;
  int  numbytes;
  int *rarray;
  int  result;
  int  xsize;
  int  ysize;

  xsize = 200;
  ysize = 200;
/*
  Allocate memory.
*/
  rarray = NULL;
  garray = NULL;
  barray = NULL;
  numbytes = xsize * ysize * sizeof ( int );  

  rarray = ( int * ) malloc ( numbytes );

  if ( rarray == NULL ) {
    printf ( "\n" );
    printf ( "PPMA_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  garray = ( int * ) malloc ( numbytes );

  if ( garray == NULL ) {
    printf ( "\n" );
    printf ( "PPMA_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  barray = ( int * ) malloc ( numbytes );

  if ( barray == NULL ) {
    printf ( "\n" );
    printf ( "PPMA_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }
/*
  Set the data.
*/
  indexr = rarray;
  indexg = garray;
  indexb = barray;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      if ( j >= i ) {
        *indexr = 255;
        *indexg = 0;
        *indexb = 0;
      }
      else if ( ( xsize - 1 ) * j + ( ysize - 1 ) * i <= 
        ( xsize - 1 ) * ( ysize - 1 ) ) {
        *indexr = 0;
        *indexg = 255;
        *indexb = 0;
      }
      else {
        *indexr = 0;
        *indexg = 0;
        *indexb = 255;
      }
      indexr = indexr + 1;
      indexg = indexg + 1;
      indexb = indexb + 1;
    }
  }
/*
  Compute the maximum.
*/
  maxrgb = 0;
  indexr = rarray;
  indexg = garray;
  indexb = barray;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      maxrgb = MAX ( maxrgb, *indexr );
      maxrgb = MAX ( maxrgb, *indexg );
      maxrgb = MAX ( maxrgb, *indexb );
      indexr = indexr + 1;
      indexg = indexg + 1;
      indexb = indexb + 1;
    }
  }
/*
  Write the data to the file.
*/
  result = ppma_write ( fileout_name, xsize, ysize, maxrgb, rarray, garray,
    barray );

  if ( rarray != NULL ) {
    free ( rarray );
  }

  if ( garray != NULL ) {
    free ( garray );
  }

  if ( barray != NULL ) {
    free ( barray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMA_WRITE_TEST: Fatal error!\n" );
    printf ( "  PPMA_WRITE failed.\n" );
    return 1;
  }

  return 0;
}
/******************************************************************************/

int ppmb_read ( char *filein_name, int *xsize, int *ysize, int *maxrgb,
  int **rarray, int **garray, int **barray ) {

/******************************************************************************/

/*
  Purpose:

    PPMB_READ reads the header and data from a binary portable pixel map file.
 
  Modified:
 
    04 October 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the binary
    portable pixel map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int *MAXRGB, the maximum RGB value.

    Output, int **RARRAY, **GARRAY, **BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMB_READ, equals
    1, if the file could not be read,
    0, if the file was read.
*/
  FILE *filein;
  int   numbytes;
  int   result;

  filein = fopen ( filein_name, "rb" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return 1;
  }
/*
  Read the header.
*/
  result = ppmb_read_header ( filein, xsize, ysize, maxrgb );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  PPMB_READ_HEADER failed.\n" );
    return 1;
  }
/*
  Allocate storage for the data.
*/
  numbytes = ( *xsize ) * ( *ysize ) * sizeof ( int );

  *rarray = ( int * ) malloc ( numbytes );

  if ( *rarray == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  *garray = ( int * ) malloc ( numbytes );

  if ( *garray == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  *barray = ( int * ) malloc ( numbytes );

  if ( *barray == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }
/*
  Read the data.
*/
  result = ppmb_read_data ( filein, *xsize, *ysize, *rarray, *garray, *barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  PPMB_READ_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( filein );

  return 0;
}
/******************************************************************************/

int ppmb_read_data ( FILE *filein, int xsize, int ysize, int *rarray, 
  int *garray, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PPMB_READ_DATA reads the data in a binary portable pixel map file.

  Modified:

    04 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the binary
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *RARRAY, *GARRAY, *BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMB_READ_DATA, equals
    1, if the data could not be read,
    0, if the data was read.
*/
  int   i;
  int   int_val;
  int  *indexb;
  int  *indexg;
  int  *indexr;
  int   j;
  int   k;
  int   numval;

  indexr = rarray;
  indexg = garray;
  indexb = barray;
  numval = 0;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      for ( k = 0; k < 3; k++ ) {

        int_val = fgetc ( filein );

        if ( int_val == EOF ) {
          printf ( "\n" );
          printf ( "PPMB_READ_DATA: Failed reading data byte %d.\n", numval );
          return 1;
        }
        else {
          if ( k == 0 ) {
            *indexr = int_val;
            indexr = indexr + 1;
          }
          else if ( k == 1 ) {
            *indexg = int_val;
            indexg = indexg + 1;
          }
          else if ( k == 2 ) {
            *indexb = int_val;
            indexb = indexb + 1;
          }
        }
        numval = numval + 1;
      }
    }
  }
  return 0;
}
/******************************************************************************/

int ppmb_read_header ( FILE *filein, int *xsize, int *ysize, int *maxrgb ) {

/******************************************************************************/

/*
  Purpose:

    PPMB_READ_HEADER reads the header of a binary portable pixel map file.

  Modified:

    29 April 1999

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEIN, a pointer to the file containing the binary
    portable pixel map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, int *MAXRGB, the maximum RGB value.

    Output, int PPMB_READ_HEADER, equals
    1, if the header could not be read,
    0, if the header was read.
*/
  int   c_val;
  int   count;
  int   flag;
  int   nchar;
  int   state;
  char  string[80];

  state = 0;
  nchar = 0;

  for ( ;; ) {

    c_val = fgetc ( filein );

    if ( c_val == EOF ) {
      return 1;
    }
/*
  If not whitespace, add the character to the current string.
*/
    flag = isspace ( c_val );

    if ( !flag ) {
      string[nchar] = c_val;
      nchar = nchar + 1;
    }
/*
  See if we have finished an old item, or begun a new one.
*/
    if ( state == 0 ) {
      if ( !flag ) {
        state = 1;
      }
      else {
        return 1;
      }
    }
    else if ( state == 1 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        if ( strcmp ( string, "P6" ) != 0 && strcmp ( string, "p6" ) != 0 ) {
          printf ( "\n" );
          printf ( "PPMB_READ_HEADER: Fatal error.\n" );
          printf ( "  Bad magic number = %s.\n", string );
          return 1;
        }
        nchar = 0;
        state = 2;
      }
    }
    else if ( state == 2 ) {
      if ( !flag ) {
        state = 3;
      }
    }
    else if ( state == 3 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", xsize );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        state = 4;
      }
    }
    else if ( state == 4 ) {
      if ( !flag ) {
        state = 5;
      }
    }
    else if ( state == 5 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", ysize );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        state = 6;
      }
    }
    else if ( state == 6 ) {
      if ( !flag ) {
        state = 7;
      }
    }
    else if ( state == 7 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", maxrgb );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        return 0;
      }
    }
  }
}
/******************************************************************************/

int ppmb_read_test ( char *filein_name ) {

/******************************************************************************/

/*
  Purpose:

    PPMB_READ_TEST tests the binary portable pixel map read routines.

  Modified:

    04 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEIN_NAME, the name of the file containing the binary
    portable pixel map data.

    Output, int PPMB_READ_TEST, equals
    1, if the test could not be carried out,
    0, if the test was carried out.
*/
  int *barray;
  int *garray;
  int  maxrgb;
  int *rarray;
  int  result;
  int  xsize;
  int  ysize;

  rarray = NULL;
  garray = NULL;
  barray = NULL;
/*
  Read the data.
*/
  result = ppmb_read ( filein_name, &xsize, &ysize, &maxrgb, &rarray,
    &garray, &barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_READ_TEST: Fatal error!\n" );
    printf ( "  PPMB_READ failed.\n" );
    if ( rarray != NULL ) {
      free ( rarray );
    }
    if ( garray != NULL ) {
      free ( garray );
    }
    if ( barray != NULL ) {
      free ( barray );
    }
    return 1;
  }
/*
  Check the data.
*/
  result = ppm_check_data ( xsize, ysize, maxrgb, rarray, garray, barray );

  if ( rarray != NULL ) {
    free ( rarray );
  }
  if ( garray != NULL ) {
    free ( garray );
  }
  if ( barray != NULL ) {
    free ( barray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "  PPM_CHECK_DATA reports bad data from the file.\n" );
    return 1;
  }

  printf ( "\n" );
  printf ( "  PPM_CHECK_DATA passes the data from the file.\n" );

  return 0;
}
/******************************************************************************/

int ppmb_write ( char *fileout_name, int xsize, int ysize, int maxrgb,
  int *rarray, int *garray, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PPMB_WRITE writes the header and data for a binary portable pixel map file.
 
  Modified:
 
    03 October 1998
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the binary
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXRGB, the maximum RGB value.

    Input, int *RARRAY, *GARRAY, *BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMB_WRITE, equals
    1, if the file could not be written,
    0, if the file was written.
*/
  FILE *fileout;
  int   result;
/*
  Open the output file.
*/
  fileout = fopen ( fileout_name, "wb" );

  if ( fileout == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", fileout_name );
    return 1;
  }
/*
  Write the header.
*/
  result = ppmb_write_header ( fileout, xsize, ysize, maxrgb );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  PPMB_WRITE_HEADER failed.\n" );
    return 1;
  }
/*
  Write the data.
*/
  result = ppmb_write_data ( fileout, xsize, ysize, rarray, garray, barray );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  PPMB_WRITE_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( fileout );

  return 0;
}
/******************************************************************************/

int ppmb_write_data ( FILE *fileout, int xsize, int ysize, int *rarray,
  int *garray, int *barray ) {

/******************************************************************************/

/*
  Purpose:

    PPMB_WRITE_DATA writes the data for a binary portable pixel map file.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the binary
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int *RARRAY, *GARRAY, *BARRAY, the arrays of XSIZE by YSIZE 
    data values.

    Output, int PPMB_WRITE_DATA, equals
    1, if the data could not be written,
    0, if the data was written.
*/
  int  i;
  int *indexb;
  int *indexg;
  int *indexr;
  int  j;

  indexr = rarray;
  indexg = garray;
  indexb = barray;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      fputc ( *indexr, fileout );
      fputc ( *indexg, fileout );
      fputc ( *indexb, fileout );
      indexr = indexr + 1;
      indexg = indexg + 1;
      indexb = indexb + 1;
    }
  }
  return 0;
}
/******************************************************************************/

int ppmb_write_header ( FILE *fileout, int xsize, int ysize, int maxrgb ) {

/******************************************************************************/

/*
  Purpose:

    PPMB_WRITE_HEADER writes the header of a binary portable pixel map file.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILEOUT, a pointer to the file to contain the binary
    portable pixel map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, int MAXRGB, the maximum RGB value.

    Output, int PPMB_WRITE_HEADER, equals
    1, if the header could not be written,
    0, if the header was written.
*/
  fprintf ( fileout, "P6 %d %d %d ", xsize, ysize, maxrgb );

  return 0;
}
/******************************************************************************/

int ppmb_write_test ( char *fileout_name ) {

/******************************************************************************/

/*
  Purpose:

    PPMB_WRITE_TEST tests the binary portable pixel map write routines.

  Modified:

    03 October 1998

  Author:

    John Burkardt

  Parameters:

    Input, char *FILEOUT_NAME, the name of the file to contain the binary
    portable pixel map data.

    Output, int PPMB_WRITE_TEST equals
    1, if the test could not be carried out,
    0, if the test was carried out.
*/

  int *barray;
  int *garray;
  int  i;
  int *indexb;
  int *indexg;
  int *indexr;
  int  j;
  int  maxrgb;
  int  numbytes;
  int *rarray;
  int  result;
  int  xsize;
  int  ysize;

  xsize = 200;
  ysize = 200;
/*
  Allocate memory.
*/
  rarray = NULL;
  garray = NULL;
  barray = NULL;
  numbytes = xsize * ysize * sizeof ( int );  

  rarray = ( int * ) malloc ( numbytes );

  if ( rarray == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  garray = ( int * ) malloc ( numbytes );

  if ( garray == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

  barray = ( int * ) malloc ( numbytes );

  if ( barray == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE_TEST: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }
/*
  Set the data.
*/
  indexr = rarray;
  indexg = garray;
  indexb = barray;
  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      if ( j >= i ) {
        *indexr = 255;
        *indexg = 0;
        *indexb = 0;
      }
      else if ( ( xsize - 1 ) * j + ( ysize - 1 ) * i <= 
        ( xsize - 1 ) * ( ysize - 1 ) ) {
        *indexr = 0;
        *indexg = 255;
        *indexb = 0;
      }
      else {
        *indexr = 0;
        *indexg = 0;
        *indexb = 255;
      }
      indexr = indexr + 1;
      indexg = indexg + 1;
      indexb = indexb + 1;
    }
  }
/*
  Compute the maximum.
*/
  maxrgb = 0;
  indexr = rarray;
  indexg = garray;
  indexb = barray;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      maxrgb = MAX ( maxrgb, *indexr );
      maxrgb = MAX ( maxrgb, *indexg );
      maxrgb = MAX ( maxrgb, *indexb );
      indexr = indexr + 1;
      indexg = indexg + 1;
      indexb = indexb + 1;
    }
  }
/*
  Write the data to the file.
*/
  result = ppmb_write ( fileout_name, xsize, ysize, maxrgb, rarray, garray,
    barray );

  if ( rarray != NULL ) {
    free ( rarray );
  }

  if ( garray != NULL ) {
    free ( garray );
  }

  if ( barray != NULL ) {
    free ( barray );
  }

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE_TEST: Fatal error!\n" );
    printf ( "  PPMB_WRITE failed.\n" );
    return 1;
  }

  return 0;
}
