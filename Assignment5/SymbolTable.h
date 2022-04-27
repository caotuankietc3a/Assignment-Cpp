#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SplayNode{
public:
    std::string ID;
    int LevelofBlock;
    std::string type;
    int index = 0;
    SplayNode* left;
    SplayNode* right;
    SplayNode* parent;

    SplayNode(): ID(""), LevelofBlock(0), type(""), left(nullptr), right(nullptr), parent(nullptr){};
    SplayNode(std::string id, const int LoB, std::string t): ID(id), LevelofBlock(LoB), type(t), left(nullptr), right(nullptr), parent(nullptr){};
};

class SplayTree{
public:
    SplayNode* root;
    SplayNode* rootN[1000];
    SplayTree(){
        root = nullptr;
        for(int i = 0; i < 1000; i++) rootN[i] =nullptr;
    } 

    ~SplayTree(){
        DeleteSub(root);
        for(int i = 0; i < 1000; i++) DeleteSub(rootN[i]);
    }
    
    friend int checkKey(const SplayNode*, const SplayNode*); 
    
    void R_Rotate(SplayNode*&, SplayNode*);

    void L_Rotate(SplayNode*&, SplayNode*);

    void Splay(SplayNode*&, SplayNode*);

    void Insert(SplayNode*&, std::string, int, std::string, std::string, int&, int&, SplayTree&);

    void DeleteNode(SplayNode*&, std::string, int, std::string);

    void findNode(SplayNode*&, const std::string&, int, std::string, int&, int&, int&);

    void Print_Pre(SplayNode*, int&);

    void DeleteSub(SplayNode*&);

    void DeleteTree(SplayTree&);
};

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);

    bool checkFunction(std::string);

    bool checkNumber(std::string);

    bool checkString(std::string);

    bool checkType_Func(SplayTree&, std::string, std::string, int, int&, int&, std::string);

    friend int checkKey(const SplayNode*, const SplayNode*);

    friend bool CheckEqual(SplayNode*, SplayNode*);

};
#endif
