#include <irrlicht.h>
#include <driverChoice.h>

#include "MyReceiver.h"

using namespace irr;

extern int main(int argc, char** argv)
{

    video::E_DRIVER_TYPE driverType=driverChoiceConsole();
    if (driverType==video::EDT_COUNT)  return 1;

    IrrlichtDevice *device = createDevice(driverType, core::dimension2d< u32 >(640, 480), 16, false);
    if (device == 0) return 1;
    device->setWindowCaption(L"Der'mo");

    video::IVideoDriver *driver = device->getVideoDriver();
    scene::ISceneManager *smgr = device->getSceneManager();
    gui::IGUIEnvironment *guienv = device->getGUIEnvironment();
    gui::IGUIStaticText *staticText = guienv->addStaticText(L"Hello Cruel World! Fuck you...", core::rect<s32>(10, 10, 300, 22), true);
    io::IFileSystem *fs = device->getFileSystem();

    // Load quake map
    fs->addFileArchive("media/maps/map-20kdm2.pk3");
    scene::IAnimatedMesh *mesh = smgr->getMesh("20kdm2.bsp");
    scene::ISceneNode *node = nullptr;
    if (mesh)
        node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
    if (node)
        node->setPosition(core::vector3df(-1400, -110, -1400));

    // Add event receiver
    MyReceiver rec;
    device->setEventReceiver(&rec);

    // Add mesh
    scene::IAnimatedMesh *mesh2 = smgr->getMesh("media/models/syndey.md2");
    if (!mesh) {
        device->drop();
        return 1;
    }
    // Add node and attach the mesh to it
    scene::IAnimatedMeshSceneNode *node2 = smgr->addAnimatedMeshSceneNode(mesh2);
    if (node2) {
        //node2->setMaterialFlag(video::EMF_LIGHTING, false);
        node2->setMD2Animation(scene::EMAT_STAND);
        node2->setMaterialTexture(0, driver->getTexture("media/textures/syndey.bmp"));
        node2->setScale(core::vector3df(1.8, 1.8, 1.8));
    }

    // Add camera
    scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS();
    camera->setPosition(core::vector3df(0, 0, 30));

    // Hide cursor
    device->getCursorControl()->setVisible(false);

    scene::ILightSceneNode *light = smgr->addLightSceneNode(0, core::vector3df(50.0f,10.0f,0.0f), video::SColorf(1.0f,1.0f,1.0f,1.0f), 500.0f );

    int lastFPS = -1;
    while(device->run()) {
        if (device->isWindowActive()) {
            if(rec.IsKeyDown(KEY_KEY_W)) camera->setPosition(camera->getPosition() + core::vector3df(0.1,0,0));
            if(rec.IsKeyDown(KEY_KEY_S)) camera->setPosition(camera->getPosition() + core::vector3df(-0.1,0,0));
            if(rec.IsKeyDown(KEY_KEY_A)) camera->setPosition(camera->getPosition() + core::vector3df(0,0,0.1));
            if(rec.IsKeyDown(KEY_KEY_D)) camera->setPosition(camera->getPosition() + core::vector3df(0,0,-0.1));

            int fps = driver->getFPS();
            if (lastFPS != fps)
            {
                core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
                str += driver->getName();
                str += "] FPS:";
                str += fps;

                device->setWindowCaption(str.c_str());
                lastFPS = fps;
                staticText->setText(str.c_str());
            }

            driver->beginScene(true, true, video::SColor(0,3,243,250));
            smgr->drawAll();
            guienv->drawAll();
            driver->endScene();
        } else
            device->yield();
    }

    device->drop();


    return 0;
}
