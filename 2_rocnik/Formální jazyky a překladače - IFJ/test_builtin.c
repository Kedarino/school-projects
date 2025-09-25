#include "builtin.h" // Make sure this is the correct relative path to your header file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test for readString function
void testReadString() {
    printf("Please enter a string (for testing purposes, type 'test'): ");
    char* str = readString();
    assert(strcmp(str, "test") == 0);
    free(str);
}

// Test for readInt function
void testReadInt() {
    printf("Please enter an integer (for testing purposes, type '42'): ");
    int* num = readInt();
    assert(num && *num == 42);
    free(num);
}

// Test for readDouble function
void testReadDouble() {
    printf("Please enter a double (for testing purposes, type '3.14'): ");
    double* num = readDouble();
    assert(num && *num == 3.14);
    free(num);
}

// Test for write function
void testWrite() {
    // Redirect stdout to a buffer to test the output
    fflush(stdout); // Flush any buffered data
    FILE *original_stdout = freopen("test_output.txt", "w", stdout);
    if (!original_stdout) {
        perror("freopen failed");
        exit(EXIT_FAILURE);
    }

    write("Hello, %s!", "World");
    fflush(stdout); // Make sure all output is flushed to the file

    // Close the redirected stdout to finalize the writing
    fclose(stdout);

    // Open the test output file for reading
    FILE *test_output = fopen("test_output.txt", "r");
    if (!test_output) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    // Read the output from the file
    char buffer[128];
    if (fgets(buffer, sizeof(buffer), test_output) == NULL) {
        perror("fgets failed");
        fclose(test_output);
        exit(EXIT_FAILURE);
    }
    fclose(test_output);

    // Assert the contents of the buffer
    assert(strcmp(buffer, "Hello, World!") == 0);

    // Clean up test output file
    remove("test_output.txt");

    // No need to reopen stdout if you never closed it
    // stdout will still be open and pointing to the original standard output
}



// Test for Int2Double function
void testInt2Double() {
    double result = Int2Double(42);
    assert(result == 42.0);
}

// Test for Double2Int function
void testDouble2Int() {
    int result = Double2Int(3.14);
    assert(result == 3);
}

// Test for length function
void testLength() {
    int len = length("test");
    assert(len == 4);
}

// Test for substring function
void testSubstring() {
    char str[] = "Hello, World!";
    char* sub = substring(str, 0, 5);
    assert(strcmp(sub, "Hello") == 0);
    free(sub);
}

// Test for ord function
void testOrd() {
    int value = ord("A");
    assert(value == 'A');
}

// Test for chr function
void testChr() {
    char* str = chr(65);
    assert(strcmp(str, "A") == 0);
    free(str);
}

int main() {
    // Call each test function
    testReadString();
    printf("testread string good");
    testReadInt();
    testReadDouble();
    testWrite();
    testInt2Double();
    testDouble2Int();
    testLength();
    testSubstring();
    testOrd();
    testChr();

    printf("All tests passed successfully.\n");
    return 0;
}