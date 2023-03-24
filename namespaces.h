#ifndef NAMESPACES_H
#define NAMESPACES_H

#include <utility>

namespace details {

struct coords {

    float x;
    float y;
    float z;
};
}

namespace sampletypes {

typedef details::coords vector3d;
typedef std::pair<float, float> vector2d;
typedef int* buffer;
typedef void openGLfunctions;

struct shader_program;

struct quaternion {

    vector3d xyz;

    float angle;
};

struct matrix4x4 {
    bool matrix[4][4];
};

struct vertex_data {

    vector3d position;
    vector3d normal;

    vector2d texCoords;

    vector3d tangent;
    vector3d bitangent;
};

struct material {

    vector3d ambience_color;
    vector3d diffuse_color;
    vector3d specular_color;

    float shinnes;

    char* path_to_diffusemap;
    char* path_to_normalmap;
};

}


#endif // NAMESPACES_H
