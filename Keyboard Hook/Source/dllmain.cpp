// Entry point for the DLL application
#include <Windows.h>
#include "../Header/KeyboardHook.h"
#include "../Header/Error.h"

using namespace std;

// The main driver for the keyboard hook
int __stdcall Main(HMODULE hModule)
{
   // The one and only... keyboard hook
   KeyboardHook hook;
   try {

      // Attach the hook
      if (!hook.setHook())
         throw Error{ 10, "Could not set hook." };

      // cout << "Hook attached." << endl;
      MessageBox(NULL, TEXT("Keyboard hook attached."), TEXT("Key Consumer"), MB_ICONINFORMATION | MB_TOPMOST | MB_SYSTEMMODAL);

      // Start up the message handler
      MessageHandler handler;
      handler.handle();

      // Remove the hook completely
      if (!hook.removeHook())
         throw Error{ 20, "Could not remove hook." };

      // cout << "Hook removed." << endl;
      // MessageBox(NULL, TEXT("Keyboard hook removed."), TEXT("Key Consumer"), MB_ICONINFORMATION | MB_TOPMOST);
   }
   // Print error message
   catch (const Error error)
   {
      cout << error << endl;
   }

   // Free the library and exit the thread
   FreeLibraryAndExitThread(hModule, NULL);
   return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
   HANDLE hThread;

   // Why are we here?
   switch (fdwReason)
   {
      // Just attached the DLL, start up the hook with a worker thread
      case DLL_PROCESS_ATTACH:
         hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, 0);
         break;
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
      case DLL_PROCESS_DETACH:
         break;
   }

   return TRUE;
}

