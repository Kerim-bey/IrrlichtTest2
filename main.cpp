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

    scene::ISceneNode *node3 = smgr->addSphereSceneNode();
    if (node3) {
        node3->setPosition(core::vector3df(0, 0, 30));
        node3->setMaterialTexture(0, driver->getTexture("media/textures/sphere.png"));
    }

    scene::ISceneNode *node4 = smgr->addCubeSceneNode(10.f, node3);
    if (node4) {
        node4->setMaterialTexture(0, driver->getTexture("media/textures/cube.png"));
        scene::ISceneNodeAnimator *anim = smgr->createFlyCircleAnimator(core::vector3df(0, 0, 0), 20);
        if (anim) {
            node4->addAnimator(anim);
            anim->drop();
        }
    }

    scene::IAnimatedMeshSceneNode *node5 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/models/ninja.b3d"));
    if (node5) {
        scene::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(core::vector3df(-100, -45, 60), core::vector3df(100, -45, 60),
            3500, true);
        if (anim) {
            node5->addAnimator(anim);
            anim->drop();
        }

        node5->setFrameLoop(0, 13);
        node5->setAnimationSpeed(15);

        node5->setScale(core::vector3df(10, 10, 10));
        node5->setRotation(core::vector3df(0, 90, 0));
    }

    // Add camera
    scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS();
    camera->setPosition(core::vector3df(0, 0, 30));

    // Hide cursor
    device->getCursorControl()->setVisible(false);

    scene::ILightSceneNode *light = smgr->addLightSceneNode(0, core::vector3df(50.0f,10.0f,0.0f), video::SColorf(1.0f,1.0f,1.0f,1.0f), 500.0f );

    int lastFPS = -1;
    u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = 30.f;
    while(device->run()) {
        if (device->isWindowActive()) {
            const u32 now = device->getTimer()->getTime();
            const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // time in seconds
            then = now;

            core::vector3df node3Pos = node3->getPosition();
            if(rec.IsKeyDown(KEY_KEY_W)) node3Pos.X += MOVEMENT_SPEED * frameDeltaTime;
            if(rec.IsKeyDown(KEY_KEY_S)) node3Pos.X -= MOVEMENT_SPEED * frameDeltaTime;
            if(rec.IsKeyDown(KEY_KEY_A)) node3Pos.Z += MOVEMENT_SPEED * frameDeltaTime;
            if(rec.IsKeyDown(KEY_KEY_D)) node3Pos.Z -= MOVEMENT_SPEED * frameDeltaTime;
            if(rec.IsKeyDown(KEY_SPACE)) node3Pos.Y += MOVEMENT_SPEED * frameDeltaTime;
            if(rec.IsKeyDown(KEY_LSHIFT)) node3Pos.Y -= MOVEMENT_SPEED * frameDeltaTime;
            node3->setPosition(node3Pos);

            int fps = driver->getFPS();
            if (lastFPS != fps)
            {
                core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
                str += driver->getName();
                str += "] FPS:";
                str += fps;

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
