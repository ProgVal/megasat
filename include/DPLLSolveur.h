#ifndef DavisPutnamSolver_H
#define DavisPutnamSolver_H

#include"Solveur.h"

class DPLLSolveur : public Solveur
{
public:
    DPLLSolveur(Formule &formule);
    bool isSatifiable();
};

#endif