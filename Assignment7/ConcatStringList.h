#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"
struct CharArrayList {
private:
  char *s;
  int length;

public:
  CharArrayList();
  CharArrayList(const char *str);
  int getLength();
  char *getString();
  char &operator[](int index) const;
  ~CharArrayList();
};

struct CharAlNode {
private:
  CharArrayList *literalString;
  CharAlNode *next;

public:
  CharAlNode();
  CharAlNode(const char *str);
  CharArrayList *&getLiteralString();
  CharAlNode *&getNext();
  ~CharAlNode();
};

struct ReferenceNode {
private:
  CharAlNode *node;
  int numOfRef;
  ReferenceNode *refNodeNext;
  // int checkCount;

public:
  ReferenceNode();
  ~ReferenceNode();
  ReferenceNode(CharAlNode *&node, int numOfRef);
  ReferenceNode *&getRefNodeNext();
  CharAlNode *&getNode();
  // int getCheckCount();
  // void increaseCheckCount(int num);
  // void decreaseCheckCount(int num);
  void increaseNumOfRef(int num);
  static ReferenceNode *decreaseNumOfRef(ReferenceNode *&refNode,
                                         CharAlNode *&toNode, int num);
  int getNumOfRef();
};

struct DeleteStringNode {
private:
  ReferenceNode *refHead = nullptr;
  ReferenceNode *refTail = nullptr;
  DeleteStringNode *deleteStrNodeNext = nullptr;

public:
  DeleteStringNode(ReferenceNode *&refHead, ReferenceNode *&refTail);
  ~DeleteStringNode();
  DeleteStringNode *&getDeleteStrNodeNext();
  ReferenceNode *&getRefHead();
  ReferenceNode *&getRefTail();
};

class ConcatStringList {
public:
  class ReferencesList;   // forward declaration
  class DeleteStringList; // forward declaration

public:
  static ReferencesList refList;
  static DeleteStringList delStrList;

  // TODO: may provide some attributes
private:
  CharAlNode *head;
  CharAlNode *tail;
  int size;

public:
  ConcatStringList();
  ConcatStringList(const char *str);
  int length() const;
  char get(int index) const;
  int indexOf(char c) const;
  std::string toString() const;
  ConcatStringList concat(const ConcatStringList &otherS) const;
  ConcatStringList subString(int from, int to) const;
  ConcatStringList reverse() const;
  ~ConcatStringList();

public:
  class ReferencesList {
    // TODO: may provide some attributes
  private:
    ReferenceNode *refNodeHead = nullptr;
    int totalRefs = 0;

  public:
    int size() const;
    int refCountAt(int index) const;
    std::string refCountsString() const;
    void increaseTotalRefs(int num);
    ReferenceNode *&getRefNodeHead();
    static void
    addRefNodeToReferencesList(ConcatStringList::ReferencesList &refList,
                               CharAlNode *&toNode, int numOfRef);
    bool checkSumZeroInRefList();
  };

  class DeleteStringList {
    // TODO: may provide some attributes
  private:
    DeleteStringNode *deleteStrNodehead = nullptr;
    int length = 0;

  public:
    int size() const;
    std::string totalRefCountsString() const;
    DeleteStringNode *&getDeleteStrNodehead();
    void increaseTotalLength(int num);
    void decreaseTotalLength(int num);
  };
};

#endif // __CONCAT_STRING_LIST_H__
