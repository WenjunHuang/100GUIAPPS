import androidx.compose.desktop.Window
import androidx.compose.foundation.*
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.itemsIndexed
import androidx.compose.foundation.lazy.rememberLazyListState
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.rounded.Check
import androidx.compose.material.icons.rounded.KeyboardArrowDown
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.runtime.getValue
import androidx.compose.runtime.setValue
import androidx.compose.ui.unit.dp

val testData = listOf(
    Todo("Go Shopping", true),
    Todo("Refactor Backend", false),
    Todo("Add new feature to frontend", true),
    Todo("Go Shopping", true),
    Todo("Refactor Backend", false),
    Todo("Add new feature to frontend", true),
    Todo("Add new feature to frontend", true),
    Todo("Go Shopping", true),
    Todo("Refactor Backend", false),
)

@Composable
fun Item(modifier: Modifier = Modifier, todo: Todo) {
    Box(modifier = modifier) {
        Row(
            horizontalArrangement = Arrangement.Start,
            verticalAlignment = Alignment.CenterVertically,
        ) {
            Box(
                modifier = Modifier
                    .requiredSize(width = 32.dp, height = 32.dp)
                    .border(
                        border = BorderStroke(1.dp, Styles.BorderHintColor),
                        shape = CircleShape
                    ),
                contentAlignment = Alignment.Center
            ) {
                if (!todo.active)
                    Icon(
                        Icons.Rounded.Check, contentDescription = null,
                        modifier = Modifier.size(width = 24.dp, height = 24.dp),
                        tint = Styles.SecondaryColor
                    )
            }
            Spacer(Modifier.width(16.dp))
            Text(
                todo.text,
                style = if (todo.active) Styles.ActiveItemTextStyle else Styles.CompletedItemTextStyle,
                modifier = Modifier.weight(1f),
            )
        }
    }
}

@Composable
fun CreateNewTodo() {
    var newItem by remember { mutableStateOf("") }
    Row(
        verticalAlignment = Alignment.CenterVertically,
        horizontalArrangement = Arrangement.Start,
    ) {
        Icon(
            Icons.Rounded.KeyboardArrowDown, contentDescription = null,
            modifier = Modifier.size(width = 64.dp, height = 64.dp),
            tint = Styles.BorderHintColor
        )
        TextField(
            newItem, onValueChange = {
                newItem = it
            },
            modifier = Modifier.weight(1f),
            textStyle = Styles.CreateItemTextStyle,
            placeholder = {
                Text(
                    "What needs to be done?",
                    style = Styles.CreateItemHintTextStyle
                )
            },
            singleLine = true,
            colors = TextFieldDefaults.textFieldColors(
                backgroundColor = Color.Transparent,
                disabledIndicatorColor = Color.Transparent,
                unfocusedIndicatorColor = Color.Transparent,
                focusedIndicatorColor = Color.Transparent
            )
        )
    }
}

@Composable
fun ItemsBody(modifier: Modifier) {
    Box(modifier = modifier) {
        val listState = rememberLazyListState()

        LazyColumn(state =listState) {
            itemsIndexed(testData) { index, todo ->
                Item(
                    modifier = Modifier.padding(
                        start = 24.dp,
                        top = 8.dp,
                        end = 8.dp,
                        bottom = 8.dp
                    )
                        .fillMaxWidth(),
                    todo
                )
                if (index != testData.size - 1)
                    Divider(color = Styles.BorderHintColor, thickness = 1.dp)
            }
        }
        VerticalScrollbar(
            modifier = Modifier.align(Alignment.CenterEnd).fillMaxHeight(),
            adapter = rememberScrollbarAdapter(
                scrollState = listState,
            )
        )
//        VerticalScrollbar(adapter = rememberScrollbarAdapter(stateVertical),
//        modifier = Modifier.align(Alignment.CenterEnd).fillMaxHeight())
    }
}

@Composable
fun StatusBarFilterButton(name: String, selected: Boolean) {
    Box(
        modifier = (if (selected) Modifier.border(
            1.dp, color = Styles.BorderHintColor,
            shape = RoundedCornerShape(3.dp)
        ) else Modifier).padding(horizontal = 16.dp, vertical = 8.dp)
    ) {
        Text(
            name,
            style = Styles.StatusTextStyle,
            color = Styles.StatusColor
        )
    }
}

@Composable
fun StatusBar(modifier: Modifier = Modifier) {
    Box(modifier = Modifier.padding(8.dp)) {
        Row(verticalAlignment = Alignment.CenterVertically) {
            Text(
                text = "3 items left",
                style = Styles.StatusTextStyle,
                color = Styles.StatusColor
            )
            Row(
                modifier = Modifier.weight(2f)
            ) {
                Spacer(modifier = modifier.weight(1f))
                StatusBarFilterButton("All", true)
                Spacer(modifier = modifier.width(8.dp))
                StatusBarFilterButton("Active", false)
                Spacer(modifier = modifier.width(8.dp))
                StatusBarFilterButton("Completed", false)
            }
            Spacer(modifier = modifier.weight(1f))
        }
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
                modifier = Modifier
                    .fillMaxSize()
                    .background(Color.White)
                    .drawColoredShadow(
                        offsetX = 0.dp, offsetY = 35.dp, shadowRadius = 60.dp, color = Color.Black,
                        alpha = 0.2f,
                    ),
            ) {
                Column(
                    Modifier.fillMaxHeight(),
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {
                    CreateNewTodo()
                    Divider(
                        color = Styles.BorderHintColor,
                        thickness = 1.dp
                    )
                    ItemsBody(modifier = Modifier.fillMaxWidth().weight(1f))
                    Divider(
                        color = Styles.BorderHintColor,
                        thickness = 1.dp
                    )
                    StatusBar()
                }
            }
        }
    }
}
