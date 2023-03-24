#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include "namespaces.h"

class WorldEngineObject {
public:

    virtual void rotate(const sampletypes::quaternion &r) = 0;

    virtual void translate(const sampletypes::vector3d &t) = 0;

    virtual void scale(float s) = 0;

    virtual void setGlobalTransform(const sampletypes::matrix4x4 &q) = 0;

    virtual void draw(sampletypes::shader_program *program,
                      sampletypes::openGLfunctions *functions) = 0;
};

class object3D : public WorldEngineObject {

public:

    object3D();

    void rotate(const sampletypes::quaternion &r);

    void translate(const sampletypes::vector3d &t);

    void scale(float s);

    void setGlobalTransform(const sampletypes::matrix4x4 &q);

    void draw(sampletypes::shader_program *program,
              sampletypes::openGLfunctions *functions);

    void init(); //*this = builder.BuildMaterial().BuildFromBuffer();

    void loadObjectFromFile(char* path);
            //*this = builder.BuildMaterial().BuildFromFile()

    virtual ~object3D();

protected:

    void calculateTBN(const sampletypes::buffer &vertexdata);


private:

    sampletypes::buffer vertex_buff;
    sampletypes::buffer index_buff;

    sampletypes::quaternion m_rotate;
    sampletypes::vector3d m_translate;
    sampletypes::matrix4x4 m_globalTransforms;

    float m_scale;

};

class skybox : public WorldEngineObject {
public:

    skybox(); // box = builder.BuildMaterial().BuildFromBuffer();

    void rotate(const sampletypes::quaternion &r);

    void translate(const sampletypes::vector3d &t);

    void scale(float s);

    void setGlobalTransform(const sampletypes::matrix4x4 &q);

    void draw(sampletypes::shader_program *program,
              sampletypes::openGLfunctions *functions);

    virtual ~skybox();

private:

    typedef object3D block;

    block *box;
};

class camera : public WorldEngineObject {
public:

    void rotate(const sampletypes::quaternion &r);

    void translate(const sampletypes::vector3d &t);

    void scale(float s);

    void setGlobalTransform(const sampletypes::matrix4x4 &q);

    void draw(sampletypes::shader_program *program,
              sampletypes::openGLfunctions *functions);

    void rotateX(const sampletypes::quaternion &r);

    void rotateY(const sampletypes::quaternion &r);

    void addObject(WorldEngineObject* obj);

    const sampletypes::matrix4x4 &getViewMatrix() const;

    virtual ~camera();

protected:

    void updateViewMatrix();

private:

    sampletypes::quaternion m_rotate;
    sampletypes::quaternion m_rotateX;
    sampletypes::quaternion m_rotateY;

    sampletypes::vector3d m_translate;

    float m_scale;

    sampletypes::matrix4x4 m_globalTransforms;
    sampletypes::matrix4x4 viewMatrix;
};


#endif // ENTITIES_H
