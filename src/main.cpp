#include "uai.h"

#include "evaluation_nn_base.h"

#include <iostream>

using namespace std;

int main()
{
    cout << "ataxx.cpp by Gediminas Masaitis" << endl;

    EvaluationNnueBase::init();

    Uai uai;
    uai.run();

    return 0;
}