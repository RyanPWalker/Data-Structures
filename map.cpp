/***********************************************************************
* Program:
*    Week 10, Map
*    Brother Ercanbrack, CS 235
* Author:
*    Ryan Walker
* Summary: 
*    This is a driver program to exercise the Map class.
************************************************************************/

#include <fstream>
#include <iostream>      // for CIN and COUT
#include <iomanip>
#include <string>        // for STRING
#include <map>
#include <set>
#include <algorithm>
using namespace std;

/**********************************************************************
* getFileName()
* To save on space in main, this function will simply
* prompt the user for the filename.
**********************************************************************/
void getFileName(char fileName[])
{
	cout << "Enter filename: ";
	cin >> fileName;
}

/**********************************************************************
 * MAIN
 * This is just a simple program to test the map.
 * It will read from a file, purify the string of
 * any capitalization, punctuation, or spaces, and 
 * then insert it into a map by string name with
 * an increment value for the number of times each
 * word is found.
 ***********************************************************************/
int main()
{
	char fileName[256];
	map<string, int> m1;
	set< std::pair<int, string> > s1; // set will be used to order by number of appearance
   set< std::pair<int, string> >::reverse_iterator rit; // for displaying from the set
	int i = 0;

	getFileName(fileName);

	ifstream fin(fileName);
   if (fin.fail())
   {
   	cout << "Failed to read file." << endl;
      return -1;
   }

   int count = 0;
   string words;
   
   while (fin >> words)
   {
      // this will be our final string after it has been purified
      string result;
      // transforms all letters to lowercase before they even enter the map
      std::transform(words.begin(), words.end(), words.begin(), ::tolower);
      // removes all spaces
      words.erase(remove_if(words.begin(), words.end(), ::isspace), words.end());
      // removes all punctuation from the string.
      std::remove_copy_if(words.begin(), words.end(),            
                        std::back_inserter(result), // store output           
                        std::ptr_fun<int, int>(&std::ispunct)  
                       );
   	++m1[result];
      count++; // keep count of how many words
   }
   
   fin.close();

   
	cout << "\nNumber of words processed: " << count << endl;
   cout << "100 most common words found and their frequencies:\n";


   for (map<string, int>::iterator it = m1.begin(); it != m1.end(); ++it)
   {
   	// insert into the set to sort the map
   	string a = it->first;
   	int b = it->second;
   	s1.insert(std::pair<int, string>(b, a));
   }

   for (rit = s1.rbegin(); rit != s1.rend(); ++rit)
   {
   	// display the top 100 most appearing words
      int b = rit->first;
      string a = rit->second;
      std::pair <string, int> p = std::make_pair( a, b );
      cout << setw(23) << p.first << " - " << p.second << endl;
  		i++;
      if (i >= 100)
      	break;
      }

   return 0;
}