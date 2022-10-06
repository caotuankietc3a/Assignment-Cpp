#include "ConcatStringList.h"

void tc1() {
  ConcatStringList s1("Hello,_world");
  // test length
  cout << "s1's length: " << s1.length() << endl;
  // test get
  cout << "char at index 2: " << s1.get(2) << endl;
  try {
    cout << "char at index 22: " << s1.get(22) << endl;
  } catch (out_of_range &ofr) {
    cout << "Exception out_of_range: " << ofr.what() << endl;
  }
  // test indexOf
  cout << "in s1, index of 'o': " << s1.indexOf('o') << endl;
  cout << "in s1, index of 'a': " << s1.indexOf('a') << endl;
  // test toString
  cout << "s1 toString: " << s1.toString() << endl;
}

void tc2() {
  ConcatStringList s1("Hello,");
  ConcatStringList s2("_this_is");
  ConcatStringList s3("_an_assignment");

  // ConcatStringList s4 = s1.concat(s2).concat(s3);
  // cout << "s4's length: " << s4.length() << endl;
  // try {
  //   cout << "char at index 11: " << s4.get(11) << endl;
  //   cout << "s4's char: " << s4.get(-1) << endl;
  // } catch (out_of_range &ofr) {
  //   cout << "Exception out_of_range: " << ofr.what() << endl;
  // }
  // try {
  //   std::cout << "subString s4: " << s4.subString(1, 1).toString() <<
  //   std::endl;
  // }
  // catch (const std::logic_error& ofr) {
  //   cout << "Exception logic_error: " << ofr.what() << endl;
  // }
  // std::cout << "reverse s4: " << s4.reverse().toString() << std::endl;
  // cout << "in s4, index of 'o': " << s4.indexOf('o') << endl;
  // cout << "s4: " << s4.toString() << endl;
  // cout << "refList's size(): " << ConcatStringList::refList.size() << endl;
  // cout << "refList's refCountAt(int index): "
  //      << ConcatStringList::refList.refCountAt(10) << endl;
  // cout << "refList's refCountsString(): "
  //      << ConcatStringList::refList.refCountsString() << endl;
  // cout << "delStrList's size(): " << ConcatStringList::delStrList.size()
  //      << endl;
  // cout << "delStrList's totalRefCountsString(): "
  //      << ConcatStringList::delStrList.totalRefCountsString() << endl;

  // test concat
  // ConcatStringList s4 = s1.concat(s2);
  // cout << "s4: " << s4.toString() << endl;

  // ConcatStringList s5 = s1.concat(s2).concat(s3);
  // cout << "s5: " << s5.toString() << endl;
  // // test subString
  // ConcatStringList s6 = s5.subString(5, 15);
  // cout << "s6: " << s6.toString() << endl;
  // //
  // // // test reverse
  // ConcatStringList s7 = s5.reverse();
  // cout << "s7: " << s7.toString() << endl;

  // cout << "---After deleting s1---" << endl;
  // cout << ConcatStringList::refList.refCountsString() << endl;
  // cout << ConcatStringList::delStrList.totalRefCountsString() << endl;
}

void tc3() {
  ConcatStringList *s1 = new ConcatStringList("Hello"); // 5
  ConcatStringList *s2 = new ConcatStringList("_world"); // 6
  ConcatStringList *s3 = new ConcatStringList("_an_assignment"); // 14
  ConcatStringList *s4 = new ConcatStringList(s1->concat(*s2).concat(*s3));
  //
  // ConcatStringList *s5 = new ConcatStringList(*s4);

  // cout << "s4's length: " << s4->length() << endl;
  // try {
  //   cout << "char at index 11: " << s4->get(11) << endl;
  // } catch (out_of_range &ofr) {
  //   cout << "Exception out_of_range: " << ofr.what() << endl;
  // }
  // // test indexOf
  // cout << "in s4, index of 'o': " << s4->indexOf('o') << endl;
  // cout << "in s4, index of 'a': " << s4->indexOf('a') << endl;
  // cout << "in s4, index of 'd': " << s4->indexOf('d') << endl;
  // // test toString
  // cout << "s4 toString: " << s4->toString() << endl;

  cout << "s4's length: " << s4->length() << endl;
  try {
    cout << "char at index 11: " << s4->get(11) << endl;
    cout << "s4's char: " << s4->get(-1) << endl;
  } catch (out_of_range &ofr) {
    cout << "Exception out_of_range: " << ofr.what() << endl;
  }
  try {
    std::cout << "subString s4: " << s4->subString(1, 1).toString()
              << std::endl;
  } catch (const std::logic_error &ofr) {
    cout << "Exception logic_error: " << ofr.what() << endl;
  }
  std::cout << "reverse s4: " << s4->reverse().toString() << std::endl;
  cout << "in s4, index of 'o': " << s4->indexOf('_') << endl;
  cout << "s4: " << s4->toString() << endl;
  cout << "refList's size(): " << ConcatStringList::refList.size() << endl;

  try {
    cout << "refList's refCountAt(int index): "
         << ConcatStringList::refList.refCountAt(0) << endl;
  } catch (out_of_range &ofr) {
    cout << "Exception out_of_range: " << ofr.what() << endl;
  }
  cout << "refList's refCountsString(): "
       << ConcatStringList::refList.refCountsString() << endl;
  cout << "delStrList's size(): " << ConcatStringList::delStrList.size()
       << endl;
  cout << "delStrList's totalRefCountsString(): "
       << ConcatStringList::delStrList.totalRefCountsString() << endl;

  cout << "---After deleting s4---" << endl;
  delete s4;
  cout << ConcatStringList::refList.refCountsString() << endl;
  cout << ConcatStringList::delStrList.totalRefCountsString() << endl;

  cout << "---After deleting s3---" << endl;
  delete s3;
  cout << ConcatStringList::refList.refCountsString() << endl;
  cout << ConcatStringList::delStrList.totalRefCountsString() << endl;

  // cout << "---After deleting s2---" << endl;
  // delete s2;
  // cout << ConcatStringList::refList.refCountsString() << endl;
  // cout << ConcatStringList::delStrList.totalRefCountsString() << endl;

  // cout << "---After deleting s1---" << endl;
  // delete s1;
  // cout << ConcatStringList::refList.refCountsString() << endl;
  // cout << ConcatStringList::delStrList.totalRefCountsString() << endl;

  // cout << "s5's length: " << s5->length() << endl;
  // try {
  //   cout << "char at index 11: " << s5->get(11) << endl;
  // } catch (out_of_range &ofr) {
  //   cout << "Exception out_of_range: " << ofr.what() << endl;
  // }
  // // test indexOf
  // cout << "in s5, index of 'o': " << s5->indexOf('o') << endl;
  // cout << "in s5, index of 'a': " << s5->indexOf('a') << endl;
  // // test toString
  // cout << "s5 toString: " << s5->toString() << endl;

  // cout << ConcatStringList::refList.refCountsString() << endl;
  // cout << ConcatStringList::delStrList.totalRefCountsString() << endl;

  // cout << "---After deleting s1---" << endl;
  // delete s1;
  // cout << ConcatStringList::refList.refCountsString() << endl;
  // cout << ConcatStringList::delStrList.totalRefCountsString() << endl;
  //
  // cout << "---After deleting s3---" << endl;
  // delete s3;
  // cout << ConcatStringList::refList.refCountsString() << endl;
  // cout << ConcatStringList::delStrList.totalRefCountsString() << endl;
  //
  // cout << "---After deleting s2---" << endl;
  // delete s2;
  // cout << ConcatStringList::refList.refCountsString() << endl;
  // cout << ConcatStringList::delStrList.totalRefCountsString() << endl;
}

int main() {
  // tc1();
  // tc2();
  tc3();
  return 0;
}
