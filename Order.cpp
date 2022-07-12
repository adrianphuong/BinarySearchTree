#include<fstream>
#include "Dictionary.h"

using namespace std;

int main(int argc, char * argv[]){
   ifstream in;
   ofstream out;
   std::string line = "";
   if (argc != 3){
      cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
      exit(EXIT_FAILURE);
   }
   in.open(argv[1]);
   out.open(argv[2]);
   int count = 1;
   Dictionary D = Dictionary();
   while(getline(in, line)){
       if(line != ""){
	       D.setValue(line, count);
       } 
       else {
	       break;
       }
       count++;
   }
   out << D << endl;
   out << D.pre_string() << endl;
   in.close();
   out.close();
   return 0;
}
