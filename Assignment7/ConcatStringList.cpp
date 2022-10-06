#include "ConcatStringList.h"
#include <string>
char *reverseString(char *str, int length);

ConcatStringList::ReferencesList ConcatStringList::refList =
    ConcatStringList::ReferencesList();

ConcatStringList::DeleteStringList ConcatStringList::delStrList =
    ConcatStringList::DeleteStringList();

/////////////////////// CharArrayList ////////////////////////////
CharArrayList::CharArrayList() {
  this->s = nullptr;
  this->length = 0;
}

CharArrayList::CharArrayList(const char *str) {
  this->length = 0;
  const char *i = str;
  while (*i != '\0') {
    i++;
    this->length++;
  }

  /////////////////////// Need to reallocate memory //////////////////////
  this->s = new char[this->length + 1];
  char *j = this->s;
  while (*str != '\0') {
    *this->s++ = *str++;
  }
  *this->s = '\0';
  this->s = j;
}

char *CharArrayList::getString() { return this->s; }

int CharArrayList::getLength() {
  if (this->s != nullptr)
    return this->length;
  return -1;
}

char &CharArrayList::operator[](int index) const {
  if (index < 0 || index >= this->length)
    throw out_of_range("Index of string is invalid");
  return this->s[index];
}

CharArrayList::~CharArrayList() {
  if (this->s) {
    delete[] this->s;
    this->s = nullptr;
    this->length = 0;
  }
}

//////////////////////////// CharAlNode ////////////////////////////
CharAlNode::CharAlNode() {
  this->literalString = new CharArrayList();
  this->next = nullptr;
}

CharAlNode::CharAlNode(const char *str) {
  if (str) {
    this->literalString = new CharArrayList(str);
    this->next = nullptr;
  } else {
    this->literalString = new CharArrayList();
    this->next = nullptr;
  }
}

CharArrayList *&CharAlNode::getLiteralString() { return this->literalString; }

CharAlNode *&CharAlNode::getNext() { return this->next; }

CharAlNode::~CharAlNode() {
  if (this->literalString) {
    delete this->literalString;
    this->literalString = nullptr;
    this->next = nullptr;
  }
}

/////////////////////////// ConcatStringList ////////////////////////////
ConcatStringList::ConcatStringList() {
  this->head = nullptr;
  this->tail = nullptr;
  this->size = 0;
}

ConcatStringList::ConcatStringList(const char *str) {
  CharAlNode *node = nullptr;
  if (str) {
    node = new CharAlNode(str);
  }
  this->size = node->getLiteralString()->getLength();
  this->head = node;
  this->tail = node;

  ConcatStringList::ReferencesList::addRefNodeToReferencesList(refList, node,
                                                               2);
}

ConcatStringList::~ConcatStringList() {
  ReferenceNode *ptrHead = nullptr, *ptrTail = nullptr;
  if (this->head == this->tail) {
    ptrHead = ptrTail = ReferenceNode::decreaseNumOfRef(
        refList.getRefNodeHead(), this->head, 2);
  } else {
    ptrHead = ReferenceNode::decreaseNumOfRef(refList.getRefNodeHead(),
                                              this->head, 1);
    ptrTail = ReferenceNode::decreaseNumOfRef(refList.getRefNodeHead(),
                                              this->tail, 1);
  }

  ////////////////////////// Add DeleteStringNode //////////////////////
  DeleteStringNode *i = delStrList.getDeleteStrNodehead();
  if (!i) {
    delStrList.getDeleteStrNodehead() = new DeleteStringNode(ptrHead, ptrTail);
  } else {
    while (i->getDeleteStrNodeNext()) {
      i = i->getDeleteStrNodeNext();
    }
    i->getDeleteStrNodeNext() = new DeleteStringNode(ptrHead, ptrTail);
  }
  delStrList.increaseTotalLength(1);

  ////////////////////////// Remove DeleteStringNode //////////////////////
  i = delStrList.getDeleteStrNodehead();
  DeleteStringNode *preI = nullptr;
  while (i) {
    if (i->getRefHead()->getNumOfRef() + i->getRefTail()->getNumOfRef() == 0) {
      ////////////////////////// Delete CharAlNode head and tail
      //////////////////////////////

      // CharAlNode *nextDeletedAlNode = i->getRefHead()->getNode();
      // CharAlNode *&deletedAlNode = i->getRefHead()->getNode();
      // bool foundTail = false;
      // while (nextDeletedAlNode && !foundTail) {
      //   nextDeletedAlNode = nextDeletedAlNode->getNext();
      //   if (deletedAlNode) {
      //     if (deletedAlNode == i->getRefTail()->getNode()) {
      //       foundTail = true;
      //       delete deletedAlNode;
      //       break;
      //     }
      //     deletedAlNode->getNext() = nullptr;
      //     delete deletedAlNode;
      //     deletedAlNode = nullptr;
      //   }
      //   deletedAlNode = nextDeletedAlNode;
      // }
      // CharAlNode *&k = i->getRefHead()->getNode();
      // CharAlNode *&k1 = i->getRefTail()->getNode();
      // k = k1 = nullptr;

      ////////////////////////// Remove DeleteStringNode
      /////////////////////////
      DeleteStringNode *j = i;
      if (preI)
        preI->getDeleteStrNodeNext() = i->getDeleteStrNodeNext();
      else
        delStrList.getDeleteStrNodehead() = i->getDeleteStrNodeNext();

      i = i->getDeleteStrNodeNext();
      if (j) {
        delete j;
      }
      delStrList.decreaseTotalLength(1);
    } else {
      preI = i;
      i = i->getDeleteStrNodeNext();
    }
  }

  ////////////////////// Remove all refNodes in refList //////////////////
  if (refList.checkSumZeroInRefList()) {
    ReferenceNode *deletedRefNode = refList.getRefNodeHead();
    ReferenceNode *nextDeletedRefNode = refList.getRefNodeHead();
    while (nextDeletedRefNode) {
      nextDeletedRefNode = nextDeletedRefNode->getRefNodeNext();
      deletedRefNode->getRefNodeNext() = nullptr;
      if (deletedRefNode) {
        delete deletedRefNode;
      }
      deletedRefNode = nextDeletedRefNode;
    }
    refList.increaseTotalRefs(-refList.size());
    refList.getRefNodeHead() = nullptr;
  }
}

int ConcatStringList::length() const { return this->size; }

char ConcatStringList::get(int index) const {
  if (index >= 0 && index < this->size) {
    CharAlNode *node = this->head;
    int totalLen = 0;
    while (node != nullptr) {
      totalLen += node->getLiteralString()->getLength();
      if (index + 1 > totalLen) {
        node = node->getNext();
      } else {
        totalLen -= node->getLiteralString()->getLength();
        return node->getLiteralString()->getString()[index - totalLen];
      }
    }
  }
  throw out_of_range("Index of string is invalid!");
}

int ConcatStringList::indexOf(char c) const {
  CharAlNode *node = this->head;
  int totalLen = 0;
  while (node) {
    CharArrayList *str = node->getLiteralString();
    for (int i = 0; i < str->getLength(); i++) {
      if (str->getString()[i] == c) {
        return i + totalLen;
      }
    }
    totalLen += node->getLiteralString()->getLength();
    node = node->getNext();
  }
  return -1;
}

std::string ConcatStringList::toString() const {
  CharAlNode *node = this->head;
  std::string result = "";
  while (node) {
    CharArrayList *str = node->getLiteralString();
    for (int i = 0; i < str->getLength(); i++) {
      result += str->getString()[i];
    }
    if (node == this->tail)
      break;
    node = node->getNext();
  }
  return result;
}

ConcatStringList
ConcatStringList::concat(const ConcatStringList &otherS) const {
  this->tail->getNext() = otherS.head;
  ConcatStringList newConcatStringList = ConcatStringList();
  newConcatStringList.head = this->head;
  newConcatStringList.tail = otherS.tail;
  newConcatStringList.size = otherS.length() + this->length();

  ReferenceNode *i = refList.getRefNodeHead();
  ReferenceNode *preI = nullptr;
  ReferenceNode *j = nullptr;
  ReferenceNode *preJ = nullptr;
  bool findHead = false, findTail = false;

  while (i && (!findHead || !findTail)) {
    j = i->getRefNodeNext();

    if ((i->getNode() == this->head && !findHead) ||
        (i->getNode() == otherS.tail && !findTail)) {

      if (i->getNode() == this->head && !findHead)
        findHead = true;
      else if (i->getNode() == otherS.tail && !findTail)
        findTail = true;

      i->increaseNumOfRef(1);

      ///////////////////// 3->0->0->0 //////////////////
      if (j && j->getNumOfRef() == 0) {
        break;
      }

      bool isFound = false;
      while (i && j && i->getNumOfRef() > j->getNumOfRef() &&
             j->getNumOfRef() != 0) {
        preJ = j;
        j = j->getRefNodeNext();
        if (!isFound)
          isFound = true;
      }

      if (isFound) {
        if (preI)
          preI->getRefNodeNext() = i->getRefNodeNext();
        else
          refList.getRefNodeHead() = i->getRefNodeNext();
        i->getRefNodeNext() = j;

        if (preJ)
          preJ->getRefNodeNext() = i;
      }
      if (findHead && findTail)
        break;
      i = refList.getRefNodeHead();
      preI = nullptr;
      j = nullptr;
      preJ = nullptr;
    } else {
      preI = i;
      i = i->getRefNodeNext();
    }
  }
  return newConcatStringList;
}

ConcatStringList ConcatStringList::subString(int from, int to) const {
  if ((from < 0 || from >= this->length()) || (to < 0 || to > this->length()))
    throw out_of_range("Index of string is invalid");

  if (from >= to)
    throw logic_error("Invalid range");

  int totalLen = 0;
  CharAlNode *node = this->head;
  ConcatStringList newConcatStringList = ConcatStringList();
  bool isFirstTime = true;
  while (node && from < to) {
    const char *str = node->getLiteralString()->getString();
    int lengthStr = node->getLiteralString()->getLength();
    totalLen += lengthStr;

    if (from + 1 > totalLen) {
      node = node->getNext();
    } else {
      char *newStr = nullptr;
      bool found = false;
      int lengthNewStr = 0;
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
        newConcatStringList.head = newConcatStringList.tail =
            new CharAlNode(newStr);
        isFirstTime = false;
      } else {
        newConcatStringList.tail->getNext() = new CharAlNode(newStr);
        newConcatStringList.tail = newConcatStringList.tail->getNext();
      }

      newConcatStringList.size +=
          newConcatStringList.tail->getLiteralString()->getLength();

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
  ConcatStringList newConcatStringList = ConcatStringList();
  bool isFirstTime = true;
  while (node) {
    char *reversedStr = reverseString(node->getLiteralString()->getString(),
                                      node->getLiteralString()->getLength());
    if (isFirstTime) {
      newConcatStringList.head = newConcatStringList.tail =
          new CharAlNode(reversedStr);
      isFirstTime = false;
    } else {
      CharAlNode *newNode = new CharAlNode(reversedStr);
      newNode->getNext() = newConcatStringList.head;
      newConcatStringList.head = newNode;
    }
    newConcatStringList.size +=
        newConcatStringList.tail->getLiteralString()->getLength();
    delete[] reversedStr;
    if (node == this->tail)
      break;
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
  char *newStr = new char[length + 1];
  int i;
  for (i = 0; i < length; i++) {
    newStr[i] = str[length - i - 1];
  }
  newStr[i] = '\0';
  return newStr;
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

ReferenceNode::~ReferenceNode() {}

CharAlNode *&ReferenceNode::getNode() { return this->node; }

ReferenceNode *&ReferenceNode::getRefNodeNext() { return this->refNodeNext; }

int ReferenceNode::getNumOfRef() { return this->numOfRef; }

void ReferenceNode::increaseNumOfRef(int num) { this->numOfRef += num; }

ReferenceNode *ReferenceNode::decreaseNumOfRef(ReferenceNode *&refNode,
                                               CharAlNode *&toNode, int num) {
  ReferenceNode *i = refNode;
  ReferenceNode *j = nullptr, *preJ = nullptr, *preI = nullptr;
  while (i) {
    if (i->getNode() == toNode) {
      i->numOfRef -= num;

      if (i->numOfRef != 0) {
        //////////////// Move i->numOfRef to the bottom //////////////////////
        if (preI) {
          j = refNode;
          while (j && j->getNumOfRef() < i->getNumOfRef()) {
            preJ = j;
            j = j->getRefNodeNext();
          }
          preI->getRefNodeNext() = i->getRefNodeNext();
          i->getRefNodeNext() = j;
          if (preJ)
            preJ->getRefNodeNext() = i;
          else
            refNode = i;
        }
      } else {
        //////////////// Move i->numOfRef to the bottom //////////////////////
        j = i->getRefNodeNext();
        if (j && j->getNumOfRef() == 0)
          break;
        bool isFound =
            false; /////////// To assure that this element dont't need to move.
        while (j && j->getNumOfRef() != 0) {
          preJ = j;
          j = j->getRefNodeNext();
          if (!isFound)
            isFound = true;
        }
        if (isFound) {
          if (preI) {
            preI->getRefNodeNext() = i->getRefNodeNext();
          } else {
            refNode = i->getRefNodeNext();
          }

          i->getRefNodeNext() = j;
          if (preJ)
            preJ->getRefNodeNext() = i;
        }
      }
      break;
    }
    preI = i;
    i = i->getRefNodeNext();
  }
  return i;
}

/////////////////////////////////////////// ReferencesList ////////////////
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
      refList.increaseTotalRefs(1);
      return;
    }
    ///////////////// addTail or addBetween //////////////
    while (i && i->getRefNodeNext() && i->getNumOfRef() != 0 &&
           i->getRefNodeNext()->getNumOfRef() != 0 &&
           i->getNumOfRef() < numOfRef &&
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

bool ConcatStringList::ReferencesList::checkSumZeroInRefList() {
  ReferenceNode *i = this->refNodeHead;
  int sum = 0;
  while (i) {
    if (i->getNumOfRef())
      return false;

    i = i->getRefNodeNext();
  }
  return true;
}

int ConcatStringList::ReferencesList::size() const { return this->totalRefs; }

int ConcatStringList::ReferencesList::refCountAt(int index) const {
  if (index < 0 || index >= this->totalRefs)
    throw out_of_range("Index of references list is invalid!");

  ReferenceNode *refNode = this->refNodeHead;
  for (int i = 0; i < index && refNode; i++) {
    refNode = refNode->getRefNodeNext();
  }

  return refNode->getNumOfRef();
}

std::string ConcatStringList::ReferencesList::refCountsString() const {
  std::string result = "RefCounts[";
  std::string test = "";
  ReferenceNode *i = this->refNodeHead;
  if (i) {
    while (i) {
      result = result + std::to_string(i->getNumOfRef());
      // test += i->getNode()->getLiteralString()->getString();
      if (i->getRefNodeNext()) {
        // test += "@";
        result += ",";
      }
      i = i->getRefNodeNext();
    }
  }
  return result + "]";
}

////////////////////////////////// DeleteStringNode //////////////////////////
DeleteStringNode::DeleteStringNode(ReferenceNode *&refHead,
                                   ReferenceNode *&refTail) {
  this->refHead = refHead;
  this->refTail = refTail;
  this->deleteStrNodeNext = nullptr;
}

DeleteStringNode::~DeleteStringNode() {}

DeleteStringNode *&DeleteStringNode::getDeleteStrNodeNext() {
  return this->deleteStrNodeNext;
}

ReferenceNode *&DeleteStringNode::getRefHead() { return this->refHead; }

ReferenceNode *&DeleteStringNode::getRefTail() { return this->refTail; }

////////////////////////////////// DeleteStringList //////////////////////////

DeleteStringNode *&ConcatStringList::DeleteStringList::getDeleteStrNodehead() {
  return this->deleteStrNodehead;
}

void ConcatStringList::DeleteStringList::increaseTotalLength(int num) {
  this->length += num;
}

void ConcatStringList::DeleteStringList::decreaseTotalLength(int num) {
  this->length -= num;
}

int ConcatStringList::DeleteStringList::size() const { return this->length; }

//////////////////////////////// Need to check again /////////////////////////
std::string ConcatStringList::DeleteStringList::totalRefCountsString() const {
  std::string result = "TotalRefCounts[";
  DeleteStringNode *i = delStrList.getDeleteStrNodehead();
  while (i) {
    if (i->getRefHead()->getNumOfRef() + i->getRefTail()->getNumOfRef()) {
      result += std::to_string(i->getRefHead()->getNumOfRef() +
                               i->getRefTail()->getNumOfRef());
      if (i->getDeleteStrNodeNext())
        result += ",";
    }
    i = i->getDeleteStrNodeNext();
  }
  return result + "]";
}
