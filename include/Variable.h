#ifndef VARIABLE_H
#define VARIABLE_H

#include<vector>

class Variable
{
public:
    Variable();
    Variable(int id_e);
    ~Variable();
    Variable(const Variable& other);
    int getId() const;
    bool isAssigned() const;
    bool getVal() const;
    void setVal(bool e);

private:
    int id;         /// Le num�ro de la variable
    bool assigned;  /// Si la variable a d�j� �t� assign�e
    bool val;       /// La valeur assign�e. a ne pas prendre en compte si assigned==false.
};

bool operator<(int const a, Variable const& b); /** Ok, un peu overkill mais c'était sympa de ne pas se poser la question en codant **/
bool operator<(Variable const &a, int const b); /** Tout ceci sert � avoir un ordre arbitraire sur les variables comme d�crit dans le sujet **/
bool operator<(Variable const &a, Variable const& b); /** En particulier pour ordonner les seaux. **/
bool operator>(int const a, Variable const& b);
bool operator>(Variable const &a, int const b);
bool operator>(Variable const &a, Variable const& b);
bool operator<=(int const a, Variable const& b);
bool operator<=(Variable const &a, int const b);
bool operator<=(Variable const &a, Variable const& b);
bool operator>=(int const a, Variable const& b);
bool operator>=(Variable const &a, int const b);
bool operator>=(Variable const &a, Variable const& b);
bool operator==(Variable const &a, Variable const& b);
bool operator==(int const a, Variable const& b);
bool operator==(Variable const &a, int const b);
bool operator!=(Variable const &a, Variable const& b);
bool operator!=(int const a, Variable const& b);
bool operator!=(Variable const &a, int const b);
#endif // VARIABLE_H
