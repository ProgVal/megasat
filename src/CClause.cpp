#include "Clause.h"
#include<algorithm>
#include<iostream>

using namespace std;

bool egalite_clauses(Clause const* a, Clause const* b) ///Ca aurait pu �tre utile
{
    return (*a==*b);
}

bool comp(const Literal* a, const Literal* b) ///Idem
{
    return a->getId()<b->getId();
}

bool comp_lit(const Literal* a, const Literal* b) /**Un peu subtil, a servi par le passe.
Avant de me mettre aux unordered_set, j'ai utilis� des set. Et avant cela des vector.
Et pour gagner un peu de temps, j'avais le bon gout de trier mes vectors Selon cet ordre ou le pr�c�dent.
**/
{
    if(a->getAbsId()==b->getAbsId())
        return a->getId()>b->getId();

    return a->getAbsId()<b->getAbsId();
}


Clause::~Clause()
{}

void Clause::print() const ///Pour le debugage
{
    for(Literal* c : literaux)
    {
        c->print();
        cout<<" ";
    }
    cout<<endl;
}

void Clause::supprimer(Literal* l) ///Supprime toutes les occurences d'un lit�ral.
{
    literaux.erase(l);
    n=literaux.size();
}

unordered_set<Literal*> Clause::getLiteraux() const
{
    return literaux;
}

int Clause::polariteLiteral(Literal* l1, Literal* l2) const ///Utile pour distinguer les deux parties pour les mariages.
{
    if(literaux.count(l1)!=0)
        return 1;
    else if(literaux.count(l2)!=0)
        return -1;
    return 0;
}

void Clause::fusionner(Clause* c) /** Fusionne la clause avec une autre.
L'utilisation des pointeurs sur les literaux assure (grace � la m�thode insert) qu'il n'y a pas de doublons.
**/
{
    unordered_set<Literal*> lit(c->getLiteraux());

    for(Literal* l : lit)
        literaux.insert(l);
}

bool Clause::isTautologie() const ///Test simplement si un literal apparait avec les deux polatit�s.
{
    vector<bool> found_pos(V,false);
    vector<bool> found_neg(V,false);

    for(Literal* l : literaux)
    {
        if(l->getId()>0)
            found_pos[l->getId()-1]=true;
        else
            found_neg[-l->getId()-1]=true;
    }

    for(int i=0;i<V;++i)
        if(found_neg[i] && found_pos[i])
            return true;

    return false;
}

void Clause::literalPresent(int id, bool& found_pos, bool& found_neg) const /**Test la pr�sence d'une variable et de sa n�gation.
On obtient le retour grace aux r�f�rences en argument. On obtient ainsi la polarite.
**/
{
    for(Literal* l : literaux)
    {
        if(l->getPolarite() && l->getId()==id)
            found_pos=true;
        else if(l->getId()==id)
            found_neg=true;
    }
}

void Clause::literauxPresents(vector<bool>& found_pos, vector<bool>& found_neg) const /** Idem que pr�c�demment mais avec
toutes les variables
**/
{
    for(Literal* l : literaux)
    {
        if(l->getPolarite())
            found_pos[l->getId()-1];
        else
            found_neg[-l->getId()-1];
    }
}

int Clause::indiceMax() const ///Donne l'indice maximum des variables de la clause (pour ranger dans les seaux).
{
    int sup=0;

    for(Literal* l : literaux)
        if(*l>sup)
            sup=l->getAbsId();

    return sup;
}

int Clause::eval() const /**Evalue une clause
Renvoie 0 si la clause est fausse.
1 si la clause est vraie.
2 si une variable n'est pas encore initialis�e.
NB : 1 peut �tre renvoy� alors que certaines variables ne sont pas encore d�finies.
**/
{
    for(Literal* l : literaux)
    {
        if(!l->isAssigned())
            return 2;
        else if(l->getVal())
            return 1;
    }
    return 0;
}

bool Clause::estSurclause(const Clause* c) const ///Test si la clause est une surclause de la clause donn�e en argument.
{
    unordered_set<Literal*> lit(c->getLiteraux());

    for(Literal* l : literaux)
        lit.erase(l);

    return lit.size()==0;
}

int Clause::size() const
{
    return n;
}

int Clause::getV() const
{
    return V;
}

bool Clause::isVide() const
{
    return (n==0);
}

Clause::Clause(const Clause& other) : literaux(other.literaux), V(other.V), n(other.n)
{}

Clause::Clause() : literaux(unordered_set<Literal*>()), V(0), n(0)
{}

Clause::Clause(unordered_set<Literal*> e, int V_e) : literaux(e), V(V_e), n(e.size())
{}

bool operator==(Clause const &a_, Clause const& b_)
{
    Clause a(a_);
    Clause b(b_);
    if(a.size()!=b.size())
        return false;

    unordered_set<Literal*> t(a.getLiteraux());
    unordered_set<Literal*> u(b.getLiteraux());

    for(Literal* l : t)
        if(u.erase(l)==0)
            return false;

    if(u.size()!=0)
        return false;

    return true;
}
