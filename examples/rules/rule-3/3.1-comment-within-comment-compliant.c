// MISRA C:2025 Rule 3.1 - Comment sequences within comments
// Category: Required
// Status: COMPLIANT
//
// Comments do not contain nested comment sequences.

// COMPLIANT: No nested comment characters

/* COMPLIANT: Block comment without nested sequences.
   This is a properly formatted block comment
   that spans multiple lines. */

// COMPLIANT: URLs can be written without the protocol
// See example.com/docs for details
// Or use: example[dot]com/docs

// COMPLIANT: Code examples in comments use different notation
// Instead of: if (x /* comment */) { }
// Write: if (x [see note 1]) { }

/*
 * COMPLIANT: Multi-line block comment
 * with asterisk decoration
 * - No nested comment characters
 * - Clear formatting
 */

// For documenting comment syntax, use escape or alternative notation:
// - Use [slash][star] instead of showing actual sequences
// - Or describe: "opening block comment sequence"
