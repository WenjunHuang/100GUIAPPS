package com.github.wenjunhuang.uiapps.calculator
import javafx.application.Application
import javafx.stage._
import javafx.scene._
import javafx.scene.control._
import com.thoughtworks.binding._

final class FxmlSample extends Application {

  @fxml override def start(primaryStage: Stage): Unit = {
    val scene: Binding[Scene] = <Scene><Label>Hello, World!</Label></Scene>
    fxml.show(primaryStage, scene)
  }

}

object FxmlSample {
  def main(args: Array[String]): Unit = {
    Application.launch(classOf[FxmlSample], args: _*)
  }
}
