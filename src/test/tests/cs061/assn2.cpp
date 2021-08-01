/*
 * Copyright 2020 University of California Riverside. All rights reserved
 */

#define  API_VER 2
#include "framework.h"
#include <sstream>
#include <algorithm>
#include <regex>

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

std::string BuildErrorLabel(const std::string &expected, const std::string &got) {
    return  error_prefix + got + error_conjunction + expected;
}

std::string ReplaceNewLines(std::string const &str) {
    std::regex nle("\\n");
    return std::regex_replace(str, nle, "\\\\n");
}

void ExecuteTest(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points, int32_t a, int32_t b) {
    bool success = true;

    std::ostringstream input;
    input << a << b;
    tester.setInputString(input.str());
    tester.setSolutionInputString(input.str());

    success &= sim.runUntilHalt();
    success &= sol_sim.runUntilHalt();

    std::string solution = ReplaceNewLines(tester.getSolutionOutput());
    std::string output = ReplaceNewLines(tester.getOutput());

    std::string label = BuildErrorLabel(solution, output); 
    tester.verify(label, success && tester.checkContain(tester.getOutput(), tester.getSolutionOutput()), total_points);    
}

void TestNegative(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 2, 9);
}

void TestPositive(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 8, 4);
}

void TestZeroResult(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 6, 6);
}

void TestZeroInput(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 0, 0);
}

void TestSubtractZero(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, 3, 0);
}

void setup(Tester &tester) {
    tester.registerCompTest("TestNegative", TestNegative, 25, false);
    tester.registerCompTest("TestPositive", TestPositive, 25, false);
    tester.registerCompTest("TestZero", TestZeroResult, 15, false);
    tester.registerCompTest("TestZero", TestZeroInput, 15, false);
    tester.registerCompTest("TestZero", TestSubtractZero, 20, false);
}

