#include "ConcatStringTree.h"
#include <string>
ConcatStringTree::ConcatStringTree(const char *s) {
  if (s) {
    const char *i = s;
    int sLength = 0;
    string str = "";
    while (*i != '\0') {
      str += *i;
      i++;
      sLength++;
    }
    this->pRoot = new ConcatStringNode(str, sLength);
    this->treeLength = sLength;
  }
}

int ConcatStringTree::length() const { return this->treeLength; }

int ConcatStringTree::indexOfHelper(ConcatStringNode *pR, char c) {
  if (!pR)
    return -1;
  int idxLeft = indexOfHelper(pR->pLeft, c);
  if (idxLeft != -1)
    return idxLeft;
  int idxRight = indexOfHelper(pR->pRight, c);
  if (idxRight != -1)
    return idxRight;
  return (pR->checkDataIsNull()) ? -1 : pR->data.find(c, 0);
}

char ConcatStringTree::get(int index) {
  if (index >= 0 && index < this->treeLength) {
    if (this->pRoot) {
      ConcatStringNode *p = this->pRoot;
      while (p->pLeft || p->pRight) {
        if (p->leftLength > index) {
          p = p->pLeft;
        } else {
          index -= p->leftLength;
          p = p->pRight;
        }
      }
      return p->data[index];
    }
  }
  throw out_of_range("Index of string is invalid!");
}

int ConcatStringTree::indexOf(char c) { return indexOfHelper(this->pRoot, c); }

string ConcatStringTree::toStringPreOrderHelper(ConcatStringNode *pR) const {
  if (!pR)
    return "";
  string result =
      to_string(pR->leftLength) + "," + to_string(pR->length) + "," +
      (pR->checkDataIsNull() ? "<NULL>;" : ("\"" + pR->data + "\";"));

  return result + toStringPreOrderHelper(pR->pLeft) +
         toStringPreOrderHelper(pR->pRight);
}

string ConcatStringTree::toStringPreOrder() const {
  string str = "ConcatStringTree[" + toStringPreOrderHelper(this->pRoot);
  str[str.length() - 1] = ']';
  return str;
}

string ConcatStringTree::toStringHelper(ConcatStringNode *pR) const {
  if (!pR)
    return "";

  return toStringHelper(pR->pLeft) + toStringHelper(pR->pRight) +
         (pR->checkDataIsNull() ? "" : pR->data);
}

string ConcatStringTree::toString() const {
  return "ConcatStringTree[\"" + toStringHelper(this->pRoot) + "\"]";
}
