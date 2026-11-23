#include "PGameBuilder.h"
#include <Windows.h>
#include <fstream>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

using namespace std;

bool PGameBuilder::BuildGame() {
	string buildCommand = GetBuildCommand();
	if (buildCommand == "") {
		return false;
	}

    SECURITY_ATTRIBUTES securityAttributes{};
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;

    HANDLE readPipe = nullptr;
    HANDLE writePipe = nullptr;

    if (!CreatePipe(&readPipe, &writePipe, &securityAttributes, 0))
        return "";

    SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA startupInfo{};
    startupInfo.cb = sizeof(STARTUPINFOA);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = writePipe;
    startupInfo.hStdError = writePipe;
    startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

	//STARTUPINFOA startupinfo{};
	//startupinfo.cb = sizeof(startupinfo);

	PROCESS_INFORMATION processInfo{};

    bool success = CreateProcessA(nullptr, buildCommand.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, SOLUTION_DIR, &startupInfo,
        &processInfo);

    CloseHandle(writePipe);

    if (!success)
    {
        CloseHandle(readPipe);
        return false;
    }

    char buffer[4096];
    DWORD bytesRead = 0;

    while (ReadFile(readPipe, buffer, sizeof(buffer), &bytesRead, nullptr)
        && bytesRead > 0)
    {
        
    }

	WaitForSingleObject(processInfo.hProcess, INFINITE);

	DWORD buildExitCode = 1;
	GetExitCodeProcess(processInfo.hProcess, &buildExitCode);
    CloseHandle(readPipe);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	if (buildExitCode != 0) {
		return false;
	}

	return true;
}

string PGameBuilder::GetBuildCommand() {
    SECURITY_ATTRIBUTES securityAttributes{};
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;

    HANDLE readPipe = nullptr;
    HANDLE writePipe = nullptr;

    if (!CreatePipe(&readPipe, &writePipe, &securityAttributes, 0))
        return "";

    SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA startupInfo{};
    startupInfo.cb = sizeof(STARTUPINFOA);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = writePipe;
    startupInfo.hStdError = writePipe;
    startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    PROCESS_INFORMATION processInfo{};

    string vswherePath =
        R"(C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe)";

    string commandLine =
        "\"" + vswherePath + "\""
        " -latest -products *"
        " -requires Microsoft.Component.MSBuild"
        R"( -find MSBuild\**\Bin\MSBuild.exe)";

    BOOL success = CreateProcessA(
        nullptr,
        commandLine.data(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &startupInfo,
        &processInfo
    );

    CloseHandle(writePipe);

    if (!success)
    {
        CloseHandle(readPipe);
        return "";
    }

    string msbuildRoute;
    char buffer[4096];
    DWORD bytesRead = 0;

    while (ReadFile(readPipe, buffer, sizeof(buffer), &bytesRead, nullptr)
        && bytesRead > 0)
    {
        msbuildRoute.append(buffer, bytesRead);
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);

    CloseHandle(readPipe);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    while (!msbuildRoute.empty() && (msbuildRoute.back() == '\r' || msbuildRoute.back() == '\n' || msbuildRoute.back() == ' ')) {
        msbuildRoute.pop_back();
    }

	string buildConfig = "Release";
#ifdef _DEBUG
	buildConfig = "Debug";
#endif
	string buildPlatform = string(BUILD_PLATFORM);
	string gamePath = string(SOLUTION_DIR) + "PGame/PGame.vcxproj";
	string buildCommand = "\"" + msbuildRoute + "\" \"" + gamePath + "\" /p:Configuration=" + buildConfig +
        " /p:Platform=" + buildPlatform;

	return buildCommand;
}