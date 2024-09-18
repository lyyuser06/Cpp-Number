#include "Integer.hh"
#include <iostream>
#include <cassert>

extern Debugger debug;

void test_large_numbers() {
    // Large numbers addition
    Integer a(true, "999999999999999999");
    Integer b(true, "888888888888888888");
    Integer c = a + b;
    std::cout << "999999999999999999 + 888888888888888888 = " << c;  // Expected: 1888888888888888887
    assert(c.getSign() == true);
    assert(strcmp(c.getNum(), "1888888888888888887") == 0);

    // Large numbers subtraction
    Integer d(false, "1234567890123456789");
    Integer e = a - d;
    std::cout << "999999999999999999 - (-1234567890123456789) = " << e;  // Expected: 2234567890123456788
    assert(e.getSign() == true);
    assert(strcmp(e.getNum(), "2234567890123456788") == 0);

    Integer a1(true, "999999999999999999");
    Integer b1(true, "88888888888888");
    Integer c1 = a1 + b1;
    std::cout << "999999999999999999 + 88888888888888 = " << c1;  // Expected: 1888888888888888887
    

    Integer a2(true, "999999999999999999");
    Integer b2(true, "99999999999");
    Integer c2 = a2 - b2;
    std::cout << "999999999999999999 - 999999999999 = " << c2;  // Expected: 1888888888888888887

    Integer a3(true, "999999999999999999");
    Integer b3(true, "8888888888888888888");
    Integer c3 = a3 - b3;
    std::cout << "999999999999999999 - 888888888888888888 = " << c3;  // Expected: 1888888888888888887
}


void test_opposite_numbers() {
    // Opposite numbers addition
    Integer a(true, "123456789");
    Integer b(false, "123456789");
    Integer c = a + b;
    std::cout << "123456789 + (-123456789) = " << c;  // Expected: 0
    assert(c.getSign() == true);
    assert(strcmp(c.getNum(), "0") == 0);
}

void test_zero_operations() {
    // Zero addition and subtraction
    Integer zero(true, "0");
    Integer a(true, "12345");
    Integer b(false, "12345");

    Integer c = zero + a;
    std::cout << "0 + 12345 = " << c;  // Expected: 12345
    assert(c.getSign() == true);
    assert(strcmp(c.getNum(), "12345") == 0);

    Integer d = zero + b;
    std::cout << "0 + (-12345) = " << d;  // Expected: -12345
    assert(d.getSign() == false);
    assert(strcmp(d.getNum(), "12345") == 0);

    Integer e = zero - a;
    std::cout << "0 - 12345 = " << e;  // Expected: -12345
    assert(e.getSign() == false);
    assert(strcmp(e.getNum(), "12345") == 0);

    Integer f = a - zero;
    std::cout << "12345 - 0 = " << f;  // Expected: 12345
    assert(f.getSign() == true);
    assert(strcmp(f.getNum(), "12345") == 0);

    Integer g = zero - zero;
    std::cout << "0 - 0 = " << g;  // Expected: 0
    assert(g.getSign() == true);
    assert(strcmp(g.getNum(), "0") == 0);
}

void test_int_boundary() {
    // Integer boundary values addition and subtraction
    Integer max_int(true, "2147483647");  // INT_MAX
    Integer min_int(false, "2147483648"); // INT_MIN (in absolute value)

    // Addition INT_MAX + 1 should overflow
    try {
        Integer overflow = max_int + Integer(true, "1");
        std::cout << "Overflow test failed." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "INT_MAX + 1 correctly caused overflow: " << e.what();
    }

    // Addition INT_MIN - 1 should overflow
    try {
        Integer underflow = min_int - Integer(true, "1");
        std::cout << "Underflow test failed." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "INT_MIN - 1 correctly caused underflow: " << e.what();
    }

    // INT_MAX + INT_MIN should result in -1
    Integer result = max_int + min_int;
    std::cout << "2147483647 + (-2147483648) = " << result;  // Expected: -1
    assert(result.getSign() == false);
    assert(strcmp(result.getNum(), "1") == 0);
}

void test_mul()
{
    Integer mi(true, "987654321987654321");  
    Integer ni(true, "12345671234567"); 

    Integer mi1(false, "987654321987654321");  
    Integer ni1(true, "12345671234567"); 

    Integer mi2(true, "987654321987654321");  
    Integer ni2(false, "12345671234567"); 

    Integer mi3(false, "987654321987654321");  
    Integer ni3(false, "12345671234567"); 

    Integer mi4(true, "12345671234567");  
    Integer ni4(true, "987654321987654321"); 

    Integer mi5(false, "12345671234567");  
    Integer ni5(true, "987654321987654321"); 

    Integer mi6(true, "12345671234567");  
    Integer ni6(false, "987654321987654321");

    Integer mi7(false, "12345671234567");  
    Integer ni7(false, "987654321987654321");  

    Integer mi8(true, "0");  
    Integer ni8(false, "987654321987654321");

    Integer mi9(true, "0");  
    Integer ni9(true, "987654321987654321");

    Integer mi10(true, "0");  
    Integer ni10(true, "0");

    Integer mn = mi * ni;
    std::cout << "m * n = " << mn << std::endl;

    Integer mn1 = mi1 * ni1;
    std::cout << "m * n = " << mn1 << std::endl;

    Integer mn2 = mi2 * ni2;
    std::cout << "m * n = " << mn2 << std::endl;  

    Integer mn3 = mi3 * ni3;
    std::cout << "m * n = " << mn3 << std::endl;

    Integer mn4 = mi4 * ni4;
    std::cout << "m * n = " << mn4 << std::endl;

    Integer mn5 = mi5 * ni5;
    std::cout << "m * n = " << mn5 << std::endl;  

    Integer mn6 = mi6 * ni6;
    std::cout << "m * n = " << mn6 << std::endl;  

    Integer mn7 = mi7 * ni7;
    std::cout << "m * n = " << mn7 << std::endl;  

    Integer mn8 = mi8 * ni8;
    std::cout << "m * n = " << mn8 << std::endl;  

    Integer mn9 = mi9 * ni9;
    std::cout << "m * n = " << mn9 << std::endl;

    Integer mn10 = mi10 * ni10;
    std::cout << "m * n = " << mn10 << std::endl;    
}

int main() {
    /*
    std::cout << "Testing Large Numbers:" << std::endl;
    test_large_numbers();
    std::cout << "Large Numbers Tests Passed!" << std::endl;

    std::cout << "Testing Opposite Numbers:" << std::endl;
    test_opposite_numbers();
    std::cout << "Opposite Numbers Tests Passed!" << std::endl;

    std::cout << "Testing Zero Operations:" << std::endl;
    test_zero_operations();
    std::cout << "Zero Operations Tests Passed!" << std::endl;

    std::cout << "Testing Integer Boundary Values:" << std::endl;
    test_int_boundary();
    std::cout << "Integer Boundary Tests Passed!" << std::endl;
    */
    // debug.debugModeOn();
    test_mul();
    return 0;
}
