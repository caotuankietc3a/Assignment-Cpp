#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

const int MAXSCOPE = 50;

class Node {
    string IDname;
    string Type;
    string value;
    int scopeLevel;
    Node* next;
public:
    Node() {
        next = nullptr;
    }

    Node (string IDname, string type, int scopeLevel) : IDname(IDname), Type(type), value(""), scopeLevel(scopeLevel), next(nullptr) {}

    friend class SymbolTable;
};

class SymbolTable {
    Node* head[MAXSCOPE];

    bool insert(string name, string type, const int& scopeLevel); // Redeclared
    bool assign(string name, string val, string& typeOfVal, int& typeOfError, const int& scopeLevel);  // Undeclared, TypeMismatch
    bool lookUp(string name, int& posFounded);  // Undeclared
    bool checkIDname(const string IDname);
    bool checkValue(const string value, string& typeOfVal, const int& scopeLevel);
    bool checkExist(string name, int scopeLevel);
public:
    SymbolTable();
    ~SymbolTable(); 
    void run(string filename);
};
#endif