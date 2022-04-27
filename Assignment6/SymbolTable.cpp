#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    ifstream file(filename);
    std::string currentline;
    int levelOfBlock = 0;
    HashTable hashTable;
    std::string func = "";
    int countInsert = 0;
    while(getline(file, currentline)){
        int start = 0;
        std::string command = tokenize(currentline, ' ', start);

        if(command == "LINEAR" || command == "DOUBLE"){
            std::string size = tokenize(currentline, ' ', start);
            if(!check_Parameter(size, currentline)) throw InvalidInstruction(currentline);
            int m = convertToInt(size);

            std::string const_Probing = tokenize(currentline, ' ', start);
            if(!check_Parameter(const_Probing, currentline)) throw InvalidInstruction(currentline);
            int c = convertToInt(const_Probing);

            hashTable.table.setSize(m);
            hashTable.c1 = c;

            Vector<Symbol> v(m, m+10);
            hashTable.table = v;

            if(command == "LINEAR") func = "LINEAR";
            if(command == "DOUBLE") func = "DOUBLE";
        }
        else if(command == "QUADRATIC"){
            std::string size = tokenize(currentline, ' ', start);
            if(!check_Parameter(size, currentline)) throw InvalidInstruction(currentline);
            int m = convertToInt(size);

            std::string const_Probing1 = tokenize(currentline, ' ', start);
            if(!check_Parameter(const_Probing1, currentline)) throw InvalidInstruction(currentline);
            int c1 = convertToInt(const_Probing1);

            std::string const_Probing2 = tokenize(currentline, ' ', start);
            if(!check_Parameter(const_Probing2, currentline)) throw InvalidInstruction(currentline);
            int c2 = convertToInt(const_Probing2);

            hashTable.table.setSize(m);
            hashTable.c1 = c1;
            hashTable.c2 = c2;

            Vector<Symbol> v(m, m+10);
            hashTable.table = v;
            func = "QUADRATIC";
        }
        else if(command == "INSERT"){
            std::string idName = tokenize(currentline, ' ', start);

            if(!checkID(idName)) throw InvalidInstruction(currentline);

            std::string numOfParameter = tokenize(currentline, ' ', start);
            if(numOfParameter != ""){
                if(levelOfBlock != 0) throw InvalidDeclaration(idName);
                int index = 0;
                int num = convertToInt(numOfParameter);
                if(func == "LINEAR") std::cout << hashTable.Linear(idName, levelOfBlock, num, NO, command, currentline, index) << std::endl;
                else if(func == "QUADRATIC") std::cout << hashTable.Quadratic(idName, levelOfBlock, num, NO, command, currentline, index) << std::endl;
                else if(func == "DOUBLE") std::cout << hashTable.Double(idName, levelOfBlock, num, NO, command, currentline, index) << std::endl;
                
            }
            else{
                int indeXX = 0;
                if(func == "LINEAR") std::cout << hashTable.Linear(idName, levelOfBlock, -1, NO, command, currentline, indeXX) << std::endl;
                else if(func == "QUADRATIC") std::cout << hashTable.Quadratic(idName, levelOfBlock, -1, NO, command, currentline, indeXX) << std::endl;
                else if(func == "DOUBLE") std::cout << hashTable.Double(idName, levelOfBlock, -1, NO, command, currentline, indeXX) << std::endl;
            }
            countInsert++;
        }
        else if(command == "ASSIGN"){

            int totalNum = 0;

            std::string var1 = tokenize(currentline, ' ', start);
            if(var1 == "") throw InvalidInstruction(currentline);

            std::string var2 = tokenize(currentline, '$', start);
            if(var2 == "") throw InvalidInstruction(currentline);

            if(isNumber(var2)){
                if(!checkID(var1)) throw InvalidInstruction(currentline);
                int index = 0;

                if(func == "LINEAR") totalNum += hashTable.Linear(var1, levelOfBlock, -1, NO, command, currentline, index);
                else if(func == "QUADRATIC") totalNum += hashTable.Quadratic(var1, levelOfBlock, -1, NO, command, currentline, index);
                else totalNum += hashTable.Double(var1, levelOfBlock, -1, NO, command, currentline, index);
                hashTable.table[index].type = NUMBER;
            }
            else if(isString(var2)){
                int index = 0;
                if(!checkID(var1)) throw InvalidInstruction(currentline);

                if(func == "LINEAR") totalNum += hashTable.Linear(var1, levelOfBlock, -1, NO, command, currentline, index);
                else if(func == "QUADRATIC") totalNum += hashTable.Quadratic(var1, levelOfBlock, -1, NO, command, currentline, index);
                else totalNum += hashTable.Double(var1, levelOfBlock, -1, NO, command, currentline, index);                
                hashTable.table[index].type = STRING;
            }
            else{

                start = 0;
                std::string nameFunc = tokenize(var2, '(', start);

                // existed IdName.
                if(nameFunc == var2){
                    int index = 0;
                    if(!checkID(var2)) throw InvalidInstruction(currentline);
                    if(func == "LINEAR") totalNum += hashTable.Linear(var2, levelOfBlock, -1, NO, command, currentline, index);
                    else if(func == "QUADRATIC") totalNum += hashTable.Quadratic(var2, levelOfBlock, -1, NO, command, currentline, index);
                    else totalNum += hashTable.Double(var2, levelOfBlock, -1, NO, command, currentline, index);

                    if(hashTable.table[index].numOfPara != -1) throw TypeMismatch(currentline);

                    int index1 = 0;
                    if(!checkID(var1)) throw InvalidInstruction(currentline);
                    if(func == "LINEAR") totalNum += hashTable.Linear(var1, levelOfBlock, -1, NO, command, currentline, index1);
                    else if(func == "QUADRATIC") totalNum += hashTable.Quadratic(var1, levelOfBlock, -1, NO, command, currentline, index1);
                    else totalNum += hashTable.Double(var1, levelOfBlock, -1, NO, command, currentline, index1);

                    if(hashTable.table[index1].numOfPara != -1) throw TypeMismatch(currentline);

                    if(hashTable.table[index].type == NO){
                        if(hashTable.table[index1].type == NO) throw TypeCannotBeInferred(currentline);

                        hashTable.table[index].type = hashTable.table[index1].type;
                    } 
                    else{
                        if(hashTable.table[index1].type == NO){
                            hashTable.table[index1].type = hashTable.table[index].type;
                        }
                        else{
                            if(hashTable.table[index].type != hashTable.table[index1].type) throw TypeMismatch(currentline);
                        }
                    }
                }
                // existed Function name.
                else{
                    int index = 0;
                    totalNum += check_Func_Count(hashTable, var2, levelOfBlock, func, currentline, index, command);

                    int index1 = 0;
                    if(!checkID(var1)) throw InvalidInstruction(currentline);
                    if(func == "LINEAR") totalNum += hashTable.Linear(var1, levelOfBlock, -1, NO, command, currentline, index1);
                    else if(func == "QUADRATIC") totalNum += hashTable.Quadratic(var1, levelOfBlock, -1, NO, command, currentline, index1);
                    else totalNum += hashTable.Double(var1, levelOfBlock, -1, NO, command, currentline, index1);
                    
                    if(hashTable.table[index].type == VOID) throw TypeMismatch(currentline);
                    if(hashTable.table[index1].numOfPara != -1) throw TypeMismatch(currentline);

                    if(hashTable.table[index].type == NO){
                        if(hashTable.table[index1].type == NO) throw TypeCannotBeInferred(currentline);

                        hashTable.table[index].type = hashTable.table[index1].type;
                    } 
                    else{
                        if(hashTable.table[index1].type == NO){
                            hashTable.table[index1].type = hashTable.table[index].type;
                        }
                        else{
                            if(hashTable.table[index].type != hashTable.table[index1].type) throw TypeMismatch(currentline);
                        }
                    }
                }
            } 
            std::cout << totalNum << std::endl;
        }
        else if(command == "CALL"){
            std::string callFunc = tokenize(currentline, '$', start);
            int totalNum = 0;
            int index = 0;
            totalNum += check_Func_Count(hashTable, callFunc, levelOfBlock, func, currentline, index, command);
            if(hashTable.table[index].type == NO){
                hashTable.table[index].type = VOID;
            }
            else if(hashTable.table[index].type != VOID) throw TypeMismatch(currentline);
            if(hashTable.table[index].numOfPara == -1) throw TypeMismatch(currentline);
            std::cout << totalNum << std::endl;
        }
        else if(command == "BEGIN"){
            levelOfBlock ++;
        }
        else if(command == "END"){
            if(levelOfBlock == 0) throw UnknownBlock();
            hashTable.Delete(levelOfBlock, countInsert);
            levelOfBlock --;
        }
        else if(command == "LOOKUP"){
            std::string idName = tokenize(currentline, '$', start);
            std::cout << hashTable.table.find_Lookup(idName, levelOfBlock) << std::endl;
        }
        else if(command == "PRINT"){
            Symbol x;
            bool found = false;
            int dem = 0;
            if(countInsert != 0){
                for(int i = 0; i < hashTable.table.getSize(); i++){
                    if(hashTable.table[i] != x){
                        std::cout << i << " " << hashTable.table[i].Id << "//" << hashTable.table[i].LoB;
                        if(dem < countInsert - 1) std::cout << ";";
                        dem++;
                        found = true;
                    } 
                }
                if(found) std::cout << endl;    
            }
        }
        else throw InvalidInstruction(currentline);
    }

    if(levelOfBlock != 0) throw UnclosedBlock(levelOfBlock);
}

// function for SymbolTable class

bool SymbolTable::checkID(const std::string& ID){
    regex e("[[:lower:]][[:w:]]*");
    return regex_match(ID, e);
}

int SymbolTable::convertToInt(const std::string& str){
    int ans = 0;
    int size = str.size();
    for(int i = 0; i < size; i++){
        int x = int(str[i] - 48);
        ans += x * power(10, size-1-i);
    }
    return ans;
}

long long SymbolTable::Decode(const std::string& str, int LOB){

    long long ans = 0;

    std::string s = "";
    s += to_string(LOB);

    for(int i = 0; i < str.size(); i++){
        int x = int(str[i]) - 48;
        s += std::to_string(x);
    }

    for(int i = 0; i < s.size(); i ++){
        ans = ans*10 + (int)s[i]-48;
    }

    return ans;
}

long long SymbolTable::power(const int& x, const int& y){

    long long ans = 1;
    for(int i = 0; i < y; i++){
        ans *= x; 
    }
    return ans;
}

bool SymbolTable::check_Parameter(const std::string& para, const std::string& currentline){
    if(para.size() > 6 || para == "") throw InvalidInstruction(currentline);

    for(int i = 0; i < (int)para.size(); i++){
        if(para[i] < '0' || para[i] > '9') return false;
    }

    return true;
}

string SymbolTable::tokenize(const string& str, char c, int& start){
    std::string ans = "";
    for(int i = start; i < (int)str.size(); i++){
        if(str[i] == c){
            start = i + 1;
            return ans;
        }
        ans += str[i];
    }

    start = (int)str.size();
    return ans;
}

bool SymbolTable::isNumber(const std::string& para){
    regex e("[[:digit:]]+");
    return regex_match(para,e);
}

bool SymbolTable::isString(const std::string& para){
    regex e("'[[:alnum:],' ']*'");
    return regex_match(para, e);
}

int SymbolTable::check_Func_Count(HashTable& hashTable, const std::string& func, int LoB, const std::string& command, const std::string& currentline, int& index, const std::string& name){
    int totalNum = 0;
    int start = 0;
    int count = 0;
    int indexXX = 0;
    std::string nameFunc = tokenize(func, '(', start);


    if(func[func.size() - 1] != ')') throw InvalidInstruction(currentline);
    if(command == "LINEAR") totalNum += hashTable.Linear(nameFunc, LoB, -1, NO, name, currentline, index);
    else if(command == "QUADRATIC") totalNum += hashTable.Quadratic(nameFunc, LoB, -1, NO, name, currentline, index);
    else totalNum += hashTable.Double(nameFunc, LoB, -1, NO, name, currentline, index);

    if(func[func.size() - 1] == ')' && func[start] == ')'){
        if(hashTable.table[index].numOfPara == -1) throw TypeMismatch(currentline);
        if(count == hashTable.table[index].numOfPara) return totalNum;
        else throw InvalidInstruction(func);
    } 

    while(start != func.size()){
        std::string para = tokenize(func, ',', start);

        if(start == func.size()){
            para = para.substr(0, para.size()-1);
        }
        if(name == "ASSIGN" || name == "CALL"){
            if(hashTable.table[index].typeFunc[count] == NO){
                 if(isNumber(para)) {
                     (hashTable.table[index].typeFunc)[count] = NUMBER;
                 }
                 else if(isString(para)){
                     hashTable.table[index].typeFunc[count] = STRING;
                 }   
                 else{
                    if(command == "LINEAR") totalNum += hashTable.Linear(para, LoB, -1, NO, name, currentline, indexXX);
                    else if(command == "QUADRATIC") {
                        totalNum += hashTable.Quadratic(para, LoB, -1, NO, name, currentline, indexXX);   
                    }
                    else totalNum += hashTable.Double(para, LoB, -1, NO, name, currentline, indexXX);

                    if(hashTable.table[indexXX].numOfPara != -1) throw TypeMismatch(currentline);
                    if(hashTable.table[indexXX].type == NO) throw TypeCannotBeInferred(currentline);

                    if(hashTable.table[indexXX].type == STRING) {
                        hashTable.table[index].typeFunc[count] = STRING;
                    }
                    if(hashTable.table[indexXX].type == NUMBER) {
                        hashTable.table[index].typeFunc[count] = NUMBER;
                    }
                 }
             }
            else{
                if(isNumber(para) || isString(para)){

                    if(hashTable.table[index].typeFunc[count] != NUMBER && isNumber(para)) throw TypeMismatch(currentline);
                    if(hashTable.table[index].typeFunc[count] != STRING && isString(para)) throw TypeMismatch(currentline);
                }
                else{
                    if(command == "LINEAR") totalNum += hashTable.Linear(para, LoB, -1, NO, name, currentline, indexXX);
                    else if(command == "QUADRATIC") totalNum += hashTable.Quadratic(para, LoB, -1, NO, name, currentline, indexXX);
                    else totalNum += hashTable.Double(para, LoB, -1, NO, name, currentline, indexXX);

                    if(hashTable.table[indexXX].numOfPara != -1) throw TypeMismatch(currentline);
                    if(hashTable.table[indexXX].type == NO){

                        if(hashTable.table[index].typeFunc[count] == STRING) {
                            hashTable.table[indexXX].type = STRING;
                        }
                        else if(hashTable.table[index].typeFunc[count] == NUMBER) {
                            hashTable.table[indexXX].type = NUMBER;
                        }
                    }
                    else{
                        if(name == "CALL"){
                            if(hashTable.table[indexXX].numOfPara == -1) throw TypeMismatch(currentline);
                        }

                        if(hashTable.table[indexXX].numOfPara != -1) throw TypeMismatch(currentline);
                        if(hashTable.table[indexXX].type == NO) throw TypeCannotBeInferred(currentline);    
                    }
                }  
             } 
        }
        count ++;
    }
    if(count != hashTable.table[index].numOfPara) throw InvalidInstruction(func);

    return totalNum;
}

// HashTable class function
HashTable::~HashTable(){
    for(int i = 0; i < table.getSize(); i++){
        if(table[i].numOfPara != -1) {
            delete[] table[i].typeFunc;
        }
    }
}

int HashTable::HashFunc(long long key, int size){
    return key % size;
}

int HashTable::Linear(const std::string& key, int LoB, int num, dataType type, const std::string& func, const std::string& str, int& index){
    SymbolTable s;
    Symbol x;
    int Level = LoB;

    for(int j = LoB; j >= 0; j--){
        Symbol y(key, j, num, type);
        long long newkey = s.Decode(key, j);
        long long HFunc = this->HashFunc(newkey, this->table.getSize());

        for(int i = 0; i < table.getSize(); i++){
            int m = table.getSize();
            int HProbing = (HFunc + this->c1*i) % table.getSize();
            if(func == "INSERT"){
                if(j == Level){
                    if(this->table[HProbing] == y) {
                        throw Redeclared(key);
                    }
                    if(this->table[HProbing] == x){
                        this->table[HProbing] = y;
                        index = HProbing;
                        return i;
                    }    
                }
                else {
                    throw Overflow(str);
                }
            }
            else if(func == "ASSIGN" || func == "CALL"){
                if(this->table[HProbing] == y){
                    index = HProbing;
                    return i;
                }
            }
        }  
    }
    if(func == "ASSIGN" || func == "CALL") throw Undeclared(key);
    return INT32_MIN;
}

int HashTable::Quadratic(const std::string& key, int LoB, int num, dataType type, const std::string& func, const std::string& str, int& index){
    SymbolTable s;
    Symbol x;
    int Level = LoB;

    for(int j = LoB; j >= 0; j--){
        int count = 0;
        Symbol y(key, j, num, type);
        long long newkey = s.Decode(key, j);
        int HFunc = this->HashFunc(newkey, this->table.getSize());

        for(int i = 0; i < table.getSize(); i++){
            int HProbing = (HFunc + this->c1 * i + this->c2 * i* i) % table.getSize();
            if(func == "INSERT"){
                if(j == Level){
                    if(this->table[HProbing] == y) {
                       throw Redeclared(key);
                    }
                    if(this->table[HProbing] == x){
                        this->table[HProbing] = y;
                        index = HProbing;
                        return i;
                    }    
                }
                else {
                    throw Overflow(str);
                }
                
            }
            else if(func == "ASSIGN" || func == "CALL"){
                if(this->table[HProbing] == y){
                    index = HProbing;
                    return i;
                }
            }
        }  
    }
    
    if(func == "ASSIGN" || func == "CALL") throw Undeclared(key);
    return INT32_MIN;
}

int HashTable::Double(const std::string& key, int LoB, int num, dataType type, const std::string& func, const std::string& str, int& index){
    SymbolTable s;
    Symbol x;
    int Level = LoB;
    
    for(int j = LoB; j >= 0; j--){
        Symbol y(key, j, num, type);
        long long newkey = s.Decode(key, j);

        int HFunc1 = HashFunc(newkey, this->table.getSize());
        int HFunc2 = 1 + HashFunc(newkey, this->table.getSize() - 2);
        for(int i = 0; i < table.getSize(); i++){
            int HProbing = (HFunc1 + this->c1 * i * HFunc2) % this->table.getSize();
            if(func == "INSERT"){
                if(j == Level){
                    if(this->table[HProbing] == y) {
                        throw Redeclared(key);
                    }
                    if(this->table[HProbing] == x){
                        this->table[HProbing] = y;
                        index = HProbing;
                        return i;
                    }    
                }
                else {
                    throw Overflow(str);
                }
                
            }
            else if(func == "ASSIGN" || func == "CALL"){
                if(this->table[HProbing] == y){
                    index = HProbing;
                    return i;
                }
            }
        }  
    }
    
    if(func == "ASSIGN" || func == "CALL") throw Undeclared(key);
    return INT32_MIN;
}

void HashTable::Delete(int LoB, int& countInsert){
    while(true){
        int index = this->table.find_Delete(LoB);

        if(index == INT32_MIN) break;;

        Symbol x;
        this->table[index] = x;
        countInsert --;
    }
}
template<class T>
void Vector<T>::setSize(int m){
    this->size = m;
}

template<class T>
Vector<T>::Vector(){
   this->size = 0; 
   this->capacity = 50;
   this->arr = new T[50];
}

template<class T>
Vector<T>::Vector(int size, int cap){
    this->size = size;
    this->capacity = cap;
    this->arr = new T[cap];
}

template<class T>
int Vector<T>::getSize(){
    return this->size;
}

template<class T>
int Vector<T>::getCap(){
    return this->capacity;
}

template<class T>
Vector<T>& Vector<T>::operator = (Vector<T>& newVec){
    if(this != &newVec){
        delete[] this->arr;
        this->arr = new T[newVec.capacity];
        for(int i = 0; i < newVec.size; i++){
            this->arr[i] = newVec[i];
        }
        this->capacity = newVec.capacity;
        this->size = newVec.getSize();
    }

    return *this;
}

template<class T>
T& Vector<T>::operator [] (int index){
    return this->arr[index];
}

template<class T>
Vector<T>::~Vector(){
    delete[] this->arr;
    this->arr = nullptr;
    this->size = 0;
    this->capacity = 0;
}

template<class T>
int Vector<T>::find_Delete(int LOB){
    for(int i = 0; i < this->getSize(); i++){
        if(this->arr[i].LoB == LOB) return i;
    }
    return INT32_MIN;
}

template<class T>
int Vector<T>::find_Lookup(const std::string& Id, int LoB){
    for(int j = LoB; j >= 0; j--){
        for(int i = 0; i < this->getSize(); i++){
            if(this->arr[i].LoB == j && this->arr[i].Id == Id) return i;
        }
    }
    throw Undeclared(Id);
    return INT32_MIN;
}
// Function for Symbol struct

bool Symbol::operator == (Symbol x){
    if(this->LoB == x.LoB && this->Id.compare(x.Id) == 0) return true;
    return false;
}

bool Symbol::operator != (Symbol x){
    if(this->LoB != x.LoB) return true;
    return (this->Id.compare(x.Id) != 0);
}

Symbol::Symbol(std::string Id, int LoB, int num, dataType dT): LoB(LoB), Id(Id), numOfPara(num), type(dT){
    if(num != -1) {
        typeFunc = new dataType[num];
        for(int i = 0; i < num; i++){
            this->typeFunc[i] = NO;
        }
    }
}
