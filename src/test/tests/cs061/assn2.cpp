/*
 * Copyright 2020 University of California Riverside. All rights reserved
 */

#define  API_VER 2
#include "framework.h"
#include <sstream>

void testBringup(lc3::sim &sim) {
    sim.writePC(0x3000);
    sim.setRunInstLimit(5000);
}

void testTeardown(lc3::sim &sim) {
    (void) sim;
}

void shutdown() {

}

static const std::string user_prompt = "ENTER two numbers (i.e '0'....'9')\n";
static const std::string error_prefix = "Failed: Program I/O differs from expected I/O: ";
static const std::string error_conjunction = ", should be: ";

std::string BuildErrorLabel(const std::string &expected, const std::string &got) {
    return  error_prefix + got + error_conjunction + expected;
}

void ExecuteTest(lc3::sim &sim, Tester &tester, double total_points, int32_t a, int32_t b) {
    bool success = true;

    success &= sim.runUntilInputRequested();
    tester.verify("User Prompt", success && tester.checkMatch(tester.getOutput(), user_prompt), total_points/5);
    tester.clearOutput();
    std::ostringstream input;
    input << a << b;
    tester.setInputString(input.str());
    success &= sim.runUntilInputRequested();

    std::ostringstream output;
    output << a << "\n" << b << "\n" << a << " - " << b << " = " << (a - b) << "\n";
    std::string expected_output = output.str();
    std::string label = BuildErrorLabel(expected_output, tester.getOutput()); 
    tester.verify(label, success && tester.checkContain(tester.getOutput(), expected_output), 4 * total_points/5);
    
    success &= sim.runUntilHalt();
}

void TestNegative(lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sim, tester, total_points, 2, 9);
}

void TestPositive(lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sim, tester, total_points, 8, 4);
}

void TestZeroResult(lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sim, tester, total_points, 6, 6);
}

void TestZeroInput(lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sim, tester, total_points, 0, 0);
}

void TestSubtractZero(lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sim, tester, total_points, 3, 0);
}

void setup(Tester &tester) {
    tester.registerTest("TestNegative", TestNegative, 25, false);
    tester.registerTest("TestPositive", TestPositive, 25, false);
    tester.registerTest("TestZero", TestZeroResult, 15, false);
    tester.registerTest("TestZero", TestZeroInput, 15, false);
    tester.registerTest("TestZero", TestSubtractZero, 20, false);
}

