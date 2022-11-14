#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"
class ReducedConcatStringTree; // forward declaration
class ParentTree;

class ConcatStringTree {
public:
  class ConcatStringNode;

private:
  ConcatStringNode *pRoot;

public:
  ConcatStringTree(const char *s);
  ConcatStringTree(ConcatStringNode *pR = nullptr) : pRoot(pR) {}
  ~ConcatStringTree();
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
  ConcatStringTree *subStringHelper(ConcatStringNode *pR, int &from, int to,
                                    int &totalLen) const;

  string reverseString(string str) const;
  ConcatStringTree reverse() const;
  ConcatStringTree *reverseHelper(ConcatStringNode *pR) const;

  int getParTreeSize(const string &query) const;
  string getParTreeStringPreOrder(const string &query) const;
  ConcatStringNode *getConcatStringNode(const string &query) const;

public:
  class ConcatStringNode {
  private:
    int length, leftLength;
    string data;
    ParentTree *pTree;
    ConcatStringNode *pLeft, *pRight;
    long long id;

  public:
    ConcatStringNode(string data = "", const int &length = 0,
                     const int &leftLength = 0, ConcatStringNode *pL = nullptr,
                     ConcatStringNode *pR = nullptr);

    bool checkDataIsNull() {
      return (this->pRight || this->pLeft) ? true : false;
    }
    ~ConcatStringNode(); // Need to deallocate pointer

  public:
    friend ConcatStringTree;
  };
};

class ParentTree {
public:
  class ParentNode;
  static long long globalId;

private:
  ParentNode *pRoot;
  int nE;

public:
  ParentTree(ParentNode *pRoot = nullptr, int nE = 0) : pRoot(pRoot), nE(nE) {}

  int size() const;

  string toStringPreOrderHelper(ParentNode *pR) const;
  string toStringPreOrder() const;

  int getHeightHelper(ParentNode *node) const;
  int getHeight() const;

  ParentNode *rightRotate(ParentNode *x);

  ParentNode *leftRotate(ParentNode *x);

  int getBalance(ParentNode *node) const;

  ParentNode *insertHelper(ParentNode *node, const long long &value);
  void insert(const long long &value);

  ParentNode *maxValueNode(ParentNode *node);
  ParentNode *removeHelper(ParentNode *node, const long long &value);
  void remove(const long long &value);

public:
  class ParentNode {
  public:
    ParentNode(long long id = 0, ParentNode *pLeft = nullptr,
               ParentNode *pRight = nullptr)
        : id(id), pLeft(pLeft), pRight(pRight) {}

  private:
    long long id;
    ParentNode *pLeft;
    ParentNode *pRight;

  public:
    friend ParentTree;
  };
};

template <class T> class Vector {
private:
  int nE;
  int cap;
  T *arr;

public:
  Vector();
  Vector(int nE, int cap);
  ~Vector();
  void setSize(int nE);
  int getSize();
  int getCap();
  T &operator[](int);
  Vector &operator=(Vector &);
  void push_back(T newElement);
  void resize(int nE, int cap);
};

class LitString {
private:
  string str;
  int numOfLinks;

public:
  LitString(string str = "", int numOfLinks = 0)
      : str(str), numOfLinks(numOfLinks) {}
};

class HashConfig {
private:
  int p;
  double c1, c2;
  double lambda;
  double alpha;
  int initSize;

  friend class ReducedConcatStringTree;

public:
  HashConfig(int p = 0, double c1 = 0, double c2 = 0, double lambda = 0,
             double alpha = 0, int initSize = 0)
      : p(p), c1(c1), c2(c2), lambda(lambda), initSize(initSize) {}
};

class LitStringHash {
private:
  Vector<LitString> litStrings;
  HashConfig hashConfig;

public:
  LitStringHash(const HashConfig &hashConfig);
  int getLastInsertedIndex() const;
  string toString() const;
};

class ReducedConcatStringTree {
public:
  class ReducedConcatStringNode;

public:
  ReducedConcatStringTree(const char *s, LitStringHash *litStringHash);
  LitStringHash *litStringHash;

  int length() const;
  char get(int index);

  int indexOfHelper(ReducedConcatStringNode *pR, char c);
  int indexOf(char c);

  string toStringPreOrder() const;
  string toStringPreOrderHelper(ReducedConcatStringNode *pR) const;

  string toString() const;
  string toStringHelper(ReducedConcatStringNode *pR) const;

  ReducedConcatStringTree concat(const ReducedConcatStringTree &otherS) const;

  ReducedConcatStringTree subString(int from, int to) const;
  ReducedConcatStringTree *subStringHelper(ReducedConcatStringNode *pR,
                                           int &from, int to,
                                           int &totalLen) const;

  string reverseString(string str) const;
  ReducedConcatStringTree reverse() const;
  ReducedConcatStringTree *reverseHelper(ReducedConcatStringNode *pR) const;

  int getParTreeSize(const string &query) const;
  string getParTreeStringPreOrder(const string &query) const;
  ReducedConcatStringNode *
  getReducedConcatStringNode(const string &query) const;

public:
  class ReducedConcatStringNode {
  private:
    int length, leftLength;
    string data;
    ReducedConcatStringNode *pLeft, *pRight;
    long long id;

  public:
    ReducedConcatStringNode(string data = "", const int &length = 0,
                            const int &leftLength = 0,
                            ReducedConcatStringNode *pL = nullptr,
                            ReducedConcatStringNode *pR = nullptr);

    bool checkDataIsNull() {
      return (this->pRight || this->pLeft) ? true : false;
    }
    ~ReducedConcatStringNode(); // Need to deallocate pointer

  public:
    friend ReducedConcatStringTree;
  };
};

#endif // __CONCAT_STRING_TREE_H__
