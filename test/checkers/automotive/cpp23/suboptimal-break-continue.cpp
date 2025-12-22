// RUN: %check_clang_tidy %s automotive-cpp23-adv-6.8 %t
// Test for automotive-cpp23-adv-6.8: Suboptimal break/continue placement
// Related MISRA C++:2023 Rule: 6.8

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_break_violations(int x) {
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: break statement should be the last statement in the compound statement [automotive-cpp23-adv-6.8]
            break;
            int y = 10; // Code after break (unreachable)
        }
    }

    while (x > 0) {
        if (x > 100) {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: break statement should be the last statement in the compound statement [automotive-cpp23-adv-6.8]
            break;
            x--; // Code after break (unreachable)
        }
        x--;
    }

    switch (x) {
        case 1: {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: break statement should be the last statement in the compound statement [automotive-cpp23-adv-6.8]
            break;
            int z = 20; // Code after break (unreachable)
        }
        default:
            break;
    }
}

void test_continue_violations(int x) {
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: continue statement should be the last statement in the compound statement [automotive-cpp23-adv-6.8]
            continue;
            int y = 10; // Code after continue (unreachable)
        }
    }

    while (x > 0) {
        if (x % 2 == 0) {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: continue statement should be the last statement in the compound statement [automotive-cpp23-adv-6.8]
            continue;
            x++; // Code after continue (unreachable)
        }
        x--;
    }

    int count = 0;
    do {
        count++;
        if (count % 3 == 0) {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: continue statement should be the last statement in the compound statement [automotive-cpp23-adv-6.8]
            continue;
            count += 5; // Code after continue (unreachable)
        }
    } while (count < 20);
}

void test_mixed_violations(int x) {
    for (int i = 0; i < 10; i++) {
        if (i == 3) {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: continue statement should be the last statement in the compound statement [automotive-cpp23-adv-6.8]
            continue;
            x++;
        }
        if (i == 7) {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: break statement should be the last statement in the compound statement [automotive-cpp23-adv-6.8]
            break;
            x--;
        }
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_break(int x) {
    // Break as last statement in compound
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            int y = 10;
            break;
        }
    }

    while (x > 0) {
        if (x > 100) {
            x = 0;
            break;
        }
        x--;
    }

    switch (x) {
        case 1: {
            int z = 20;
            break;
        }
        default:
            break;
    }
}

void test_compliant_continue(int x) {
    // Continue as last statement in compound
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            int y = 10;
            continue;
        }
    }

    while (x > 0) {
        if (x % 2 == 0) {
            x++;
            continue;
        }
        x--;
    }

    int count = 0;
    do {
        count++;
        if (count % 3 == 0) {
            count += 5;
            continue;
        }
    } while (count < 20);
}

void test_compliant_no_compound() {
    // Break/continue without braces (not in compound statement)
    for (int i = 0; i < 10; i++)
        if (i == 5)
            break;

    int x = 0;
    while (x < 10)
        if (x == 5)
            continue;
        else
            x++;
}

void test_compliant_nested(int x) {
    // Nested loops with proper placement
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            if (j == 3) {
                break; // Last in inner compound
            }
        }
        if (i == 7) {
            continue; // Last in outer compound
        }
    }
}
