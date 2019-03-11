package com.github.wenjunhuang.uiapps.calculator

import scalafx.Includes._
import scalafx.application
import scalafx.application.JFXApp
import scalafx.geometry.Pos
import scalafx.scene.Scene
import scalafx.scene.control.{Button, TextField}
import scalafx.scene.layout.GridPane
import scalafx.scene.text.Font

object Calculator extends JFXApp {
  stage = new application.JFXApp.PrimaryStage {
    title.value = "Calculator"
    scene = new Scene {
      content = new GridPane {
        val display = new TextField {
          text = "0"
          alignment = Pos.CenterRight
          prefColumnCount = 15
          font = Font(font.value.getFamily, font.value.getSize + 8.0)
        }

        GridPane.setConstraints(display, 0, 0, 6, 1)
      }
    }
  }

  def createButton(buttonText: String, clicked: Button => Unit): Button = {
    new Button {
      self =>
      text = buttonText
      onAction = { _ =>
        clicked(self)
      }
    }
  }

}
