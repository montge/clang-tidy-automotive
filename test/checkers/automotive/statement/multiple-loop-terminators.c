// RUN: %check_clang_tidy %s automotive-c23-adv-15.4 %t

// Test: Multiple loop terminators (MISRA Rule 15.4)
// Check ID: automotive-c23-adv-15.4

// ============= Violations: Multiple break in while loop =============

void test_while_multiple_breaks(int x, int y) {
    int i = 0;
    while (i < 100) {
        if (x > 10)
            break;
        if (y > 20)
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: loop has more than one break or goto terminator
            break;
        i++;
    }
}

// ============= Violations: Break and goto in while loop =============

void test_while_break_and_goto(int x) {
    int i = 0;
exit_loop:
    while (i < 100) {
        if (x > 10)
            break;
        if (x < -10)
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: loop has more than one break or goto terminator
            goto exit_loop;
        i++;
    }
}

// ============= Violations: Multiple breaks in do-while loop =============

void test_dowhile_multiple_breaks(int x, int y) {
    int i = 0;
    do {
        if (x > 10)
            break;
        if (y > 20)
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: loop has more than one break or goto terminator
            break;
        i++;
    } while (i < 100);
}

// ============= Violations: Multiple breaks in for loop =============

void test_for_multiple_breaks(int x, int y) {
    for (int i = 0; i < 100; i++) {
        if (x > 10)
            break;
        if (y > 20)
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: loop has more than one break or goto terminator
            break;
    }
}

// ============= Violations: Multiple gotos in for loop =============

void test_for_multiple_gotos(int x, int y) {
done:
    for (int i = 0; i < 100; i++) {
        if (x > 10)
            goto done;
        if (y > 20)
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: loop has more than one break or goto terminator
            goto done;
    }
}

// ============= Compliant: Nested loop breaks don't count =============

void test_nested_loop_break(int x) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (x > 10)
                break;  // This breaks inner loop only
        }
        if (x < -10)
            break;  // Only one break for outer loop - OK
    }
}

// ============= Compliant: Switch case breaks don't count =============

void test_switch_break_in_loop(int x, int y) {
    for (int i = 0; i < 100; i++) {
        switch (x) {
            case 1:
                break;  // Switch break, not loop break
            case 2:
                break;  // Switch break, not loop break
            default:
                break;  // Switch break, not loop break
        }
        if (y > 20)
            break;  // Only one loop break - OK
    }
}

// ============= Compliant: Single break =============

void test_single_break(int x) {
    while (x < 100) {
        if (x == 42)
            break;  // Only one break - OK
        x++;
    }
}

// ============= Compliant: No breaks =============

void test_no_breaks(void) {
    for (int i = 0; i < 100; i++) {
        // Do something without break
        (void)i;
    }
}

// ============= Compliant: Single goto =============

void test_single_goto(int x) {
loop_end:
    while (x < 100) {
        if (x == 42)
            goto loop_end;  // Only one terminator - OK
        x++;
    }
}
