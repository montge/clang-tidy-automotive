// Test file for: automotive-avoid-goto (Edge Cases)
// Related MISRA C:2025 Rule: 15.1
//
// This file tests edge cases for goto detection

// RUN: %check_clang_tidy %s automotive-avoid-goto %t

//===----------------------------------------------------------------------===//
// Edge Case: Goto in nested blocks
//===----------------------------------------------------------------------===//

void test_nested_blocks(int x) {
    if (x > 0) {
        if (x > 10) {
            goto deep_label;
            // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: avoid goto statement [automotive-avoid-goto]
        }
    }
deep_label:
    return;
}

//===----------------------------------------------------------------------===//
// Edge Case: Multiple goto to same label
//===----------------------------------------------------------------------===//

void test_multiple_goto(int x, int y) {
    if (x < 0) {
        goto cleanup;
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: avoid goto statement [automotive-avoid-goto]
    }
    if (y < 0) {
        goto cleanup;
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: avoid goto statement [automotive-avoid-goto]
    }
cleanup:
    return;
}

//===----------------------------------------------------------------------===//
// Edge Case: Goto in switch statement
//===----------------------------------------------------------------------===//

void test_goto_in_switch(int cmd) {
    switch (cmd) {
        case 1:
            goto error_handler;
            // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: avoid goto statement [automotive-avoid-goto]
        case 2:
            break;
        default:
            break;
    }
error_handler:
    return;
}

//===----------------------------------------------------------------------===//
// Edge Case: Goto in loop body
//===----------------------------------------------------------------------===//

void test_goto_in_loop(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] < 0) {
            goto loop_exit;
            // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: avoid goto statement [automotive-avoid-goto]
        }
    }
loop_exit:
    return;
}

//===----------------------------------------------------------------------===//
// Edge Case: Forward and backward goto
//===----------------------------------------------------------------------===//

void test_forward_backward(void) {
backward_target:
    goto forward_target;  // Forward jump
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: avoid goto statement [automotive-avoid-goto]
forward_target:
    goto backward_target; // Backward jump
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: avoid goto statement [automotive-avoid-goto]
}

//===----------------------------------------------------------------------===//
// Compliant: No goto present
//===----------------------------------------------------------------------===//

void test_no_goto(int x) {
    if (x > 0) {
        return;
    }
    // No goto, should not trigger
}

int test_loop_without_goto(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] < 0) {
            return -1;  // Using return instead of goto
        }
    }
    return 0;
}
