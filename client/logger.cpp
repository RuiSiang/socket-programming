#include<fstream>

using namespace std;

void error(string message){
  ofstream fout("error.log", ofstream::app);
  fout << message;
  fout.close();
}

void info(string message){
  ofstream fout("info.log", ofstream::app);
  fout << message;
  fout.close();
}