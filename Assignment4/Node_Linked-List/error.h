#ifndef DSA_EXCEPTION_H
#define DSA_EXCEPTION_H
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;
class InvalidInstruction : public exception
{
    string mess;

public:
    InvalidInstruction(string instruction)
    {
        mess = "Invalid: " + instruction;
    }
    const char *what() const throw()
    {
        return mess.c_str();
    }
};
class TypeMismatch : public exception
{
    string mess;

public:
    TypeMismatch(string instruction)
    {
        mess = "TypeMismatch: " + instruction;
    }
    const char *what() const throw()
    {
        return mess.c_str();
    }
};
class Undeclared : public exception
{
    string mess;

public:
    Undeclared(string instruction)
    {
        mess = "Undeclared: " + instruction;
    }
    const char *what() const throw()
    {
        return mess.c_str();
    }
};
class Redeclared : public exception
{
    string mess;

public:
    Redeclared(string instruction)
    {
        mess = "Redeclared: " + instruction;
    }
    const char *what() const throw()
    {
        return mess.c_str();
    }
};
class UnclosedBlock : public exception
{
    string mess;

public:
    UnclosedBlock(int level)
    {
        mess = "UnclosedBlock: " + to_string(level);
    }
    const char *what() const throw()
    {
        return mess.c_str();
    }
};
class UnknownBlock : public exception
{
    string mess;

public:
    UnknownBlock()
    {
        mess = "UnknownBlock";
    }
    const char *what() const throw()
    {
        return mess.c_str();
    }
};
#endif