#include<vector>
#include<unordered_set>
#include "../include/DavisPutnamSolveur.h"

DavisPutnamSolveur::DavisPutnamSolveur(Formule &formule_) : Solveur(formule_)
{}

bool DavisPutnamSolveur::isSatifiable()
{
    /*std::vector<Formule*> seaux(0);
    
    for(int i = 0; i < formule.getVariableNumber(); ++i) /// On crée les seaux.
        seaux.push_back(new Formule(0,vars,lits_pos,lits_neg));
    
    for(Clause* c : clauses) /// On remplit les seaux.
        if(!c->isTautologie())
            seaux[c->indiceMax()-1]->addClause(c);

    try
    {
        for(int i = V; i > 0; i--) ///Les résolutions dans le sens descendant
            fusionner(resoudreSeau(seaux[i-1],i),seaux);

        for(int i = 0; i < V; i++) ///La remonté
            chercherAssignation(seaux[i], i);

        return true;
    }
    catch(int e)
    {
        return false;
    }*/
    return true;
}

/*Formule* Formule::resoudreSeau(const Formule* seau, int id) const
{
    unordered_set<Clause*> pos;
    unordered_set<Clause*> neg;
    unordered_set<Clause*> autres;
    unordered_set<Clause*> all(seau->getClauses());
    Formule* sortie=new Formule(V,vars,lits_pos,lits_neg);
    Clause* work;
    
    for(Clause* c : all) ///Sépare les polarités
    {
        if(c->polariteLiteral(lits_pos[id-1],lits_neg[id-1])==1)
            pos.insert(c);
        else if(c->polariteLiteral(lits_pos[id-1],lits_neg[id-1])==-1)
            neg.insert(c);
        else
            autres.insert(c);
    }
    sortie->addClauses(autres);
    
    unsigned int i=0;
    unsigned int j=0;
    for(unordered_set<Clause*>::iterator it=pos.begin(); it!=pos.end(); ++it,++i) ///On double-boucle pour faire toutes les résolutions...
    {
        printf("c ["); /// Affichage !
        unsigned int l=0;
        for(; l<i*50/pos.size(); ++l)
            printf("#");
        for(; l<50; ++l)
            printf(" ");
        printf("] %d%%",i*100/static_cast<int>(pos.size()));
        j=0;
        for(unordered_set<Clause*>::iterator jt=neg.begin(); jt!=neg.end(); ++jt,++j)
        {
            work=resolution(*it, *jt, id);
            if(work->isVide()) ///Si on a une clause si, c'est fini et on interromp avec une exception.
            {
                for(l=0; l<58; ++l) ///Gestion du cas particulier
                    printf("\b");
                if(i*100/pos.size()==100)
                    printf("\b");
                printf("\n");
                throw 1;
            }
            if(!work->isTautologie()&& !sortie->aSousclauses(work) && !sortie->contient(work) ) ** C'est là  que c'est un peu fin.
                                                                                                 On ne prend pas les tautologies, les surclauses de clauses déja existentes et les doublons.
                                                                                                 **
            {
                sortie->supprimer_surclauses(work); /// On enlève toutes les surclauses qui sont nécessairement vérifiées.
                sortie->addClause(work);
            }
        }
        for(l=0; l<58; ++l) ///Suite de l'affichage
            printf("\b");
        if(i*100/pos.size()==100)
            printf("\b");
    }
    printf("c ["); ///Et fin de la magnifique interface utilisateur !
    int l=0;
    for(; l<50; ++l)
        printf("#");
    printf("] 100%%\n");
    
    return sortie;
}
 
 void Formule::fusionner(const Formule* e, vector<Formule*> seaux) const ///Ajoute les clauses d'une formule dans les bons seaux
 {
 unordered_set<Clause*> t=e->getClauses();
 
 for(Clause* c:t)
 seaux[c->indiceMax()-1]->addClause(c);
 }
 
 void Formule::chercher_assignation(Formule* f, int id) ///On essaie avec l'un et si ça ne marche pas, on prend l'autre...
 {
 vars[id]->setVal(true);
 if(f->eval()!=1)
 vars[id]->setVal(false);
 }*
 
 
 Clause* Formule::resolution(const Clause* c1, Clause* c2, const int id) const
 {
 Clause* sortie = new Clause(*c1);
 sortie->fusionner(c2);
 sortie->supprimer(lits_pos[id-1]);
 sortie->supprimer(lits_neg[id-1]);
 return sortie;
 }*/
