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
    device->setWindowCaption(L"Der'mo");

    IVideoDriver *driver = device->getVideoDriver();
    ISceneManager *smgr = device->getSceneManager();
    IGUIEnvironment *guienv = device->getGUIEnvironment();
    guienv->addStaticText(L"Hello Cruel World! Fuck you...", rect<s32>(10, 10, 260, 22), true);

    MyReceiver rec;
    device->setEventReceiver(&rec);

    IAnimatedMesh *mesh = smgr->getMesh("media/models/syndey.md2");
    if (!mesh) {
        device->drop();
        return 1;
    }
    IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(mesh);
    if (node) {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMT_STAND);
        node->setMaterialTexture(0, driver->getTexture("media/textures/syndey.bmp"));
    }

    ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    while(device->run())
        if (device->isWindowActive()) {
            driver->beginScene(true, true, SColor(0,3,243,250));
            smgr->drawAll();
            guienv->drawAll();
            driver->endScene();
        }

    device->drop();


    return 0;
}
