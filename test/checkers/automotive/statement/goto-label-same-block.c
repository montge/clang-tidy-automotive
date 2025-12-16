// RUN: %check_clang_tidy %s automotive-c23-req-15.2 %t

// Test: Goto and label in same block (MISRA Rule 15.2)
// Check ID: automotive-c23-req-15.2

// ============= Violations: Goto jumps to label in different block =============

void test_goto_different_block(int x) {
    if (x > 0) {
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: goto statement and its label 'done' are not in the same block
        goto done;
    }
done:
    return;
}

void test_goto_out_of_block(int x) {
    {
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: goto statement and its label 'outer' are not in the same block
        goto outer;
    }
outer:
    (void)x;
}

void test_goto_into_block(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: goto statement and its label 'inner' are not in the same block
    goto inner;
    {
inner:
        return;
    }
}

void test_goto_across_if_else(int x) {
    if (x > 0) {
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: goto statement and its label 'else_label' are not in the same block
        goto else_label;
    } else {
else_label:
        return;
    }
}

void test_goto_from_loop(int x) {
    while (x > 0) {
        if (x == 5) {
            // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: goto statement and its label 'after_loop' are not in the same block
            goto after_loop;
        }
        x--;
    }
after_loop:
    return;
}

// ============= Compliant: Goto and label in same block =============

void test_same_block(void) {
    goto end;
    (void)0;
end:
    return;
}

void test_same_block_forward(int x) {
    if (x > 0) {
        goto skip;
        (void)x;
skip:
        return;
    }
}

void test_same_block_backward(void) {
again:
    (void)0;
    goto again;
}

void test_same_block_in_if(int x) {
    if (x > 0) {
start:
        (void)x;
        if (x > 100)
            goto start;
    }
}
