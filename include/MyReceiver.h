#ifndef MYRECEIVER_H
#define MYRECEIVER_H

#include <irrlicht.h>

using namespace irr;

class MyReceiver : public IEventReceiver
{
public:
        MyReceiver();
        virtual bool OnEvent(const SEvent& event);
        virtual bool IsKeyDown(EKEY_CODE keyCode) const;

private:
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif // MYRECEIVER_H
