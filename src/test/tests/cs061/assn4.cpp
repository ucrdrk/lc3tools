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

static const std::string user_prompt = "ENTER two numbers (i.e '0'....'9')\n";
static const std::string error_prefix = "Failed: Program I/O differs from expected I/O: ";
static const std::string error_conjunction = ", should be: ";

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

    success &= sim.runUntilHalt();
    success &= sol_sim.runUntilHalt();

    std::string label = "Failed: Program register output differs from the solution register output."; 
    tester.verify(label, success && sim.readReg(reg) == sol_sim.readReg(reg), total_points);    
}

void Test01(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "9635\n", 1);
}

void Test02(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "+29374\n", 1);
}

void Test03(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "-9877\n", 1);
}

void Test04(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "-27430\n", 1);
}

void Test05(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "x+1239\n", 1);
}

void Test06(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "+23%-673\n", 1);
}

void Test07(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "+-+588\n", 1);
}

void Test08(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "\n", 1);
}

void setup(Tester &tester) {
    tester.registerCompTest("Test +ve Number without Sign", Test01, 15, false);
    tester.registerCompTest("Test 5-digit +ve Number with Sign", Test02, 15, false);
    tester.registerCompTest("Test -ve Number", Test03, 15, false);
    tester.registerCompTest("Test 5-digit -ve Number", Test04, 10, false);
    tester.registerCompTest("Test invalid initial character, +ve number w/sign", Test05, 15, false);
    tester.registerCompTest("Test invalid internal character, -ve number", Test06, 15, false);
    tester.registerCompTest("Test Double Sign, +ve number w/sign", Test07, 10, false);
    tester.registerCompTest("Test no entry", Test08, 5, false);
}

