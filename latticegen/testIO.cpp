#include <iostream>
#include <fstream>
#include <string>

int main() {
   std::string inputString;
   
   std::cin >> inputString; // Read input from user or standard input;

   if (inputString == "a")
   {
    std::cout << "yes" << std::endl;
   }
   else
   {
    std::cout << "no" << std::endl;
   }
   

   return 0;
}//incept input string a and write the input into a file
