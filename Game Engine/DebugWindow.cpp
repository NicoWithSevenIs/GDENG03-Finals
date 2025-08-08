#include "UI/DebugWindow.h"
#include <chrono>
#include <iomanip>
#include <ctime>

std::vector<std::string>* DebugStreambuf::s_logMessages = nullptr;
std::mutex* DebugStreambuf::s_logMutex = nullptr;

DebugStreambuf::DebugStreambuf() {
    m_originalCout = std::cout.rdbuf();
}

DebugStreambuf::~DebugStreambuf() {
    if (std::cout.rdbuf() == this) {
        std::cout.rdbuf(m_originalCout);
    }
}

void DebugStreambuf::SetLogStorage(std::vector<std::string>* messages, std::mutex* mutex) {
    s_logMessages = messages;
    s_logMutex = mutex;
}

int DebugStreambuf::overflow(int c) {
    if (c != EOF) {
        try {
            m_buffer += static_cast<char>(c);
            if (c == '\n') {
                if (s_logMessages && s_logMutex) {
                    std::lock_guard<std::mutex> lock(*s_logMutex);
                    if (s_logMessages->size() < 1000) {
                        s_logMessages->push_back(m_buffer);
                    }
                }
                m_buffer.clear();
            }
        }
        catch (...) {
        }
    }
    return c;
}

std::streamsize DebugStreambuf::xsputn(const char* s, std::streamsize n) {
    try {
        if (s && n > 0) {
            for (std::streamsize i = 0; i < n; ++i) {
                overflow(s[i]);
            }
        }
    }
    catch (...) {
    }
    return n;
}

DebugWindow::DebugWindow(float width, float height)
    : m_autoScroll(true), m_showTimestamps(true), m_showInfo(true),
    m_showWarnings(true), m_showErrors(true), m_debugStreambuf(nullptr),
    m_originalCout(nullptr) {
    this->width = width;
    this->height = height;
    this->m_name = "Debug Console";
    this->isOpen = false;

    memset(m_inputBuffer, 0, sizeof(m_inputBuffer));


    std::lock_guard<std::mutex> lock(m_logMutex);
    m_logMessages.push_back("[INFO] Debug Console initialized\n");
}

DebugWindow::~DebugWindow() {
    DisableCoutRedirection();
}

void DebugWindow::EnableCoutRedirection() {
    try {
        if (!m_debugStreambuf) {
            m_debugStreambuf = new DebugStreambuf();
            DebugStreambuf::SetLogStorage(&m_logMessages, &m_logMutex);
            m_originalCout = std::cout.rdbuf();
            std::cout.rdbuf(m_debugStreambuf);
        }
    }
    catch (...) {
        if (m_debugStreambuf) {
            delete m_debugStreambuf;
            m_debugStreambuf = nullptr;
        }
    }
}

void DebugWindow::DisableCoutRedirection() {
    try {
        if (m_debugStreambuf) {
            std::cout.rdbuf(m_originalCout);
            delete m_debugStreambuf;
            m_debugStreambuf = nullptr;
        }
    }
    catch (...) {
    }
}

void DebugWindow::draw() {
    if (!isOpen) return;

    static bool first_open = true;
    if (first_open) {
        EnableCoutRedirection();
        AddInfo("Cout redirection enabled - console output will now appear here");
        first_open = false;
    }

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(50, height - 450), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Debug Console", &isOpen)) {
        DrawMenuBar();
        DrawLogOutput();
        DrawInputField();
    }
    ImGui::End();
}

void DebugWindow::DrawMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            ImGui::Checkbox("Auto-scroll", &m_autoScroll);
            ImGui::Checkbox("Show timestamps", &m_showTimestamps);
            ImGui::Separator();
            ImGui::Checkbox("Show Info", &m_showInfo);
            ImGui::Checkbox("Show Warnings", &m_showWarnings);
            ImGui::Checkbox("Show Errors", &m_showErrors);
            ImGui::Separator();
            if (ImGui::MenuItem("Clear Log")) {
                Clear();
            }
            if (ImGui::MenuItem("Enable cout redirection", nullptr, m_debugStreambuf != nullptr)) {
                if (m_debugStreambuf) {
                    DisableCoutRedirection();
                }
                else {
                    EnableCoutRedirection();
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void DebugWindow::DrawLogOutput() {
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

    if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
        std::lock_guard<std::mutex> lock(m_logMutex);

        for (const auto& message : m_logMessages) {
            bool shouldShow = true;
            if (message.find("[INFO]") != std::string::npos && !m_showInfo) shouldShow = false;
            if (message.find("[WARNING]") != std::string::npos && !m_showWarnings) shouldShow = false;
            if (message.find("[ERROR]") != std::string::npos && !m_showErrors) shouldShow = false;

            if (shouldShow) {
                ImVec4 color = GetLogColor(message);
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(message.c_str());
                ImGui::PopStyleColor();
            }
        }

        if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
    }
    ImGui::EndChild();
}

void DebugWindow::DrawInputField() {
    ImGui::Separator();

    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;

    if (ImGui::InputText("Input", m_inputBuffer, IM_ARRAYSIZE(m_inputBuffer), input_text_flags)) {
        std::string command(m_inputBuffer);
        if (!command.empty()) {
            AddLog("> " + command);

            if (command == "clear") {
                Clear();
            }
            else if (command == "help") {
                AddInfo("Available commands:");
                AddInfo("  clear - Clear the console");
                AddInfo("  help - Show this help message");
                AddInfo("  test - Add test messages");
            }
            else if (command == "test") {
                AddInfo("Test info message");
                AddWarning("Test warning message");
                AddError("Test error message");
            }
            else {
                AddLog("Unknown command: " + command);
            }
        }

        strcpy_s(m_inputBuffer, "");
        reclaim_focus = true;
    }

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus) {
        ImGui::SetKeyboardFocusHere(-1);
    }
}

void DebugWindow::AddLog(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_logMutex);
    std::string logMessage = message;
    if (m_showTimestamps) {
        logMessage = GetTimestamp() + " " + logMessage;
    }
    m_logMessages.push_back(logMessage + "\n");
}

void DebugWindow::AddInfo(const std::string& message) {
    AddLog("[INFO] " + message);
}

void DebugWindow::AddWarning(const std::string& message) {
    AddLog("[WARNING] " + message);
}

void DebugWindow::AddError(const std::string& message) {
    AddLog("[ERROR] " + message);
}

void DebugWindow::Clear() {
    std::lock_guard<std::mutex> lock(m_logMutex);
    m_logMessages.clear();
}

std::string DebugWindow::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    struct tm timeinfo;
    localtime_s(&timeinfo, &time_t);

    std::stringstream ss;
    ss << std::put_time(&timeinfo, "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}

ImVec4 DebugWindow::GetLogColor(const std::string& message) {
    if (message.find("[ERROR]") != std::string::npos) {
        return ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
    }
    else if (message.find("[WARNING]") != std::string::npos) {
        return ImVec4(1.0f, 0.8f, 0.4f, 1.0f);
    }
    else if (message.find("[INFO]") != std::string::npos) {
        return ImVec4(0.4f, 0.8f, 1.0f, 1.0f);
    }
    else if (message.find(">") == 0) {
        return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    }
    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}
