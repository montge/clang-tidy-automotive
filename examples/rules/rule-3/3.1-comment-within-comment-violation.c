// MISRA C:2025 Rule 3.1 - Comment sequences within comments
// Category: Required
// Status: VIOLATION
//
// The character sequences /* and // shall not be used within a comment.
//
// Expected diagnostic:
//   warning: comment sequence within comment [automotive-avoid-comment-within-comment]

// VIOLATION: /* inside C++ style comment
// This /* nested sequence is not allowed

/* VIOLATION: // inside C style comment
   This // sequence should not appear
   in a block comment */

/* VIOLATION: Nested /* block comment
   The second /* starts here
   but there's no way to close both */

// VIOLATION: URL in comment contains //
// See http://example.com/docs for details

// Note: This rule helps prevent accidental commenting issues
// where code may be unintentionally commented out or
// comment structure becomes ambiguous.
