#include "../z3/src/api/c++/z3++.h"
#include <stdio.h>
#include <iostream>

using namespace z3;

int main() {
    std::cout << "de-Morgan example\n";
    
    context c;

    expr x = c.bool_const("x");
    expr y = c.bool_const("y");
    expr conjecture = (!(x && y)) == (!x || !y);
    
    solver s(c);
    // adding the negation of the conjecture as a constraint.
    s.add(conjecture);
    std::cout << s << "\n";
    std::cout << s.to_smt2() << "\n";
    switch (s.check()) {
        case unsat:   
            std::cout << "de-Morgan is not valid\n"; break;
        case sat:     
            std::cout << "de-Morgan is valid\n"; break;
        case unknown: 
            std::cout << "unknown\n"; break;
    }
    return 0;
}
