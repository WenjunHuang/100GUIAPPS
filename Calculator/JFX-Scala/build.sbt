name := "Calculator"

version := "0.1"

scalaVersion := "2.12.6"


val osName: SettingKey[String] = SettingKey[String]("osName")
val jfxVersion = "11.0.2"

osName := (System.getProperty("os.name") match {
  case name if name.startsWith("Linux") => "linux"
  case name if name.startsWith("Mac") => "mac"
  case name if name.startsWith("Windows") => "win"
  case _ => throw new Exception("Unknown platform!")
})

libraryDependencies := Seq(
  "org.scalafx" %% "scalafx" % "11-R16",
  "org.typelevel" %% "cats-core" % "1.6.0"
)
libraryDependencies += "org.openjfx" % "javafx-base" % jfxVersion classifier osName.value

libraryDependencies += "org.openjfx" % "javafx-controls" % jfxVersion classifier osName.value

libraryDependencies += "org.openjfx" % "javafx-fxml" % jfxVersion classifier osName.value

libraryDependencies += "org.openjfx" % "javafx-graphics" % jfxVersion classifier osName.value