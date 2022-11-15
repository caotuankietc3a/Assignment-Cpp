#include "ConcatStringTree.h"

int main() {
  // ConcatStringTree s1("acbbcab");
  // cout << s1.length() << endl;
  // cout << s1.get(1) << endl;
  // try {
  //   cout << "char at index 10: " << s1.get(10) << endl;
  // } catch (const out_of_range &ofr) {
  //   cout << "Exception out_of_range: " << ofr.what() << endl;
  // }
  // cout << s1.indexOf('b') << endl;
  // ConcatStringTree s1("Hello");
  // ConcatStringTree s2(",_t");
  // ConcatStringTree s3 = s1.concat(s2);
  // ConcatStringTree s4 = s3.subString(5, 6);
  // cout << s3.toStringPreOrder() << endl;
  // cout << s3.toString() << endl;
  // cout << s3.subString(5, 6).reverse().toString() << endl;
  // cout << s3.subString(5, 6).toString() << endl;
  // cout << s3.reverse().toString() << endl;

  // ConcatStringTree *s1 = new ConcatStringTree("a");
  // ConcatStringTree *s2 = new ConcatStringTree("b");
  // ConcatStringTree *s3 = new ConcatStringTree(s1->concat(*s2));
  //
  // cout << s3->getParTreeSize("l") << endl;
  // cout << s3->getParTreeStringPreOrder("l") << endl;
  //
  // delete s1;
  // delete s2;
  // delete s3;

  HashConfig hashConfig(2, 0.5, 0.5, 0.75, 2, 4);
  LitStringHash *litStringHash = new LitStringHash(hashConfig);
  ReducedConcatStringTree *s1 = new ReducedConcatStringTree("a", litStringHash);
  ReducedConcatStringTree *s2 =
      new ReducedConcatStringTree("bb", litStringHash);

  cout << s1->toString() << endl;
  cout << s2->toString() << endl;
  ReducedConcatStringTree *s3 =
      new ReducedConcatStringTree("bb", litStringHash);

  cout << litStringHash->getLastInsertedIndex() << endl;
  cout << litStringHash->toString() << endl;

  delete litStringHash;
  delete s3;
  delete s1;
  delete s2;

  // ConcatStringTree s1("Hello");
  // cout << "Please focus to id: " << s1.getParTreeStringPreOrder("") << endl;
  // ConcatStringTree s2("an-nyeong-ha-se-yo");
  // cout << "Please focus to id: " << s2.getParTreeStringPreOrder("") << endl;
  // ConcatStringTree s3("nee how");
  // cout << "Please focus to id: " << s3.getParTreeStringPreOrder("") << endl;
  return 0;
}
