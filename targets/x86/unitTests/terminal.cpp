#include <array>
#include "gtest/gtest.h"

#include "terminal.h"

using terminal::Command;
using terminal::Terminal;
using std::cout;
using std::endl;


template<typename... T>
struct CommandTest : public Command<T...> {
    std::tuple<T...> args;
    bool was_invoked;

    CommandTest(const char *name) : Command<T...>(name), was_invoked(false) {}

    void callback(T... args_) override {
        args = std::tuple<T...>{args_...};
        cout << "callback " << this->name << endl;
        was_invoked = true;
    }

    void check(T... t) {
        EXPECT_TRUE(was_invoked);
        was_invoked = false;
        EXPECT_EQ(std::tuple<T...>({t...}), args);
    }

    void nope() {
        EXPECT_FALSE(was_invoked);
    }
};

struct Command1 : public CommandTest<int, int> {
    Command1() : CommandTest<int, int>("abc") {}
};

struct Command2 : public CommandTest<> {
    Command2() : CommandTest<>("noargs") {}
};

struct Command3 : public CommandTest<float> {
    Command3() : CommandTest<float>("flt") {}
};

struct Command4 : public CommandTest<float, int, char*> {
    Command4() : CommandTest<float, int, char*>("flt") {}
};

Command1 cmd1;
Command2 cmd2;
Command3 cmd3;
Command4 cmd4;

template<typename T, typename... Args>
void check(T& cmd, Args... args) {
    cmd.check(args...);

    cmd1.nope();
    cmd2.nope();
    cmd3.nope();
    cmd4.nope();
}

void check_none() {
    cmd1.nope();
    cmd2.nope();
    cmd3.nope();
    cmd4.nope();
}

static char line[100];
struct TerminalTest : public ::testing::Test {
    Terminal<Command1, Command2, Command3, Command4> terminal;
    TerminalTest() : terminal(cmd1, cmd2, cmd3, cmd4) {

    }

    void SetUp() override {
    }

    void parse(const char * chr) {
        strcpy(line, chr);
        terminal.parse_line(line);
    }
};


TEST_F(TerminalTest, simple) {
    parse("abc 1 2");
    check(cmd1, 1, 2);

    parse("noargs");
    check(cmd2);

    parse("flt 3.14");
    check(cmd3, 3.14);

    parse("flt 3.1415 10 test");
    check(cmd4, 3.1415f, 10, line+14);
}

TEST_F(TerminalTest, noCommand) {
    parse("teeeee 5");
    check_none();

    parse("tester 1 2");
    check_none();

    parse("");
    check_none();

    parse("x");
    check_none();
}

TEST_F(TerminalTest, incorrectNrOfArugments) {
    parse("abc");
    check_none();

    parse("abc 1");
    check_none();

    parse("abc 1 2 3");
    check_none();

    parse("noargs 2");
    check_none();

    parse("noargs 2 4");
    check_none();
}

TEST_F(TerminalTest, whitespace) {
    parse("noargs");
    check(cmd2);

    parse("noargs ");
    check(cmd2);

    parse("noargs\t");
    check(cmd2);

    parse("noargs\n");
    check(cmd2);

    parse("flt\t   3.14    \t");
    check(cmd3, 3.14);

    parse("abc                  1750 \t\t\t\n2410    ");
    check(cmd1, 1750, 2410);
}