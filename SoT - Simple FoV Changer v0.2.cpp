#include <Windows.h>
#include <iostream>

using namespace std;

int main() {
    uintptr_t baseAddress = 0x7FF7F1C50000;   
    uintptr_t offsets[] = {0x0899B088, 0x40, 0x9B0, 0xC0, 0x20, 0x3E4};
    DWORD processId;
    cout << "Enter the PID of SoT: ";
    cin >> dec >> processId;

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (processHandle == NULL) {
        cout << "OpenProcess failed. GetLastError = " << GetLastError() << endl;
        return EXIT_FAILURE;
    }

    uintptr_t currentAddress = baseAddress;
    for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]); ++i) {
        if (i == 0) {
            currentAddress += offsets[i];
        } else {
            if (!ReadProcessMemory(processHandle, (LPCVOID)currentAddress, &currentAddress, sizeof(currentAddress), NULL)) {
                cout << "ReadProcessMemory failed at offset " << i << ". GetLastError = " << GetLastError() << endl;
                CloseHandle(processHandle);
                system("pause");
                return EXIT_FAILURE;
            }
            currentAddress += offsets[i];
        }
    }

    float originalValueFoV;
    float newValueFov = 0.0f;
    cout << "Please, insert the new value for FoV: "; cin >> newValueFov;

    if (!ReadProcessMemory(processHandle, (LPCVOID)currentAddress, &originalValueFoV, sizeof(originalValueFoV), NULL)) {
        cout << "Failed to read the final value. GetLastError = " << GetLastError() << endl;
    } else {
        cout << "Original FoV value: " << originalValueFoV << endl;
    }

    if (!WriteProcessMemory(processHandle, (LPVOID)currentAddress, &newValueFov, sizeof(newValueFov), NULL)) {
        cout << "WriteProcessMemory failed while updating the FoV value. GetLastError = " << GetLastError() << endl;
    } else {
        cout << "Memory modification successful. FoV value adjusted to: " << newValueFov << endl;
    }

    CloseHandle(processHandle);
    system("pause");
    return EXIT_SUCCESS;
}
