//
// Created by rick on 19-3-11.
//
#include "Calculator.h"
#include "Button.h"
#include <QtWidgets>
#include <functional>

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

      using namespace std::placeholders;
      // digit buttons
      for (auto i = 0; i < kNumDigitButtons; i++)
        digitButtons[i] = createButton(QString::number(i), std::mem_fn(&CalculatorPrivate::digitClicked));

      Button *pointButton = createButton(".", std::mem_fn(&CalculatorPrivate::pointClicked));
      Button *changeSignButton = createButton("\302\261", std::mem_fn(&CalculatorPrivate::changeSignClicked));

      Button *backspaceButton = createButton("Backspace", &CalculatorPrivate::backspaceClicked);
      Button *clearButton = createButton("Clear", &CalculatorPrivate::clearClicked);
      Button *clearAllButton = createButton("Clear All", &CalculatorPrivate::clearAllClicked);

      Button *clearMemoryButton = createButton("MC", &CalculatorPrivate::clearMemoryClicked);
      Button *readMemoryButton = createButton("MR", &CalculatorPrivate::readMemoryClicked);
      Button *setMemoryButton = createButton("MS", &CalculatorPrivate::setMemoryClicked);
      Button *addToMemoryButton = createButton("M+", &CalculatorPrivate::addToMemoryClicked);

      Button *divisionButton = createButton("\303\267", &CalculatorPrivate::multiplicativeOperatorClicked);
      Button *timesButton = createButton("\303\227", &CalculatorPrivate::multiplicativeOperatorClicked);
      Button *minusButton = createButton("-", &CalculatorPrivate::additiveOperatorClicked);
      Button *plusButton = createButton("+", &CalculatorPrivate::additiveOperatorClicked);

      Button *squareRootButton = createButton("Sqrt", &CalculatorPrivate::unaryOperatorClicked);
      Button *powerButton = createButton("x\302\262", &CalculatorPrivate::unaryOperatorClicked);
      Button *reciprocalButton = createButton("1/x", &CalculatorPrivate::unaryOperatorClicked);
      Button *equalButton = createButton("=", &CalculatorPrivate::equalClicked);

      auto *mainLayout = new QGridLayout;
      mainLayout->setSizeConstraint(QLayout::SetFixedSize);
      mainLayout->addWidget(display, 0, 0, 1, 6);
      mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
      mainLayout->addWidget(clearButton, 1, 2, 1, 2);
      mainLayout->addWidget(clearAllButton, 1, 4, 1, 2);

      mainLayout->addWidget(clearMemoryButton, 2, 0);
      mainLayout->addWidget(readMemoryButton, 3, 0);
      mainLayout->addWidget(setMemoryButton, 4, 0);
      mainLayout->addWidget(addToMemoryButton, 5, 0);

      for (auto i = 1; i < kNumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        mainLayout->addWidget(digitButtons[i], row, column);
      }

      mainLayout->addWidget(digitButtons[0], 5, 1);
      mainLayout->addWidget(pointButton, 5, 2);
      mainLayout->addWidget(changeSignButton, 5, 3);

      mainLayout->addWidget(divisionButton, 2, 4);
      mainLayout->addWidget(timesButton, 3, 4);
      mainLayout->addWidget(minusButton, 4, 4);
      mainLayout->addWidget(plusButton, 5, 4);

      mainLayout->addWidget(squareRootButton, 2, 5);
      mainLayout->addWidget(powerButton, 3, 5);
      mainLayout->addWidget(reciprocalButton, 4, 5);
      mainLayout->addWidget(equalButton, 5, 5);

      calculator->setLayout(mainLayout);
      calculator->setWindowTitle("Calculator");
    }

    Button *createButton(const QString &text, void (CalculatorPrivate::*clicked)(Button *)) {
      return createButton(text, std::mem_fn(clicked));
    }

    Button *createButton(const QString &text, std::function<void(CalculatorPrivate *, Button *)> clicked) {
      auto *button = new Button(text);
      QObject::connect(button, &QAbstractButton::clicked, [this, clicked, button]() {
          clicked(this, button);
      });
      return button;
    }

    void equalClicked(Button *sender) {}

    void unaryOperatorClicked(Button *sender) {}

    void additiveOperatorClicked(Button *sender) {}

    void multiplicativeOperatorClicked(Button *sender) {}

    void addToMemoryClicked(Button *sender) {

    }

    void setMemoryClicked(Button *sender) {}

    void readMemoryClicked(Button *sender) {

    }

    void clearClicked(Button *sender) {}

    void digitClicked(Button *sender) {
      int digitValue = sender->text().toInt();
      if (display->text() == "0" && digitValue == 0.0)
        return;

      if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
      }
      display->setText(display->text() + QString::number(digitValue));
    }

    void pointClicked(Button *sender) {}

    void changeSignClicked(Button *sender) {}

    void backspaceClicked(Button *sender) {}

    void clearAllClicked(Button *sender) {}

    void clearMemoryClicked(Button *sender) {}
};

Calculator::Calculator(QWidget *parent) : QWidget{parent}, d_ptr{new CalculatorPrivate(this)} {
  Q_D(Calculator);
  d->initialize();
}

Calculator::~Calculator() {

}

