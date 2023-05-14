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

bool not_printable(char c) {
    return !isprint(c) && !isspace(c);
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

std::string BuildErrorLabel(const std::string &expected, const std::string &got) {
    std::string cleaned_got = got;
    std::replace_if(cleaned_got.begin(), cleaned_got.end(), not_printable, '_');
    return  error_prefix + cleaned_got + error_conjunction + expected + "\\\"";
}

void ExecuteTest(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points, std::string input) {
    bool success = true;

    tester.setInputString(input);
    tester.setSolutionInputString(input);

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
    ExecuteTest(sol_sim, sim, tester, total_points, "\n");
}

void Test02(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "a\n");
}

void Test03(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "abbaabba\n");
}

void Test04(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "abbabba\n");
}

void Test05(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "abbacdabba\n");
}

void Test06(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "abbaaba\n");
}

void Test07(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "a man a plan a canal panama\n");
}

void Test08(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "amanaplanacanalpanama\n");
}

void Test09(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "0123456789012345678901234567890123456789012345678908765432109876543210987654321098765432109876543210\n");
}

void Test10(lc3::sim &sol_sim, lc3::sim &sim, Tester &tester, double total_points) {
    ExecuteTest(sol_sim, sim, tester, total_points, "0123456789012345678901234567890123456789012345678998765432109876543210987654321098765432109876543210\n");
}

void setup(Tester &tester) {
    tester.registerCompTest("Test Empty String", Test01, 4, false);
    tester.registerCompTest("Test String length 1", Test02, 4, false);
    tester.registerCompTest("Test Short Palindrome Even Length", Test03, 7, false);
    tester.registerCompTest("Test Short Palindrome Odd Length", Test04, 7, false);
    tester.registerCompTest("Test Short Non-palindrome Even Length", Test05, 7, false);
    tester.registerCompTest("Test Short Non-palindrome Odd Length", Test06, 7, false);
    tester.registerCompTest("Test Non-palindrome with spaces", Test07, 7, false);
    tester.registerCompTest("Test Palindrome with spaces", Test08, 7, false);
    tester.registerCompTest("Test Very Long Non-palindrome", Test09, 10, false);
    tester.registerCompTest("Test Very Long Palindrome", Test10, 10, false);
}

