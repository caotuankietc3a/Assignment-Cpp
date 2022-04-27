#include "SymbolTable.h"

int checkKey(const SplayNode* n1, const SplayNode* n2){
    if(n1->LevelofBlock < n2->LevelofBlock) return 1; // key1 < key2
    else if(n1->LevelofBlock > n2->LevelofBlock) return -1;
    else{
        if(strcmp(&(n1->ID)[0], &(n2->ID)[0]) < 0) return 1; // key1 < key2
        else if(strcmp(&(n1->ID)[0], &(n2->ID)[0]) > 0) return -1;
        else return 0;
    }
    return INT32_MIN;
}

bool CheckEqual(SplayNode* n1, SplayNode* n2){
    if(n1 == NULL || n2 == NULL) return false;
    if(n1->ID == n2->ID && n1->LevelofBlock == n2->LevelofBlock && n1->type == n2->type) return true;
    return false;
}

void SplayTree::R_Rotate(SplayNode*& root, SplayNode* node){
    SplayNode* y = node->left;
    node->left = y->right;
    if(y->right != NULL) y->right->parent = node;
    y->parent = node->parent;
    
    // subtree above points to subtree below
    if(node->parent == nullptr) root = y;
    else if(CheckEqual(node, node->parent->left)) node->parent->left = y;
    else node->parent->right = y;

    y->right = node;
    node->parent = y;
}

void SplayTree::L_Rotate(SplayNode*& root, SplayNode* node){
    SplayNode* y = node->right;
    node->right = y->left;
    
    if(y->left != nullptr) y->left->parent = node;
    y->parent = node->parent;
    
    // subtree above points to subtree below
    if(node->parent == nullptr) root = y;
    else if(CheckEqual(node, node->parent->left)) node->parent->left = y;
    else node->parent->right = y;
    
    y->left = node;
    node->parent = y; 
}

void SplayTree::Splay(SplayNode*& root, SplayNode* key){
    while(key->parent != NULL){
        if(key->parent->parent == NULL){
            if(CheckEqual(key->parent->left, key)) R_Rotate(root, key->parent);
            else L_Rotate(root, key->parent);
        }
        else{
            SplayNode* p = key->parent;
            SplayNode* q = p->parent;
            if(CheckEqual(key, p->left) && CheckEqual(key->parent, q->left)){
                R_Rotate(root, q);
                R_Rotate(root, p);
            }
            else if(CheckEqual(key, p->right) && CheckEqual(key->parent, q->right)){
                L_Rotate(root, q);
                L_Rotate(root, p);
            }
            else if(CheckEqual(key, p->right) && CheckEqual(key->parent, q->left)){
                L_Rotate(root, p);
                R_Rotate(root, q);
            }
            else if(CheckEqual(key, p->left) && CheckEqual(key->parent, q->right)){
                R_Rotate(root, p);
                L_Rotate(root, q);
            }
        }
    }
}

void SplayTree::Insert(SplayNode*& root, std::string id, int LoB, std::string t, std::string str, int& numCmp, int& numSplay, SplayTree& tree){
    
    SplayNode* temp = root;
    SplayNode* previousNode = NULL;
    SplayNode* newNode = new SplayNode(id, LoB, t);
    
    while(temp != NULL){
        previousNode = temp;
        if(checkKey(temp, newNode) == 1) {temp = temp->right; 
        numCmp++;}
        else if(checkKey(temp, newNode) == -1) {temp = temp->left; numCmp++;}
        else if(checkKey(temp, newNode) == 0) {
            delete newNode;
            DeleteTree(tree);
            throw Redeclared(str);
        }    
    }
     
    newNode->parent = previousNode;
    if(previousNode == NULL) {root = newNode; return;}
    else{
        if(checkKey(previousNode, newNode) == 1) previousNode->right = newNode;
        else if(checkKey(previousNode, newNode) == -1) previousNode->left = newNode;
    }
    
    numSplay++;
    Splay(root, newNode);
}

bool SymbolTable::checkFunction(std::string str3){
    if(str3[0] == '(') 
    {   
        if(str3[1] == ')' && str3[2] == '-' && str3[3] == '>'){
            if(str3.substr(4, str3.size() - 4) == "number" || str3.substr(4, str3.size() - 4) == "string") return true;
        }
        int start = 1;
        int end = str3.find(',', start);
        while(end != -1){
            std::string s = str3.substr(start, end - start);
            if(s != "number" && s != "string") return false;
            start = end + 1;
            end = str3.find(',', start);
        }
        end = str3.find(')', start);
        if(str3.substr(start, end - start) == "number" || str3.substr(start, end - start) == "string"){
            if(str3[1 + end] == '-' && str3[2 + end] == '>'){
                if(str3.substr(end + 3, str3.size() - (end + 3)) == "number" || str3.substr(end + 3, str3.size() - (end + 3)) == "string") return true;
            }
        }
    }
    return false;
}

bool SymbolTable::checkNumber(std::string str3){
    std::regex ex("[[:digit:]]+");
    return regex_match(str3, ex);
}

bool SymbolTable::checkString(std::string str3){
    std::regex ex("'[[:alnum:][:space:]]*'");
    return regex_match(str3, ex);
}

bool SymbolTable::checkType_Func(SplayTree& tree, std::string type, std::string s, int LoB, int& numCmp, int& numSplay, std::string str){
    if(type[0] == s[0] && s[0] == '(' && type[type.size()-1] == s[s.size()-1] && s[s.size()-1] == ')'){
        if(type.size() == 2 && s.size() == 2){
            return true;
        }
        else{int start1 = 1;
        int end1 = type.find(',');
        
        int start2 = 1;
        int end2 = s.find(',');

        while(end1 != -1 && end2 != -1){

            std::string st1 = type.substr(start1, end1 - start1);
            std::string st2 = s.substr(start2, end2 - start2);

            if(!((st1 == "number" && checkNumber(st2)) || (st1 == "string" && checkString(st2)))){
                int level = 0;
                tree.findNode(tree.root, st2, LoB, "", numCmp, numSplay, level);
                SplayNode* node = tree.root;
                if(tree.root->ID != st2) 
                {
                    tree.DeleteTree(tree);
                    throw Undeclared(str);
                }
                if(node->type != st1) return false;
            }
            
            start1 = end1 + 1;
            start2 = end2 + 1;
            end1 = type.find(',', start1);
            end2 = s.find(',', start2);
        }
        if(end1 == -1 && end2 == -1){
                string st1 = type.substr(start1, type.size() - start1 - 1);
                string st2 = s.substr(start2, s.size() - start2 - 1);
                if(!((st1 == "number" && checkNumber(st2)) || (st1 == "string" && checkString(st2)))){
                    int level = 0;
                    tree.findNode(tree.root, st2, LoB, "", numCmp, numSplay, level);
                    SplayNode* node = tree.root;
                    if(tree.root->ID != st2){
                        tree.DeleteTree(tree);
                        throw Undeclared(str);
                    } 
                    if(node->type != st1) return false;
                }
            }
        return true;}
    }
    return false;
}

void SplayTree::DeleteNode(SplayNode*& root, std::string id, int LoB, std::string t){
    if(root != NULL){ 
        SplayNode* key = new SplayNode(id, LoB, t);
        SplayNode* search = root;
        while(search){
            if(checkKey(search, key) == 1) search = search->right;
            else if(checkKey(search, key) == -1) search = search->left;
            else break;
        }
        Splay(root, search);

        SplayNode* node;

        if(root->left == NULL){
            node = root;
            if(root->right){ 
                root->right->parent = nullptr;
                root = root->right;
            }else root = nullptr;
        }
        else{
            node = root;

            if(root->right != nullptr){
                SplayNode* tempS = root->left;
                SplayNode* tempL = root->left;

                while(tempS->right) tempS = tempS->right;

                root->left = nullptr;
                tempL->parent = nullptr;
                Splay(tempL, tempS);

                SplayNode* tempR = root->right;
                root->right = nullptr;
                tempL->right = tempR;
                tempR->parent = tempL;
                root = tempL;
            }else{
                root = root->left;
                root->parent = nullptr;
            }           
        }
        delete key;
        delete node;
    }
}

void SplayTree::findNode(SplayNode*& root, const std::string& id, int LoB, std::string type, int& numCmp, int& numSplay, int& level){
    
    if(root != NULL){

        for(int i = LoB; i >=0; i--) 
        {
            SplayNode* temp = root;
            SplayNode* pre = NULL;
            SplayNode key = SplayNode(id, i, type);
            int numCmp1 = 0;
            while(temp != NULL){
                pre = temp;
                if(checkKey(temp, &key) == 1) {
                    temp = temp->right; 
                    numCmp1++;
                }
                else if(checkKey(temp, &key) == -1) {
                    temp = temp->left; 
                    numCmp1++;
                }
                else if(checkKey(temp, &key) == 0) {
                    if(temp->parent != NULL) {Splay(root, temp); numSplay++;}
                    numCmp1++;
                    level = i;
                    numCmp += numCmp1;
                    return;
                }
            }
        }
    }
}

void SplayTree::Print_Pre(SplayNode* root, int& count){
    if(root != NULL){
        std::cout << root->ID << "//" << root->LevelofBlock;
        if(count > 1) {std::cout << " ";}
        count--;
        Print_Pre(root->left, count);
        Print_Pre(root->right, count);
    }
}

void Search(SplayNode* root, int index, SplayNode*& x){
    if(root){ 
        if(root->index == index) {
            x = root;
            return;
        }
        Search(root->left, index, x);
        Search(root->right, index, x);
    }
}

void SplayTree::DeleteSub(SplayNode*& root){
    if(root){
        DeleteNode(root, root->ID, root->LevelofBlock, root->type);
        DeleteSub(root);
    }
}

void SplayTree::DeleteTree(SplayTree& tree){
    DeleteSub(tree.root);
    for(int i = 0; i < 1000; i++) DeleteSub(tree.rootN[i]);
}

void SymbolTable::run(string filename)
{
    ifstream file(filename);
    std::string str = "";
    SplayTree tree;
    int LevelOfBlock = 0;
    int count1000 = 0;
    int count[1000];
    for(int i = 0; i < 1000; i++) count[i] = 1;
    while(getline(file, str)){
        int start = 0;
        int end = (int)str.find(" ");
        std::string str1 = str.substr(start, end - start);

        if(str1 == "INSERT"){
            start = end + 1;
            end = (int)str.find(" ", start);
            std::string str2 = str.substr(start, end - start);

            // CheckID
            bool checkID = [=]() mutable->bool{
                std::regex ex("[[:lower:]][[:w:]]*");
                return regex_match(str2, ex);
            }();
            if(!checkID) throw InvalidInstruction(str);

            start = end + 1;
            end = (int)str.find(" ", start);
            std::string str3 = str.substr(start, end - start);
            start = end + 1;
            end = (int)str.find(" ", start);
            std::string str4 = str.substr(start, str.size() - start);
            int numCmp = 0, numSplay = 0;

            if(checkFunction(str3) == true) {
                if(LevelOfBlock == 0 || str4 == "true") {
                    int numCmp1 = 0, numSplay1 = 0;
                    tree.Insert(tree.rootN[0], str2, 0, str3, str,numCmp1, numSplay1, tree);
                    tree.rootN[0]->index = count[0];
                    count[0]++;

                    tree.Insert(tree.root, str2, 0, str3, str, numCmp, numSplay, tree);
                    count1000++;
                }
                else throw InvalidDeclaration(str);
            }
            else if(str3 == "number" || str3 == "string") {
                if(str4 == "false") {
                    int numCmp1 = 0, numSplay1 = 0;
                    tree.Insert(tree.rootN[LevelOfBlock], str2, LevelOfBlock, str3, str, numCmp1, numSplay1, tree);
                    tree.rootN[LevelOfBlock]->index = count[LevelOfBlock];
                    count[LevelOfBlock]++;
                    
                    tree.Insert(tree.root, str2, LevelOfBlock, str3, str, numCmp, numSplay, tree);
                    count1000++;
                }
                else if(str4 == "true") {
                    int numCmp1 = 0, numSplay1 = 0;
                    tree.Insert(tree.rootN[0], str2, 0, str3, str, numCmp1, numSplay1, tree);
                    tree.rootN[0]->index = count[0];
                    count[0]++;

                    tree.Insert(tree.root, str2, 0, str3, str, numCmp, numSplay, tree);
                    count1000++;
                }
                else throw InvalidDeclaration(str);
            }
            else throw InvalidInstruction(str);
            std::cout << numCmp << " " << numSplay << std::endl;
        }

        else if(str1 == "ASSIGN"){

            if(tree.root == NULL) throw Undeclared(str);

            int numCmp = 0, numSplay = 0;
            start = end + 1;
            end = (int)str.find(" ", start);
            std::string str2 = str.substr(start, end - start);
            start = end + 1;
            std::string str3 = str.substr(start, str.size() - start);
                end = str3.find('(');
                start = 0;
                int level = 0;
                if(end == -1){
                    if(checkString(str3)){
                        
                        tree.findNode(tree.root, str2, LevelOfBlock, "", numCmp, numSplay, level);
                        if(tree.root->ID != str2) throw Undeclared(str);

                        if(tree.root->type == "string" && checkString(str3)) std::cout<< numCmp << " " << numSplay << std::endl;
                        else throw TypeMismatch(str);
                    }
                    else if(checkNumber(str3)){
                        tree.findNode(tree.root, str2, LevelOfBlock, "", numCmp, numSplay, level);
                        if(tree.root->ID != str2) throw Undeclared(str);

                        if(tree.root->type == "number" && checkNumber(str3)) std::cout<< numCmp << " " << numSplay << std::endl;
                        else throw TypeMismatch(str);
                    }
                    else{

                        tree.findNode(tree.root, str3, LevelOfBlock, "", numCmp, numSplay, level);
                        if(tree.root->ID != str3) throw Undeclared(str);
                        SplayNode* save = tree.root;

                        tree.findNode(tree.root, str2, LevelOfBlock, "", numCmp, numSplay, level);
                        if(tree.root->ID != str2) throw Undeclared(str);

                        if(save->type == tree.root->type) std::cout << numCmp << " " << numSplay << std::endl;
                        else throw TypeMismatch(str);
                    }                    
                }
                else{
                    string nameFunc = str3.substr(start, end-start);

                    tree.findNode(tree.root, nameFunc, LevelOfBlock, "", numCmp, numSplay, level);
                    if(tree.root->ID != nameFunc) throw Undeclared(str);

                    SplayNode* node = tree.root;
                    
                        int st = node->type.find('>');
                        std::string s1 = node->type.substr(st + 1, (node->type).size() - st);

                        start = end;
                        std::string s = str3.substr(start, str3.size()- start);
                        
                        start = 0; end = node->type.find('-');
                        if(!checkType_Func(tree, node->type.substr(start, end-start), s, LevelOfBlock ,numCmp, numSplay, str)) throw TypeMismatch(str);

                        tree.findNode(tree.root, str2, LevelOfBlock, "", numCmp, numSplay, level);
                        if(tree.root->ID != str2) throw Undeclared(str);

                        if(s1 == tree.root->type){
                            std::cout << numCmp << " " << numSplay << std::endl;
                        }
                        else throw  TypeMismatch(str);
                        
                    
                }
                
        }
        else if(str1 == "BEGIN"){
            LevelOfBlock += 1;
        }
        else if(str1 == "END"){
            int numCmp = 0, numSplay = 0;
            if(LevelOfBlock == 0) throw UnknownBlock();

            for(int i = 1; i < count[LevelOfBlock]; i++){
                SplayNode* temp;
                Search(tree.rootN[LevelOfBlock], i, temp);
                    tree.DeleteNode(tree.root, temp->ID, LevelOfBlock, temp->type);
                    tree.DeleteNode(tree.rootN[LevelOfBlock], temp->ID, LevelOfBlock, temp->type);
                    count1000--;
                
            }

            count[LevelOfBlock] = 1;
            LevelOfBlock -= 1;
        }
        else if(str1 == "LOOKUP"){
            int numCmp = 0, numSplay = 0;
            int level = 0;
            start = end + 1;
            end = (int)str.find(" ", start);
            std::string str2 = str.substr(start, end - start);
            tree.findNode(tree.root, str2, LevelOfBlock, "", numCmp, numSplay, level);
            if(tree.root == NULL) throw Undeclared(str);
            if(tree.root->ID != str2) throw Undeclared(str);

            std::cout << level << std::endl;
        }
        else if(str1 == "PRINT"){
            int count = count1000;
            tree.Print_Pre(tree.root, count1000);
            if(tree.root != NULL) std::cout << std::endl;
            count1000 = count;
        }
    }
    if(LevelOfBlock != 0) throw UnclosedBlock(LevelOfBlock);

    // Delete tree at rootN[0]
    tree.DeleteTree(tree);
   
    file.close();
}
