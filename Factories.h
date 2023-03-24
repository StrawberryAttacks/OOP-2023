#ifndef FACTORIES_H
#define FACTORIES_H

#include "Entities.h"

class GraphicsPipeline {
public:
    virtual WorldEngineObject* pullout() = 0;
};

class SkyBoxFactory : public GraphicsPipeline {
public:
    WorldEngineObject* pullout() override {
        return new skybox();
    }
};

class ViewCameraFactory : public GraphicsPipeline {
public:
    WorldEngineObject* pullout() override {
        return new camera();
    }
};

class SceneObjectsFactory : public GraphicsPipeline {
public:
    WorldEngineObject* pullout() override {
        return new object3D();
    }
};
#endif // FACTORIES_H
