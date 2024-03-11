#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

// Function to handle key press events
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static bool keyPressed = false;
    if (nCode == HC_ACTION && !keyPressed)
    {
        keyPressed = true;

        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
        DWORD keyCode = kbdStruct->vkCode;
        char key = MapVirtualKey(keyCode, MAPVK_VK_TO_CHAR);

        static std::string typedInput;

        std::cout << "Key : " << key << std::endl;

        // Check if the key is a printable character
        if ((key >= 32 && key <= 126) || key == '\r')
        {
            // Append the key to the typed input
            typedInput += key;

            // List of words to detect
            std::vector<std::string> wordsToDetect = {"PORN", "SEX", "NUDE"};

            // Check if any of the words to detect are present in the typed input
            for (const auto &word : wordsToDetect)
            {
                if (typedInput.find(word) != std::string::npos)
                {
                    std::cout << "Detected word: " << word << std::endl;
                    // Open a webpage
                    ShellExecute(NULL, "open", "https://www.google.com", NULL, NULL, SW_SHOWNORMAL);

                    // Disable mouse and keyboard input
                    BlockInput(TRUE);

                    typedInput.clear(); // Clear the typed input
                    break;              // Exit loop once a word is detected
                }
            }
        }
    }
    else
    {
        keyPressed = false;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
    // Install keyboard hook
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook and exit
    UnhookWindowsHookEx(hook);
    return 0;
}
