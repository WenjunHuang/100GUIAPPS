package com.github.wenjunhuang.uiapps.calculator

import scalafx.Includes._
import scalafx.application
import scalafx.application.JFXApp
import scalafx.geometry.{Insets, Pos}
import scalafx.scene.Scene
import scalafx.scene.control.{Button, TextField}
import scalafx.scene.layout.{Border, GridPane, Region}
import scalafx.scene.text.Font
import javafx.scene.control.{Button => jfxButton}

object Calculator extends JFXApp {
  val kNumDigitButtons = 10

  stage = new application.JFXApp.PrimaryStage {
    title.value = "Calculator"
    scene = new Scene {
      content = new GridPane {
        hgap = 9.0
        vgap = 9.0
        padding = Insets(11.0)

        val display = new TextField {
          text = "0"
          alignment = Pos.CenterRight
          prefColumnCount = 15
          font = Font(font.value.getFamily, font.value.getSize + 8.0)
          editable = false
        }
        GridPane.setConstraints(display, 0, 0, 6, 1)
        children += display

        val digitButtons = (0 until kNumDigitButtons).map(num => createButton(num.toString, digitClicked))
        val pointButton = createButton(".", pointClicked)
        val changeSignButton = createButton("±", changeSignClicked)

        val backspaceButton = createButton("Backspace", backspaceClicked)
        val clearButton = createButton("Clear", clearClicked)
        val clearAllButton = createButton("Clear All", clearAllClicked)

        val clearMemoryButton = createButton("MC", clearMemoryClicked)
        val readMemoryButton = createButton("MR", readMemoryClicked)
        val setMemoryButton = createButton("MS", setMemoryClicked)
        val addToMemoryButton = createButton("M+", addToMemoryClicked)

        val divisionButton = createButton("÷", multiplicativeOperatorClicked)
        val timesButton = createButton("×", multiplicativeOperatorClicked)
        val minusButton = createButton("-", additiveOperatorClicked)
        val plusButton = createButton("+", additiveOperatorClicked)

        val squareRootButton = createButton("Sqrt", unaryOperatorClicked)
        val powerButton = createButton("x\302\262", unaryOperatorClicked)
        val reciprocalButton = createButton("1/x", unaryOperatorClicked)
        val equalButton = createButton("=", equalClicked)

        GridPane.setConstraints(backspaceButton, 0, 1, 2, 1)
        children += backspaceButton

        GridPane.setConstraints(clearButton, 2, 1, 2, 1)
        children += clearButton

        GridPane.setConstraints(clearAllButton, 4, 1, 2, 1)
        children += clearAllButton

        GridPane.setConstraints(clearMemoryButton, 0, 2)
        children += clearMemoryButton

        GridPane.setConstraints(readMemoryButton, 0, 3)
        children += readMemoryButton

        GridPane.setConstraints(setMemoryButton, 0, 4)
        children += setMemoryButton

        GridPane.setConstraints(addToMemoryButton, 0, 5)
        children += addToMemoryButton

        (1 until kNumDigitButtons).foreach { i =>
          val row = ((9 - i) / 3) + 2
          val column = ((i - 1) % 3) + 1
          children += digitButtons(i)
          GridPane.setConstraints(digitButtons(i), column, row)
        }
        children += digitButtons(0)
        GridPane.setConstraints(digitButtons(0), 1, 5)

        children += pointButton
        GridPane.setConstraints(pointButton, 2, 5)

        children += changeSignButton
        GridPane.setConstraints(changeSignButton, 3, 5)

        children += divisionButton
        GridPane.setConstraints(divisionButton, 4, 2)

        children += timesButton
        GridPane.setConstraints(timesButton, 4, 3)

        children += minusButton
        GridPane.setConstraints(minusButton,4,4)

        children += plusButton
        GridPane.setConstraints(plusButton,4,5)

        children += squareRootButton
        GridPane.setConstraints(squareRootButton,5,2)

        children += powerButton
        GridPane.setConstraints(powerButton,5,3)

        children += reciprocalButton
        GridPane.setConstraints(reciprocalButton,5,4)

        children += equalButton
        GridPane.setConstraints(equalButton,5,5)
      }
    }
    resizable = false
  }

  def createButton(buttonText: String, clicked: Button => Unit): Button = {
    new Button(new CalculatorButton) {
      self =>
      text = buttonText
      onAction = { _ =>
        clicked(self)
      }
    }
  }

  def clearClicked(button: Button) = ???

  def backspaceClicked(button: Button) = ???

  def changeSignClicked(button: Button) = ???

  def digitClicked(button: Button) = ???

  def pointClicked(button: Button) = ???

  def clearAllClicked(button: Button) = ???

  def clearMemoryClicked(button: Button) = ???

  def addToMemoryClicked(button: Button) = ???

  def multiplicativeOperatorClicked(button: Button) = ???

  def unaryOperatorClicked(button: Button) = ???

  def equalClicked(button: Button) = ???

  def readMemoryClicked(button: Button) = ???

  def setMemoryClicked(button: Button) = ???

  def additiveOperatorClicked(button: Button) = ???
}


class CalculatorButton extends jfxButton {
  setMaxSize(Double.MaxValue, Double.MaxValue)

  override protected def computePrefHeight(v: Double): Double = {
    val prefHeight = super.computePrefHeight(v)
    prefHeight + 20.0
  }

  override protected def computePrefWidth(v: Double): Double = {
    val prefWidth = super.computePrefWidth(v)
    prefWidth + 20.0
  }

}