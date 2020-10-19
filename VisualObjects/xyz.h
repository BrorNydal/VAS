#ifndef XYZ_H
#define XYZ_H

#include "VisualObjects/visualobject.h"
#include <vector>

class XYZ : public VisualObject
{
private:
    float mLineLength;
public:
   XYZ();
   ~XYZ() override;

   virtual void init() override;
   virtual void draw() override;
};

#endif // XYZ_H
