#ifndef QUADRATIC_PROBING_H_
#define QUADRATIC_PROBING_H_

#define NOMINMAX

#include <vector>

/* By using templates, you can design a single class or function
that operates on data of many types, instead of having to create
a separate class for each type. */

namespace SCGRenderer
{
	bool isPrime( int n );
	int hash( int key, int tableSize );
	int hash( const char* key, int tableSize );
	int nextPrime( int n );
	bool equals(int,int);
	bool equals(const char*, const char*, bool caseSensitive = true);


	template <class hashedObj,class hashedObjData>
	class CHashTable
	{
	public:
		CHashTable();
		CHashTable(int notFound, int size = 101 );
		int insert(const hashedObj & x , const hashedObjData & y);
		int find( const hashedObj & x ) const;
		const hashedObjData & getData( const hashedObj & x) const;
		void remove( const hashedObj & x );
		void initialize(int notFound, int size = 101 );

	private:
		enum entryType { ACTIVE, EMPTY, DELETED }; // the type of the element
		struct hashEntry
		{
			hashedObj element;
			hashedObjData elementData;
			entryType info;
			hashEntry(const hashedObj & e = hashedObj( ), entryType i = EMPTY , const hashedObjData & d = hashedObjData () )
					  : element( e ), info( i ) , elementData (d) { }
		};
	            
		std::vector<hashEntry> array;
		int currentSize;
		void initTable();  // initializes the hash table
		int findPos(const hashedObj & x, bool caseSensitive = true) const;
		bool isActive( int currentPos ) const;
		void rehash();
		const CHashTable & operator=( const CHashTable & rhs );
		int ITEM_NOT_FOUND;
	};
	//-------------------------------------------------------------------------------
	// mirette added : empty constructor
		template <class hashedObj, class hashedObjData>
	CHashTable<hashedObj, hashedObjData>::CHashTable()
	{
	}
	//-------------------------------------------------------------------------------
	// hash table constructor
	template <class hashedObj, class hashedObjData>
	CHashTable<hashedObj, hashedObjData>::CHashTable(int notFound,int size )
	: ITEM_NOT_FOUND (notFound) , array( nextPrime( size ) )
	{
		initTable();
	}
	//-------------------------------------------------------------------------------
	// mirette added: initialize after creation
	template <class hashedObj, class hashedObjData>
	void CHashTable<hashedObj, hashedObjData>::initialize(const int notFound,int size )
	{
		ITEM_NOT_FOUND = notFound;
		array.reserve(nextPrime(size));
		initTable();
	}
	//--------------------------------------------------------------------------------
	// initialize the hash table
	template <class hashedObj, class hashedObjData>
	void CHashTable<hashedObj, hashedObjData>::initTable( )
	{
		currentSize = 0;
		for( int i = 0; i < array.size(); i++ )
			array[ i ].info = EMPTY;
	}

	//--------------------------------------------------------------------------------
	// insert an entry into the hash table
	template<class hashedObj, class hashedObjData>
	int CHashTable<hashedObj, hashedObjData>::insert(const hashedObj & x, const hashedObjData & y)
	{
		int currentPos = findPos( x ); //  find the position to insert (this is case sensitive search)

		if( isActive( currentPos ) )
			return -1;
		array[ currentPos ] = hashEntry( x, ACTIVE , y);

		if( ++currentSize > array.size( ) / 2 )
			rehash( );

		return currentPos;
	}

	//--------------------------------------------------------------------------------
	/**
	* Method that performs quadratic probing resolution.
	* Return the position where the search for x terminates.
	*/
	template <class hashedObj, class hashedObjData>
	int CHashTable<hashedObj, hashedObjData>::findPos( const hashedObj & x, bool caseSensitive ) const
	{
		int collisionNum = 0;
		int currentPos = hash( x, array.size( ) );
		
		while(array[currentPos].info != EMPTY && !equals(array[currentPos].element, x, caseSensitive) )
		{
			currentPos += 2 * ++collisionNum - 1;  // Compute ith probe
			if( currentPos >= array.size( ) )
				currentPos -= array.size( );
		}


		return currentPos;
	}

	//--------------------------------------------------------------------------------
	/**
	* Return true if currentPos exists and is active.
	*/
	template <class hashedObj, class hashedObjData>
	bool CHashTable<hashedObj, hashedObjData>::isActive( int currentPos ) const
	{
		return array[ currentPos ].info == ACTIVE;
	}

	//--------------------------------------------------------------------------------
	/**
	* Expand the hash table.
	*/
	template <class hashedObj, class hashedObjData>
	void CHashTable<hashedObj, hashedObjData>::rehash( )
	{
		std::vector<hashEntry> oldArray = array;

		// Create new double-sized, empty table
		array.resize( nextPrime( 2 * oldArray.size( ) ) );
		for( int j = 0; j < array.size( ); j++ )
			array[ j ].info = EMPTY;

		// Copy table over
		currentSize = 0;
		for( int i = 0; i < oldArray.size( ); i++ )
			if( oldArray[ i ].info == ACTIVE )
				insert( oldArray[ i ].element , oldArray[i].elementData);
	}

	//--------------------------------------------------------------------------------
	template <class hashedObj, class hashedObjData>
	int CHashTable<hashedObj, hashedObjData>::find( const hashedObj & x ) const
	{
		int currentPos = findPos( x );
		if( isActive( currentPos ) )
			return currentPos;
		else // can not find the element.. try case insensitive search
		{
			currentPos = findPos(x, false);
			if (isActive(currentPos))
				return currentPos;
			else
				return ITEM_NOT_FOUND;
		}
	}

	//--------------------------------------------------------------------------------
	template <class hashedObj, class hashedObjData>
	void CHashTable<hashedObj, hashedObjData>::remove( const hashedObj & x )
	{
		int currentPos = findPos( x );
		if( isActive( currentPos ) )
			array[ currentPos ].info = DELETED;
	}

	//--------------------------------------------------------------------------------
	template <class hashedObj, class hashedObjData>
	const hashedObjData & CHashTable<hashedObj, hashedObjData>::getData( const hashedObj & x) const
	{
		int currentPos = find( x );
		if (currentPos == ITEM_NOT_FOUND)
			return NULL;

		if( isActive( currentPos ) )
			return array[currentPos].elementData;
		else
			return NULL;
	}


	//--------------------------------------------------------------------------------
	template <class hashedObj, class hashedObjData>
	const CHashTable<hashedObj, hashedObjData> & CHashTable<hashedObj, hashedObjData>::
	operator=( const CHashTable<hashedObj, hashedObjData> & rhs )
	{
		if( this != &rhs )
		{
			array = rhs.array;
			currentSize = rhs.currentSize;
		}
		return *this;
	}
}
#endif