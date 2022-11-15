#include "ConcatStringTree.h"

long long ParentTree::globalId = 0;

////////////////////////////////////////// ConcatStringTree
////////////////////////////////////////////

ConcatStringTree::ConcatStringNode::ConcatStringNode(string data,
                                                     const int &length,
                                                     const int &leftLength,
                                                     ConcatStringNode *pL,
                                                     ConcatStringNode *pR)
    : data(data), length(length), leftLength(leftLength), pLeft(pL),
      pRight(pR) {
  this->pTree = new ParentTree();
  this->pTree->insert(++ParentTree::globalId);
  this->id = ParentTree::globalId;
}

ConcatStringTree::ConcatStringNode::~ConcatStringNode() {}

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
  ConcatStringTree concatStringTree = ConcatStringTree(
      new ConcatStringNode("", otherS.length() + this->length(), this->length(),
                           this->pRoot, otherS.pRoot));
  concatStringTree.pRoot->pRight->pTree->insert(ParentTree::globalId);
  concatStringTree.pRoot->pLeft->pTree->insert(ParentTree::globalId);
  return concatStringTree;
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

int ConcatStringTree::getParTreeSize(const string &query) const {
  ConcatStringNode *node = getConcatStringNode(query);
  return node->pTree->size();
}

ConcatStringTree::ConcatStringNode *
ConcatStringTree::getConcatStringNode(const string &query) const {
  ConcatStringNode *pR = this->pRoot;
  for (int i = 0; i < query.length(); i++) {
    if (!pR)
      throw runtime_error("Invalid query: reaching NULL");
    if (query[i] == 'l') {
      pR = pR->pLeft;
    } else if (query[i] == 'r') {
      pR = pR->pRight;
    } else
      throw runtime_error("Invalid character of query");
  }
  return pR;
}

string ConcatStringTree::getParTreeStringPreOrder(const string &query) const {
  ConcatStringNode *node = getConcatStringNode(query);
  return node->pTree->toStringPreOrder();
}

ConcatStringTree::~ConcatStringTree() {
  if (this->pRoot) {
    if (this->pRoot->pLeft && this->pRoot->pLeft->pTree->size() <= 0) {
      this->pRoot->pLeft->pTree->remove(this->pRoot->id);
      if (this->pRoot->pTree->size() <= 0) {
        ((ReducedConcatStringTree *)(this))
            ->litStringHash->remove(this->pRoot->data);
      }
      delete this->pRoot->pLeft;
      this->pRoot->pLeft = nullptr;
    }

    if (this->pRoot->pRight && this->pRoot->pRight->pTree->size() <= 0) {
      this->pRoot->pRight->pTree->remove(this->pRoot->id);
      if (this->pRoot->pTree->size() <= 0) {
        ((ReducedConcatStringTree *)(this))
            ->litStringHash->remove(this->pRoot->data);
      }
      delete this->pRoot->pRight;
      this->pRoot->pRight = nullptr;
    }

    if (this->pRoot->pTree->size() <= 0) {
      this->pRoot->pTree->remove(this->pRoot->id);
      if (this->pRoot->pTree->size() <= 0) {
        ((ReducedConcatStringTree *)(this))
            ->litStringHash->remove(this->pRoot->data);
      }
      delete this->pRoot;
      this->pRoot = nullptr;
    }
  }
}

////////////////////////////////////////// ParentTree
////////////////////////////////////////////

int ParentTree::size() const { return this->nE; }

string ParentTree::toStringPreOrderHelper(ParentNode *pR) const {
  if (!pR)
    return "";
  string result = "(id=" + to_string(pR->id) + ");";

  return result + toStringPreOrderHelper(pR->pLeft) +
         toStringPreOrderHelper(pR->pRight);
}

string ParentTree::toStringPreOrder() const {
  string str = "ParentsTree[" + toStringPreOrderHelper(this->pRoot);
  str[str.length() - 1] = ']';
  return str;
}

int ParentTree::getHeightHelper(ParentNode *node) const {
  if (node == NULL)
    return 0;
  int lh = this->getHeightHelper(node->pLeft);
  int rh = this->getHeightHelper(node->pRight);
  return (lh > rh ? lh : rh) + 1;
}

int ParentTree::getHeight() const { return getHeightHelper(this->pRoot); }

ParentTree::ParentNode *ParentTree::rightRotate(ParentNode *x) {
  ParentNode *y = x->pLeft;
  ParentNode *z = y->pRight;
  y->pRight = x;
  x->pLeft = z;
  return y;
}

ParentTree::ParentNode *ParentTree::leftRotate(ParentNode *x) {
  ParentNode *y = x->pRight;
  ParentNode *z = y->pLeft;
  y->pLeft = x;
  x->pRight = z;
  return y;
}

int ParentTree::getBalance(ParentNode *node) const {
  if (!node)
    return 0;
  return getHeightHelper(node->pLeft) - getHeightHelper(node->pRight);
}

ParentTree::ParentNode *ParentTree::insertHelper(ParentNode *node,
                                                 const long long &value) {
  if (!node) {
    return new ParentNode(value);
  }

  if (value < node->id)
    node->pLeft = insertHelper(node->pLeft, value);
  else if (value >= node->id)
    node->pRight = insertHelper(node->pRight, value);

  int balance = getBalance(node);
  if (balance > 1 && value < node->pLeft->id)
    return rightRotate(node);

  if (balance < -1 && value >= node->pRight->id)
    return leftRotate(node);

  if (balance > 1 && value > node->pLeft->id) {
    node->pLeft = leftRotate(node->pLeft);
    return rightRotate(node);
  }

  if (balance < -1 && value <= node->pRight->id) {
    node->pRight = rightRotate(node->pRight);
    return leftRotate(node);
  }

  return node;
}

void ParentTree::insert(const long long &value) {
  if (ParentTree::globalId >= 10 * 10 * 10 * 10 * 10 * 10 * 10)
    throw overflow_error("Id is overflow!");
  this->pRoot = insertHelper(this->pRoot, value);
  this->nE++;
}

ParentTree::ParentNode *ParentTree::maxValueNode(ParentNode *node) {
  if (!node)
    return nullptr;
  ParentNode *current = node;
  while (current->pRight) {
    current = current->pRight;
  }
  return current;
}

ParentTree::ParentNode *ParentTree::removeHelper(ParentNode *node,
                                                 const long long &value) {
  // STEP 1: PERFORM STANDARD BST DELETE
  if (node == nullptr)
    return node;

  if (value < node->id)
    node->pLeft = removeHelper(node->pLeft, value);
  else if (value > node->id)
    node->pRight = removeHelper(node->pRight, value);
  else {
    // node with only one child or no child
    if ((node->pLeft == nullptr) || (node->pRight == nullptr)) {
      ParentNode *temp = node->pLeft ? node->pLeft : node->pRight;

      // No child case
      if (temp == nullptr) {
        temp = node;
        node = nullptr;
      } else           // One child case
        *node = *temp; // Copy the contents of
                       // the non-empty child
      delete temp;
    } else {
      // node with two children: Get the inorder
      // successor (smallest in the right subtree)
      ParentNode *temp = maxValueNode(node->pLeft);

      // Copy the inorder successor's
      // id to this node
      node->id = temp->id;

      // Delete the inorder successor
      node->pLeft = removeHelper(node->pLeft, temp->id);
    }
  }

  // If the tree had only one node
  // then return
  if (node == nullptr)
    return node;

  int balance = getBalance(node);

  // If this node becomes unbalanced,
  // then there are 4 cases

  // Left Left Case
  if (balance > 1 && getBalance(node->pLeft) > 0)
    return rightRotate(node);

  // Left Right Case
  if (balance > 1 && getBalance(node->pLeft) < 0) {
    node->pLeft = leftRotate(node->pLeft);
    return rightRotate(node);
  }

  // Right Right Case
  if (balance < -1 && getBalance(node->pRight) <= 0)
    return leftRotate(node);

  // Right Left Case
  if (balance < -1 && getBalance(node->pRight) >= 0) {
    node->pRight = rightRotate(node->pRight);
    return leftRotate(node);
  }

  return node;
}

void ParentTree::remove(const long long &value) {
  this->pRoot = removeHelper(this->pRoot, value);
  this->nE--;
}

////////////////////////////////////////// Vector
////////////////////////////////////////////

template <class T> Vector<T>::Vector() {
  this->nE = 0;
  this->cap = 50;
  this->arr = new T[50];
}

template <class T> Vector<T>::Vector(int cap) {
  this->nE = 0;
  this->cap = cap;
  this->arr = new T[cap];
}

template <class T> Vector<T> &Vector<T>::operator=(Vector<T> &newVec) {
  if (this != &newVec) {
    delete[] this->arr;
    this->arr = new T[newVec.getCap()];
    for (int i = 0; i < newVec.getSize(); i++) {
      this->arr[i] = newVec[i];
    }
    this->cap = newVec.getCap();
    this->nE = newVec.getSize();
  }

  return *this;
}

template <class T> T &Vector<T>::operator[](int index) {
  return this->arr[index];
}

template <class T> Vector<T>::~Vector() {
  delete[] this->arr;
  this->arr = nullptr;
  this->nE = 0;
  this->cap = 0;
}

template <class T> void Vector<T>::insert(T newElement, int idx) {
  // if (this->nE + 1 >= this->cap) {
  //   resize(++this->nE, this->cap * 2);
  // }
  this->nE++;
  this->arr[idx] = newElement;
}

template <class T> void Vector<T>::resize(int nE, int cap) {
  T *old = this->arr;
  this->arr = new T[cap];
  this->nE = nE;
  this->cap = cap;
  for (int i = 0; i < nE; i++) {
    this->arr[i] = old[i];
  }
  delete[] old;
}

////////////////////////////////////////// LitStringHash
////////////////////////////////////////////
long long calculatePower(int base, int powerRaised) {
  if (powerRaised != 0)
    return (base * calculatePower(base, powerRaised - 1));
  else
    return 1;
}

LitStringHash::LitStringHash(const HashConfig &hashConfig)
    : hashConfig(hashConfig),
      litStrings(Vector<LitString>(hashConfig.initSize)),
      lastInsertedIndex(-1) {}

long long LitStringHash::hashFunc(const string &s, const int &n, const int &m,
                                  const int &p) const {
  long long sum = 0;
  for (int i = 0; i < n; i++) {
    sum += (long long)s[i] * calculatePower(p, i);
  }

  return sum % m;
}

long long LitStringHash::hashProbing(const long long &hashF,
                                     const HashConfig &hashConfig,
                                     int idx) const {
  return (long long)(hashF + hashConfig.c1 * idx + hashConfig.c2 * idx * idx) %
         hashConfig.initSize;
}

int LitStringHash::quadratic(const string &s) {
  long long hashF =
      hashFunc(s, s.length(), this->hashConfig.initSize, this->hashConfig.p);
  bool found = false;
  int i = 0;
  long long hashP = INT32_MIN;
  for (; i < this->hashConfig.initSize && !found; i++) {
    hashP = hashProbing(hashF, this->hashConfig, i);
    if (this->litStrings[hashP].str == s) {
      this->litStrings[hashP].numOfLinks++;
      found = true;
      break;
    } else if (this->litStrings[hashP].numOfLinks == 0) {
      this->litStrings.insert(LitString(s, 1), hashP);
      found = true;
      break;
    }
  }
  if (!found) {
    throw runtime_error("No possible slot");
  }

  double loadFactor = ((double)this->litStrings.nE / this->hashConfig.initSize);
  if (loadFactor > this->hashConfig.lambda) {
    int newSize = (int)(this->hashConfig.alpha * this->hashConfig.initSize);
    this->litStrings.resize(this->litStrings.nE, newSize);
  }

  this->lastInsertedIndex = i;
  return i;
}

int LitStringHash::getLastInsertedIndex() const {
  return this->lastInsertedIndex;
}

string LitStringHash::toString() const {
  string result = "LitStringHash[";
  for (int i = 0; i < this->litStrings.nE; i++) {
    result += (this->litStrings.arr[i].numOfLinks > 0)
                  ? "(litS=\"" + this->litStrings.arr[i].str + "\");"
                  : "();";
  }
  result[result.length() - 1] = ']';
  return result;
}

void LitStringHash::remove(const string &s) {
  for (int i = 0; i < this->litStrings.nE; i++) {
    if (this->litStrings[i].str == s) {
      this->litStrings[i].str = "";
      this->litStrings[i].numOfLinks = 0;
      this->litStrings.nE--;
      break;
    }
  }
  this->lastInsertedIndex = -1;
}

////////////////////////////////////////// ReducedConcatStringTree
////////////////////////////////////////////

ReducedConcatStringTree::ReducedConcatStringTree(const char *s,
                                                 LitStringHash *litStringHash)
    : ConcatStringTree(s) {

  this->litStringHash = litStringHash;
  this->litStringHash->quadratic(this->pRoot->data);
}

ReducedConcatStringTree::~ReducedConcatStringTree() {}
