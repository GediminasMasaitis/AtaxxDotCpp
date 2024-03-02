#include "uai.h"

#include "evaluation.h"

#include <iostream>

using namespace std;

int main()
{
    cout << "ataxx.cpp by Gediminas Masaitis" << endl;

    Evaluation::init();

    Uai uai;
    uai.run();

    return 0;
}