//
// Created by rick on 19-3-11.
//

#pragma once
#include <memory>
#include <QWidget>

class QLineEdit;
class Button;
class CalculatorPrivate;

class Calculator: public QWidget {
    Q_OBJECT
public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Q_DECLARE_PRIVATE(Calculator)
private:
    std::unique_ptr<CalculatorPrivate> d_ptr;


};



