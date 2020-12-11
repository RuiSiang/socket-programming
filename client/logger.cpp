#include<fstream>

using namespace std;

void error(string message){
  ofstream fout("error.log");
  fout << message;
  fout.close();
}

void info(string message){
  ofstream fout("info.log");
  fout << message;
  fout.close();
}