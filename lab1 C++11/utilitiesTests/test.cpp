#include <gtest/gtest.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <cstdio>
#include "../lab1/include/employee.h"

class CreatorIntegrationTest : public ::testing::Test {
protected:
    std::string testBinaryFile = "../lab1/docs/test_data.bin";
    int testCount = 3;

    void TearDown() override {
        std::remove(testBinaryFile.c_str());
    }

    bool runCreator(const std::string& binaryFile, int count) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "%s %d", binaryFile.c_str(), count);

        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA("../lab1/bin/creator.exe", cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return exitCode == 0;
    }
};

TEST_F(CreatorIntegrationTest, CreatorCreatesBinaryFile) {
    bool success = runCreator(testBinaryFile, testCount);

    ASSERT_TRUE(success);

    std::ifstream file(testBinaryFile, std::ios::binary);
    ASSERT_TRUE(file.good());

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.close();

    EXPECT_EQ(fileSize, testCount * sizeof(employee));
}

TEST_F(CreatorIntegrationTest, CreatorFailsWithInvalidArguments) {
    char cmd[] = "../lab1/docs/invalid.bin";

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    CreateProcessA("../lab1/bin/creator.exe", cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    EXPECT_EQ(exitCode, 1);
}

class ReporterIntegrationTest : public ::testing::Test {
protected:
    std::string testBinaryFile = "../lab1/docs/test_data.bin";
    std::string testReportFile = "../lab1/docs/test_report.txt";
    double testPayRate = 15.5;

    void SetUp() override {
        std::ofstream fout(testBinaryFile, std::ios::binary);
        if (fout) {
            employee e1 = { 1, "James", 40.5 };
            employee e2 = { 2, "Shane", 38.0 };
            fout.write(reinterpret_cast<const char*>(&e1), sizeof(employee));
            fout.write(reinterpret_cast<const char*>(&e2), sizeof(employee));
            fout.close();
        }
    }

    void TearDown() override {
        std::remove(testBinaryFile.c_str());
        std::remove(testReportFile.c_str());
    }

    bool runReporter(const std::string& binaryFile, const std::string& reportFile, double payRate) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "%s %s %.2f", binaryFile.c_str(), reportFile.c_str(), payRate);

        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA("../lab1/bin/reporter.exe", cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return exitCode == 0;
    }
};

TEST_F(ReporterIntegrationTest, ReporterCreatesReportFile) {
    bool success = runReporter(testBinaryFile, testReportFile, testPayRate);

    ASSERT_TRUE(success);

    std::ifstream file(testReportFile);
    ASSERT_TRUE(file.good());
    file.close();
}

TEST_F(ReporterIntegrationTest, ReporterGeneratesCorrectContent) {
    bool success = runReporter(testBinaryFile, testReportFile, testPayRate);
    ASSERT_TRUE(success);

    std::ifstream fin(testReportFile);
    ASSERT_TRUE(fin.good());

    std::string line;
    std::getline(fin, line);
    EXPECT_NE(line.find("Report for file:"), std::string::npos);

    std::getline(fin, line);
    EXPECT_NE(line.find("ID"), std::string::npos);
    EXPECT_NE(line.find("Name"), std::string::npos);
    EXPECT_NE(line.find("Hours"), std::string::npos);
    EXPECT_NE(line.find("Salary"), std::string::npos);

    std::getline(fin, line);
    EXPECT_NE(line.find("1"), std::string::npos);
    EXPECT_NE(line.find("James"), std::string::npos);
    EXPECT_NE(line.find("40.5"), std::string::npos);

    std::getline(fin, line);
    EXPECT_NE(line.find("2"), std::string::npos);
    EXPECT_NE(line.find("Shane"), std::string::npos);
    EXPECT_NE(line.find("38"), std::string::npos);

    fin.close();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}