#include "HashTable.h"
#include <cstring>
#include "primes.h"

char * const HashTable::DELETED  = (char *) &DELETED;
int CLUSTERSIZE = 9;

HashTable::HashTable(int n){
  //bounds check
  if(n < 101){
    cout << "invalid size. Overriding to 101\n";
    n = 101;
  }
  else if(n > 199999){
    throw out_of_range("too large input!");
  }

  m_isRehashing = false;
  m_loadFactor = 0;
  m_size = 0;
  n = roundUpPrime(n);
  m_capacity = n;
  m_strings = new char* [n];
  newTable = NULL;
}

HashTable::~HashTable(){
  
  for(int i = 0; i < m_capacity; i++)
    {
      if(m_strings[i] != NULL)
	free(m_strings[i]);
    }
  m_size = 0;
  m_capacity = 0;
  m_loadFactor = 0;

  free(m_strings);

  delete newTable;
}

HashTable::HashTable (HashTable& other){

  if(other.isRehashing())
    {
      cout << "force other to stop rehashing\n";
      forceEndHash();
    }

  m_size = other.m_size;
  m_capacity = other.m_capacity;
  m_isRehashing = other.m_isRehashing;
  m_loadFactor = other.m_loadFactor;

  m_strings = new char* [other.m_capacity];

  //copy over values from other hash table, as long as they aren't NULL
  for(int i = 0; i < m_capacity; i++)
    {
      if(other.m_strings[i] != NULL)
	{
	  m_strings[i] = other.m_strings[i];
	}
    }
}

const HashTable& HashTable::operator= (HashTable& rhs){

  if(rhs.isRehashing())
    {
      cout << "force other to stop rehashing\n";
      forceEndHash();
    }

  m_size = rhs.m_size;
  m_capacity = rhs.m_capacity;
  m_isRehashing = rhs.m_isRehashing;
  m_loadFactor = rhs.m_loadFactor;

  //copy over values from other hash table, as long as they aren't NULL
  for(int i = 0; i < m_capacity; i++)
    {
      if(rhs.m_strings[i] != NULL)
        {
          m_strings[i] = rhs.m_strings[i];
        }
    }  
  return *this;
}

void HashTable::insert(const char *str){
  
  //if rehashing and num items in old table < 3%,
  //stop rehashing
  float x = loadFactor();
  if((x < .03) and (m_isRehashing == true))
    {
      cout << "force other to stop rehashing\n";
      forceEndHash();
    }

  //retrieve index from hashCode
  int index = hashCode(str) % m_capacity;
  int clusterCount = 0;
  //if ongoing rehash, insert into new table
  if(isRehashing())
    {
      cout << "insert into new table " << endl;
      newTable->insert(str);
    }

  //otherwise, insert into old table
  else
    {
      //if the string is already in the table, do not insert it again
      if(!find(str))
	{
	  //if index is not empty, add to next available index
	  //(linear probing!)
	  //also keep track of cluster
	  while((m_strings[index] != NULL) and (m_strings[index] != DELETED))
	    {
	      index++;
	      index = index % m_capacity;
	      clusterCount++;
	      
	    }
	  //reset clusterCount every time we make our insert and the cluster
	  //from linear probing is less than the invalid size.
	  //(As long as we are not in rehash mode)
	  if((clusterCount < CLUSTERSIZE) and (m_isRehashing == false))
	    {
	      clusterCount = 0;
	    }

	  //if we are in rehash mode and we find any size cluster, rehash it
	  else if((clusterCount > 1) and (isRehashing()))
	    {
	      this->rehash(index);
	    }
	  //if not in rehash mode,
	  //make a copy of the string str and insert it 
	  if(!isRehashing())
	    {	    
	      m_strings[index] = strdup(str);
	      m_size++;
	    }
	    //check load factor again after insertion
	    x = loadFactor();
	  //if any conditions are violated, begin rehashing
	    if((x > .5) or (clusterCount >= CLUSTERSIZE))
	      {
		//start rehashing
		this->rehash(index);
	      }	
	}
    }
}

void HashTable::rehash(int index){

  //if rehash is called for the first time, allocate new HashTable
  if(m_isRehashing == false)
    {
      newTable = new HashTable(m_size * 4);
      m_isRehashing = true;
    }
  
  //iterate right and hash through the cluster until we find an empty slot
  for(int i = index; m_strings[i % m_capacity] != NULL; i++)
    {

      //if string is not DELETED, hash it to newTable and deallocate it.
      if(m_strings[i % m_capacity] != DELETED)
	{
	  int x = hashCode(m_strings[i]);
	  x = x % newTable->m_capacity;

	  newTable->m_strings[x] = m_strings[i % m_capacity];
	  newTable->m_size++;
	  
	  free(m_strings[i % m_capacity]);
	  m_strings[i % m_capacity] = NULL;
	  m_size--;
	}
    }

  //now check to the left
  for(int i = index - 1; m_strings[i] != NULL; i--)
    {
      //loop around case
      if(i < 0)
	{
	  i = m_capacity - 1;
	}
      //if string is not DELETED
      if(m_strings[i % m_capacity] != DELETED)
	{
	  int x = hashCode(m_strings[i]) % newTable->m_capacity;
	  newTable->m_strings[x] = m_strings[i];
	  newTable->m_size++;
	  
	  free(m_strings[i]);
          m_strings[i] = NULL;
          m_size--;
	}
    }
}

bool HashTable::find(const char *str){

  //if rehashing and num items in old table < 3%,
  //stop rehashing
  float x = loadFactor();
  int clusterCount = 0;
  if((x < .03) and (m_isRehashing == true))
    {
      cout << "force other to stop rehashing\n";
      forceEndHash();
    }
  
  for(int i = 0; i < m_capacity; i++)
    {
      //if the array has an item at this index, and it is equiv to str,
      //return true
      if(m_strings[i] != NULL)
	{
	  clusterCount++;
	  if(strcmp(m_strings[i], str) == 0)
	    {
	      //if we are in rehash mode and we find any size cluster, rehash it
	      if((clusterCount > 1) and (m_isRehashing == true))
		{
		  this->rehash(i);
		}	  
	      return true;
	    }
	}
      //if we hit a NULL (which will break the cluster) reset counter
      else
	{
	  clusterCount = 0;
	}
    }
  //check newTable now
  if((newTable != NULL) and (newTable->m_size != 0))
    {
      for(int i = 0; i < newTable->m_capacity; i++)
	{
	  if(newTable->m_strings[i] != NULL)
	    {
	      if(strcmp(newTable->m_strings[i], str) == 1)
		{
		  return true;
		}
	    }      
	}
    }
  return false;
}

char* HashTable::remove(const char *str){

  //if rehashing and num items in old table < 3%,
  //stop rehashing
  float x = loadFactor();
  if((x < .03) and (m_isRehashing == true))
    {
      cout << "force other to stop rehashing \n";
      forceEndHash();
    }

  //if string is not in the table, return null
  if(!find(str))
    {
      return NULL;
    }

  char* item;
  int clusterCount = 0;
  //search for string, while keeping track of clusterCount
  for(int i = 0; i < m_capacity; i++)
    {
      if((m_strings[i] != NULL))     
	{
	  clusterCount++;
	  //if string has been found, set it to "deleted"
	  if(strcmp(m_strings[i], str) == 0)
	  {
	    item = m_strings[i];
	    m_strings[i] = DELETED;

	    x = loadFactor();
	    //if any condition has been violated, trigger rehashing
	    if((clusterCount >= CLUSTERSIZE) or (x > 0.5))
	      {
		this->rehash(i);
	      }
	    //if we are in rehash mode and we find any size cluster, rehash it
	    else if((clusterCount > 1) and (m_isRehashing == true))
	      {
		this->rehash(i);
	      }	    
	  }
	}
      //if we encounter a NULL, reset clusterCount to 0.
      else if(m_strings[i] == NULL)
	{
	  clusterCount = 0;
	}
    }
  return item;
}

void HashTable::forceEndHash(){
  //iterate through table, rehash remaining items
  for(int i = 0; i < m_capacity; i++)
    {
      if(m_strings[i] != NULL)
	{
	  int x = hashCode(m_strings[i]) % newTable->m_capacity;
	  newTable->m_strings[x] = m_strings[i];
	  free(m_strings[i]);
	  
	}
    }
  free(m_strings);
  currTable = this;
  currTable = newTable;  
  newTable = newTable->newTable;
  m_isRehashing = false;
}

float HashTable::loadFactor(){
  //m_size is a float, so returns a decimal
  m_loadFactor = m_size / m_capacity;
  return m_loadFactor;
}

bool HashTable::isRehashing(){
  return m_isRehashing;
}

int HashTable::tableSize(int table){
  //if table = 0, return old table capacity
  if(table == 0)
    {
        return m_capacity;
    }
  //otherwise, return new table capacity
  else if(table == 1)
    {
      cout << "return new table capacity" << endl;
      return newTable->m_capacity;
      return 0;
    }
}

int HashTable::size(int table){
  //if table = 0, return old table size
  if(table == 0)
    {
      return m_size;
    }
  //otherwise, return new table size
  else if(table == 1)
    {
      cout << "return new table size" << endl;
      return newTable->m_size;
      return 0;
    }
}

const char* HashTable::at(int index, int table){
  if((index < 0) or (index > m_capacity))
    {
      throw out_of_range("Error: Index not within range.");
    }
  //if table is 0, return str from old table
  if(table == 0)
    {
      return m_strings[index];
    }
  else if(table == 1)
    {
      cout << "returning from new table" << endl;
      return newTable->m_strings[index];
      
    }
}

void HashTable::dump(){
  if(m_size == 0)
    newTable->dump();
  
  else{
    for(int i = 0; i < m_capacity; i++)
      {
	if(m_strings[i] != NULL)
	  cout << "Index: " << i << "  Data: " << m_strings[i] << endl;
      }
  }
}
