#ifndef OPTIMIZE__
#define OPTIMIZE__
#include "parse.h"

// extern Program* prog;
// extern Env *env, *global;
extern std::unordered_map<Var*, std::pair<int, double>> varInitVal;

void optimize();

#endif