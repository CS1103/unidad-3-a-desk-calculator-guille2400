//
// Created by sanch on 10/28/2019.
//

#ifndef INC_28_10_19_TOKEN_STREAM_H
#define INC_28_10_19_TOKEN_STREAM_H

#include "Token.h"
#include "Table.h"
#include "Biblios.h"
using namespace std;
class Token_stream{
private:
    void close(){if(owns)delete ip;}
    std::istream* ip; // pointer to an input stream
    bool owns;        // does the Token_Stream own the istream ?
    Token ct{Kind::end};  // current token
public:
    explicit Token_stream (std::istream& s):ip{&s}, owns{false}{};
    explicit Token_stream (std::istream* p):ip{p}, owns{true}{};

    ~Token_stream(){close();}

    Token get(); // read and return next token
    const Token& current(); // most recently read token

    void set_input(std::istream& s){close(); ip = &s; owns = false;}
    void set_input(std::istream* p){close(); ip = p; owns = true;}
};


Token Token_stream::get() {
    char ch = 0;
    *ip>>ch;

    switch(ch){
        case 0:
            ct.kind = Kind::end; //assign
            return ct; // return
        case ';':
        case '*':
        case '/':
        case '+':
        case '-':
        case '(':
        case ')':
        case '=':
            return ct = {static_cast<Kind>(ch)};
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
            ip -> putback(ch); // put the first digit back into the input stream
            *ip >> ct.number_value; //read the number into ct
            ct.kind = Kind::number;
            return ct;
        default: //name, name =, or error
            if(isalpha(ch)){
                ip->putback(ch); // put the first character back into the input stream
                *ip>>ct.string_value; // read the string into ct
                ct.kind = Kind::name;
                return ct;
            }

            error("bad token");
            return ct={Kind::print};
    }
}

const Token &Token_stream::current() {
    return ct;
}


double expr(bool get) // add and subtract
{
    double left = term(get);
    for (;;) { // ‘‘forever’’
        switch (ts.current().kind) {
            case Kind::plus:
                left += term(true);
                break;
            case Kind::minus:
                left −= term(true);
                break;
            default:
                return left;
        }
    }
}

double term(bool get) // multiply and divide
{
    double left = prim(get);
    for (;;) {
        switch (ts.current().kind) {
            case Kind::mul:
                left ∗= prim(true);
                break;
            case Kind::div:
                if (auto d = prim(true)) {
                    left /= d;
                    break;
                }
                return error("divide by 0");
            default:
                return left;
        }
    }
}

double prim(bool get) // handle primar ies
{
    if (get) ts.get(); // read next token
    switch (ts.current().kind) {
        case Kind::number: // floating-point constant
        { double v = ts.current().number_value;
            ts.get();
            return v;
        }
        case Kind::name:
        { double& v = table[ts.current().string_value]; // find the corresponding
            if (ts.get().kind == Kind::assign) v = expr(true); // ’=’ seen: assignment
            return v;
        }
        case Kind:: minus: //unary minus
            return -prim(true);
        case Kind::lp:{
            auto e = expr(true);
            if(ts.current().kind != Kind::rp) return error("')' expected" );
            ts.get(); //eat `)`
            return e;
        }
        default:
            return error("primary expected");
    }
}

int no_of_errors;
double error(const string& s)
{
    no_of_errors++;
    cerr << "error: " << s << '\n';
    return 1;
}

#endif //INC_28_10_19_TOKEN_STREAM_H
