#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "main.h"


class Employee
{
public:
    Employee();
    //void set_id();
    void set_name();
    void set_post();
    void set_director();
private:
    //int id;
    QString name;
    QString post;
    int director;

};

#endif // EMPLOYEE_H
