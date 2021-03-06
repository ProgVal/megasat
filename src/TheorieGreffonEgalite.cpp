#include "../include/TheorieGreffonEgalite.h"
#include<climits>

using namespace std;

typedef vector<vector<vector<pair<unsigned int,int>>>> type_adjacence;

TheorieGreffonEgalite::TheorieGreffonEgalite() :
atomes(vector<AtomeEgalite>()), literalPerAtome(unordered_map<AtomeEgalite,int>()), egalite(vector<unsigned int>(0)), poids(vector<unsigned int>(0)), historiqueFusions(vector<vector<unsigned int>>(0)), causesFusions(unordered_map<AtomeEgalite,int>()), differencesParNiveau(vector<vector<AtomeEgalite>>()), varIdMax(0)
{}

void TheorieGreffonEgalite::setCorrespondanceAtomes(const std::vector<AtomeEgalite>& corr) {
    atomes = corr;

    for(unsigned int i = 0; i < atomes.size(); i++)
        literalPerAtome[atomes[i]] = static_cast<int>(i) + 1;

    varIdMax = 0;
    for(const AtomeEgalite& atome : atomes) {
        if(atome.getI() > varIdMax)
            varIdMax = atome.getI();
        if(atome.getJ() > varIdMax)
            varIdMax = atome.getJ();
    }

    egalite = vector<unsigned int>(varIdMax + 1);
    for(unsigned int i = 0; i <= varIdMax; i++) {
        egalite[i] = i;
    }
    poids = vector<unsigned int>(varIdMax + 1, 1);
    historiqueFusions = vector<vector<unsigned int>>();
    differencesParNiveau = vector<std::vector<AtomeEgalite>>();
    causesFusions = unordered_map<AtomeEgalite,int>();
}

vector<int> TheorieGreffonEgalite::onAssignation(int id, unsigned int niveau)
{
    TheorieGreffonSimple::onAssignation(id, niveau);
    if(static_cast<unsigned int>(abs(id)) > atomes.size())
        return vector<int>(0);

    if(niveau >= historiqueFusions.size())
    {
        historiqueFusions.resize(niveau + 1);
        differencesParNiveau.resize(niveau + 1);
    }

    AtomeEgalite atomeAssigne = atomes[static_cast<unsigned int>(abs(id)) - 1];
    unsigned int representantI = findCC(atomeAssigne.getI());
    unsigned int representantJ = findCC(atomeAssigne.getJ());
    if(id > 0)
    {
        vector<int> erreur = testeSiUnionCCPossible(atomeAssigne.getI(), atomeAssigne.getJ(), representantI, representantJ);
        if(!erreur.empty())
            return erreur;

        if(representantI != representantJ)
        {
            causesFusions[AtomeEgalite(representantI, representantJ)] = id;
#ifdef DEBUG
cout << "fusion de  x" << representantI << " avec x" << representantJ << " au niveau " << niveau << endl;
#endif
            if(poids[representantI] < poids[representantJ])
            {
                egalite[representantI] = representantJ;
                poids[representantJ] += poids[representantI];
                historiqueFusions[niveau].push_back(representantI);
            }
            else
            {
                egalite[representantJ] = representantI;
                poids[representantI] += poids[representantJ];
                historiqueFusions[niveau].push_back(representantJ);
            }
        }
    }
    else
    {
        differencesParNiveau[niveau].push_back(atomeAssigne);
        if(representantI == representantJ)
        {
            vector<int> chaine = construitChaineEgalites(atomeAssigne.getI());
            vector<int> chaine2 = construitChaineEgalites(atomeAssigne.getJ());
            chaine.insert(chaine.end(), chaine2.begin(), chaine2.end());
            chaine.push_back(id);
            return chaine;
        }
    }

    return vector<int>(0);
}

void TheorieGreffonEgalite::unionCC(unsigned int i, unsigned int j)
{
    if(poids[i] < poids[j])
    {
        egalite[i] = j;
        poids[j] += poids[i];
    }
    else
    {
        egalite[j] = i;
        poids[i] += poids[j];
    }
}

unsigned int TheorieGreffonEgalite::findCC(unsigned int i) const
{
    while(egalite[i] != i)
        i = egalite[i];
    return i;
}

vector<int> TheorieGreffonEgalite::construitChaineEgalites(unsigned int i)
{
    vector<int> chaine;
    while(egalite[i] != i)
    {
        chaine.push_back(causesFusions[AtomeEgalite(i, egalite[i])]);
        i = egalite[i];
    }

    return chaine;
}

std::vector<int> TheorieGreffonEgalite::testeSiUnionCCPossible(unsigned int i, unsigned int j, unsigned int repFI, unsigned int repFJ)
{
    for(auto& atomeBag : differencesParNiveau)
        for(AtomeEgalite& atomeP : atomeBag)
        {
            unsigned int repI = findCC(atomeP.getI());
            unsigned int repJ = findCC(atomeP.getJ());
            if((repI == repFI && repJ == repFJ) || (repI == repFJ && repJ == repFI))
            {
                vector<int> chaine1 = construitChaineEgalites(i);
                vector<int> chaine2 = construitChaineEgalites(j);
                vector<int> chaine3 = construitChaineEgalites(atomeP.getI());
                vector<int> chaine4 = construitChaineEgalites(atomeP.getJ());
                vector<int> chaine;
                chaine.reserve(chaine1.size() + chaine2.size() + chaine3.size() + chaine4.size() + 2);
                chaine.insert(chaine.end(), chaine1.begin(), chaine1.end());
                chaine.insert(chaine.end(), chaine2.begin(), chaine2.end());
                chaine.insert(chaine.end(), chaine3.begin(), chaine3.end());
                chaine.insert(chaine.end(), chaine4.begin(), chaine4.end());
                chaine.push_back(- literalPerAtome[atomeP]);
                chaine.push_back(literalPerAtome[AtomeEgalite(i, j)]);
                return chaine;
            }
        }
    return vector<int>(0);
}

void TheorieGreffonEgalite::onBacktrack(unsigned int l)
{
    TheorieGreffonSimple::onBacktrack(l);

    unsigned long niveau = historiqueFusions.size();
    while(niveau > l)
    {
        niveau--;
        for(auto iter = historiqueFusions[niveau].rbegin(); iter != historiqueFusions[niveau].rend(); iter++)
        {
            unsigned int merge = *iter;
            unsigned int other = egalite[merge];
            poids[other] -= poids[merge];
            egalite[merge] = merge;
#ifdef DEBUG
            cout << "suppression de la fusion de x" << merge << " avec x" << other << endl;
#endif
        }
    }
    if(historiqueFusions.size() >= l)
        historiqueFusions.erase(historiqueFusions.begin() + static_cast<int>(l), historiqueFusions.end());
    if(differencesParNiveau.size() >= l)
        differencesParNiveau.erase(differencesParNiveau.begin() + static_cast<int>(l), differencesParNiveau.end());
}

pair<vector<AtomeEgalite>,vector<AtomeEgalite>> TheorieGreffonEgalite::getEtatCourant() const
{
    vector<AtomeEgalite> egalites;
    for(unsigned int i = 0; i <= varIdMax; i++) {
        if(egalite[i] != i)
            egalites.push_back(AtomeEgalite(i, findCC(i)));
    }

    vector<AtomeEgalite> inegalites;
    for(auto& atomeBag : differencesParNiveau)
        for(const AtomeEgalite& atome : atomeBag)
            inegalites.push_back(AtomeEgalite(findCC(atome.getI()), findCC(atome.getJ())));

    return pair<vector<AtomeEgalite>,vector<AtomeEgalite>>(egalites, inegalites);
}

vector<int> TheorieGreffonEgalite::getTPropagations(unsigned int niveau)
{
    vector<int> literauxAAssigner;
    for(unsigned int i = 0; i < atomes.size(); ++i)
    {
        if(valVariables[i] == INCONNU)
        {
            unsigned int representantI = findCC(atomes[i].getI());
            unsigned int representantJ = findCC(atomes[i].getJ());
            if(representantI == representantJ)
            {
                literauxAAssigner.push_back(static_cast<int>(i) + 1);
                TheorieGreffonSimple::onAssignation(static_cast<int>(i) + 1, niveau);
            }
        }
    }
    return literauxAAssigner;
}
