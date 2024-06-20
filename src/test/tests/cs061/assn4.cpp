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
    return  error_prefix + got + error_conjunction + expected;
}

void ReplaceNewLines(std::string &str) {
    for(std::string::size_type pos = str.find("\n"); pos != std::string::npos; pos = str.find("\n")) {
        str.replace(pos, 1, "\\n");
    }
}

void ExecuteTest(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points, std::string input, uint16_t reg) {
    bool success = true;

    tester.setInputString(input);
    tester.setSolutionInputString(input);

    sim.setRunInstLimit(50000);    
    success &= sim.runUntilHalt();
    sol_sim.setRunInstLimit(50000);    
    success &= sol_sim.runUntilHalt();

    std::string label = "Failed: Program register output differs from the solution register output."; 

    if (sim.didExceedInstLimit()) {
        label = error_instruction_limit;
    }

    tester.verify(label, success && sim.readReg(reg) == sol_sim.readReg(reg), total_points);    
}

void Test01(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "9635\n", 4);
}

void Test02(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "+29374\n", 4);
}

void Test03(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "-9877\n", 4);
}

void Test04(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "-27430\n", 4);
}

void Test05(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "x+1239\n", 4);
}

void Test06(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "+23%-673\n", 4);
}

void Test07(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "+-+588\n", 4);
}

void Test08(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "\n", 4);
}

void setup(Tester &tester) {
    tester.registerCompTest("Test01", Test01, 15, false);
    tester.registerCompTest("Test02", Test02, 15, false);
    tester.registerCompTest("Test03", Test03, 15, false);
    tester.registerCompTest("Test04", Test04, 10, false);
    tester.registerCompTest("Test05", Test05, 15, false);
    tester.registerCompTest("Test06", Test06, 15, false);
    tester.registerCompTest("Test07", Test07, 10, false);
    tester.registerCompTest("Test08", Test08, 5, false);
}

