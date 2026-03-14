// RUN: %clang_cc1 -fcxx-exceptions -fexceptions -load %llvmshlibdir/gutyansky_a_lab1_ClangAST%pluginext -plugin gutyansky_a_ast_noexcept_plugin -fsyntax-only %s 2>&1 | FileCheck %s
// CHECK: Function empty updated to noexcept
// CHECK-NEXT: Function emptyNoExcept was not updated
// CHECK-NEXT: Function simpleNoThrow updated to noexcept
// CHECK-NEXT: Function simpleThrow was not updated
// CHECK-NEXT: Function conditionalThrow was not updated
// CHECK-NEXT: Function simpleCall updated to noexcept
// CHECK-NEXT: Function simpleCallThrow was not updated
// CHECK-NEXT: Function lambdaThrow was not updated
// CHECK-NEXT: Function lambdaNoThrow updated to noexcept
// CHECK-NEXT: Function newThrow was not updated
// CHECK-NEXT: Function fact updated to noexcept
// CHECK-NEXT: Function recursiveThrow was not updated
// CHECK-NEXT: Function pointerCall was not updated
// CHECK-NEXT: Function Bar was not updated
// CHECK-NEXT: Function Bar updated to noexcept
// CHECK-NEXT: Function methodNoThrow updated to noexcept
// CHECK-NEXT: Function methodThrow was not updated
// CHECK-NEXT: Function methodVirtualNoThrow was not updated
// CHECK-NEXT: Function constructNoThrow updated to noexcept
// CHECK-NEXT: Function constructThrow was not updated
// CHECK-NEXT: Function callMethodNoThrow updated to noexcept
// CHECK-NEXT: Function callMethodThrow was not updated
// CHECK-NEXT: Function callMethodVirtual was not updated

void empty() {}

void emptyNoExcept() noexcept {}

int simpleNoThrow() { return -1; }

void simpleThrow() { 
    throw "Exception";
}

void conditionalThrow(bool cond) {
    if (cond) {
        throw "EEEE";
    }
}

int simpleCall() {
    return simpleNoThrow();
}

void simpleCallThrow() {
    conditionalThrow(false);
}

void lambdaThrow() {
    auto f = []() { throw "aaaa"; };
    f();
}

void lambdaNoThrow() {
    auto f = []() { int b = 2 + 3; };
    f();
}

void newThrow() {
    int* a = new int;
    delete a;
}

int fact(int n) {
    if (n <= 1) return 1;
    return n * fact(n - 1);
}

int recursiveThrow(int n) {
    if (n <= 1) throw 1;
    return n * fact(n - 1);
}

void pointerCall(void (*f)(int)) {
    f(123);
}

class Bar {
public:
    Bar() { throw 0; }
    Bar(int x) {}

    void methodNoThrow() {}
    void methodThrow() { throw 42; }

    virtual void methodVirtualNoThrow() {}
};

void constructNoThrow() {
    Bar b(111);
}

void constructThrow() {
    Bar b;
}

void callMethodNoThrow() {
    Bar b(111);
    b.methodNoThrow();
}

void callMethodThrow() {
    Bar b(111);
    b.methodThrow();
}

void callMethodVirtual() {
    Bar b(111);
    b.methodVirtualNoThrow();
}