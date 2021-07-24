import org.jetbrains.compose.compose
import org.jetbrains.compose.desktop.application.dsl.TargetFormat
import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

plugins {
    kotlin("jvm") version "1.5.10"
    id("org.jetbrains.compose") version "0.4.0"
}

group = "com.github.wenjunhuang"
version = "1.0"

repositories {
    jcenter()
    mavenCentral()
    maven { url = uri("https://maven.pkg.jetbrains.space/public/p/compose/dev") }
}

dependencies {
    implementation(compose.desktop.currentOs)
    implementation("com.arkivanov.decompose:decompose:0.2.6")
    implementation("com.arkivanov.decompose:extensions-compose-jetbrains:0.2.6")
    implementation("org.xerial:sqlite-jdbc:3.36.0.1")
    implementation("org.jooq:jooq:3.15.1")
    implementation("io.arrow-kt:arrow-fx:0.12.1")
}

sourceSets {
    named("main") {
        resources { srcDir("../../assets") }
    }
}

tasks.withType<KotlinCompile>() {
    kotlinOptions.jvmTarget = "11"
}

compose.desktop {
    application {
        mainClass = "MainKt"
        nativeDistributions {
            targetFormats(TargetFormat.Dmg, TargetFormat.Msi, TargetFormat.Deb)
            packageName = "TodoMVC"
            packageVersion = "1.0.0"
        }
    }
}
