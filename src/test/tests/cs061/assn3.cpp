/*
 * Copyright 2020 University of California Riverside. All rights reserved
 */

#define  API_VER 2
#include "framework.h"
#include <sstream>
#include <algorithm>

void testBringup(lc3::sim &sim) {
    sim.writePC(0x3000);
    sim.setRunInstLimit(5000);
}

void testTeardown(lc3::sim &sim) {
    (void) sim;
}

void shutdown() {

}

static const std::string error_prefix = "Failed: Program I/O differs from expected I/O: \\n\\\"";
static const std::string error_conjunction = "\\\", should be: \\n\\\"";
static const std::string error_instruction_limit = "Failed: Program exceeded instruction limit!";

std::string BuildErrorLabel(const std::string &expected, const std::string &got) {
    return  error_prefix + got + error_conjunction + expected + "\\\"";
}

void ReplaceNewLines(std::string &str) {
    for(std::string::size_type pos = str.find("\n"); pos != std::string::npos; pos = str.find("\n")) {
        str.replace(pos, 1, "\\n");
    }
}

void ReplaceUnprintables(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char ch) -> unsigned char {
        return isprint(ch) ? ch : '?';
    });
}

void ExecuteTest(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points, int32_t address, int32_t value) {
    bool success = true;

    sim.writeMem(address, value);
    sol_sim.writeMem(address, value);

    sim.setRunInstLimit(50000);    
    success &= sim.runUntilHalt();
    sol_sim.setRunInstLimit(50000);    
    success &= sol_sim.runUntilHalt();

    std::string label = BuildErrorLabel(tester.getSolutionOutput(), tester.getOutput()); 
    ReplaceNewLines(label);
    ReplaceUnprintables(label);

    if (sim.didExceedInstLimit()) {
        label = error_instruction_limit;
    }

    tester.verify(label, success && tester.checkContain(tester.getOutput(), tester.getSolutionOutput()), total_points);    
}

void Test01(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 0xca01, 0xffff);
}

void Test02(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 0xca01, 0xaf19);
}

void Test03(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 0xca01, 0x8000);
}

void Test04(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 0xca01, 0x0100);
}

void Test05(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 0xca01, 0x0000);
}

void setup(Tester &tester) {
    tester.registerCompTest("Test01", Test01, 20, false);
    tester.registerCompTest("Test02", Test02, 20, false);
    tester.registerCompTest("Test03", Test03, 20, false);
    tester.registerCompTest("Test04", Test04, 20, false);
    tester.registerCompTest("Test05", Test05, 20, false);
}

