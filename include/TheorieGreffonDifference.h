#ifndef THEORIEGREFFONDIFFERENCE_H
#define THEORIEGREFFONDIFFERENCE_H

#include "TheorieGreffon.h"
#include "AtomeDifference.h"
#include<unordered_map>
#include<map>

class TheorieGreffonDifference : public TheorieGreffonSimple
{
public:
    TheorieGreffonDifference();
    virtual std::vector<int> onAssignation(int id, unsigned int niveau);
    virtual void onBacktrack(unsigned int l);
    void setCorrespondanceAtomes(const std::vector<AtomeDifference>& corr);
    std::pair<std::map<unsigned int,int>,std::vector<AtomeDifference>> getEtatCourant() const;
    std::map<unsigned int,int> getAssignation() const;

private:
    std::vector<AtomeDifference> atomes;
    std::unordered_map<AtomeDifference,int> literalPerAtome;
    std::vector<std::vector<std::vector<std::pair<unsigned int,int> > > > adjacence;
    unsigned int varIdMax;

    std::vector<AtomeDifference> testePresenceCycleDePoidsNegatif(unsigned int depart);
};

#endif
