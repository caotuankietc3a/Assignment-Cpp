// The library here is concretely set, students are not allowed to include any
// other libraries.

#include "doctorStrange.h"
using namespace std;
int readFile(const string &filename, string &HP, string &LV, string &EXP,
             string &TS, string &events) {
  // This function is used to read the input file,
  // DO NOT MODIFY THIS FUNTION
  ifstream myfile(filename);
  if (myfile.is_open()) {
    myfile >> HP >> LV >> EXP >> TS;
    // ignore the carriage return
    getline(myfile, events);
    // get events
    getline(myfile, events);
    return 1;
  } else
    return 0;
}

int main(int argc, char *argv[]) {
  string filename = "/home/caparies/Desktop/GanhCaBon/Assignment-Cpp/Assignment3/input.txt";
  string HP, LV, EXP, TS, events;

  bool isRead = readFile(filename, HP, LV, EXP, TS, events);
  if (isRead) {
    cout << handleEvents(HP, LV, EXP, TS, events);
  } else {
    cout << "Cannot read input file";
  }
  return 0;
}
