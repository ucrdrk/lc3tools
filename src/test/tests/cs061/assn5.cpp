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
static const std::string error_prefix = "Failed: Program I/O differs from expected I/O: \\\"";
static const std::string error_conjunction = "\\\", should be: \n\\\"";

std::string BuildErrorLabel(const std::string &expected, const std::string &got) {
    return  error_prefix + got + error_conjunction + expected + "\\\"";
}

void ReplaceNewLines(std::string &str) {
    for(std::string::size_type pos = str.find("\n"); pos != std::string::npos; pos = str.find("\n")) {
        str.replace(pos, 1, "\\n");
    }
}

void ExecuteTest(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points, std::string input, uint16_t addr, uint16_t val) {
    bool success = true;

    tester.setInputString(input);
    tester.setSolutionInputString(input);

    sim.writeMem(addr, val);
    sol_sim.writeMem(addr, val);

    sim.setRunInstLimit(50000);    
    success &= sim.runUntilHalt();
    sol_sim.setRunInstLimit(50000);
    success &= sol_sim.runUntilHalt();

    std::string label = BuildErrorLabel(tester.getSolutionOutput(), tester.getOutput()); 
    ReplaceNewLines(label);
    tester.verify(label, success && tester.checkContain(tester.getOutput(), tester.getSolutionOutput()), total_points);    
}

void Test01(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "x7", 0xB600, 0x0000);
}

void Test02(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "17", 0xB600, 0x0000);
}

void Test03(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "17", 0xB600, 0x8000);
}

void Test04(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "17", 0xB600, 0xabcd);
}

void Test05(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "17", 0xB600, 0xffff);
}

void Test06(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "27", 0xB600, 0x0000);
}

void Test07(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "27", 0xB600, 0x8000);
}

void Test08(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "27", 0xB600, 0xabcd);
}

void Test09(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "27", 0xB600, 0xffff);
}

void Test10(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "37", 0xB600, 0x0000);
}

void Test11(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "37", 0xB600, 0x8000);
}

void Test12(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "37", 0xB600, 0xabcd);
}

void Test13(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "37", 0xB600, 0xffff);
}

void Test14(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "47", 0xB600, 0x0000);
}

void Test15(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "47", 0xB600, 0x8000);
}

void Test16(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "47", 0xB600, 0xabcd);
}

void Test17(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "47", 0xB600, 0xffff);
}

void Test18(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "58\n7", 0xB600, 0x0000);
}

void Test19(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "517\n12\n7", 0xB600, 0x0000);
}

void Test20(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "5-6\n7", 0xB600, 0x0000);
}

void Test21(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "5g3\n7", 0xB600, 0x0000);
}

void Test22(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "5\n5\n7", 0xB600, 0x0000);
}

void Test23(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "515\n7", 0xB600, 0x8000);
}

void Test24(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "51\n7", 0xB600, 0xabcd);
}

void Test25(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "51\n7", 0xB600, 0xffff);
}

void Test26(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "67", 0xB600, 0x0000);
}

void Test27(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "67", 0xB600, 0x8000);
}

void Test28(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "67", 0xB600, 0xabcd);
}

void Test29(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "67", 0xB600, 0xffff);
}

void Test30(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "7", 0xB600, 0x0000);
}

void setup(Tester &tester) {
    tester.registerCompTest("Test Menu Error", Test01, 4, false);
    tester.registerCompTest("Test Option 1 with x0000", Test02, 3, false);
    tester.registerCompTest("Test Option 1 with x8000", Test03, 3, false);
    tester.registerCompTest("Test Option 1 with xABCD", Test04, 3, false);
    tester.registerCompTest("Test Option 1 with xFFFF", Test05, 3, false);
    tester.registerCompTest("Test Option 2 with x0000", Test06, 3, false);
    tester.registerCompTest("Test Option 2 with x8000", Test07, 3, false);
    tester.registerCompTest("Test Option 2 with xABCD", Test08, 3, false);
    tester.registerCompTest("Test Option 2 with xFFFF", Test09, 3, false);
    tester.registerCompTest("Test Option 3 with x0000", Test10, 4, false);
    tester.registerCompTest("Test Option 3 with x8000", Test11, 4, false);
    tester.registerCompTest("Test Option 3 with xABCD", Test12, 4, false);
    tester.registerCompTest("Test Option 3 with xFFFF", Test13, 4, false);
    tester.registerCompTest("Test Option 4 with x0000", Test14, 4, false);
    tester.registerCompTest("Test Option 4 with x8000", Test15, 4, false);
    tester.registerCompTest("Test Option 4 with xABCD", Test16, 4, false);
    tester.registerCompTest("Test Option 4 with xFFFF", Test17, 4, false);
    tester.registerCompTest("Test Option 5 with 8, x0000", Test18, 3, false);
    tester.registerCompTest("Test Option 5 Error Case #1 with x0000", Test19, 3, false);
    tester.registerCompTest("Test Option 5 Error Case #2 with x0000", Test20, 3, false);
    tester.registerCompTest("Test Option 5 Error Case #3 with x0000", Test21, 3, false);
    tester.registerCompTest("Test Option 5 Error Case #4 with x0000", Test22, 3, false);
    tester.registerCompTest("Test Option 5 with 15, x8000", Test23, 3, false);
    tester.registerCompTest("Test Option 5 with 1, xABCD", Test24, 3, false);
    tester.registerCompTest("Test Option 5 with 1, xFFFF", Test25, 3, false);
    tester.registerCompTest("Test Option 6 with x0000", Test26, 3, false);
    tester.registerCompTest("Test Option 6 with x8000", Test27, 3, false);
    tester.registerCompTest("Test Option 6 with xABCD", Test28, 3, false);
    tester.registerCompTest("Test Option 6 with xFFFF", Test29, 3, false);
    tester.registerCompTest("Test Option 7", Test30, 4, false);
}

