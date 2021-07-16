import androidx.compose.desktop.Window
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material.Icon
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.shadow
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.Shadow
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.unit.dp

@Composable
fun Item(todo: Todo) {
    Row(
        modifier = Modifier.padding(
            start = 24.dp,
            top = 8.dp,
            end = 8.dp,
            bottom = 8.dp
        )
    ) {

    }
}

@Composable
fun CreateNewTodo() {
    Row() {

    }
}

@Composable
fun ItemsBody() {
    LazyColumn {

    }
}

fun main() = Window(title = "TodoMVC") {
    Box(
        modifier = Modifier
            .fillMaxWidth()
            .fillMaxHeight()
            .background(Styles.BackgroundColor)
    ) {
        Column(
            Modifier.fillMaxWidth()
                .padding(horizontal = 32.dp, vertical = 16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Box(modifier = Modifier.padding(bottom = 16.dp)) {
                Text("todos", style = Styles.TitleTextStyle)
            }
            Surface(
                modifier = Modifier.background(Color.White).weight(1f).fillMaxWidth()
                    .shadow(5.dp),
            ) {

            }
        }
    }
}
