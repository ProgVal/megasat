#include "../include/GraphvizOutput.h"
#include<sstream>

using namespace std;

GraphvizOutput::GraphvizOutput(Graphe& graphe_, Formule& formule_, map<string, int> correspondances_, int k_) :
graphe(graphe_), formule(formule_), k(k_), tailleCodeCouleurSommet(static_cast<int>(ceil(log2(k_)))), correspondances(correspondances_)
{}

void GraphvizOutput::affiche(std::streambuf* sortie, bool avecColoriage)
{
    ostream out(sortie);
    out << "graph G {\n";

    for(Arete arete : graphe.getAretes())
        out << arete.getExtremiteGauche() + 1 << " -- " << arete.getExtremiteDroite() + 1 << ";\n";

    if(avecColoriage)
    {
        for(int sommet = 0; sommet < graphe.getSommetNumber(); sommet++)
        {
            out << sommet + 1 << " [color=\"" << (static_cast<double>(getCouleur(sommet))) / k << ",0.5,1\",style=filled];\n";
        }
    }

    out << "}\n";
}

int GraphvizOutput::getCouleur(int sommet)
{
    int couleur = 0;
    for(int bit = 0; bit < tailleCodeCouleurSommet; bit++)
    {
        couleur *= 2;
        ostringstream os;
        os << sommet << '-' << bit;
        int varId = correspondances[os.str()];
        //on met la variable a 0 si elle n'existe pas (pas de contrainte dessus et 0 permet de rester en dessous de k)
        if(varId != 0 && formule.getVar(varId)->getVal())
            couleur += 1;
    }

    return couleur;
}
