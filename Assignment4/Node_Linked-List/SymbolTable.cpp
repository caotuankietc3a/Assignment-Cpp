#include "SymbolTable.h"

void SymbolTable::run(string filename) {
    ifstream file(filename);                         
    string currentLine;          // ON NO
    int scopeLevel = 0;
    int sizeOfHead = 0;

    while (getline(file, currentLine)) {
        int start = 0;
        int end = currentLine.find(" ");
/* COMMAND */
        string command = currentLine.substr(start, end-start);

        if (command == "INSERT") {
            start = end+1; 
            end = currentLine.find(" ", start);

/* <identifier_name> */
            string identifier_name = currentLine.substr(start, end-start);
            // COMMAND    <identifier_name> <type> , many spaces between COMMAND & <identifier_name>
            if (!checkIDname(identifier_name)){
                throw InvalidInstruction(currentLine);
                return;
            }

/* <type> */
            string type = currentLine.substr(end+1);
            // "COMMAND <identifier_name>""   || "COMMAND <identifier_name>     "
            if (type != "number" && type != "string") {
                throw InvalidInstruction(currentLine);
                return;
            }

            if (!insert(identifier_name, type, scopeLevel)) {
                throw Redeclared(currentLine);
                return;
            }
            else {
                cout << "success" << '\n';
            }
        }

        else if (command == "ASSIGN") {
            start = end+1;
            end = currentLine.find(" ",start);
            int typeOfError = -1; // Undeclared(0), TypeMismatch(1);
/* <identifier_name> */
            string identifier_name = currentLine.substr(start, end - start);
            if (!checkIDname(identifier_name)) {
                throw InvalidInstruction(currentLine);
                return;
            }

/* <value> */
            string val = currentLine.substr(end+1);
            string typeOfVal;
            
            if (!assign(identifier_name, val, typeOfVal, typeOfError, scopeLevel)) {
                // Undeclared     -- checked IDname & type
                if (typeOfError == 0) {
                    throw Undeclared(currentLine);
                } 
                // TypeMismatch
                else if (typeOfError == 1) { // typeOfError = 1
                    throw TypeMismatch(currentLine);
                }
                return;
            }
            else {
                cout << "success" << '\n';
            }
        }

        else if (command == "BEGIN") {
            scopeLevel++; 
            sizeOfHead++;
        } 

        else if (command == "END") {
            if (scopeLevel-1 < 0) {
                throw UnknownBlock();
                return;
            }

            Node* tmp = head[scopeLevel];
            while (head[scopeLevel] != nullptr) {
                head[scopeLevel] = head[scopeLevel]->next;
                delete tmp;
                tmp = head[scopeLevel];
            }
            scopeLevel--;
        }

        else if (command == "LOOKUP") {
/* <identifier_name> */
            string identifier_name = currentLine.substr(end+1);        
            if (!checkIDname(identifier_name)) {
                throw InvalidInstruction(currentLine);
                return;
            } 

            int scopeFounded = scopeLevel;

            if (!lookUp(identifier_name, scopeFounded)) {
                throw Undeclared(currentLine);
                return;
            }
            else {
                cout << scopeFounded << '\n';
            }
        }

        else if (command == "PRINT") {
            string str = "";
            string tmp;
            for (int i = scopeLevel; i >= 0; i--) {
                Node* start = head[i]; 
                while (start != nullptr) {
                    tmp = " " + start->IDname + "//";  
                    if (str.find(tmp) == string::npos) {
                        tmp = tmp + to_string(start->scopeLevel);
                        str = tmp + str;   // PRINT
                    }
                    start = start->next;
                }
            }
            if (str != "") {
                str.erase(0,1);
                str += '\n';
            }
            else 
                str = "\n";
            cout << str;
        }

        else if (command == "RPRINT") {
            string str = "";
            string tmp;
            for (int i = scopeLevel; i >= 0; i--) {
                Node* start = head[i];
                while (start != nullptr) {
                    tmp = " " + start->IDname + "//";
                    if (str.find(tmp) == string::npos) {
                        tmp = tmp + to_string(start->scopeLevel);
                        str = str + tmp;   // RPRINT
                    }
                    start = start->next;
                }
            }
            if (str != "") {
                str.erase(0,1);
                str += '\n';
            }
            else 
                str = "\n";
            cout << str;
        }
        else {
            throw InvalidInstruction(currentLine);
            return;
        }
    }
////////////    checking scope   ///////////////
    if (scopeLevel > 0) {
        throw UnclosedBlock(scopeLevel);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SymbolTable::SymbolTable() {
    for (int i = 0; i < MAXSCOPE; i++) {
        head[i] = nullptr; 
    }
}

SymbolTable::~SymbolTable() {
    for (int i = 0; i < MAXSCOPE; i++) {
        if (head[i]) {
            Node* start = head[i];
            while (head[i] != nullptr) {
                head[i] = head[i]->next;
                delete start;
                start = head[i];
            }
        }
    }
}

bool SymbolTable::insert(string name, string type, const int& scopeLevel) {
    // redeclared
    // check At scopeLevel if name exist
    if (checkExist(name, scopeLevel)) {
        return false;
    } 
    // Insert in list at index is scopeLevel;
    Node* tmp = new Node(name, type, scopeLevel);
    tmp->next = head[scopeLevel];
    head[scopeLevel] = tmp;
    
    return true;
}

bool SymbolTable::assign(string name, string val, string& typeOfVal, int& typeOfError, const int& scopeLevel) { // Undeclared, TypeMismatch
    for (int i = scopeLevel; i >= 0; i--) {
        Node* start = head[i];
        while (start != nullptr) {
            if (start->IDname == name) {
                if (!checkValue(val, typeOfVal, scopeLevel)) { // InvalidInstruction
                    typeOfError = 0; 
                    return false;
                } 
                if (typeOfVal != start->Type) {  // TypeMismatch
                    typeOfError = 1;
                    return false;
                }
                else {
                    start->value = val;
                    return true;
                }
            }
            start = start->next;
        }
    }
    typeOfError = 0;  // Undeclared
    return false;
} 

bool SymbolTable::lookUp(string name, int& scopeFounded) { // Undeclared
    for (; scopeFounded >= 0; scopeFounded--) {
        if (checkExist(name, scopeFounded)) {
            return true; 
        }
    }
    return false;
} 

////////////////////////////////  CHECKING SYNTAX OF <identifier_name> && <type> && <value>    /////////////////////////////////////////////////////////////////////
bool SymbolTable::checkIDname(const string IDname) {
    regex e("[[:lower:]][[:w:]]*");
    return regex_match(IDname, e);    
}

bool SymbolTable::checkValue(const string value, string& typeOfVal, const int& scopeLevel) {
    regex e1("'[[:alnum:],' ']*'"); // const string
    regex e2("[[:digit:]]+");       // const number
    if (regex_match(value , e1)) {
        typeOfVal = "string";
        return true;
    }
    if (regex_match(value, e2)) {
        typeOfVal = "number";
        return true;
    }
    // check if value is an IDname that was assigned before
    for (int i = scopeLevel; i >= 0; i--) {
        Node* start = head[i];
        while (start != nullptr) {
            if (start->IDname == value) {
                typeOfVal = start->Type;
                return true;
            } 
            start = start->next;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SymbolTable::checkExist(string name, int scopeLevel) {
    Node* temp = head[scopeLevel];
    while (temp != nullptr) {
        if (temp->IDname == name) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}