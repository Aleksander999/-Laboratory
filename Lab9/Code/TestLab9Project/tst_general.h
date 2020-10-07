#ifndef TST_GENERAL_H
#define TST_GENERAL_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lockcontroller.h"

using namespace testing;

class KeypadMock : public IKeypad{
public:
    MOCK_METHOD(
        bool, isActive, (), (override)
    );
    MOCK_METHOD(
        void, wait, (), (override)
    );
    MOCK_METHOD(
        PasswordResponse, requestPassword, (), (override)
    );
};

class LatchMock : public ILatch{
public:
    MOCK_METHOD(
        bool, isActive, ()
    );
    MOCK_METHOD(
        DoorStatus, open, ()
    );
    MOCK_METHOD(
        DoorStatus, close, ()
    );
    MOCK_METHOD(
        DoorStatus, getDoorStatus, ()
    );
};

TEST(DoorLocker, TestEx1)
{
    KeypadMock keypad;
    LatchMock latch;
    LockController locker(&keypad, &latch);

    EXPECT_CALL(keypad, wait())
        .Times(1);

    locker.wait();
}

TEST(DoorLocker, TestEx2)
{
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);

    EXPECT_CALL(latch, getDoorStatus())
        .Times(1)
        .WillOnce(
            Return(DoorStatus::CLOSE)
        );

    ASSERT_EQ(locker.isDoorOpen(), false);
}

TEST(DoorLocker, TestEx3)
{
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);

    EXPECT_CALL(latch, getDoorStatus())
        .Times(1)
        .WillOnce(
            Return(DoorStatus::OPEN)
        );

    ASSERT_EQ(locker.isDoorOpen(), true);
}

TEST(DoorLocker, TestEx4)
{
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);

    EXPECT_CALL(latch, open())
        .Times(1)
        .WillOnce(
            Return(DoorStatus::OPEN)
        );

    ASSERT_EQ(locker.unlockDoor(), DoorStatus::OPEN);
}

TEST(DoorLocker, TestEx5)
{
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);

    EXPECT_CALL(latch, open())
        .Times(1)
        .WillOnce(
            Return(DoorStatus::OPEN)
        );

    ASSERT_EQ(locker.lockDoor(), DoorStatus::CLOSE);
}

TEST(DoorLocker, TestEx6)
{
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);

    EXPECT_CALL(keypad, isActive())
       .Times(1)
       .WillOnce(
            Return(true)
        );

    EXPECT_CALL(latch, isActive())
       .Times(1)
       .WillOnce(
            Return(true)
        );

    ASSERT_EQ(locker.hardWareCheck(), HardWareStatus::OK);
}

TEST(DoorLocker, TestEx7)
{
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(nullptr, &latch);


    EXPECT_CALL(keypad, isActive())
            .Times(AtLeast(0))
            .WillOnce(
                Return(true)
            );

    EXPECT_CALL(latch, isActive())
            .Times(AtLeast(0))
            .WillOnce(
                Return(true)
            );

    ASSERT_EQ(locker.hardWareCheck(), HardWareStatus::ERROR);
}

TEST(DoorLocker, TestEx8)
{
    KeypadMock keypad;
    LatchMock latch;
    LockController locker(&keypad, &latch);

    EXPECT_CALL(keypad, isActive())
            .Times(AtLeast(0))
            .WillOnce(
                Return(true)
            );

    EXPECT_CALL(latch, isActive())
            .Times(AtLeast(0))
            .WillOnce(
                Return(false)
            );

    ASSERT_EQ(locker.hardWareCheck(), HardWareStatus::ERROR);
}

TEST(DoorLocker, TestEx9){
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);

    PasswordResponse response{
        PasswordResponse::Status::OK,
        "0000"
    };

    EXPECT_CALL(keypad, requestPassword)
        .Times(1)
        .WillOnce(
            Return(response)
        );

    ASSERT_EQ(locker.isCorrectPassword(), true);
}

TEST(DoorLocker, TestEx10){
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);

    PasswordResponse response{
        PasswordResponse::Status::OK,
        "7777"
    };

    EXPECT_CALL(keypad, requestPassword)
        .Times(1)
        .WillOnce(
            Return(response)
        );

    ASSERT_EQ(locker.isCorrectPassword(), false);
}

TEST(DoorLocker, TestEx11){
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);


    PasswordResponse pswdDefault{
        PasswordResponse::Status::OK,
        "0000"
    };

    PasswordResponse pswdNew{
        PasswordResponse::Status::OK,
        "0000"
    };

    EXPECT_CALL(keypad, requestPassword)
            .Times(3)
            .WillOnce(Return(pswdDefault))
            .WillOnce(Return(pswdNew))
            .WillOnce(Return(pswdNew));

    locker.resetPassword();

    ASSERT_EQ(locker.isCorrectPassword(), true);
}

TEST(DoorLocker, TestEx12){
    KeypadMock keypad;
    LatchMock latch;

    LockController locker(&keypad, &latch);


    PasswordResponse pswdDefault{
        PasswordResponse::Status::OK,
        "0000"
    };

    PasswordResponse pswdChanged{
        PasswordResponse::Status::OK,
        "9876"
    };

    PasswordResponse pswdNew{
        PasswordResponse::Status::OK,
        "7777"
    };

    EXPECT_CALL(keypad, requestPassword)
        .Times(5)
        .WillOnce(Return(pswdDefault))
        .WillOnce(Return(pswdChanged))
        .WillOnce(Return(pswdChanged))
        .WillOnce(Return(pswdNew))
        .WillOnce(Return(pswdNew));

    locker.resetPassword();
    locker.resetPassword();

    ASSERT_EQ(locker.isCorrectPassword(), true);
}

#endif // TST_GENERAL_H
