#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

enum dataType{
    NUMBER,
    STRING,
    NO,
    VOID
};

template<class T>
class Vector{
    private:
        int size;
        int capacity;
    public:
        T* arr;
        Vector();
        Vector(int, int);
        ~Vector();
        void setSize(int);
        int getSize();
        int getCap();
        T& operator [] (int);
        Vector& operator = (Vector&);
        int find_Delete(int);
        int find_Lookup(const std::string&, int);
};

struct Symbol{
    int LoB;
    std::string Id;
    int numOfPara;
    dataType type;
    dataType* typeFunc = nullptr;

    /* TypeFunction typeFunc; */
    Symbol(): LoB(0), Id(""), numOfPara(-1), type(NO){}
    Symbol(std::string, int, int, dataType);
      
    bool operator == (Symbol);
    bool operator != (Symbol);
};

class HashTable{
    public:
        int c1, c2;
        Vector<Symbol> table;
        
        HashTable(): c1(0), c2(0){}
        // construct for Linear probing and Double probing
        HashTable(const int& c1): c1(c1), c2(0){}

        ~HashTable();

        // construct for Quadratic probing
        HashTable(const int& c1, const int& c2): c1(c1), c2(c2){}

        // get hash function value
        int HashFunc(long long, int);

        int Linear(const std::string&, int, int, dataType, const std::string&, const std::string&, int&);
        int Quadratic(const std::string&, int, int, dataType, const std::string&, const std::string&, int&);
        int Double(const std::string&, int, int, dataType, const std::string&, const std::string&, int&);

        void Delete(int, int&);
        
};
class SymbolTable
{
    public:
        SymbolTable() {}
        void run(string filename);

        bool checkID(const std::string& ID);

        int convertToInt(const std::string&);

        long long Decode(const std::string&, int);
        long long power(const int&, const int&);
        bool check_Parameter(const std::string&, const std::string&);

        std::string tokenize(const string& , char, int&);

        bool isNumber(const std::string&);
        bool isString(const std::string&);

        int check_Func_Count(HashTable&, const std::string&, int, const std::string&, const std::string&, int&, const std::string&);
};
#endif
