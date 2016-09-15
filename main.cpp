#include <irrlicht.h>
#include <driverChoice.h>

#include "MyReceiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

extern int main(int argc, char** argv)
{

        video::E_DRIVER_TYPE driverType=driverChoiceConsole();
        if (driverType==video::EDT_COUNT)  return 1;

        IrrlichtDevice *device = createDevice(driverType, core::dimension2d< u32 >(640, 480), 16, false);
        if (device == 0) return 1;

        IVideoDriver* driver = device->getVideoDriver();
        ISceneManager* smgr = device->getSceneManager();

        ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(0,40,0), vector3df(0,0,0));

        MyReceiver rec;
        device->setEventReceiver(&rec);

        IMeshSceneNode *cube = smgr->addCubeSceneNode();
        cube->setMaterialFlag(video::EMF_LIGHTING, false);
        cube->setPosition(vector3df(0, 0, 0));
        cube->setRotation(vector3df(150, 50, 50));
        cube->setMaterialTexture(0, driver->getTexture("textures/cube.png"));

        IMeshSceneNode *sphere = smgr->addSphereSceneNode(5, 16, cube);
        sphere->setMaterialFlag(video::EMF_LIGHTING, false);
        sphere->setPosition(vector3df(10, 10, 10));
        sphere->setMaterialTexture(0, driver->getTexture("textures/cube.png"));
        sphere->setScale(vector3df(0.5, 0.5, 0.5));
        double angle = 0;

        while(device->run())
        if (device->isWindowActive())
        {
                sphere->setPosition(vector3df(10*2*cos(angle)/sqrt(2), 10, 10*2*sin(angle)/sqrt(2)));
                angle += 3.14159265/360;

                if(rec.IsKeyDown(KEY_UP)) cube->setPosition(cube->getPosition() + vector3df(0.1,0,0));
                if(rec.IsKeyDown(KEY_DOWN)) cube->setPosition(cube->getPosition() + vector3df(-0.1,0,0));
                if(rec.IsKeyDown(KEY_LEFT)) cube->setPosition(cube->getPosition() + vector3df(0,0,0.1));
                if(rec.IsKeyDown(KEY_RIGHT)) cube->setPosition(cube->getPosition() + vector3df(0,0,-0.1));

                camera->setTarget(cube->getPosition());
                driver->beginScene(true, true, SColor(0,3,243,250));
                smgr->drawAll();
                driver->endScene();
        }

        device->drop();

        return 0;
}
