#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

class ConcatStringTree {
public:
  class ConcatStringNode;

private:
  ConcatStringNode *pRoot = nullptr;
  int treeLength = 0;

public:
  ConcatStringTree(const char *s);
  int length() const;
  char get(int index);

  int indexOfHelper(ConcatStringNode *pR, char c);
  int indexOf(char c);

  string toStringPreOrder() const;
  string toStringPreOrderHelper(ConcatStringNode *pR) const;

  string toString() const;
  string toStringHelper(ConcatStringNode *pR) const;
  ConcatStringTree concat(const ConcatStringTree &otherS) const;
  ConcatStringTree subString(int from, int to) const;
  ConcatStringTree reverse() const;

  int getParTreeSize(const string &query) const;
  string getParTreeStringPreOrder(const string &query) const;

  class ConcatStringNode {
  public:
    ConcatStringNode(string data = "", const int &length = 0,
                     const int &leftLength = 0, ConcatStringNode *pL = nullptr,
                     ConcatStringNode *pR = nullptr)
        : data(data), length(length), leftLength(leftLength), pLeft(pL),
          pRight(pR) {}
    bool checkDataIsNull() {
      return (this->pRight || this->pLeft) ? true : false;
    }
    ConcatStringNode &operator=(const ConcatStringNode &) = default;
    ~ConcatStringNode();

  private:
    int length, leftLength;
    string data;
    ConcatStringNode *pLeft, *pRight;
    friend ConcatStringTree;
  };
};

class ReducedConcatStringTree; // forward declaration

class HashConfig {
private:
  int p;
  double c1, c2;
  double lambda;
  double alpha;
  int initSize;

  friend class ReducedConcatStringTree;
};

class LitStringHash {
public:
  LitStringHash(const HashConfig &hashConfig);
  int getLastInsertedIndex() const;
  string toString() const;
};

class ReducedConcatStringTree /* */ {
public:
  ReducedConcatStringTree(const char *s, LitStringHash *litStringHash);
  LitStringHash *litStringHash;
};

#endif // __CONCAT_STRING_TREE_H__
