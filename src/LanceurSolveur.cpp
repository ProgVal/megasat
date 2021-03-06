#include "../include/LanceurSolveur.h"
#include "../include/VariableNonAssigneeProvider.h"
#include "../include/DavisPutnamSolveur.h"
#include "../include/DPLLSolveur.h"
#include "../include/DPLLSurveilleSolveur.h"
#include "../include/MinisatSolveur.h"
#include "../include/ErreurResolutionException.h"

using namespace std;

LanceurSolveur::LanceurSolveur(ArgumentsParser& arguments_, string debutCommentaire_, SolveurType solveurParDefaut_, HeuristiqueType heuristiqueParDefaut_) : arguments(arguments_), debutCommentaire(debutCommentaire_), solveurParDefaut(solveurParDefaut_), heuristiqueParDefaut(heuristiqueParDefaut_)
{}

LanceurSolveur::~LanceurSolveur()
{}

Formule LanceurSolveur::execute(Formule& formule, TheorieGreffon& theorieGreffon)
{
    VariableNonAssigneeProvider* heuristique = nullptr;
    switch(getHeuristique())
    {
        case SIMPLE:
            heuristique = new VariableNonAssigneeProviderSimple();
            break;
        case RAND:
            heuristique = new VariableNonAssigneeProviderRand();
            break;
        case MALIN:
            heuristique = new VariableNonAssigneeProviderMalin();
            break;
        case MOMS:
            heuristique = new VariableNonAssigneeProviderMOMS();
            break;
        case DLIS:
            heuristique = new VariableNonAssigneeProviderDLIS();
            break;
        case DLISVARIANTE:
            heuristique = new VariableNonAssigneeProviderDLISVariante();
            break;
    }

    GestionConflits* gestionConflits = nullptr;
    if(arguments.getOption("cl-interac"))
    {
        if(arguments.getOption("v"))
            cout << debutCommentaire << " Utilisation du clause learning interactif." << endl;
        gestionConflits = new GestionConflitsApprentissage(1);
    }
    else if(arguments.getOption("cl"))
    {
        if(arguments.getOption("v"))
            cout << debutCommentaire << " Utilisation du clause learning." << endl;
        gestionConflits = new GestionConflitsApprentissage();
    }
    else
    {
        gestionConflits = new GestionConflits();
    }

    Solveur* solveur = nullptr;
    switch(getSolveur())
    {
        case DPLL:
            solveur = new DPLLSolveur(formule, *heuristique, *gestionConflits, theorieGreffon);
            break;
        case WATCHED_LITERALS:
            solveur = new DPLLSurveilleSolveur(formule, *heuristique, *gestionConflits, theorieGreffon);
            break;
        case DAVIS_PUTNAM:
            solveur = new DavisPutnamSolveur(formule);
            break;
        case MINISAT:
            solveur = new MinisatSolveur(formule);
            break;
    }

    if(solveur->isSatifiable())
    {
        formule = solveur->getFomule();
        if(arguments.getOption("v"))
            gestionConflits->afficheStatistiques(getBufferSortie(), debutCommentaire);
        if(arguments.getOption("c"))
        {
            if(!formule.isConsistant())
            {
                if(arguments.getOption("v"))
                    cout << debutCommentaire << " La résolution est inconsistante." << endl;
                throw ErreurResolutionException();
            }
            else if(arguments.getOption("v"))
                cout << debutCommentaire << " La résolution est consistante." << endl;
        }

        delete solveur;
        delete heuristique;
        delete gestionConflits;
        return formule;
    }
    else
    {
        if(arguments.getOption("v"))
            gestionConflits->afficheStatistiques(getBufferSortie(), debutCommentaire);
        delete solveur;
        delete heuristique;
        delete gestionConflits;
        throw InsatisfiableException();
    }
}

SolveurType LanceurSolveur::getSolveur()
{
    SolveurType solveur = solveurParDefaut;

    if(arguments.getOption("dpll"))
    {
        solveur = DPLL;
    }
    else if(arguments.getOption("wl"))
    {
        solveur = WATCHED_LITERALS;
    }
    else if(arguments.getOption("dp"))
    {
        solveur = DAVIS_PUTNAM;
    }
    else if(arguments.getOption("rapide"))
    {
        solveur = MINISAT;
    }

    if(arguments.getOption("v"))
    {
        ostream out(getBufferSortie());

        switch(solveur)
        {
            case DPLL:
                out << debutCommentaire << " Utilisation de DPLL." << endl;
                break;
            case WATCHED_LITERALS:
                out << debutCommentaire << " Utilisation des Watched Literals." << endl;
                break;
            case DAVIS_PUTNAM:
                out << debutCommentaire << " Utilisation de Davis Putnam." << endl;
                break;
            case MINISAT:
                out << debutCommentaire << " Utilisation d'un solveur maison." << endl;
        }
    }

    return solveur;
}

HeuristiqueType LanceurSolveur::getHeuristique()
{
    HeuristiqueType heuristique = heuristiqueParDefaut;

    if(arguments.getOption("simple"))
    {
        heuristique = SIMPLE;
    }
    if(arguments.getOption("rand"))
    {
        heuristique = RAND;
    }
    else if(arguments.getOption("malin"))
    {
        heuristique = MALIN;
    }
    else if(arguments.getOption("moms"))
    {
        heuristique = MOMS;
    }
    else if(arguments.getOption("dlis"))
    {
        heuristique = DLIS;
    }
    else if(arguments.getOption("dlisVariante"))
    {
        heuristique = DLISVARIANTE;
    }

    if(arguments.getOption("v"))
    {
        ostream out(getBufferSortie());

        switch(heuristique)
        {
            case SIMPLE:
                out << debutCommentaire << " Choix des variables non assignées par défaut." << endl;
                break;
            case RAND:
                out << debutCommentaire << " Choix des variables non assignées de manière aléatoire." << endl;
                break;
            case MALIN:
                out << debutCommentaire << " Choix des variables non assignées suivant leur fréquence d'apparition." << endl;
                break;
            case MOMS:
                out << debutCommentaire << " Choix des variables non assignées avec l'heuristique MOMS." << endl;
                break;
            case DLIS:
                out << debutCommentaire << " Choix des variables non assignées avec l'heuristique DLIS." << endl;
                break;
            case DLISVARIANTE:
                out << debutCommentaire << " Choix des variables non assignées avec la variante de l'heuristique DLIS." << endl;
                break;
        }
    }

    return heuristique;
}

streambuf* LanceurSolveur::getBufferSortie()
{
    return cout.rdbuf();
}

vector<string> LanceurSolveur::getNomsOptions()
{
    vector<string> liste(15);
    liste[0]="dpll";
    liste[1]="wl";
    liste[2]="dp";
    liste[3]="simple";
    liste[4]="rand";
    liste[5]="malin";
    liste[6]="moms";
    liste[7]="dlis";
    liste[8]="dlisVariante";
    liste[9]="v";
    liste[10]="s";
    liste[11]="cl";
    liste[12]="cl-interac";
    liste[13]="rapide";
    liste[14]="c";
    return liste;
}
