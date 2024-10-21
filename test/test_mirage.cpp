#include "../z3/src/api/c++/z3++.h"
#include <stdio.h>

z3::expr_vector to_expr_vector(z3::context &c,
                               std::vector<z3::expr> const &_vec) {
  z3::expr_vector vec(c);
  for (auto const &e : _vec) {
    vec.push_back(e);
  }
  return vec;
}

int main()
{
    z3::context c;

    // sorts
    z3::sort P = c.uninterpreted_sort("P");
    z3::sort I = c.int_sort();

    // function symbol
    z3::func_decl add = c.function("add", P, P, P);
    z3::func_decl mul = c.function("mul", P, P, P);
    z3::func_decl div = c.function("div", P, P, P);
    z3::func_decl exp = c.function("exp", P, P);
    z3::func_decl silu = c.function("silu", P, P);
    z3::func_decl rms = c.function("rms", I, P, P);
    z3::func_decl red = c.function("red", I, P, P);
    z3::func_decl subpattern = c.function("subpattern", P, P, c.bool_sort());

    // create solver
    z3::solver s(c);
    z3::params p(c);
    p.set("mbqi", true);
    p.set("rlimit", 80000u);
    s.set(p);

    // variables
    z3::expr x = c.constant("x", P);
    z3::expr y = c.constant("y", P);
    z3::expr z = c.constant("z", P);
    z3::expr i = c.int_const("i");
    z3::expr i1 = c.int_const("i1");
    z3::expr i2 = c.int_const("i2");

    // formulas
    s.add(forall(x, subpattern(x, x)));
    s.add(forall(x, y, z, implies(subpattern(x, y) && subpattern(y, z), subpattern(x, z))));
    s.add(forall(x, y, add(x, y) == add(y, x)));
    s.add(forall(x, y, mul(x, y) == mul(y, x)));
    s.add(forall(x, y, z, add(add(x, y), z) == add(x, add(y, z))));
    s.add(forall(x, y, z, mul(mul(x, y), z) == mul(x, mul(y, z))));
    s.add(forall(x, y, z, add(mul(x, z), mul(y, z)) == mul(add(x, y), z)));
    s.add(forall(x, y, z, add(div(x, z), div(y, z)) == div(add(x, y), z)));
    s.add(forall(x, y, z, mul(x, div(y, z)) == div(mul(x, y), z)));

    s.add(forall(x, y, subpattern(x, add(x, y))));
    s.add(forall(x, y, subpattern(x, mul(x, y))));
    s.add(forall(x, y, subpattern(x, div(x, y))));
    s.add(forall(x, y, subpattern(y, div(x, y))));
    s.add(forall(x, subpattern(x, exp(x))));
    s.add(forall(x, subpattern(x, silu(x))));
    s.add(forall(x, i, subpattern(x, rms(i, x))));
    s.add(forall(x, i, subpattern(x, red(i, x))));

    s.add(forall(x, x == red(c.int_val(0), x)));
    s.add(forall(x, i1, i2, red(i1, red(i2, x)) == red(i1 + i2, x)));
    s.add(forall(to_expr_vector(c, {x, y, i, i1, i2}),
                red(i, add(red(i1, x), red(i2, y))) ==
                    add(red(i + i1, x), red(i + i2, y))));
    s.add(forall(x, y, i, red(i, mul(x, y)) == mul(red(i, x), y)));
    s.add(forall(x, y, i, red(i, div(x, y)) == div(red(i, x), y)));
    z3::check_result result = s.check();
    return 0;
}
