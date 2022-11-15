#include "ConcatStringTree.h"

int main() {
  ConcatStringTree s1("acbbcab");
  std::cout << "message" << std::endl;
  cout << s1.length() << endl;
  cout << s1.get(1) << endl;
  try {
    cout << "char at index 10: " << s1.get(10) << endl;
  } catch (const out_of_range &ofr) {
    cout << "Exception out_of_range: " << ofr.what() << endl;
  }
  cout << s1.indexOf('b') << endl;
  return 0;
}
