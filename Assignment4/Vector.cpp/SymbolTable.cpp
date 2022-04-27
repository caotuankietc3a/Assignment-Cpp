#include "SymbolTable.h"
vector<string> tokenize (string s, string del = " "){
    //Used for testcase tokenizer, students don't need to take care of this function.
    vector<string> res;
    int start = 0;
    int end = (int)s.find(del);
    while (end != -1 && int(s[start]) != 39) {
        res.push_back(s.substr(start, end - start));
        start = (int)end + (int)del.size();
        end = (int)s.find(del, start);
    }
    res.push_back(s.substr(start, s.size()));
    return res;
}

void SymbolTable::run(string filename)
{   
    ifstream file(filename);
    string str = "";
    vector<string> check;
    vector<string> check_type;
    vector<int> block_type;
    int size = 0;
    while(getline(file, str)){
        vector<string> vec;
        vec = tokenize(str);
        
        if(vec[0] == "INSERT"){
                int index = -1;
                for(int i = block_type.size()-1; i>= 0; i--){
                    if(check[i] == vec[1]) {index = i;}
                }
                if(index == -1 || ( index >= 0 && size != block_type[index])){
                        check.push_back(vec[1]);
                        check_type.push_back(vec[2]);
                        block_type.push_back(size);
                        cout << "success" << endl;
                }
                else 
                {
                    throw Redeclared(str);
                }

        }
        else if(vec[0] == "ASSIGN"){
            bool found = false;
            string is_Num = "";
            string is_String = "";
            bool is_Identifier = false;
            if(vec.size() < 3) throw Undeclared(str);

            
            int index = -1;
            for(int i = check.size()-1; i >= 0; i--) {
                if(vec[1] == check[i]) {index = i; break;}
            }
            if(index == -1) throw Undeclared(str);
            if(vec[2][0] >= '0' && vec[2][0] <= '9'){
                for(int i = 1; i < vec[2].size() && !found; i++) {
                    if(vec[2][i] < '0' || vec[2][i] > '9') found = true;
                }
                if(!found) is_Num = "number";
                if(is_Num == check_type[index]) cout << "success" << endl;
                else throw TypeMismatch(str);
            }
            else if(int(vec[2][0]) ==  39){
                for(int i = 1; i < vec[2].size() - 1; i++){
                    if((vec[2][i] >= '0' && vec[2][i] <= '9') || (vec[2][i] <= 'Z' && vec[2][i] >= 'A') || (vec[2][i] >= 'a' && vec[2][i] <= 'z') || vec[2][i] == ' ' || vec[2][i] == '_') continue;
                    else found = true;
                }

                if(!found && int(vec[2][vec[2].size() - 1]) == 39) is_String = "string";
                if(is_String == check_type[index]) cout << "success" << endl;
                else throw TypeMismatch(str);
            }
            else{
                int index1 = -1;
                for(int i = check.size()-1; i >= 0; i--) {
                if(vec[2] == check[i]) {index1 = i; break;}
                }
                if(index1 == -1) throw Undeclared(str);
                if(check_type[index] == check_type[index1]) is_Identifier = true;///////
                if(is_Identifier) cout << "success" << endl;
                else throw TypeMismatch(str);
            }
        }
        else if(vec[0] == "BEGIN" || vec[0] == "END"){
            if(vec[0] == "END") {
                if(size == 0) throw UnknownBlock();
                int index = -1;
                for(int i = 0; i < block_type.size(); i++) if(block_type[i] == size) {index = i; break;}
                if(index != -1){check.erase(check.begin() + index, check.end());
                check_type.erase(check_type.begin() + index, check_type.end());
                block_type.erase(block_type.begin() + index, block_type.end());
                }
                size --;
            }
            else{
                size++;
            }
        }
        else if(vec[0] == "LOOKUP"){
            int index = 0;
            bool found = false;
            for(int i = check.size() - 1; i >= 0; i--){
                if(vec[1] == check[i]) {
                    index = i;
                    found = true;
                    break;
                }
            }
            if(found) cout << block_type[index] << endl;
            else{
                throw Undeclared(str);
            }
        }
        else if(vec[0] == "PRINT"){
            if(check.size() != 0){
                vector<string> check1;
                vector<int> block_type1;
                for(int i = check.size() -1; i >= 0; i--){
                    vector<string>::iterator it = find(check1.begin(), check1.end(), check[i]);
                    if(it == check1.end()) {
                        check1.push_back(check[i]);
                        block_type1.push_back(block_type[i]);
                    }
                }
                for(int i = check1.size()-1; i >= 0; i--){
                    cout << check1[i] << "//" << block_type1[i];
                    if(i != 0) cout << " ";
                }
                cout << endl;
            }
        }
        else if(vec[0] == "RPRINT" ){
            if(check.size() != 0) 
            {
                vector<string> check1;
                vector<int> block_type1;
                for(int i = check.size() -1; i >= 0; i--){
                    vector<string>::iterator it = find(check1.begin(), check1.end(), check[i]);
                    if(it == check1.end()) {
                        check1.push_back(check[i]);
                        block_type1.push_back(block_type[i]);
                    }
                }
                for(int i = 0; i < check1.size(); i++){
                    cout << check1[i] << "//" << block_type1[i] ;
                    if(i != check1.size() -1) cout << " ";
                }
                cout << endl;
            }
        }
        else throw InvalidInstruction(str);
    }
    if(size > 0) throw UnclosedBlock(size);
    else if(size < 0) throw UnknownBlock();
    file.close();
}