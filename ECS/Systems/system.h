#ifndef SYSTEM_H
#define SYSTEM_H

#include <QOpenGLFunctions_4_1_Core>

/*
 * funksjoner som binder sammen components
 */
class System : public QOpenGLFunctions_4_1_Core
{
public:
    System();
};

#endif // SYSTEM_H
