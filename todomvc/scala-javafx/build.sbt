name := "TodoMVC"

version := "1.0"

scalaVersion := "2.13.6"

idePackagePrefix := Some("com.github.wenjunhuang.todomvc")


lazy val osName        = System.getProperty("os.name") match {
  case n if n.startsWith("Linux") => "linux"
  case n if n.startsWith("Mac") => "mac"
  case n if n.startsWith("Windows") => "win"
  case _ => throw new Exception("Unknown platform!")
}
Compile / unmanagedResourceDirectories ++= Seq(baseDirectory.value / ".." / ".." / "assets")
libraryDependencies ++=
  Seq(
    "org.scalafx" %% "scalafx" % "16.0.0-R24",
    "org.typelevel" %% "cats-effect" % "3.1.0")

lazy val javaFXModules = Seq("base", "controls", "fxml", "graphics", "media", "swing", "web")
libraryDependencies ++= javaFXModules.map(m => "org.openjfx" % s"javafx-$m" % "16" classifier osName)
addCompilerPlugin("com.olegpy" %% "better-monadic-for" % "0.3.1")
