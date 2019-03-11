//
// Created by rick on 19-3-11.
//

#include "Button.h"

Button::Button(const QString &text, QWidget *parent):QToolButton(parent) {
  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
  setText(text);
}

QSize Button::sizeHint() const {
  QSize size = QToolButton::sizeHint();
  size.rheight() += 20;
  size.rwidth() = qMax(size.width(),size.height());
  return size;
}
