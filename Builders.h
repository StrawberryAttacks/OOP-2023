#ifndef BUILDERS_H
#define BUILDERS_H

#include "Entities.h"
#include "namespaces.h"

class GraphicsBuilder {
public:

    virtual GraphicsBuilder* buildMaterial(char* path) = 0;

    virtual WorldEngineObject* buildFromBuffer(sampletypes::buffer vertex_buff,
                                 sampletypes::buffer indexes) = 0;

    virtual WorldEngineObject* buildFromFile(char* path) = 0;

    virtual ~GraphicsBuilder();
 };



class Object3dBuilder : public GraphicsBuilder {
public:

    GraphicsBuilder* buildMaterial(char* path) override;

    object3D* buildFromBuffer(sampletypes::buffer vertex_buff,
                         sampletypes::buffer indexes) override;

    object3D* buildFromFile(char* path) override;

    virtual ~Object3dBuilder();

private:
    sampletypes::material map;
};

#endif // BUILDERS_H
