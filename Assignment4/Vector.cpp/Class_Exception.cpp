#include <iostream>
#include <exception>
using namespace std;
class myException : public exception{
    public:
    myException(){};
    // don't need to const throw().
    void myRun(){
        throw myException();
        return;
    }
    const char* myWhat() const throw(){
        return "C++ of myException!!!!";
    }
};
int main(){
    myException* myEx = new myException();
    try{
        myEx->myRun();
        cout << "Successfull!!!!";
    }
    catch(myException& ex){
        cout << ex.myWhat() << endl;
    } 
    delete myEx;
}