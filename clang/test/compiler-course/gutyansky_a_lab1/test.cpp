// RUN: %clang_cc1 -fcxx-exceptions -fexceptions -load %llvmshlibdir/gutyansky_a_lab1_ClangAST%pluginext -plugin gutyansky_a_ast_noexcept_plugin -fsyntax-only %s 2>&1 | FileCheck %s

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

// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> col:6 empty 'void () noexcept'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> col:6 emptyNoExcept 'void () noexcept'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> col:5 used simpleNoThrow 'int () noexcept'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:9:6 simpleThrow 'void ()'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:13:6 used conditionalThrow 'void (bool)'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:19:5 simpleCall 'int () noexcept'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:23:6 simpleCallThrow 'void ()'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:27:6 lambdaThrow 'void ()'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:32:6 lambdaNoThrow 'void () noexcept'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:37:6 newThrow 'void ()'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:42:5 used fact 'int (int) noexcept'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:47:5 recursiveThrow 'int (int)'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:52:6 pointerCall 'void (void (*)(int))'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:67:6 constructNoThrow 'void () noexcept'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:71:6 constructThrow 'void ()'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:75:6 callMethodNoThrow 'void () noexcept'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:80:6 callMethodThrow 'void ()'
// CHECK: FunctionDecl {{0x[0-9a-fA-F]+}} <{{.*}}> line:85:6 callMethodVirtual 'void ()'