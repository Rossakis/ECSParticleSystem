#include "PerformanceLogger.h"
#include <fstream>
#include <cstring>
#include <string>
#include <sys/stat.h>

#if defined(_WIN32)
    #include <windows.h>
    #include <intrin.h>
    #include <direct.h>
#endif

#if defined(__APPLE__)
    #include <sys/types.h>
    #include <sys/sysctl.h>
#endif

#if defined(__linux__)
    #include <sys/sysinfo.h>
#endif

// For OpenGL GPU info
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
    #include <GL/gl.h>
#endif

PerformanceLogger::PerformanceLogger()
    : frameCount(0), fpsSum(0.0f), ecsUsed(false), simulationTime(0.0f), particleCount(0) {}

void PerformanceLogger::LogFrame(float fps, bool ecsActive) {
    frameCount++;
    fpsSum += fps;
    ecsUsed = ecsActive;
}

void PerformanceLogger::SetSimulationTime(float seconds) {
    simulationTime = seconds;
}

void PerformanceLogger::SetParticleCount(int count) {
    particleCount = count;
}

void PerformanceLogger::WriteLog(const std::string& prefix, const std::string& filename) {
    std::string outDir = "OutputData/";
#if defined(_WIN32)
    _mkdir(outDir.c_str());
#else
    mkdir(outDir.c_str(), 0777);
#endif
    std::string outFilename = outDir + prefix + filename;
    std::ofstream file(outFilename, std::ios::out);
    if (!file.is_open()) return;

    file << "Simulation Time: " << simulationTime << " seconds\n";
    file << "Particle Count: " << particleCount << "\n"; // <-- Add this
    file << "Average FPS: " << GetAverageFPS() << "\n";
    file << "ECS Used: " << (ecsUsed ? "Yes" : "No") << "\n";
    file << "Machine Specs:\n";
    file << "  CPU: " << GetCPUBrand() << "\n";
    file << "  RAM: " << GetRAMSizeMB() << " MB\n";
    file << "  GPU: " << GetGPUInfo() << "\n";
    file.close();
}

float PerformanceLogger::GetAverageFPS() const {
    return frameCount > 0 ? fpsSum / frameCount : 0.0f;
}

void PerformanceLogger::SetECSUsed(bool used) {
    ecsUsed = used;
}

std::string PerformanceLogger::GetCPUBrand() {
#if defined(_WIN32)
    int cpuInfo[4] = { -1 };
    char brand[0x40] = { 0 };
    __cpuid(cpuInfo, 0x80000000);
    unsigned int nExIds = cpuInfo[0];
    if (nExIds >= 0x80000004) {
        __cpuid((int*)cpuInfo, 0x80000002);
        std::memcpy(brand, cpuInfo, sizeof(cpuInfo));
        __cpuid((int*)cpuInfo, 0x80000003);
        std::memcpy(brand + 16, cpuInfo, sizeof(cpuInfo));
        __cpuid((int*)cpuInfo, 0x80000004);
        std::memcpy(brand + 32, cpuInfo, sizeof(cpuInfo));
    }
    return std::string(brand);
#elif defined(__APPLE__)
    char brand[256];
    size_t size = sizeof(brand);
    if (sysctlbyname("machdep.cpu.brand_string", &brand, &size, NULL, 0) == 0) {
        return std::string(brand);
    }
    return "Unknown";
#elif defined(__linux__)
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("model name") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos)
                return line.substr(pos + 2);
        }
    }
    return "Unknown";
#else
    return "Unknown";
#endif
}

size_t PerformanceLogger::GetRAMSizeMB() {
#if defined(_WIN32)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status)) {
        return static_cast<size_t>(status.ullTotalPhys / (1024 * 1024));
    }
    return 0;
#elif defined(__linux__)
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return static_cast<size_t>(info.totalram * info.mem_unit / (1024 * 1024));
    }
    return 0;
#elif defined(__APPLE__)
    int64_t mem = 0;
    size_t len = sizeof(mem);
    if (sysctlbyname("hw.memsize", &mem, &len, NULL, 0) == 0) {
        return static_cast<size_t>(mem / (1024 * 1024));
    }
    return 0;
#else
    return 0;
#endif
}

std::string PerformanceLogger::GetGPUInfo() {
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
    // OpenGL context must be active!
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    if (vendor && renderer) {
        return std::string(reinterpret_cast<const char*>(vendor)) + " " +
               std::string(reinterpret_cast<const char*>(renderer));
    }
    return "Unknown";
#else
    return "Unknown";
#endif
}