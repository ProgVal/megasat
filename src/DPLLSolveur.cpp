#include<vector>
#include<unordered_set>
#include "../include/DPLLSolveur.h"

DPLLSolveur::DPLLSolveur(Formule &formule_) : AbstractDPLLSolveur(formule_)
{}

DPLLSolveur::~DPLLSolveur()
{}

bool DPLLSolveur::isSatifiable()
{
    formule.supprimerTautologies();
    formule.simplifier();
    if(aClauseVide())
        return false;
    if(formule.isVide())
        return true;

    try
    {
        assigneUneVariable();
        return true;
    }
    catch(InsatisfiableException)
    {
        return false;
    }
}

void DPLLSolveur::assigneVariable(int varId, bool val)
{
    Variable* var = formule.getVar(varId);
    var->setVal(val);
    std::cout << "assigne " << var->getId() << " a " << var->getVal() << std::endl;

    formule.simplifier();

    //TODO: remove
    std::cout << "etat :" << std::endl;
    formule.print();

    if(aClauseVide())
        throw InsatisfiableException();
    if(formule.isVide())
        return;

    assigneUneVariable();
}

bool DPLLSolveur::aClauseVide()
{
    for(auto clause : formule.getClauses())
    {
        if(clause->isVide())
            return true;
    }
    return false;
}
