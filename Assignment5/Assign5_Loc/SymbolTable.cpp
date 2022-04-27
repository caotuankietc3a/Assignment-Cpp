#include "SymbolTable.h"

void SymbolTable::run(string filename) {
    ifstream file(filename);
    string currentLine;
    this->storage.resize(1);

    while (getline(file, currentLine)) {
        int start = 0;
        string command = tokenize(currentLine, ' ', start);
        if (command == "INSERT") {
/* INSERT <identifier_name> <type> <static> */

            // take name and check if is valid
            string name = tokenize(currentLine, ' ', start); 
            // if name is not valid throw error InvalidInstruction
            if (!checkIDname(name)) {
                throw InvalidInstruction(currentLine);
                return;
            }
            string type = tokenize(currentLine, ' ', start);
            // if type is not valid, throw InvalidInstruction
            if (!checkType(type)) {
                throw InvalidInstruction(currentLine);
                return;
            }
            string Static = tokenize(currentLine, ' ', start);
            // if Static is not "true" or "false", throw InvalidInstruction
            if (Static != "true" && Static != "false") {
                throw InvalidInstruction(currentLine);
                return;
            }
            
            // After passing all checking above, Instruction is valid
            // Check if name is declared before
            int num_comp = 0; // use for nothing
            int num_splay = 0; // use for nothing
            symbol target(name, this->currentLevel);
            target.level = (Static == "true" ? 0 : this->currentLevel);
            // find target without splay node
            Node* nodeFound = findNode(this->root, target, num_comp, num_splay);

            // if type is a function (mean not a string and number) but the currentLevel != 0 throw InvalidDeclaration
            if (target.level != 0 && type != "string" && type != "number") {
                throw InvalidDeclaration(currentLine);
                return;
            }
            // if target is found in table, throw error Redeclared 
            if (nodeFound != nullptr && nodeFound->data == target) {
                throw Redeclared(currentLine);
                return;
            }


            num_comp = 0;
            num_splay = 0;
            target.Static = (Static == "true" ? true : false);
            target.type = type;
            // splay here
            Node* nodeInserted = insert(target, num_comp, num_splay);
            // insert successfully
            // insert it into the storage at index currentLevel too(for the order thing to delete)
            storage[nodeInserted->data.level].push_back(nodeInserted->data);
            std::cout << num_comp << " " << num_splay << '\n';
        }
        else if (command == "ASSIGN") {
/* ASSIGN <identifier_name> <value> */
            // check value before checking name
            string name = tokenize(currentLine, ' ', start);
            string value = currentLine.substr(start);
            int num_comp = 0;
            int num_splay = 0;
    // case value is number
            if (isNumber(value)) {
                // value's type is number (check value before name)
                // check if name is valid, if not, throw InvalidInstruction
                if (!checkIDname(name)) {
                    throw InvalidInstruction(currentLine);
                    return;
                } 
                Node* nodeToAssign = findNodeWithName(name, num_comp, num_splay);
                // if can't found node to assign, throw Undeclared
                if (nodeToAssign == nullptr) {
                    throw Undeclared(currentLine);
                    return;
                }
                // when nodeToAssign is found, check if the node to assign has the same type with value
                if (nodeToAssign->data.type != "number") {
                    throw TypeMismatch(currentLine);
                    return;
                }
                std::cout << num_comp << " " << num_splay << '\n';
            }
    // case value is string
            else if (isString(value)) {
                // value's type is number (check value before name)
                // check if name is valid, if not, throw InvalidInstruction
                if (!checkIDname(name)) {
                    throw InvalidInstruction(currentLine);
                    return;
                } 
                Node* nodeToAssign = findNodeWithName(name, num_comp, num_splay);
                if (nodeToAssign == nullptr) {
                    throw Undeclared(currentLine);
                    return;
                }
                // when nodeToAssign is found, check if the node to assign has the same type with value
                if (nodeToAssign->data.type != "string") {
                    throw TypeMismatch(currentLine);
                    return;
                }
                // assign successfully
                std::cout << num_comp << " " << num_splay << '\n';
            }
            else if (isFunction(value)) {
                int i = 0; // index for taking each part in value which is function call (name, lsAtrb[123,'abc',...])
                // take function name from value
                string functName = tokenize(value, '(', i);
                // check if function name is valid, if not throw InvalidInstruction
                if (!checkIDname(functName)) {
                    throw InvalidInstruction(currentLine);
                    return;
                }
                // if function name is valid, find that function, if not found, throw Undeclared
                symbol funct(functName, 0);
                // find function Node and splay that node
                Node* funcNode = find(funct, num_comp, num_splay);
                // if not found
                if (funcNode == nullptr) {
                    throw Undeclared(currentLine);
                    return;
                }
                // if found
                // compare type of function's atributes and type of value is matched or not
                // also check error may happen with atributes
                int typeOfError = -1;
                // 1. TypeMismatch
                // 2. Undeclared
                // 3. InvalidInstruction
                if (!checkMatchAtrributes(funcNode->data.type, value, typeOfError, num_comp, num_splay)) {
                    if (typeOfError == 1) {
                        throw TypeMismatch(currentLine);
                    }
                    else if (typeOfError == 2) {
                        throw Undeclared(currentLine);
                    }
                    else if (typeOfError == 3) {
                        throw InvalidInstruction(currentLine);
                    }
                    return;
                }

                // check if name is valid or not (check type before name)
                if (!checkIDname(name)) {
                    throw InvalidInstruction(currentLine);
                    return;
                }
                // if name is valid find the Node with that name
                Node* nodeToAssign = findNodeWithName(name, num_comp, num_splay);
                if (nodeToAssign == nullptr) {
                    throw Undeclared(currentLine);
                    return;
                }
                this->root = splay(nodeToAssign);

                // take the return type of function and compare with type of node to assign
                string returnType = takeReturnType(funcNode->data.type);
                if (nodeToAssign != nullptr && nodeToAssign->data.type != returnType) {
                    throw TypeMismatch(currentLine);
                    return;
                }
                // assign successfully
                cout << num_comp << " " << num_splay << '\n';
            }
            // if type of value is not one the three above, check if value is a name declared before
            else {
                // check if value is an identifier_name
                if (!checkIDname(value)) {
                    throw InvalidInstruction(currentLine);
                    return;
                }
                // value is a valid identifier_name, find value
                Node* nodeValueFound = findNodeWithName(value, num_comp, num_splay);
                // if can't find the name (which is value), throw Undeclared
                if (nodeValueFound == nullptr) {
                    throw Undeclared(currentLine);
                    return;
                }
                else {
                    // if found splay that node
                    this->root = splay(nodeValueFound); 
                }
                // find the node to assign
                Node* nodeToAssign = findNodeWithName(name, num_comp, num_splay);
                // if can't find the name to assign, throw Undeclared
                if (nodeToAssign == nullptr) {
                    throw Undeclared(currentLine);
                    return;
                }
                else {
                    this->root = splay(nodeToAssign);
                }
                // if value and name are found; check if they are the same type, if not throw TypeMismatch;
                // type of nodeValueFound is "string" or "number"
                string typeOfValue; // to take value of value(which is a identifier_name) :
                // 1. number/string
                // 2. type of return type of value if value is a function
                if (nodeValueFound->data.type == "number" || nodeValueFound->data.type == "string") {
                    typeOfValue = nodeValueFound->data.type;
                }
                // case value is function
                else {
                    typeOfValue = takeReturnType(nodeValueFound->data.type);
                }
                if (nodeToAssign->data.type != typeOfValue) {
                    throw TypeMismatch(currentLine);
                    return;
                }
                cout << num_comp << " " << num_splay << '\n';
            }
        }
        else if (command == "BEGIN") {
            this->currentLevel++;
            storage.resize(this->currentLevel+1);
        }
        else if (command == "END") {
            if (this->currentLevel - 1 < 0) {
                throw UnknownBlock();
                return;
            }
            // find top down every node with currentLevel to remove 
            for (int i = 0; i < storage[this->currentLevel].size(); i++) {
                remove(storage[currentLevel][i]);
            }
            this->currentLevel--;
            storage.resize(this->currentLevel+1);
        }
        else if (command == "LOOKUP") {
            int num_comp = 0;
            int num_splay= 0;
            string name = tokenize(currentLine, ' ', start);
            Node* nodeFound = findNodeWithName(name, num_comp, num_splay);  
            if (nodeFound == nullptr) {
                throw Undeclared(currentLine);
                return;
            }
            this->root = splay(nodeFound);
            cout << nodeFound->data.level << '\n';
        }
        else if (command == "PRINT") {
            Node* lastNode = findLastNodeToPrint(this->root);
            print(this->root, lastNode);
        }
        else {
            throw InvalidInstruction(currentLine);
            return;
        }
    }

    if (this->currentLevel > 0) {
        throw UnclosedBlock(this->currentLevel);
        return;
    }
}

/////////     SYMBOL          //////////////////////////
bool symbol::operator == (symbol x) {
    if (this->level != x.level) return false;
    return (this->name.compare(x.name) == 0);
}
bool symbol::operator != (symbol x) {
    if (this->level != x.level) return true;
    return (this->name.compare(x.name) != 0);
}
bool symbol::operator > (symbol x) {
    if (this->level > x.level) return true;
    else if (this->level < x.level) return false;
    return (this->name.compare(x.name) > 0);
}
bool symbol::operator < (symbol x) {
    if (this->level < x.level) return true;
    else if (this->level > x.level) return false;
    return (this->name.compare(x.name) < 0); 
}
bool symbol::operator >= (symbol x) {
    if (this->level > x.level) return true;
    else if (this->level < x.level) return true;
    return (this->name.compare(x.name) >= 0);
}
bool symbol::operator <= (symbol x) {
    if (this->level < x.level) return true;
    else if (this->level > x.level) return false;
    return (this->name.compare(x.name) <= 0);
}

///////////  DECTOR       /////////////////////////////
template<class T>
void dector<T>::ensureCap() {
    T* old = this->arr;
    this->arr = new T[this->capacity+10];
    for (int i = 0; i < this->Size; i++) {
        this->arr[i] = old[i];
    }
    delete[] old;
    this->capacity += 10;
}

template<class T>
dector<T>::dector() {
    this->Size = 0;
    this->capacity = 50;
    this->arr = new T[50];
}

template<class T>
dector<T>::~dector() {
    delete[] this->arr;
    this->Size = 0;
    this->capacity = 0;
}

template<class T>
int dector<T>::size() {
    return this->Size;
}

template<class T>
void dector<T>::resize(int n) {
    T* old = this->arr;
    this->arr= new T[2*n];
    this->Size = n;
    this->capacity = 2*n;
    for (int i = 0; i < n; i++) {
        this->arr[i] = old[i];
    }
    delete[] old;
}

template<class T>
T& dector<T>::operator[] (int index) {
    return this->arr[index];
}

template<class T>
dector<T>& dector<T>::operator = (dector<T>& rhs) {
    if (this != &rhs) {
        delete[] this->arr;
        this->arr = new T[rhs.capacity];
        for (int i = 0; i < rhs.size(); i++) {
            this->arr[i] = rhs.arr[i];
        }
        this->Size = rhs.size();
    }
    return *this;
}

template<class T>
void dector<T>::push_back(T newElement) {
    if (this->Size+1 >= this->capacity) {
        ensureCap();
    }
    this->Size++;
    this->arr[this->Size-1] = newElement;
}
///////////  SYMBOL TABLE /////////////////////////////
void SymbolTable::deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

    
Node* SymbolTable::rotateLeft(Node* node) {
    if (node->right == nullptr) return node;
    Node* temp = node;
    node = node->right;
    temp->right = node->left;
    if (node->left != nullptr) {
        node->left->parent = temp;
    }
    node->left = temp;
    node->parent = temp->parent;
    temp->parent = node;

    Node* p = node->parent;
    if (p != nullptr) {
        if (p->left == temp)
            p->left = node;
        else 
            p->right = node;
    }
    return node;
}

Node* SymbolTable::rotateRight(Node* node) {
    if (node->left == nullptr) return node;
    Node* temp = node;
    node = node->left;
    temp->left = node->right;
    if (node->right != nullptr) {
        node->right->parent = temp;
    }
    node->right = temp;
    node->parent = temp->parent;
    temp->parent = node;

    Node* p = node->parent;
    if (p != nullptr) {
        if (p->left == temp) {
            p->left = node;
        }
        else {
            p->right = node;
        }
    }
    return node;
}

Node* SymbolTable::splay(Node* node) {
    if (node == nullptr || this->root == node) return this->root;
    while (node->parent != nullptr) {
        Node* p = node->parent;
        Node* g = p->parent;
        if (g == nullptr) {
            if (p->left == node) {
                node = rotateRight(p);
            }
            else {
                node = rotateLeft(p);
            }
            return node;
        }
        if (g->left == p && p->left == node) {
            node = rotateRight(g);
            node = rotateRight(node);
        }
        else if (g->left == p && p->right == node) {
            node = rotateLeft(p);
            node = rotateRight(g);
        }
        else if (g->right == p && p->left == node) {
            node = rotateRight(p);
            node = rotateLeft(g);
        }
        else {
            node = rotateLeft(g);
            node = rotateLeft(node);
        }
    }
    return node;
}

string SymbolTable::tokenize(string s, char c, int& start) {
    string ans = "";
    for (int i = start; i < (int)s.size(); i++) {
        if (s[i] == c) {
            start = i+1;
            s[i] = '\0';
            return ans;
        }
        ans += s[i];
    }
    start = s.size();
    return ans;
}

Node* SymbolTable::insertNode(Node*& root, Node* parent, symbol x, int& num_comp, int& num_splay) {
    if (root == nullptr) {
        root = new Node();
        root->data = x;
        root->parent = parent;
        num_splay = (parent == nullptr ? 0 : 1);
        return root;
    }
    Node* nodeInserted;
    if (x < root->data) {
        num_comp++;
        nodeInserted = insertNode(root->left, root, x, num_comp, num_splay);
    }
    else {
        num_comp++;
        nodeInserted = insertNode(root->right, root, x, num_comp, num_splay);
    }
    return nodeInserted;
}


Node* SymbolTable::insert(symbol x, int& num_comp, int& num_splay) {
    Node* nodeInserted = insertNode(this->root, nullptr, x, num_comp, num_splay);
    this->root = splay(nodeInserted);
    return nodeInserted;
}


Node* SymbolTable::findNode(Node* root, symbol target, int& num_comp, int& num_splay) {
    if (root == nullptr) {
        return root;
    }
    if (root->data == target) {
        num_comp++;
        num_splay = (root->parent == nullptr ? 0 : 1);
        return root;
    }
    if (root->data > target) {
        num_comp++;
        if (root->left == nullptr) return root;
        return findNode(root->left, target, num_comp, num_splay);
    }
    else {
        num_comp++;
        if (root->right == nullptr) return root;
        return findNode(root->right, target, num_comp, num_splay);
    }
}

Node* SymbolTable::findNodeWithName(const string name, int& num_comp, int& num_splay) {
    int num_compInc = 0;
    int num_splayInc = 0;
    symbol target;
    target.name = name;
    Node* nodeToAssign;
    for (int i = this->currentLevel; i >= 0; i--) {
        num_compInc = 0;
        num_splayInc = 0;
        target.level = i;
        nodeToAssign = findNode(this->root, target, num_compInc, num_splayInc);
        if (nodeToAssign != nullptr && nodeToAssign->data == target) {
            num_comp += num_compInc;
            num_splay += num_splayInc;
            return nodeToAssign;
        }
    }
    return nullptr;
}

Node* SymbolTable::find(symbol target, int& num_comp, int& num_splay) {
    Node* nodeFound = findNode(this->root, target, num_comp, num_splay);
    this->root = splay(nodeFound); 
    if (nodeFound->data != target) return nullptr;
    return nodeFound;
}

void SymbolTable::remove(symbol target) {
    if (this->root == nullptr) {
        return;
    }

    int tempNum_comp; // use for nothing
    int tempNum_splay;
    Node* a = findNode(this->root, target, tempNum_comp, tempNum_splay);
    this->root = splay(a);
    if (a == nullptr || a->data != target) {
        return;
    }
    if (a->left == nullptr && a->right == nullptr) {
        this->root = nullptr;
    }
    // a has only left or right
    else if (a->left == nullptr) {
        this->root = this->root->right;
        root->parent = nullptr;
    }
    else if (a->right == nullptr) {
        root = root->left;
        root->parent = nullptr;
    }
    // a has both left and right
    else {
        root = root->left;
        root->parent = nullptr;
        Node* run = root;
        while (run->right != nullptr) {
            run = run->right;
        }
        root = splay(run);
        root->right = a->right;
        if (root->right)
            root->right->parent = root;
    }
    delete a;
}

bool SymbolTable::checkIDname(const string name) {
    regex e("[[:lower:]][[:w:]]*");
    return regex_match(name, e);    
}

bool SymbolTable::checkType(const string type) {
    if (type == "string" || type == "number") return true;
    if (type[0] != '(') return false;
    int start = 0;
    tokenize(type, '(', start);
    string lsAtrb = tokenize(type, ')', start);
    if (lsAtrb != "") {
        int i = 0;
        while (i < (int)lsAtrb.size()) {
            string atrb = tokenize(lsAtrb,',', i);
            if (atrb != "number" && atrb != "string") return false;
        }
    }
    if (type[start++] != '-' && type[start++] != '>') return false;
    string returnType = type.substr(start+1);
    if (returnType != "number" && returnType != "string") return false;
    return true;
}


bool SymbolTable::isNumber(const string value) {
    regex e("[[:digit:]]+");       // const number
    return regex_match(value, e);
}

bool SymbolTable::isString(const string value) {
    regex e("'[[:alnum:],' ']*'"); // const string
    return regex_match(value, e);
}

bool SymbolTable::isFunction(const string value) {
    int start = 0;
    string functName = tokenize(value, '(', start);
    if (functName == "") return false;
    if (value[start-1] != '(') return false;
    string lsAtrb = tokenize(value, ')', start);
    if (lsAtrb != "") {
        int i = 0;
        while (i < (int)lsAtrb.size()) {
            string atrb = tokenize(lsAtrb, ',', i);
            // In case atribute is not either number or string, check if it is a valid name
            if (!isNumber(atrb) && !isString(atrb) && !checkIDname(atrb)) {
                return false;
            }
        }
    }
    if (value[start-1] != ')') return false;
    return true;
}

bool SymbolTable::checkMatchAtrributes(const string type, const string value, int& typeOfError, int& num_comp, int& num_splay) {
    // type function:    (number,number)->number
    // value:            sum(1,1)
    // if type is not a function, return false;
    if (type == "number" || type == "string") return false;

    int i1 = 0; // type index tokenize
    int i2 = 0; // value index tokenize
    tokenize(type, '(', i1);
    string lsAtrbType = tokenize(type, ')', i1);
    tokenize(value, '(', i2);
    string lsAtrbVal = tokenize(value, ')', i2);
    i1 = 0;
    i2 = 0;
    int tempNum_comp = 0;
    int tempNum_splay = 0;
    while (i1 < (int)lsAtrbType.size()) {
        string atrbType = tokenize(lsAtrbType, ',', i1);
        string atrbVal = tokenize(lsAtrbVal, ',', i2);
        tempNum_comp = 0; // number of comparision to add to num_comp
        tempNum_splay = 0; // number of splay to add to num_splay
        if (atrbType == "number") {
            if (!isNumber(atrbVal)) {
                // check if atribute of value is a name declared before
                // check if that name is valid
                if (!checkIDname(atrbVal)) {
                    if (isString(atrbVal)) {
                        typeOfError = 1;
                        return false;
                    }
                    typeOfError = 3;
                    return false;
                }
                // find the name, if can not found throw Undeclared
                Node* nodeAtrbVal = findNodeWithName(atrbVal, tempNum_comp, tempNum_splay);
                this->root = splay(nodeAtrbVal);
                if (nodeAtrbVal == nullptr) {
                    typeOfError = 2;
                    return false;
                }
                // in case the atribute of value is found, check if has the same type with number
                if (nodeAtrbVal->data.type != "number") {
                    typeOfError = 1;
                    return false;
                }
            }
        }
        else if (atrbType == "string") {
            if (!isString(atrbVal)) {
                // check if atribute of value is a name declared before
                // check if that name is valid
                if (!checkIDname(atrbVal)) {
                    if (isNumber(atrbVal)) {
                        typeOfError = 1;
                        return false;
                    }
                    typeOfError = 3;
                    return false;
                }
                // find the name, if can not found throw Undeclared
                Node* nodeAtrbVal = findNodeWithName(atrbVal, tempNum_comp, tempNum_splay);
                this->root = splay(nodeAtrbVal);
                if (nodeAtrbVal == nullptr) {
                    typeOfError = 2;
                    return false;
                }
                // in case the atribute of value is found, check if has the same type with number
                if (nodeAtrbVal->data.type != "string") {
                    typeOfError = 1;
                    return false;
                }
            }
        }
        num_comp += tempNum_comp;
        num_splay += tempNum_splay;
    }
    return true;
}

string SymbolTable::takeReturnType(const string type) {
    int i = type.find(">");
    return type.substr(i+1);
}

string SymbolTable::toString(Node* node) {
    if (node == nullptr) return "";
    string ans = "";
    ans += node->data.name + "//";
    string level = to_string(node->data.level);
    ans += level;
    return ans;
}

Node* SymbolTable::findLastNodeToPrint(Node* root) {
    if (root == nullptr || (root->left == nullptr && root->right == nullptr)) {
        return root;
    }
    if (root->left == nullptr) {
        return findLastNodeToPrint(root->right);
    }
    if (root->right == nullptr) {
        return findLastNodeToPrint(root->left);
    }
    return findLastNodeToPrint(root->right);
}

void SymbolTable::print(Node* root, Node* lastNode) {
    if (root == nullptr) return;
    cout << toString(root);
    cout << (root == lastNode ? '\n' : ' ');
    print(root->left, lastNode);
    print(root->right, lastNode);
}
