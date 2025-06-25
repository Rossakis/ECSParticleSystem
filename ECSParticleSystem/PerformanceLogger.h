#pragma once
#include <string>

class PerformanceLogger {
public:
    PerformanceLogger();
    void LogFrame(float fps, bool ecsActive);
    void WriteLog(const std::string& prefix = "", const std::string& filename = "performance_log.txt");
    float GetAverageFPS() const;
    void SetECSUsed(bool used);
    void SetSimulationTime(float seconds);
    void SetParticleCount(int count); // <-- Add this

private:
    int frameCount;
    float fpsSum;
    bool ecsUsed;
    float simulationTime;
    int particleCount; // <-- Add this

    std::string GetCPUBrand();
    size_t GetRAMSizeMB();
    std::string GetGPUInfo();
};