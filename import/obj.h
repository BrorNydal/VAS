#ifndef OBJ_H
#define OBJ_H

#include "vertex.h"
#include "vec2.h"
#include "vec3.h"

#include <vector>

class OBJ
{
private:
    std::string mTag;
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;

public:
    OBJ();
    bool readOBJ(std::string fileName);

    /**
     * Number of vertices in obj-file.
     * @return returns number of vertices in this obj-file
     */
    unsigned int vertexCount() const;
    /**
     * Number of indices in obj-file.
     * @return returns the number of indices in this obj-file
     */
    unsigned int indexCount() const;

    bool operator == (const OBJ &obj) const;
    bool operator != (const OBJ &obj) const;

    //setters and getters

    /**
     * Get tag of obj.
     * @return returns tag of obj.
     */
    std::string getTag() const;
    /**
     * Gets the date for the vertex vector.
     * @return returns pointer to first element in vertex vector
     */
    Vertex *getVertexData();
    /**
     *
     * @return
     */
    unsigned int *getIndexData();

};

#endif // OBJ_H
