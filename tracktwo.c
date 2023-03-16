// ============================================================
//
// tracktwo.c
//
// This is the starter program for the track two C assignment.
// This program will take a 40 bit value, convert to 160 bit
// binary, check for errors, and return card values or bad card
//
// ============================================================

#include <stdio.h>
#include <errno.h>
#include <ctype.h>  // "isdigit" may be of use to you
#include <string.h> 

void expand( const char hex[], char binstring[] );
char byte_at( int position, const char binstring[] );
int digits( int position, int length, const char binstring[] );

#define IN_SIZE     40          // 40 characters per data line
#define EXPAND_SIZE (IN_SIZE*4) // Expanded to ASCII string
#define START       ';'
#define SEP         '='
#define END         '?'
#define TRUE        1
#define FALSE       0

/* ============================================================
expand

This function takes a 40 bit hexadecimal number and converts
it into a 160 bit binary number into 160 plances in an array.

Inputs:		40 character binary number
Outputs:	None
Returns:	None
Author:		Justin Tobiason
============================================================ */
void expand( const char hex[], char binstring[])
{
	int change = 0;

	static char *convert[] = { "0000", "0001", "0010", "0011",
							   "0100", "0101", "0110", "0111",
							   "1000", "1001", "1010", "1011",
							   "1100", "1101", "1110", "1111" };

	for(int i = 0; i < IN_SIZE; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			change = isdigit(hex[i]) ? hex[i] - '0' : hex[i] - 'A' + 10;
			binstring[i * 4 + j] = convert[change][j];
		}
	}
}

/* ============================================================
byte_at

This fucntion takes 160 characters from an array and, with
odd parity in mind, checks for errors or a corresponding value
to the next 4 binary values.

Inputs:		160 char array of 1's or 0's 
Outputs:	None
Returns:	one char
Author:		Justin Tobiason
============================================================ */

char byte_at(int position, const char binstring[])
{
	int value = 0; //This stores total of all 1's
	int out = 0; //This is a temp place for values 
	int total = 0; //This is our total after decoding
	int power = 1; //This tracks power for binary conversion

	int c = 1;
	int x = position + 1;
	int i = position;
	int r = 0;
	r = position + 5;
	static char convert[] = { '0', '1', '2', '3',
							  '4', '5', '6', '7',
							  '8', '9', ':', ';',
							  '<', '=', '>', '?' };

	for (; i < r; i++)
	{

		if (binstring[i] == '1')
		{
			value++;
		}
	}

	for (; x < r; x++)
	{
		out += binstring[r-c] - '0';
		out *= power;
		total += out;
		power *= 2;
		out = 0;
		c++;
	}


	char last = convert[total];

	if (value % 2 != 0) //If odd parity return digit else E
	{
		return last;
	}
	else
	{
		return 'E';
	}
}

/* ============================================================
byte_at

This fucntion takes 160 characters from an array and, with
odd parity in mind, sends values five by five to check for
start sentinal, seperator, and end sential depending on
length. If a length of 16 will check for start sentinal,
16 characters in between, and a seperator. A length of
10 will correspond with looking for seperator, 10 digits, and 
end sentinal. 

Inputs:		160 char array of 1's or 0's 
Outputs:	prints card values
Returns:	value of one or zero.
Author:		Justin Tobiason
============================================================ */

int digits(int position, int length, const char binstring[])
{

	static char five_bit_val_card[IN_SIZE];
	static char five_bit_val_other[IN_SIZE];
	int counter = 0;

	int ret_val = FALSE;

	static int check_one = 0; //Checks values of 16 digit card numbers
	static int check_two = 0; //Checks values of 10 digit etc

	static int try = 0; //Checks to make sure this had both 16 and 10 digit values.

	//checks for card number
	if (length == 16)
	{
		try++;
		for(int i = 0; i < EXPAND_SIZE; i += 5)
    	{

    		int j = i;
    		five_bit_val_card[0] = byte_at(j, binstring);
    		if (five_bit_val_card[0] == START)
    		{
    			for(int x = j; x < (j + 85); x +=5)
    			{
    				if(isdigit(byte_at(x + 5,binstring)))
    				{
    					five_bit_val_card[counter] = byte_at(x + 5, binstring);
    					ret_val = TRUE;
    					counter++;
    				}
    				else if(byte_at(x+5,binstring) == SEP && counter == 16)
    				{
    					check_one = 1;
    					ret_val = TRUE;
    					break;

    				}
    				else if (counter != 16)
    				{
    					ret_val = FALSE;
    					break;
    				}

    			}
    			break;
    		}
    	}
	}
	//Checks for 10 digits of ex, srv, and cvv
	else if (length == 10)
	{
		try++;
		for(int i = 0; i < EXPAND_SIZE; i += 5)
    	{

    		int j = i;
    		five_bit_val_other[0] = byte_at(j, binstring);
    		if (five_bit_val_other[counter] == SEP)
    		{
    			for(int x = j; x < (j + 55); x +=5)
    			{
    				if (isdigit(byte_at(x + 5,binstring)))
    				{
    					five_bit_val_other[counter] = byte_at(x + 5, binstring);
    					ret_val = TRUE;
    					counter++;
    				}
    				else if (byte_at(x+5,binstring) == END && counter == 10)
    				{
    					check_two = 1;
    					ret_val = TRUE;
    					break;
    				}
    				else
    				{
    					ret_val = FALSE;
    					break;
    				}
    			}
    			break;
    		}
    	}
	}


	if (try == 2) //This checks both calls were successful at obtaining digits
	{
		if(check_one != 0 && check_two != 0)
    	{
  		  	for (int p = 0; p < 16; p++)
    		{
    			printf("%c", five_bit_val_card[p]);
    		}
    		printf("\n");

    		for (int p = 0; p < 4; p++)
    		{
    			printf("%c", five_bit_val_other[p]);
    						
    		}
    		printf("\n");
    		for( int o = 4; o < 7; o++)
    		{
    			printf("%c", five_bit_val_other[o]);
    						
    		}
    		printf("\n");
    		for (int q = 7; q < 10; q++)
    		{
    			printf("%c", five_bit_val_other[q]);
    						
    		}
    		printf("\n");
    	}
    	try = 0; //reset value
    	check_one = 0; //reset value
    	check_two = 0; //reset value
	}



    return ret_val;
}

int main( int ac, char *av[] )
{
    char     data[ IN_SIZE + 3 ]; // 1 for '\r', 1 for '\n', 1 for the null
    char     expanded[ EXPAND_SIZE + 1 ];
    FILE     *infile;

    int card = 0; //retains True or False for 16 digit check
    int other = 0; //retains True or False for 10 digit check

    if ( ac != 2 )
    {
        fprintf( stderr, "%s: You need to supply me with the name of the data file!\n", av[ 0 ] );
        return( 1 );
    }
    
    infile = fopen( av[ 1 ], "r" );
    if ( infile == NULL )
    {
        fprintf( stderr, "%s: ", av[ 0 ] );
        perror( av[ 1 ] );
        return( 1 );
    }

    // We want the newline and null so things stay in sync.
    while ( fgets( data, IN_SIZE + 3, infile ) != NULL )
    {
    	expand(data, expanded); //Expands out line of data to binary


    	card = digits(0, 16, expanded); //Looks for 16 digit card number
    	other = digits(0, 10, expanded); //Looks for 10 digits of ex, srv, and cvv
    	if (card == FALSE || other == FALSE)
    	{
    		printf("Bad Card\n");
    	}
        // Here, "data" is a string with 40 hex characters plus the
        // newline and a null character at the end.
    }

    

    return( 0 );
}


