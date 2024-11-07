#include "../z3/src/api/c++/z3++.h"
#include <stdio.h>
#include <iostream>

using namespace z3;
int main() {
    std::cout << "de-Morgan example\n";
    z3::context c;
    Z3_enable_trace("solver_na2as");
    z3::sort P = c.uninterpreted_sort("P");
    z3::expr x = c.constant("x", P);
    z3::expr y = c.constant("y", P);
    z3::expr z = c.constant("z", P);
    z3::func_decl add = c.function("add", P, P, P);
    z3::expr commutative = forall(x, y, add(x, y) == add(y, x));
    z3::expr not_commutative = exists(x, y, add(x, y) != add(y, x));
    // z3::expr associative = forall(x, y, z, add(add(x, y), z) == add(x, add(y, z)));
    ::std::string str = commutative.to_string(); //"(forall ((x P) (y P)) (! (= (add x y) (add y x)) :weight 0))"
    str = not_commutative.to_string();
    solver s(c);
    // adding the negation of the conjecture as a constraint.
    s.add(commutative);
    // s.add(associative);
    s.add(not_commutative);
    // std::cout << s << "\n";
    // std::cout << s.to_smt2() << "\n";
    switch (s.check()) {
        case unsat:   
            std::cout << "is not valid\n"; break;
        case sat:     
            std::cout << "is valid\n"; break;
        case unknown: 
            std::cout << "unknown\n"; break;
    }
    return 0;
}
