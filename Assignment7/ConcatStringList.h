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
  CharArrayList literalString;
  CharAlNode *next;

public:
  CharAlNode();
  CharAlNode(const char *str);
  CharArrayList getLiteralString();
  CharAlNode *&getNext();
  ~CharAlNode();
};

struct ReferenceNode {
private:
  CharAlNode *node;
  int numOfRef;
  ReferenceNode *refNodeNext;

public:
  ReferenceNode();
  ReferenceNode(CharAlNode *&node, int numOfRef);
  ReferenceNode *&getRefNodeNext();
  CharAlNode *&getNode();
  void increaseNumOfRef(int num);
  int getNumOfRef();
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
    ReferenceNode *refNodeHead;
    int totalRefs = 0;

  public:
    // ReferencesList();
    // ReferencesList(CharAlNode *&node, int numOfRef);
    int size() const;
    int refCountAt(int index) const;
    void increaseTotalRefs(int num);
    std::string refCountsString() const;
    ReferenceNode *&getRefNodeHead();
    static void addRefNodeToReferencesList(ConcatStringList::ReferencesList &refList, CharAlNode*& toNode, int numOfRef);
  };

  class DeleteStringList {
    // TODO: may provide some attributes

  public:
    int size() const;
    std::string totalRefCountsString() const;
  };
};

#endif // __CONCAT_STRING_LIST_H__
