#ifndef GestionConflits_H
#define GestionConflits_H

#include<vector>
#include "Formule.h"
#include "ConstructeurPreuve.h"

class GestionConflits
{
public:
    GestionConflits(int prochainConflit = 0);
    virtual ~GestionConflits();
    virtual void onBeggining(Formule* formule);
    virtual void onDeduction(Literal* literal, int clauseUid, int profondeurPile);
    virtual void onChoix(int literalId, int profondeurPile);
    virtual std::pair<int,std::pair<int,std::vector<int>>> onConflit(int clauseUid, int profondeurPile);
    int getConflitsNum() const __attribute__((pure));
protected:
    int conflitsNum;
    int prochainConflit;
};

class GestionConflitsApprentissage : public GestionConflits
{
public:
    GestionConflitsApprentissage(int prochainConflit = 0);
    void onBeggining(Formule* formule);
    void onDeduction(Literal* literal, int clauseUid, int profondeurPile);
    void onChoix(int literal, int profondeurPile);
    std::pair<int,std::pair<int,std::vector<int>>> onConflit(int clauseUid, int profondeurPile);
    int getNiveauBacktrack(const std::vector<int>& clause) const;
private:
    void displayInterface(ConstructeurPreuve constructeurPreuve);
    void addClause(const Clause* clause);
    void addClause(std::vector<int>& clause, int uid);
    int getLiteralConflictuel(int clauseUid) const;
    void updateNiveauChoix();
    void nettoyageNiveaux(int niveauFutur);

    std::vector<std::vector<int>> clauses;
    std::vector<std::pair<int,std::vector<int>>> pileDeDeductions;
    std::vector<int> niveauChoix;
};

#endif
