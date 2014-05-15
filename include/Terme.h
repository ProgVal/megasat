#ifndef TERME_H
#define TERME_H

#include<string>
#include<vector>

class Terme
{
public:
    Terme();
    Terme(int variable);
    Terme(std::string f, const std::vector<Terme*>& arguments);
    Terme(const Terme& F);///Obligatoire pour pouvoir utiliser un attribut pointeur
    Terme& operator=(const Terme& F);///Idem
    ~Terme();
    bool isVariable() const {return parametres.empty();};
    bool isFonction() const {return !isVariable();};
    std::string getSymbole() const {return symbole;};
    int getVariable() const {return variable;};
    std::vector<Terme*> getParametres() const {return parametres;};
    void free();
    bool isConflit(Terme t) const;
    bool isConflitInsurmontable(Terme t) const;
    size_t hash() const;
    bool operator==(const Terme& that) const __attribute__((pure));
    std::string toString();

private:
    std::vector<Terme*> parametres;
    std::string symbole;
    int variable;
};
#endif // TERME_H
