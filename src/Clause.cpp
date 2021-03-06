#include "../include/Clause.h"
#include<algorithm>
#include<iostream>
#include<list>

using namespace std;

int Clause::nextUid = 0;

Clause::Clause(int variableNumber_) : literaux(unordered_set<Literal*>()), variableNumber(variableNumber_), uid(Clause::genUid())
{}

Clause::Clause(int variableNumber_, int uid_) : literaux(unordered_set<Literal*>()), variableNumber(variableNumber_), uid(uid_)
{}

void Clause::print() const ///Pour le debugage
{
    cout << "c ";
    for(Literal* c : literaux)
    {
        c->print();
        cout << " ";
    }
    cout << endl;
}

void Clause::addLiteral(Literal* nouveauLiteral)
{
    literaux.insert(nouveauLiteral);
}

void Clause::supprimer(Literal* l) ///Supprime toutes les occurences d'un litéral.
{
    literaux.erase(l);
}

unordered_set<Literal*> Clause::getLiteraux() const
{
    return literaux;
}

void Clause::fusionner(Clause* c) /** Fusionne la clause avec une autre.
L'utilisation des pointeurs sur les literaux assure (grace à la méthode insert) qu'il n'y a pas de doublons.
**/
{
    unordered_set<Literal*> lit(c->getLiteraux());

    for(Literal* l : lit)
        literaux.insert(l);
}

bool Clause::literalPresent(Literal* literal) const
{
    return literaux.count(literal) != 0;
}

Polarite Clause::polariteLiteral(int id) const ///Utile pour distinguer les deux parties pour les mariages.
{
    bool posTrouve = literalPresent(id);
    bool negTrouve = literalPresent(-id);
    if(posTrouve && negTrouve)
        return TAUTOLOGIE;
    if(!posTrouve && !negTrouve)
        return ABSENT;
    if(posTrouve)
        return POSITIF;
    return NEGATIF;
}

bool Clause::isTautologie() const ///Test simplement si un literal apparait avec les deux polarités.
{
    /*cout<<":"<<" "<<variableNumber<<" "<<literaux.size()<<endl;
    print();*/
    for(int i=1; i<variableNumber+1; ++i)
    {
        //cout<<i<<" ";
        if(polariteLiteral(i) == TAUTOLOGIE)
            return true;
    }


    return false;
}

bool Clause::literalPresent(int id) const
{
    for(Literal* literal : literaux)
        if(literal->getId() == id)
            return true;

    return false;
}

int Clause::indiceMax() const ///Donne l'indice maximum des variables de la clause (pour ranger dans les seaux).
{
    int sup = 0;

    for(Literal* l : literaux)
        if(l->getAbsId() > sup)
            sup = l->getAbsId();

    return sup;
}

int Clause::eval() const
{
    bool contientInconnu = false;
    for(Literal* l : literaux)
    {
        if(!l->isAssigne())
            contientInconnu = true;
        else if(l->getVal())
            return VRAI;
    }
    return contientInconnu ? INCONNU : FAUX;
}

bool Clause::isSurclause(const Clause* c) const ///Test si la clause est une surclause de la clause donnée en argument.
{
    unordered_set<Literal*> lit(c->getLiteraux());

    for(Literal* l : literaux)
        lit.erase(l);

    return lit.empty();
}

size_t Clause::size() const
{
    return literaux.size();
}

int Clause::getNombreDeVariables() const
{
    return variableNumber;
}

bool Clause::isVide() const
{
    return size() == 0;
}

bool Clause::contientLiteralVrai() const
{
    for(Literal* l : literaux)
        if(l->isAssigne() && l->getVal())
            return true;

    return false;
}

void Clause::supprimerLiterauxFaux()
{
    list<Literal*> literauxFaux;

    for(Literal* l : literaux)
        if(l->isAssigne() && (!l->getVal()))
            literauxFaux.push_front(l); //on ne peut supprimer directement car cela invaliderait l'itérateur

    supprimerLiteraux(literauxFaux);
}

void Clause::supprimerLiteraux(list<Literal*> literauxASupprimer)
{
    for(Literal* l : literauxASupprimer)
        literaux.erase(l);
}

int Clause::getUid() const
{
    return uid;
}

int Clause::genUid()
{
    ++Clause::nextUid;
    return Clause::nextUid;
}

bool operator==(Clause const &a, Clause const& b)
{
    if(a.size() != b.size())
        return false;

    unordered_set<Literal*> literauxA(a.getLiteraux());
    unordered_set<Literal*> literauxB(b.getLiteraux());

    for(Literal* l : literauxA)
        if(literauxB.erase(l) == 0)
            return false;

    return literauxB.empty();
}
