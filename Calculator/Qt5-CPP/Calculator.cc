//
// Created by rick on 19-3-11.
//
#include "Calculator.h"
#include "Button.h"
#include <QtWidgets>

constexpr int kNumDigitButtons = 10;

struct CalculatorPrivate {
    Calculator *calculator;

    double sumInMemory;
    double sumSoFar;
    double factorSoFar;
    QString pendingAdditiveOperator;
    QString pendingMultiplicativeOperator;
    bool waitingForOperand;

    QLineEdit *display;
    Button *digitButtons[kNumDigitButtons];

    CalculatorPrivate(Calculator *calculator) : calculator{calculator} {}

    void initialize() {
      sumInMemory = 0.0;
      sumSoFar = 0.0;
      factorSoFar = 0.0;
      waitingForOperand = true;
      display = new QLineEdit("0");
      display->setReadOnly(true);
      display->setAlignment(Qt::AlignRight);
      display->setMaxLength(15);

      QFont font = display->font();
      font.setPointSize(font.pointSize() + 8);
      display->setFont(font);

      // digit buttons
      for (auto i = 0; i < kNumDigitButtons; i++)
        digitButtons[i] = createButton(QString::number(i), CalculatorPrivate::digitClicked);

      Button *pointButton = createButton(".", CalculatorPrivate::pointClicked);
      Button *changeSignButton = createButton("\302\261", CalculatorPrivate::changeSignClicked);

      Button *backspaceButton = createButton("Backspace", CalculatorPrivate::backspaceClicked);
      Button *clearButton = createButton("Clear",CalculatorPrivate::clearClicked);
      Button *clearAllButton = createButton("Clear All",CalculatorPrivate::clearAllClicked);
    }

    static Button *createButton(const QString &text, std::function<void(Button *)> clicked) {
      auto *button = new Button(text);
      QObject::connect(button, &QAbstractButton::clicked, [=]() {
          clicked(button);
      });
      return button;
    }

    static void clearClicked(Button* sender){}
    static void digitClicked(Button *sender) {}

    static void pointClicked(Button *sender) {}

    static void changeSignClicked(Button *sender) {}

    static void backspaceClicked(Button *sender) {}

    static void clearAllClicked(Button *sender){}
};

Calculator::Calculator(QWidget *parent) : QWidget{parent}, d_ptr{new CalculatorPrivate(this)} {
  Q_D(Calculator);
  d->initialize();
}

