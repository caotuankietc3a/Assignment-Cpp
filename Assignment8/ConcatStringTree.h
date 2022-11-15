#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"
class ReducedConcatStringTree; // forward declaration
class ParentTree;
class LitString;

class ConcatStringTree {
public:
  class ConcatStringNode;

private:
  ConcatStringNode *pRoot;
  bool isCSTree;

public:
  ConcatStringTree(const char *s, bool isCSTree = true);
  ConcatStringTree(ConcatStringNode *pR = nullptr, bool isCSTree = true)
      : pRoot(pR), isCSTree(isCSTree) {}
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
    friend class ReducedConcatStringTree;
  };

public:
  friend class ReducedConcatStringTree;
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
    friend class ParentTree;
  };
};

template <class T> class Vector {
private:
  int nE;
  int cap;
  T *arr;

public:
  Vector();
  Vector(int cap);
  ~Vector();
  T &operator[](int);
  Vector &operator=(Vector &);
  void insert(T newElement, int idx);
  void resize(int nE, int cap);

public:
  friend class LitStringHash;
};

class LitString {
private:
  string str;
  int numOfLinks;

public:
  LitString(string str = "", int numOfLinks = 0)
      : str(str), numOfLinks(numOfLinks) {}

private:
  friend class LitStringHash;
};

class HashConfig {
private:
  int p;
  double c1, c2;
  double lambda;
  double alpha;
  int initSize; // m

private:
  friend class ReducedConcatStringTree;
  friend class LitStringHash;

public:
  HashConfig(int p = 0, double c1 = 0, double c2 = 0, double lambda = 0,
             double alpha = 0, int initSize = 0)
      : p(p), c1(c1), c2(c2), lambda(lambda), initSize(initSize) {}
};

class LitStringHash {
private:
  Vector<LitString> litStrings;
  HashConfig hashConfig;
  int lastInsertedIndex;

public:
  LitStringHash(const HashConfig &hashConfig);
  ~LitStringHash();

  int getLastInsertedIndex() const;

  string toStringHelper(const Vector<LitString> &vecLitStrings) const;
  string toString() const;

  int quadratic(const string &s);
  long long hashProbing(const long long &hashF, const HashConfig &hashConfig,
                        int idx) const;
  long long hashFunc(const string &s, const int &n, const int &m,
                     const int &p) const;
  void remove(const string &s);

private:
  friend long long calculatePower(int base, int powerRaised);
};

class ReducedConcatStringTree : public ConcatStringTree {
public:
  LitStringHash *litStringHash = nullptr;
  ReducedConcatStringTree(const char *s, LitStringHash *litStringHash);
  ~ReducedConcatStringTree();

  // ReducedConcatStringTree concat(const ReducedConcatStringTree &otherS)
  // const;

  // ReducedConcatStringTree subString(int from, int to) const;
  // ReducedConcatStringTree *subStringHelper(ReducedConcatStringNode *pR,
  //                                          int &from, int to,
  //                                          int &totalLen) const;
  // string reverseString(string str) const;
  // ReducedConcatStringTree reverse() const;
  // ReducedConcatStringTree *reverseHelper(ReducedConcatStringNode *pR) const;
  //
  // int getParTreeSize(const string &query) const;
  // string getParTreeStringPreOrder(const string &query) const;
  // ReducedConcatStringNode *
  // getReducedConcatStringNode(const string &query) const;
};

#endif // __CONCAT_STRING_TREE_H__
