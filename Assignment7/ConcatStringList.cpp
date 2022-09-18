#include "ConcatStringList.h"
char *reverseString(char *str, int length);

ConcatStringList::ReferencesList ConcatStringList::refList =
    ConcatStringList::ReferencesList();

ConcatStringList::DeleteStringList ConcatStringList::delStrList =
    ConcatStringList::DeleteStringList();

/////////////////////// CharArrayList ////////////////////////////
CharArrayList::CharArrayList() {
  this->s = new char[]{""};
  this->length = 0;
}

CharArrayList::CharArrayList(const char *str) {
  this->length = 0;
  while (*str != '\0') {
    str++;
    this->length++;
  }

  /////////////////////// Need to reallocate memory //////////////////////
  this->s = new char[this->length + 1];
  while (*str != '\0') {
    *s++ = *str++;
  }
  *s = '\0';
}

char *CharArrayList::getString() { return this->s; }

int CharArrayList::getLength() { return this->length; }

char &CharArrayList::operator[](int index) const {
  if (index < 0 || index >= this->length)
    throw out_of_range("Index of string is invalid");
  return this->s[index];
}

CharArrayList::~CharArrayList() { delete[] this->s; }

//////////////////////////// CharAlNode ////////////////////////////
CharAlNode::CharAlNode() {
  this->literalString = CharArrayList();
  this->next = nullptr;
}

CharAlNode::CharAlNode(const char *str) {
  if (str) {
    this->literalString = CharArrayList(str);
    this->next = nullptr;
  } else {
    this->literalString = CharArrayList();
    this->next = nullptr;
  }
}

CharArrayList CharAlNode::getLiteralString() { return this->literalString; }

CharAlNode *&CharAlNode::getNext() { return this->next; }

CharAlNode::~CharAlNode() {}

/////////////////////////// ConcatStringList ////////////////////////////
ConcatStringList::ConcatStringList() {
  this->head = nullptr;
  this->tail = nullptr;
}

ConcatStringList::ConcatStringList(const char *str) {
  CharAlNode *node = nullptr;
  if (str) {
    node = new CharAlNode(str);
  }
  this->size = node->getLiteralString().getLength();
  this->head = node;
  this->tail = node;

  ConcatStringList::ReferencesList::addRefNodeToReferencesList(refList, node,
                                                               2);
  // if (refList.getRefNodeHead()) {
  //   ReferenceNode *i = refList.getRefNodeHead();
  //   ReferenceNode *newRefNode = new ReferenceNode(node, 2);
  //   ///////////////// addHead ///////////////
  //   if (i->getNumOfRef() >= 2) {
  //     newRefNode->getRefNodeNext() = refList.getRefNodeHead();
  //     refList.getRefNodeHead() = newRefNode;
  //     return;
  //   }
  //   ///////////////// addTail or addBetween //////////////
  //   while (i && i->getRefNodeNext() && i->getNumOfRef() < 2 &&
  //          i->getRefNodeNext()->getNumOfRef() < 2) {
  //     i = i->getRefNodeNext();
  //   }
  //   newRefNode->getRefNodeNext() = i->getRefNodeNext();
  //   i->getRefNodeNext() = newRefNode;
  // } else {
  //   refList.getRefNodeHead() = new ReferenceNode(node, 2);
  // }
  // refList.increaseTotalRefs(1);
}

ConcatStringList::~ConcatStringList() {}

int ConcatStringList::length() const { return this->size; }

char ConcatStringList::get(int index) const {
  if (index >= 0 && index < this->size) {
    CharAlNode *node = this->head;
    int totalLen = 0;
    while (node != nullptr) {
      totalLen += node->getLiteralString().getLength();
      if (index + 1 > totalLen) {
        node = node->getNext();
      } else {
        totalLen -= node->getLiteralString().getLength();
        return node->getLiteralString()[index - totalLen];
      }
    }
  }
  throw out_of_range("Index of string is invalid!");
}

int ConcatStringList::indexOf(char c) const {
  CharAlNode *node = this->head;
  int totalLen = 0;
  while (node) {
    CharArrayList str = node->getLiteralString();
    for (int i = 0; i < str.getLength(); i++) {
      if (str[i] == c) {
        return i + totalLen;
      }
    }
    totalLen += node->getLiteralString().getLength();
    node = node->getNext();
  }
  return -1;
}

std::string ConcatStringList::toString() const {
  CharAlNode *node = this->head;
  std::string result = "ConcatStringList[\"";
  while (node) {
    CharArrayList str = node->getLiteralString();
    for (int i = 0; i < str.getLength(); i++) {
      result += str[i];
    }
    node = node->getNext();
  }
  return result + "\"]";
}

ConcatStringList
ConcatStringList::concat(const ConcatStringList &otherS) const {
  this->head->getNext() = otherS.head;
  ConcatStringList newConcatStringList = ConcatStringList();
  newConcatStringList.head = this->head;
  newConcatStringList.tail = otherS.tail;
  newConcatStringList.size = otherS.length() + this->length();

  ReferenceNode *i = refList.getRefNodeHead();
  ReferenceNode *preI = nullptr;
  ReferenceNode *nextI = nullptr;
  bool findHead = false, findTail = false;
  while (i && (!findHead || !findTail)) {
    nextI = i->getRefNodeNext();
    if ((i->getNode() == this->head && !findHead) ||
        (i->getNode() == otherS.tail && !findTail)) {
      if (i->getNode() == this->head)
        findHead = true;
      if (i->getNode() == otherS.tail)
        findTail = true;

      i->increaseNumOfRef(1);
      while (i && nextI && i->getNumOfRef() > nextI->getNumOfRef()) {
        i->getRefNodeNext() = nextI->getRefNodeNext();
        nextI->getRefNodeNext() = i;
        if (!preI)
          refList.getRefNodeHead() = nextI; // head must point to the first one

        preI = nextI;
        nextI = i->getRefNodeNext();
      }
      i = refList.getRefNodeHead();
      preI = nullptr;
      nextI = nullptr;
    } else {
      preI = i;
      i = i->getRefNodeNext();
    }
  }
  return newConcatStringList;
}

ConcatStringList ConcatStringList::subString(int from, int to) const {
  if ((from < 0 || from > this->length()) || (to < 0 || to > this->length()))
    throw out_of_range("Index of string is invalid");

  if (from >= to)
    throw logic_error("Invalid range");

  // ConcatStringList newConcatStringList = ConcatStringList();
  int totalLen = 0;
  CharAlNode *node = this->head;
  ConcatStringList newConcatStringList;
  bool isFirstTime = true;
  while (node) {
    const char *str = node->getLiteralString().getString();
    int lengthStr = node->getLiteralString().getLength();
    totalLen += lengthStr;

    if (from + 1 > totalLen) {
      node = node->getNext();
    } else {
      char *newStr = nullptr;
      bool found = false;
      int lengthNewStr;
      //////////////// Check if from and to variable are on the same CharAlNode
      if ((from >= totalLen - lengthStr && from < totalLen) &&
          (to >= totalLen - lengthStr && to < totalLen)) {
        /////////////////////// Need to deallocate memory //////////////////////
        newStr = new char[to - from + 1];
        lengthNewStr = to - from + 1;
        found = true;
      } else {
        /////////////////////// Need to deallocate memory //////////////////////
        newStr = new char[totalLen - from + 1];
        lengthNewStr = totalLen - from + 1;
      }

      /////////////////// Create newStr //////////////////////////////
      int i = 0;
      while (i < lengthNewStr - 1 && from < to) {
        newStr[i] = str[from - (totalLen - lengthStr)];
        i++;
        from++;
      }
      newStr[i] = '\0';

      if (isFirstTime) {
        newConcatStringList = ConcatStringList(newStr);
        isFirstTime = false;
      } else {
        newConcatStringList.tail->getNext() = new CharAlNode(newStr);
        newConcatStringList.tail = newConcatStringList.tail->getNext();
      }

      ///////////////////////////////// deallocate memory
      ///////////////////////////
      delete[] newStr;
      if (found)
        break;
      else
        node = node->getNext();
    }
  }

  ////////////////////////// Add to ReferencesList ////////////////////////
  if (newConcatStringList.head == newConcatStringList.tail)
    ConcatStringList::ReferencesList::addRefNodeToReferencesList(
        refList, newConcatStringList.head, 2);
  else {
    ConcatStringList::ReferencesList::addRefNodeToReferencesList(
        refList, newConcatStringList.head, 1);
    ConcatStringList::ReferencesList::addRefNodeToReferencesList(
        refList, newConcatStringList.tail, 1);
  }
  return newConcatStringList;
}

ConcatStringList ConcatStringList::reverse() const {
  CharAlNode *node = this->head;
  ConcatStringList newConcatStringList;
  bool isFirstTime = true;
  while (node) {
    char *reversedStr = reverseString(node->getLiteralString().getString(),
                                      node->getLiteralString().getLength());
    if (isFirstTime) {
      newConcatStringList = ConcatStringList(reversedStr);
      isFirstTime = false;
    } else {
      CharAlNode *newNode = new CharAlNode(reversedStr);
      newNode->getNext() = newConcatStringList.head;
      newConcatStringList.head = newNode;
    }
    node = node->getNext();
  }

  ////////////////////////// Add to ReferencesList ////////////////////////
  if (newConcatStringList.head == newConcatStringList.tail)
    ConcatStringList::ReferencesList::addRefNodeToReferencesList(
        refList, newConcatStringList.head, 2);
  else {
    ConcatStringList::ReferencesList::addRefNodeToReferencesList(
        refList, newConcatStringList.head, 1);
    ConcatStringList::ReferencesList::addRefNodeToReferencesList(
        refList, newConcatStringList.tail, 1);
  }
  return newConcatStringList;
}

char *reverseString(char *str, int length) {
  for (int i = 0; i < length / 2; i++) {
    char x = str[i];
    str[i] = str[length - i - 1];
    str[length - i - 1] = x;
  }
  return str;
}

////////////////////////////////////////// ReferenceNode
/////////////////////////////
ReferenceNode::ReferenceNode() {
  this->node = nullptr;
  this->numOfRef = 0;
  this->refNodeNext = nullptr;
}

ReferenceNode::ReferenceNode(CharAlNode *&node, int numOfRef) {
  this->node = node;
  this->numOfRef = numOfRef;
  this->refNodeNext = nullptr;
}

CharAlNode *&ReferenceNode::getNode() { return this->node; }

ReferenceNode *&ReferenceNode::getRefNodeNext() { return this->refNodeNext; }

int ReferenceNode::getNumOfRef() { return this->numOfRef; }

void ReferenceNode::increaseNumOfRef(int num) { this->numOfRef += num; }

/////////////////////////////////////////// ReferencesList ////////////////
// ConcatStringList::ReferencesList::ReferencesList(CharAlNode *&node,
//                                                  int numOfRef) {
//   this->reference = node;
//   this->numOfRef = numOfRef;
// }
//
ReferenceNode *&ConcatStringList::ReferencesList::getRefNodeHead() {
  return this->refNodeHead;
}

void ConcatStringList::ReferencesList::increaseTotalRefs(int num) {
  this->totalRefs += num;
}

void ConcatStringList::ReferencesList::addRefNodeToReferencesList(
    ConcatStringList::ReferencesList &refList, CharAlNode *&toNode,
    int numOfRef) {
  if (refList.getRefNodeHead()) {
    ReferenceNode *i = refList.getRefNodeHead();
    ReferenceNode *newRefNode = new ReferenceNode(toNode, numOfRef);

    ///////////////// addHead ///////////////
    if (i->getNumOfRef() >= numOfRef) {
      newRefNode->getRefNodeNext() = refList.getRefNodeHead();
      refList.getRefNodeHead() = newRefNode;
      return;
    }
    ///////////////// addTail or addBetween //////////////
    while (i && i->getRefNodeNext() && i->getNumOfRef() < numOfRef &&
           i->getRefNodeNext()->getNumOfRef() < numOfRef) {
      i = i->getRefNodeNext();
    }
    newRefNode->getRefNodeNext() = i->getRefNodeNext();
    i->getRefNodeNext() = newRefNode;
  } else {
    refList.getRefNodeHead() = new ReferenceNode(toNode, numOfRef);
  }
  refList.increaseTotalRefs(1);
}

//
// ConcatStringList::ReferencesList
// *&ConcatStringList::ReferencesList::getNext() {
//   return this->refNext;
// }
