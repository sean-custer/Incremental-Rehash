#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <cstring>
#include <iostream>
using namespace std;

//NOTES
//Rehashing triggers:
//  1. If load factor > 0.5 (check load factor at beginning of every
//     insert, find, and remove.
//
//  2. If linear probing during an insert/find/remove requires checking
//     10 or more hash slots.
//
// In either case, new hash table size should be four times the number
// of items in the old table when inc. rehashing is triggered.
//
// Table size should be rounded up to the next prime. Size must be different

class HashTable{
public:
  //constructor
  //if user requests size < 101, override and make 101
  //max size: 199,999
  //must round to nearest prime >= n.
  HashTable(int n=101) ;

  //destructor
  //strings must be deallocated using free().
  ~HashTable() ;

  //copy construktohr
  //if other HashTable has ongoing rehash, force other to finish rehashing
  HashTable (HashTable& other);

  //overloaded assignment operator
  //if ongoing rehash, force rhs to finish and copy resulting table
  const HashTable& operator= (HashTable& rhs) ;
  
  //inserts a copy of Cstring str into hash table
  //should insert into new table if there is ongoing inc. rehash
  //do not insert second copy of same value
  //should stop rehashing if num items in old table is < 3%
  void insert(const char *str) ;

  //returns true if found, false otherwise
  //should stop rehashing if num items in old table is < 3%
  bool find(const char *str) ;

  //deallocate string returned using free()
  //when item is removed, mark slot as "deleted"
  char * remove(const char *str) ;

  //declares char* constant DELETED as static so we can extend the scope
  static char * const DELETED ;
  
  //returns true if there is an ongoing inc rehash
  bool isRehashing() ;

  //function that begins rehashing:
  //  rehashes clusters at a specific index where
  //  insert(), find(), or remove() occur
  void rehash(int index);

  //hashes every remaining item in current table, and deletes the table
  void forceEndHash();
  
  //returns size of hash table
  //if ongoing rehash, tableSize(0) returns old size and
  //tableSize(1) returns new size
  int tableSize(int table=0) ;
  int size(int table=0) ;

  //returns the loadFactor in decimal form
  float loadFactor() ;
  
  //function to compute hashCode
  unsigned int hashCode(const char *str){
    unsigned int val = 0 ;
    const unsigned int thirtyThree = 33 ;  // magic number from textbook
    
    int i = 0 ;
    while (str[i] != '\0') {
      val = val * thirtyThree + str[i] ;
      i++ ;
    }
    return val ;
  }
  //returns pointer to string stored at index slot of hash table
  //if invalid index, throw out_of_range
  const char * at(int index, int table=0) ;

  void dump() ;
private:
  //num of items currently in table
  float m_size;
  //size of table
  int m_capacity;
  bool m_isRehashing;
  char** m_strings;

  HashTable* currTable;
  //pointer to eventual new HashTable
  HashTable* newTable;
  float m_loadFactor;
  
  
};



#endif
