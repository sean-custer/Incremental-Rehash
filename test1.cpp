// File: test1.cpp
//
// Testing basic insert, find and remove without
// triggering incremental rehashing
//

#include <iostream>
#include <cstdlib>
using namespace std ;

#include "HashTable.h"


int main() {
   HashTable T(107) ;

   
   T.insert("tributes") ;	// 21
   T.insert("skulduggery") ;	// 22
   T.insert("convulse") ;	// 23
   T.insert("frothed") ;	// 24
   T.insert("horrify") ;	// 25
   T.insert("blackmailers") ;	// 26
   T.insert("defenestrated") ;	// 27
   T.insert("garrison") ;	// 23 -> 28
   T.insert("lidless") ;	// 22 -> 29  
   //   T.insert("eye") ;            // 21 -> 30
   
   cout << "----------------------------------------------------\n" ;
   cout << "Original hash table\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;


   cout << "\n\nDo some find() and remove() operations...\n\n" ;

   const char *str ;
   char *ptr ;
   bool found ;

   if( T.find(str="skulduggery") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( T.find(str="lidless") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( T.find(str="defenestrated") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( T.find(str="peircing") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   ptr = T.remove(str="garrison") ;
   if (ptr == NULL) {
      cout << "String " << str << " not found, not removed\n" ;
   } else {
      cout << "Removed string = " << ptr << endl ;
      free(ptr) ;
   }

   ptr = T.remove(str="infractions") ;
   if (ptr == NULL) {
      cout << "String " << str << " not found, not removed\n" ;
   } else {
      cout << "Removed string = " << ptr << endl ;
      free(ptr) ;
   }

   if( T.find(str="garrison") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( T.find(str="lidless") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   cout << "\n\n" ;
   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after finds and removes\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;
   
   cout << "\n\nNext insert should reuse DELETED slots...\n\n" ;

   T.insert("undying") ;        // 25 -> 28 

   cout << "\n\n" ;
   cout << "----------------------------------------------------\n" ;
   cout << "Final hash table\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;
   return 0 ;

}
