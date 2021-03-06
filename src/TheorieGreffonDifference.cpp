#include "../include/TheorieGreffonDifference.h"
#include<climits>
#include<stack>

using namespace std;

typedef vector<vector<vector<pair<unsigned int,int>>>> type_adjacence;

TheorieGreffonDifference::TheorieGreffonDifference() :
atomes(vector<AtomeDifference>()), literalPerAtome(unordered_map<AtomeDifference,int>()), adjacence(type_adjacence(0)), varIdMax(0)
{}

void TheorieGreffonDifference::setCorrespondanceAtomes(const vector<AtomeDifference>& corr)
{
    atomes = corr;

    for(unsigned int i = 0; i < atomes.size(); i++)
    {
        AtomeDifference atome = atomes[i];
        literalPerAtome[atome] = static_cast<int>(i) + 1;
        literalPerAtome[AtomeDifference(atome.getJ(), atome.getI(), -atome.getN() - 1)] = -static_cast<int>(i) - 1;
    }

    varIdMax = 0;
    for(AtomeDifference atome : atomes) {
        if(atome.getI() > varIdMax)
            varIdMax = atome.getI();
        if(atome.getJ() > varIdMax)
            varIdMax = atome.getJ();
    }

    adjacence = type_adjacence(varIdMax + 1, vector<vector<pair<unsigned int,int>>>());
}

vector<int> TheorieGreffonDifference::onAssignation(int id, unsigned int niveau)
{
    TheorieGreffonSimple::onAssignation(id, niveau);
    if(static_cast<unsigned int>(abs(id)) > atomes.size())
        return vector<int>(0);

    if(niveau >= adjacence[0].size())
    {
        for(unsigned long i = 0; i <= varIdMax; i++)
            adjacence[i].resize(niveau + 1);
    }

    AtomeDifference atomeAssigne = atomes[static_cast<unsigned int>(abs(id)) - 1];
    if(id > 0)
    {
        adjacence[atomeAssigne.getI()][niveau].push_back(pair<unsigned int,int>(atomeAssigne.getJ(), atomeAssigne.getN()));
#ifdef DEBUG
        cout << "c ajout de x" << atomeAssigne.getI() - 1 << " - x" << atomeAssigne.getJ() - 1 << " <= " << atomeAssigne.getN() << " au niveau " << niveau << endl;
#endif
    }
    else //si non(xi - xj <= n) -> xi - xj > n -> xj - xi < -n -> xj - xi <= -n -1
    {
        adjacence[atomeAssigne.getJ()][niveau].push_back(pair<unsigned int,int>(atomeAssigne.getI(), -atomeAssigne.getN() - 1));
#ifdef DEBUG
        cout << "c ajout de x" << atomeAssigne.getI() - 1 << " - x" << atomeAssigne.getJ() - 1 << " > " << atomeAssigne.getN() << " au niveau " << niveau << endl;
#endif
    }

    vector<AtomeDifference> cycle = testePresenceCycleDePoidsNegatif(atomeAssigne.getI());

    vector<int> clauseAAjouter(cycle.size());
    for(unsigned int i = 0; i < cycle.size(); i++)
    {
        clauseAAjouter[i] = -literalPerAtome[cycle[i]];
    }

    return clauseAAjouter;
}


vector<AtomeDifference> TheorieGreffonDifference::testePresenceCycleDePoidsNegatif(unsigned int depart)
{
    vector<int> poids(varIdMax + 1, INT_MAX);
    poids[depart] = 0;
    vector<pair<unsigned int,int>> pere(varIdMax + 1);
    for(unsigned int sommet = 0; sommet < varIdMax + 1; ++sommet)
        pere[sommet] = pair<unsigned int,int>(sommet, 0);

    for(unsigned int i = 1; i <= varIdMax + 1; i++)
    {
        bool nothingChanged = true;
        for(unsigned int sommet = 0; sommet <= varIdMax; sommet++)
        {
            if(poids[sommet] == INT_MAX)
                continue;
            for(auto& sacNiveau : adjacence[sommet])
                for(const pair<unsigned int,int>& arete : sacNiveau)
                {
                    int nouveauPoids = poids[sommet] + arete.second;
                    if(nouveauPoids < poids[arete.first])
                    {
                        poids[arete.first] = nouveauPoids;
                        nothingChanged = false;
                        pere[arete.first] = pair<unsigned int,int>(sommet, arete.second);
                    }
                }
        }
        if(nothingChanged)
            return vector<AtomeDifference>(0);
    }

    //Recherche de l'existance de cycles de poids negatif via un parcours en profondeur sur les parents
    enum COLOR { BLANC, GRIS, NOIR };
    vector<COLOR> coloriage(varIdMax + 1, BLANC);

    for(unsigned int sommetDepart = 0; sommetDepart <= varIdMax; sommetDepart++)
    {
        if(coloriage[sommetDepart] != BLANC)
            continue;
        stack<pair<unsigned int,int>> parcourus;
        parcourus.push(pair<unsigned int,int>(sommetDepart, 0));
        unsigned int prochainSommet = sommetDepart;
        while(true)
        {
            unsigned int sommet = prochainSommet;
            coloriage[sommet] = GRIS;
            pair<unsigned int, int> suivant = pere[sommet];
            parcourus.push(suivant);
            prochainSommet = suivant.first;
            if(prochainSommet == sommet || coloriage[prochainSommet] == NOIR)
                break;
            if(coloriage[prochainSommet] == GRIS)
            {
#ifdef DEBUG
                cout << "c Cycle de poids negatif :" << endl;
#endif
                vector<AtomeDifference> aretesProblemes;
                while(!parcourus.empty())
                {
                    pair<unsigned int, int> top = parcourus.top();
                    parcourus.pop();
                    if(!parcourus.empty())
                    {
                        aretesProblemes.push_back(AtomeDifference(top.first, parcourus.top().first, top.second));
#ifdef DEBUG
                        cout << "c    x" << top.first - 1 << " - x" << parcourus.top().first - 1 << " <= " << top.second << endl;
#endif
                    }
                }
                return aretesProblemes;
            }
        }
        while(!parcourus.empty())
        {
            pair<unsigned int, int> fait = parcourus.top();
            parcourus.pop();
            coloriage[fait.first] = NOIR;
        }
    }
    return vector<AtomeDifference>(0);
}

void TheorieGreffonDifference::onBacktrack(unsigned int l)
{
    TheorieGreffonSimple::onBacktrack(l);
    for(unsigned int i = 0; i <= varIdMax; i++)
        if(adjacence[i].size() >= l)
            adjacence[i].erase(adjacence[i].begin() + static_cast<int>(l), adjacence[i].end());
}

template<typename T> inline bool vector_in(vector<T>& vector, T& elem) __attribute__((pure));

template<typename T> inline bool vector_in(vector<T>& vector, T& elem)
{
    for(T& el : vector)
        if(el == elem)
            return true;
    return false;
}

pair<map<unsigned int,int>,vector<AtomeDifference>> TheorieGreffonDifference::getEtatCourant() const
{
    vector<AtomeDifference> differences;
    for(unsigned int sommet = 0; sommet <= varIdMax; sommet++)
        for(auto& sacNiveau : adjacence[sommet])
            for(const pair<unsigned int,int>& arete : sacNiveau)
                differences.push_back(AtomeDifference(sommet, arete.first, arete.second));

    map<unsigned int,int> valeurs;
    bool changes = true;
    while(changes)
    {
        changes = false;
        vector<AtomeDifference> differences2;
        for(AtomeDifference atome : differences)
        {
            if(valeurs.count(atome.getI()) > 0)
            {
                atome = AtomeDifference(0, atome.getJ(), atome.getN() - valeurs[atome.getI()]);
            }
            if(valeurs.count(atome.getJ()) > 0)
            {
                atome = AtomeDifference(atome.getI(), 0, atome.getN() + valeurs[atome.getJ()]);
            }
            if(atome.getI() == 0 && atome.getJ() == 0)
                continue;

            if(atome.getJ() == 0)
            {
                AtomeDifference atome2(0, atome.getI(), -atome.getN());
                if(vector_in<AtomeDifference>(differences, atome2))
                {
                    valeurs[atome.getI()] = atome.getN();
                    changes = true;
                    continue;
                }
            }
            else if(atome.getI() == 0)
            {
                AtomeDifference atome2(atome.getJ(), 0, -atome.getN());
                if(vector_in<AtomeDifference>(differences, atome2))
                {
                    valeurs[atome.getJ()] = -atome.getN();
                    changes = true;
                    continue;
                }
            }
            differences2.push_back(atome);
        }
        differences = differences2;
    }
    return pair<map<unsigned int,int>,vector<AtomeDifference>>(valeurs, differences);
}


map<unsigned int,int> TheorieGreffonDifference::getAssignation() const
{
    //init matrice d'adjacence
    vector<vector<int>> matriceAdjacence(varIdMax + 1, vector<int>(varIdMax + 1, INT_MAX));
    for(unsigned int sommet = 0; sommet <= varIdMax; sommet++)
    {
        int level = 0;
        for(auto& sacNiveau : adjacence[sommet])
        {
            for(const pair<unsigned int,int>& arete : sacNiveau)
                if(arete.second < matriceAdjacence[sommet][arete.first])
                    matriceAdjacence[sommet][arete.first] = arete.second;
            level++;
        }
    }

    //Floyd-Warshall
    for(unsigned int k = 0; k <= varIdMax; k++)
        for(unsigned int i = 0; i <= varIdMax; i++)
            for(unsigned int j = 0; j <= varIdMax; j++)
            {
                if(matriceAdjacence[i][k] != INT_MAX && matriceAdjacence[k][j] != INT_MAX)
                {
                    int val = matriceAdjacence[i][k] + matriceAdjacence[k][j];
                    if(val < matriceAdjacence[i][j])
                        matriceAdjacence[i][j] = val;
                }
            }

    //Assignation
    map<unsigned int,int> valeurs;
    for(unsigned int sommetDepart = 0; sommetDepart <= varIdMax; sommetDepart++)
    {
        if(valeurs.count(sommetDepart) > 0)
            continue;
        valeurs[sommetDepart] = 0;

        stack<unsigned int> aParcourir;
        aParcourir.push(sommetDepart);
        while(!aParcourir.empty()) {
            unsigned int sommet = aParcourir.top();
            aParcourir.pop();
            for(unsigned int fils = 0; fils <= varIdMax; fils++)
            {
                if(valeurs.count(fils) > 0)
                    continue;
                if(matriceAdjacence[fils][sommet] != INT_MAX)
                { //fils - sommet = n -> fils = sommet + n
                    valeurs[fils] = valeurs[sommet] + matriceAdjacence[fils][sommet];
                    aParcourir.push(fils);
                }
                else if(matriceAdjacence[sommet][fils] != INT_MAX)
                { //sommet - fils = n -> fils = sommet - n
                    valeurs[fils] = valeurs[sommet] - matriceAdjacence[sommet][fils];
                    aParcourir.push(fils);
                }
            }
        }
    }

    valeurs.erase(0);
    return valeurs;
}
