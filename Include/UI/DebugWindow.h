#pragma once

#include "UI/UIScreen.h"
#include <vector>
#include <string>
#include <sstream>
#include <mutex>
#include <iostream>
#include <streambuf>

class DebugStreambuf : public std::streambuf {
public:
    DebugStreambuf();
    ~DebugStreambuf();

protected:
    virtual int overflow(int c) override;
    virtual std::streamsize xsputn(const char* s, std::streamsize n) override;

private:
    std::string m_buffer;
    std::streambuf* m_originalCout;
    static std::vector<std::string>* s_logMessages;
    static std::mutex* s_logMutex;

public:
    static void SetLogStorage(std::vector<std::string>* messages, std::mutex* mutex);
};

class DebugWindow : public UIScreen {
private:
    std::vector<std::string> m_logMessages;
    std::mutex m_logMutex;
    bool m_autoScroll;
    bool m_showTimestamps;
    char m_inputBuffer[256];
    DebugStreambuf* m_debugStreambuf;
    std::streambuf* m_originalCout;

    bool m_showInfo;
    bool m_showWarnings;
    bool m_showErrors;

public:
    DebugWindow(float width, float height);
    ~DebugWindow();

    void draw() override;

    void AddLog(const std::string& message);
    void AddInfo(const std::string& message);
    void AddWarning(const std::string& message);
    void AddError(const std::string& message);

    void Clear();

    void EnableCoutRedirection();
    void DisableCoutRedirection();

private:
    void DrawLogOutput();
    void DrawInputField();
    void DrawMenuBar();
    std::string GetTimestamp();
    ImVec4 GetLogColor(const std::string& message);
};
