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
  }
}

int ConcatStringTree::length() const { return this->pRoot->length; }

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
  if (!(index >= 0 && index < this->pRoot->length)) {
    throw out_of_range("Index of string is invalid!");
  }
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

ConcatStringTree
ConcatStringTree::concat(const ConcatStringTree &otherS) const {
  return ConcatStringTree(
      new ConcatStringNode("", otherS.length() + this->length(), this->length(),
                           this->pRoot, otherS.pRoot));
}

ConcatStringTree *ConcatStringTree::subStringHelper(ConcatStringNode *pR,
                                                    int &from, int to,
                                                    int &totalLen) const {
  if (!pR)
    return nullptr;

  ConcatStringTree *tLeft = subStringHelper(pR->pLeft, from, to, totalLen);
  ConcatStringTree *tRight = subStringHelper(pR->pRight, from, to, totalLen);

  if (pR->checkDataIsNull()) {
    return new ConcatStringTree(tLeft->concat(*tRight));
  }

  string str = "";
  if (pR->length + totalLen <= to) {
    if (from >= pR->length) {
      from -= totalLen;
      totalLen += pR->length;
      return nullptr;
    }
    str = pR->data.substr(from, pR->length);
    from = 0;
    totalLen += pR->length;
  } else {
    str = pR->data.substr(from, to - totalLen);
    totalLen += pR->length;
  }
  return new ConcatStringTree(new ConcatStringNode(str, str.length()));
}

ConcatStringTree ConcatStringTree::subString(int from, int to) const {
  if ((from < 0 || from >= this->length()) || (to < 0 || to > this->length()))
    throw out_of_range("Index of string is invalid");

  if (from >= to)
    throw logic_error("Invalid range");

  int totalLen = 0;
  return *subStringHelper(this->pRoot, from, to, totalLen);
}

string ConcatStringTree::reverseString(string str) const {
  int n = str.length();
  for (int i = 0; i < n / 2; i++) {
    char x = str[i];
    str[i] = str[n - i - 1];
    str[n - i - 1] = x;
  }
  return str;
}

ConcatStringTree *ConcatStringTree::reverseHelper(ConcatStringNode *pR) const {
  if (!pR)
    return nullptr;

  ConcatStringTree *tRight = reverseHelper(pR->pRight);
  ConcatStringTree *tLeft = reverseHelper(pR->pLeft);

  if (pR->checkDataIsNull()) {
    return new ConcatStringTree(tLeft->concat(*tRight));
  }

  string str = reverseString(pR->data);
  return new ConcatStringTree(new ConcatStringNode(str, str.length()));
}

ConcatStringTree ConcatStringTree::reverse() const {
  return *reverseHelper(this->pRoot);
}
