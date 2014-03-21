#include <string.h>
#include <Math.h>
#include "Functs.h"

namespace SCGRenderer
{
	/**
	* A hash routine for string objects.
	*/
	int hash( const char* key, int tableSize )
	{
		// we will turn all hashed keys to lower case
		char * lwrKey = new char[strlen(key)+1];
		strcpy(lwrKey,key);
		//strlwr(lwrKey);		

		int hashVal = 0;
		for( int i = 0; i < strlen(lwrKey); i++ )
		{
			lwrKey[i] = tolower(lwrKey[i]);
			hashVal = 37 * hashVal + lwrKey[ i ];
		}
		delete lwrKey;

		hashVal %= tableSize;
		if( hashVal < 0 )
			hashVal += tableSize;

		return hashVal;
	}

	//-------------------------------------------------------------------------------------------
	/**
	* A hash routine for ints.
	*/
	int hash( int key, int tableSize )
	{
		if( key < 0 ) 
			key = -key;
		return key % tableSize;
	}

	//-------------------------------------------------------------------------------------------
	bool equals(int x,int y)
	{
		if (x==y)
			return true;
		else
			return false;
	};

	//-------------------------------------------------------------------------------------------
	bool equals(const char* x, const char* y, bool caseSensitive)
	{
		if (caseSensitive)
		{
			if (!strcmp(x,y))  // used to be strcmpi
				return true;
			else
				return false;
		}
		else
		{
			if (strlen(x) != strlen(y))
				return false;
			
			for( int i = 0; i < strlen(x); i++ )
			{
				if (tolower(x[i]) != tolower(y[i]))
					return false;
			}
			return true;
		}
	}
	//--------------------------------------------------------------------------------------------
	bool isPrime( int n )
	{
		if( n == 2 || n == 3 )
			return true;
		if( n == 1 || n % 2 == 0 )
			return false;

		for( int i = 3; i * i <= n; i += 2 )
			if( n % i == 0 )
				return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------
	// returns a prime number >= n
	int nextPrime( int n )
	{
		if( n % 2 == 0 )
			n++;
		for( ; !isPrime( n ); n += 2 );
			return n;
	}
	//--------------------------------------------------------------------------------------------
	int round (double value)
	{
		if (value < 0)
			return -(floor(-value + 0.5));
		else
			return floor( value + 0.5);
	}
}