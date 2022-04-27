#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

template <class T>
class dector {
private:
    int Size;
    int capacity;
    T* arr;
    void ensureCap();
public:
    dector();
    ~dector();
    int size();
    T & operator [] (int index);
    dector& operator = (dector & rhs);
    void push_back(T newElement);
    void resize(int n);
};

struct symbol {
    string name;
    string type;
    int level;
    bool Static;

    symbol() : name(""), type(""), level(0), Static(false) {}
    symbol(string name, int level) : name(name), type(""), level(level), Static(false) {}
    bool operator == (symbol x); 
    bool operator != (symbol x); 
    bool operator > (symbol x); 
    bool operator < (symbol x);
    bool operator >= (symbol x);
    bool operator <= (symbol x);
};

class Node {
    symbol data;
    Node* parent;
    Node* left;
    Node* right;
    Node () : data(), parent(nullptr), left(nullptr), right(nullptr) {}
    Node (symbol data, Node* parent) : data(data), parent(parent), left(nullptr), right(nullptr) {}
    friend class SymbolTable;
};

class SymbolTable {
private:
    Node* root;
    int currentLevel;
    void deleteTree(Node* root);
    // for store the order of node inserted
    // dector[i][j] to store Node at level i
    dector<dector<symbol>> storage;

    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    Node* splay(Node* node);

    string tokenize(string s, char c, int& start);

    Node* insertNode(Node*& root, Node* parent, symbol x, int& num_comp, int& num_splay); // add Node and count number of comparision and splay, return node inserted
    Node* insert(symbol x, int& num_comp, int& num_splay); // insert Node and splay the node inserted

    
    Node* findNode(Node* root, symbol target, int& num_comp, int& num_splay); // find Node and count number of comparision and splay, without splay, return node found
    // can use this function to find node value
    Node* findNodeWithName(const string name, int& num_comp, int& num_splay); // find node to assign, return null if can not find, only count number of comparision and splay without splaying
    Node* find(symbol target, int& num_comp, int& num_splay); // find Node and splay the node found, return node found

    void remove(symbol target);

    bool checkIDname(const string name); // check if identifier_name is valid
    bool checkType(const string type); // check if type is valid
    bool isNumber(const string value); // check if value is number
    bool isString(const string value); // check if value is string
    bool isFunction(const string value); // check if value is function, this function doesn't check name of function call is valid or not
    bool checkMatchAtrributes(const string type, const string value, int& typeOfError, int& num_comp, int& num_splay);
    // compare atribute type of function and value (only for function type)
    // checkMatchAtrributes function also change the type of error include:
    // also count the number of coparision and splay if the atribute is a name. And splay the node with that name too.
    // 1. TypeMismatch
    // 2. Undeclared
    // 3. InvalidInstruction

    string takeReturnType(const string type); // (number,string)->string, return string

    string toString(Node* node);
    Node* findLastNodeToPrint(Node* root);
    void print(Node* root, Node* lastNode);

public:
    // size of storage is at least 1
    SymbolTable() { this->root = nullptr; currentLevel = 0; this->storage.resize(1); }
    ~SymbolTable() { deleteTree(this->root); }
    void run(string filename);
};
#endif
