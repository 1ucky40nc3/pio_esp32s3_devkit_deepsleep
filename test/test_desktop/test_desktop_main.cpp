using namespace std;
#include <unity.h>

int add(int a, int b)
{
    return a + b;
}

void test_add()
{
    TEST_ASSERT_EQUAL_INT(2, add(1, 1));
}

void setUp() {}

void tearDown() {}

int main()
{
    UNITY_BEGIN(); // Initialize Unity

    // Run the tests
    RUN_TEST(test_add);

    UNITY_END(); // End Unity

    return 0;
}