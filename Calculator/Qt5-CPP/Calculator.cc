//
// Created by rick on 19-3-11.
//
#include "Calculator.h"
#include "Button.h"
#include <QtWidgets>
#include <functional>

constexpr int kNumDigitButtons = 10;

struct CalculatorPrivate {
  Calculator* calculator;

  double sumInMemory;
  double sumSoFar;
  double factorSoFar;
  QString pendingAdditiveOperator;
  QString pendingMultiplicativeOperator;
  bool waitingForOperand;

  QLineEdit* display;
  Button* digitButtons[kNumDigitButtons];

  CalculatorPrivate(Calculator* calculator) : calculator{calculator} {}

  void initialize() {
    sumInMemory       = 0.0;
    sumSoFar          = 0.0;
    factorSoFar       = 0.0;
    waitingForOperand = true;
    display           = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);

    using namespace std::placeholders;
    // digit buttons
    for (auto i = 0; i < kNumDigitButtons; i++)
      digitButtons[i] = createButton(
          QString::number(i), std::mem_fn(&CalculatorPrivate::digitClicked));

    Button* pointButton =
        createButton(".", std::mem_fn(&CalculatorPrivate::pointClicked));
    Button* changeSignButton = createButton(
        "\302\261", std::mem_fn(&CalculatorPrivate::changeSignClicked));

    Button* backspaceButton =
        createButton("Backspace", &CalculatorPrivate::backspaceClicked);
    Button* clearButton =
        createButton("Clear", &CalculatorPrivate::clearClicked);
    Button* clearAllButton =
        createButton("Clear All", &CalculatorPrivate::clearAllClicked);

    Button* clearMemoryButton =
        createButton("MC", &CalculatorPrivate::clearMemoryClicked);
    Button* readMemoryButton =
        createButton("MR", &CalculatorPrivate::readMemoryClicked);
    Button* setMemoryButton =
        createButton("MS", &CalculatorPrivate::setMemoryClicked);
    Button* addToMemoryButton =
        createButton("M+", &CalculatorPrivate::addToMemoryClicked);

    Button* divisionButton = createButton(
        "\303\267", &CalculatorPrivate::multiplicativeOperatorClicked);
    Button* timesButton = createButton(
        "\303\227", &CalculatorPrivate::multiplicativeOperatorClicked);
    Button* minusButton =
        createButton("-", &CalculatorPrivate::additiveOperatorClicked);
    Button* plusButton =
        createButton("+", &CalculatorPrivate::additiveOperatorClicked);

    Button* squareRootButton =
        createButton("Sqrt", &CalculatorPrivate::unaryOperatorClicked);
    Button* powerButton =
        createButton("x\302\262", &CalculatorPrivate::unaryOperatorClicked);
    Button* reciprocalButton =
        createButton("1/x", &CalculatorPrivate::unaryOperatorClicked);
    Button* equalButton = createButton("=", &CalculatorPrivate::equalClicked);

    auto* mainLayout = new QGridLayout;
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
      int row    = ((9 - i) / 3) + 2;
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

  Button* createButton(const QString& text,
                       void (CalculatorPrivate::*clicked)(Button*)) {
    return createButton(text, std::mem_fn(clicked));
  }

  Button*
  createButton(const QString& text,
               std::function<void(CalculatorPrivate*, Button*)> clicked) {
    auto* button = new Button(text);
    QObject::connect(button, &QAbstractButton::clicked,
                     [this, clicked, button]() { clicked(this, button); });
    return button;
  }

  bool calculate(double rightOperand, const QString& pendingOperator) {
    if (pendingOperator == "+") {
      sumSoFar += rightOperand;
    } else if (pendingOperator == "-") {
      sumSoFar -= rightOperand;
    } else if (pendingOperator == "\303\227") {
      factorSoFar *= rightOperand;
    } else if (pendingOperator == "\303\267") {
      if (rightOperand == 0.0)
        return false;
      factorSoFar /= rightOperand;
    }
    return true;
  }

  void clearAll() {
    sumSoFar    = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    display->setText("0");
    waitingForOperand = true;
  }
  void abortOperation() {
    clearAll();
    display->setText("####");
  }
  void equalClicked(Button* sender) {
    double operand = display->text().toDouble();
    if (!pendingMultiplicativeOperator.isEmpty()) {
      if (!calculate(operand, pendingMultiplicativeOperator)) {
        abortOperation();
        return;
      }
      operand     = factorSoFar;
      factorSoFar = 0.0;
      pendingMultiplicativeOperator.clear();
    }

    if (!pendingAdditiveOperator.isEmpty()) {
      if (!calculate(operand, pendingAdditiveOperator)) {
        abortOperation();
        return;
      }
      pendingAdditiveOperator.clear();
    } else {
      sumSoFar = operand;
    }
    display->setText(QString::number(sumSoFar));
    sumSoFar          = 0.0;
    waitingForOperand = true;
  }

  void unaryOperatorClicked(Button* sender) {
    QString clickedOperator = sender->text();
    double operand          = display->text().toDouble();
    double result           = 0.0;

    if (clickedOperator == "Sqrt") {
      if (operand < 0.0) {
        abortOperation();
        return;
      }
      result = std::sqrt(operand);
    } else if (clickedOperator == "x\302\262") {
      result = std::pow(operand, 2.0);
    } else if (clickedOperator == "1/x") {
      if (operand == 0.0) {
        abortOperation();
        return;
      }
      result = 1.0 / operand;
    }
    display->setText(QString::number(result));
    waitingForOperand = true;
  }

  void additiveOperatorClicked(Button* sender) {
    QString clickedOperator = sender->text();
    double operand          = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
      if (!calculate(operand, pendingMultiplicativeOperator)) {
        abortOperation();
        return;
      }
      display->setText(QString::number(factorSoFar));
      operand     = factorSoFar;
      factorSoFar = 0.0;
      pendingMultiplicativeOperator.clear();
    }

    if (!pendingAdditiveOperator.isEmpty()) {
      if (!calculate(operand, pendingAdditiveOperator)) {
        abortOperation();
        return;
      }
      display->setText(QString::number(sumSoFar));
    } else {
      sumSoFar = operand;
    }

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand       = true;
  }

  void multiplicativeOperatorClicked(Button* sender) {
    QString clickedOperator = sender->text();
    double operand          = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
      if (!calculate(operand, pendingMultiplicativeOperator)) {
        abortOperation();
        return;
      }
      display->setText(QString::number(factorSoFar));
    } else {
      factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand             = true;
  }

  void addToMemoryClicked(Button* sender) {
    equalClicked(sender);
    sumInMemory += display->text().toDouble();
  }

  void setMemoryClicked(Button* sender) {
    equalClicked(sender);
    sumInMemory = display->text().toDouble();
  }

  void readMemoryClicked(Button* sender) {
    display->setText(QString::number(sumInMemory));
    waitingForOperand = true;
  }

  void clearClicked(Button* sender) {
//    if (waitingForOperand)
//      return;
    display->setText("0");
//    waitingForOperand = true;
  }

  void digitClicked(Button* sender) {
    int digitValue = sender->text().toInt();
    if (display->text() == "0" && digitValue == 0.0)
      return;

    if (waitingForOperand) {
      display->clear();
      waitingForOperand = false;
    }
    display->setText(display->text() + QString::number(digitValue));
  }

  void pointClicked(Button* sender) {
    if (waitingForOperand)
      display->setText("0");
    if (!display->text().contains('.'))
      display->setText(display->text() + '.');
    waitingForOperand = false;
  }

  void changeSignClicked(Button* sender) {
    QString text = display->text();
    double value = text.toDouble();
    if (value > 0.0)
      text.prepend("-");
    else if (value < 0.0)
      text.remove(0, 1);

    display->setText(text);
  }

  void backspaceClicked(Button* sender) {
    if (waitingForOperand)
      return;
    QString text = display->text();
    text.chop(1);
    if (text.isEmpty()) {
      text              = "0";
      waitingForOperand = true;
    }
    display->setText(text);
  }

  void clearAllClicked(Button* sender) { clearAll(); }

  void clearMemoryClicked(Button* sender) {
    sumInMemory = 0.0;
  }
};

Calculator::Calculator(QWidget* parent)
    : QWidget{parent}, d_ptr{new CalculatorPrivate(this)} {
  Q_D(Calculator);
  d->initialize();
}

Calculator::~Calculator() {}
